---
title:  "Server lecture section1 [5/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.28 18:00:00
---

# Thread Local Storage

쓰레드마다 개별적으로 가지고있는 저장공간이 존재    
각 쓰레드에서 필요한 데이터들을 모두 TLS로 옮겨온 후에 사용    
스택은 불안정한 메모리, TLS는 자신만의 전역 메모리    

C++11에서 TLS를 사용하는 표준이 정의됨    
```cpp
#include <thread>

thread_local int32 LThreadid = 0;

void ThreadMain(int32 threadid)
{
	LThreadid = threadid;

	while (true)
	{
		cout << "Hi! I am Thread " << LThreadid << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadid = i + 1;
		threads.push_back(thread(ThreadMain, threadid));
	}

	for (thread& t : threads)
		t.join();
}
```

***

# Lock-Based Stack / Queue

```cpp
template<typename T>
class LockStack
{
	public:
		LockStack() {}
		LockStack(const LockStack&) = delete;
		LockStack& operator=(const LockStack&) = delete;

		void Push(T value)
		{
			lock_guard<mutex> lock(_mutex);
			_stack.push(std::move(value));
			_condVar.notify_one();
		}

		bool TryPop(T& value)
		{
			lock_guard<mutex> lock(_mutex);
			if (_stack.empty())
				return false;
			
			value = std::move(_stack.top());
			_stack.pop(); 
			return true;
		}

		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _stack.empty() == false; });
			value = std::move(_stack.top());
			_stack.pop();
		}

	private:
		stack<T> _stack;
		mutex _mutex;
		condition_variable _condVar;
};

template<typename T>
class LockQueue
{
	public:
		LockQueue() {}
		
		LockQueue(const LockQueue&) = delete;
		LockQueue& operator=(const LockQueue&) = delete;

		void Push(T value)
		{
			lock_guard<mutex> lock(_mutex);
			_queue.push(std::move(value));
			_condVar.notify_one();
		}

		bool TryPop(T& value)
		{
			lock_guard<mutex> lock(_mutex);
			if (_queue.empty())
				return false;
			
			value = std::move(_queue.front());
			_queue.pop();
			return true;
		}

		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _queue.empty() == false; });
			value = std::move(_queue.front());
			_queue.pop();
		}
	private:
		queue<T> _queue;
		mutex _mutex;
		condition_variable _condVar;
};
```

```cpp
LockQueue<int32> q;
LockStack<int32> s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{
		int32 data = 0;
		if (q.TryPop(data))
			cout << data << endl;
	}
}

int main()
{
	thread t1(Push);
	thread t2(Pop);

	t1.join();
	t2.join();
}
```

C++에서의 `pop()`은 `empty()`여부를 확인 후 `top()`으로 데이터를 체크하고, 이후에 `pop()`을 하는 방식으로 이루어짐    
그러나 멀티쓰레드 환경에서의 `empty()`는 거의 의미가 없음    
또한 `pop`하기 전 데이터를 확인하여 실행 여부를 결정하는 것보다 그냥 크래시가 나도록 하는 것이 게임에서는 더 유리함    

***

# Lock-Free Stack #1

```cpp
template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}
		T data;
		Node *next;
	};

	public:
		void Push(const T& value)
		{
			Node* node = new Node(value);
			node->next = _head;

			/* compare_echange_week은 내부적으로 주석의 코드와 같이 동작
			if (_head == node->next)
			{
				_head = node;
				return true;
			}
			else
			{
				node->next = _head;
				return false;
			}
			*/

			//_head의 값이 바뀌었는지 확인
			while (_head.compare_exchange_weak(node->next, node) == false)
			{
				// node->next = _head 가 자동으로 이루어짐
			}
			// while문 통과시 _head = node 가 자동으로 이루어짐
		}

		bool TryPop(T& value)
		{
			Node* oldHead = _head;
			
			while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
			{
				// oldHead = _head 가 자동으로 이루어짐
			}
			// while문 통과시 _head = oldHead->next 가 자동으로 이루어짐

			if (oldHead == nullptr)
				return false;

			// Exception 신경쓰지 않음 -> 오류상황 발생시 크래시가 나도록 하는게 유리
			value = oldHead->data;

			// 삭제를 보류
			//delete oldHead;
			
			return value;
		}

	private:
		atomic<Node*> _head;
};
```

LockFree 구조의 경우 기존 자료구조를 사용할 수 없어 직접 구성해야함    
`stack`의 경우 `head` 노드를 조작하는 동작에서 쓰레드끼리의 간섭이 발생하므로 행동이 원자적으로 이루어질 수 있게 해야함    
* `compare_exchange_weak` 구문 사용
* `pop`으로 추출해낸 `head` 노드를 바로 지워버릴경우 문제 발생    

***

# Lock-Free Stack #2

```cpp
template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{
		}
		T data;
		Node *next;
	};

	public:
		void Push(const T& value)
		{
			Node* node = new Node(value);
			node->next = _head;

			while (_head.compare_exchange_weak(node->next, node) == false)
			{

			}
		}

		bool TryPop(T& value)
		{
			++_popCount;

			Node* oldHead = _head;
			
			while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
			{
			}

			if (oldHead == nullptr)
			{
				--_popCount;
				return false;
			}

			value = oldHead->data;
			TryDelete(oldHead);
			return value;
		}

		void TryDelete(Node* oldHead)
		{
			if (_popCount == 1)
			{
				// 혼자임을 확인, 삭제 예약된 다른 데이터들도 삭제 시도
				Node* node = _pendingList.exchange(nullptr);
				if (--_popCount == 0)
				{
					// 끼어든 애가 없음 -> 삭제 진행
					// 이제와서 끼어들더라도 CAS를 통해 이미 데이터를 분리해둔 상태
					DeleteNodes(node);
				}
				else if (node)
				{
					// 끼어들었으니 다시 원복
					ChainPendingNodeList(node);
				}
				delete oldHead;
			}
			else
			{
				// 누군가 있기 때문에 삭제 예약만 함
				ChainPendingNode(oldHead);
				--_popCount;
			}
		}

		void ChainPendingNodeList(Node* first, Node* last)
		{
			last->next = _pendingList;

			while(_pendingList.compare_exchange_weak(last->next, first) == false)
			{
			}
		}

		void ChainPendingNodeList(Node* node)
		{
			Node* last = node;
			
			while (last->next)
				last = last->next;

			ChainPendingNodeList(node, last);
		}

		void ChainPendingNode(Node* node)
		{
			ChainPendingNodeList(node, node);
		}

		static void DeleteNodes(Node* node)
		{
			while (node)
			{
				Node* next = node->next;
				delete node;
				node = next;
			}
		}

	private:
		atomic<Node*> _head;
		atomic<uint32> _popCount = {0}; // Pop을 실행중인 쓰레드 개수
		atomic<Node*> _pendingList; // 삭제되어야할 노드들(첫번째 노드)
};
```

`TryPop()`에서 현재 `_head`를 `oldHead`로 저장하는데, 이 때 `oldHead`가 `nullptr`이 아니라면 스택에 1개 이상의 값이 존재한다는 의미    
이 때 while문에서 CAS를 통해 `oldHead`의 값이 `_head`와 같은지(즉 다른 쓰레드의 간섭이 없었는지)를 확인하여 같다면 `_head`를 `oldHead->next`로 바꾸는 작업을 원자적으로 실행    
* 이 경우 `TryPop()`을 시도중인 다른 쓰레드에서는 `_head`의 값이 바뀌어 `oldHead`와 달라졌으므로 `while`문에서의 CAS를 계속해서 시도

`TryDelete()`에서 `_popCount`가 1을 넘는다면 다른 쓰레드의 간섭이 있다는 뜻이므로 `oldHead`를 즉각적으로 삭제하는 대신 삭제 대기 리스트에 올려둠    
1이라면 다른 쓰레드의 간섭이 없다는 것을 의미하기 때문에 `_pendingList`를 읽음    
* 읽은 후에 `_popCount`가 바뀌지 않았다면 여전히 다른 쓰레드의 간섭이 없는 것이므로 `_pendingList`에 저장된 모든 노드를 삭제    
* 바뀌었다면 간섭이 있는 것이므로 `_pendingList`를 원상태로 돌려놓고, 현재 노드만 삭제함

***

# Lock-Free Stack #3

```cpp
template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value)), next(nullptr)
		{
		}
		shared_ptr<T> data;
		shared_ptr<Node> next;
	};

	public:
		void Push(const T& value)
		{
			shared_ptr<Node> node = make_shared<Node>(value);
			node->next = std::atomic_load(&_head);

			while (std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
			{
			}
		}

		shared_ptr<T> TryPop()
		{
			shared_ptr<Node> oldHead = std::atomic_load(&_head);

			while (oldHead && std::atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false)
			{
			}

			if (oldHead == nullptr)
				return shared_ptr<T>();
			
			return oldHead->data;
		}

	private:
		shared_ptr<Node> _head;
};
```

`atomic_is_lock_free()` 함수를 통해 위 클래스가 실제로 LockFree로 동작하지 않음을 확인할 수 있음    

```cpp
template<typename T>
class LockFreeStack
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{
		}
		shared_ptr<T> data;
		atomic<int32> internalCount = {0};
		CountedNodePtr next;
	};

	public:
		void Push(const T& value)
		{
			CountedNodePtr node;
			node.ptr = new Node(value);
			node.externalCount = 1;

			node.ptr->next = _head;
			while (_head.compare_exchange_weak(node.ptr->next, node) == false)
			{
			}
		}

		shared_ptr<T> TryPop()
		{
			CountedNodePtr oldHead = _head;
			while (true)
			{
				// 참조권 획득 (externalCount를 현 시점 기준 +1 한 애가 이김)
				IncreaseHeadCount(oldHead);
				// 최소한 externalCount >= 2 일테니 삭제 x
				Node* ptr = oldHead.ptr;

				// 데이터 없음
				if (ptr == nullptr)
					return shared_ptr<T>();

				// 소유권 획득 (ptr->next로 head를 바꿔치기 한 애가 이김)
				if (_head.compare_exchange_strong(oldHead, ptr->next))
				{
					shared_ptr<T> res;
					res.swap(ptr->data);

					// 나 말고 또 누가 있는가?
					const int32 countincrease = oldHead.externalCount - 2;

					// fetch_add는 연산 이전의 값을 리턴
					if (ptr->internalCount.fetch_add(countincrease) == -countincrease)
						delete ptr;

					return res;
				}
				else if (ptr->internalCount.fetch_sub(1) == 1)
				{
					// 참조권은 얻었으나, 소유권은 실패 -> 뒷수습
					delete ptr;
				}
			}
		}

	private:
		void IncreaseHeadCount(CountedNodePtr& oldCounter)
		{
			while (true)
			{
				CountedNodePtr newCounter = oldCounter;
				newCounter.externalCount++;

				if (_head.compare_exchange_strong(oldCounter, newCounter))
				{
					oldCounter.externalCount = newCounter.externalCount;
					break;
				}
			}
		}
		atomic<CountedNodePtr> _head;
};
```

LockFree는 동시에 여려 쓰레드가 진입할 수는 있다는 장점이 있음    
단, 참조권 획득 / 소유권 획득의 과정에서 경합에서 밀려났을 경우 처음부터 로직을 다시 실행해야한다는 단점이 있음    
