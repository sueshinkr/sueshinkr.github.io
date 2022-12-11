---
title:  "Server lecture section1 [6/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.31 18:00:00
---

# Lock-Free Queue (참고만)

```cpp
template<typename T>
class LockFreeQueue
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount; // 참조권
		Node* ptr = nullptr;
	};

	struct NodeCounter
	{
		uint32 internalCount : 30; // 참조권 반환 관련, 비트를 30과 2로 쪼개어서 사용
		uint32 externalCountRemaining : 2; // Push, Pop 다중 참조권 관련
	};

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount.internalCount = 0;
			newCount.externalCountRemaining = 2;
			count.store(newCount);

			next.ptr = nullptr;
			next.externalCount = 0;
		}

		void ReleaseRef()
		{
			NodeCounter oldCounter = count.load();

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.internalCount--;

				// 끼어들 수 있음
				if (count.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
						delete this;

					break;
				}
			}
		}

		atomic<T*> data;
		atomic<NodeCounter> count;
		CountedNodePtr next;
	};

	public:
		LockFreeQueue()
		{
			CountedNodePtr node;
			node.ptr = new Node;
			node.externalCount = 1;

			_head.store(node);
			_tail.store(node);
		}

		LockFreeQueue(const LockFreeQueue&) = delete;
		LockFreeQueue& operator=(const LockFreeQueue&) = delete;

		void Push(const T& value)
		{
			unique_ptr<T> newData = make_unique<T>(value);

			CountedNodePtr dummy;
			dummy.ptr = new Node;
			dummy.externalCount = 1;

			CountedNodePtr oldTail = _tail.load(); // data = nullptr

			while (true)
			{
				// 참조권 획득 (externalCount를 현시점 기준 +1 한 애가 이김)
				IncreaseExternalCount(_tail, oldTail);

				// 소유권 획득 (data를 먼저 교환한 애가 이김)
				T* oldData = nullptr;
				if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
				{
					oldTail.ptr->next = dummy;
					oldTail = _tail.exchange(dummy);
					FreeExternalCount(oldTail);

					newData.release(); // 데이터에 대한 unique_ptr의 소유권 포기
					break;
				}

				// 소유권 경쟁 패배
				oldTail.ptr->ReleaseRef();
			}
		}

		shared_ptr<T> TryPop()
		{
			CountedNodePtr oldHead = _head.load();

			while (true)
			{
				// 참조권 획득 (externalCount를 현시점 기준 +1 한 애가 이김)
				IncreaseExternalCount(_head, oldHead);

				Node* ptr = oldHead.ptr;
				if (ptr == _tail.load().ptr)
				{
					ptr->ReleaseRef();
					return shared_ptr<T>();
				}

				// 소유권 획득 (head = ptr->next)
				if (_head.compare_exchange_strong(oldHead, ptr->next))
				{
					T* res = ptr->data.load();
					FreeExternalCount(oldHead);
					return shared_ptr<T>(res);
				}

				ptr->ReleaseRef();
			}
		}

	private:
		static void IncreaseExternalCount(atomic<CountedNodePtr>& counter, CountedNodePtr& oldCounter)
		{
			while (true)
			{
				CountedNodePtr newCounter = oldCounter;
				newCounter.externalCount++;

				if (counter.compare_exchange_strong(oldCounter, newCounter))
				{
					oldCounter.externalCount = newCounter.externalCount;
					break;
				}
			}
		}

		static void FreeExternalCount(CountedNodePtr& oldNodePtr)
		{
			Node* ptr = oldNodePtr.ptr;
			const int32 countIncrease = oldNodePtr.externalCount - 2;

			NodeCounter oldCounter = ptr->count.load();

			while(true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.externalCountRemaining--;
				newCounter.internalCount += countIncrease;

				if (ptr->count.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
						delete ptr;
					break;	
				}
			}
		}

	private:
		atomic<CountedNodePtr> _head;
		atomic<CountedNodePtr> _tail;
};
```

***

# ThreadManager

```cpp
// CoreGlobal.h
#ifndef COREGLOBAL_H
#define COREGLOBAL_H

extern class ThreadManager* GThreadManager;

class CoreGlobal
{
	public:
		CoreGlobal();
		~CoreGlobal();
};

#endif


// CoreGlobal.cpp
ThreadManager* GThreadManager = nullptr;

CoreGlobal::CoreGlobal()
{
	GThreadManager = new ThreadManager();
}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
}
```

```cpp
// CoreTls.h
#ifndef CORETLS_H
#define CORETLS_H

extern thread_local uint32 LThreadId;

#endif


// CoreTls.cpp
thread_local uint32 LThreadId = {0};
```

```cpp
// CoreMacro.h
/*-------------
	Crash
-------------*/
#define CRASH(cause) 						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(exptr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}
```

```cpp
// ThreadManager.h
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>
#include <vector>
#include <functional>

using namespace std;

class ThreadManager
{
	public:
		ThreadManager();
		~ThreadManager();

		void	Launch(function<void(void)> callback);
		void	Join();

		static void	InitTLS();
		static void DestroyTLS();
	private:
		Mutex _lock;
		vector<thread> _threads;
};

#endif


// ThreadManager.cpp
ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
	{
		InitTLS();
		callback();
		DestroyTLS();
	}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SthreadId = {1};
	LThreadId = SthreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}
```

`ThreadManager` 클래스를 따로 만들어 쓰레드 관리를 편하게 함    
* `Launch()` 메소드를 통해 쓰레드 생성
	* 이 때 `InitTLS()` 메소드를 호출하여 TLS 생성    
	* `callback()` 메소드로 쓰레드를 진입시킬 함수를 호출 
	* 쓰레드가 진입한 함수에서 빠져나오면 `DestroyTLS()` 메소드로 TLS 소멸
* `Join()` 메소드로 생성된 모든 쓰레드들이 종료되기까지 대기

