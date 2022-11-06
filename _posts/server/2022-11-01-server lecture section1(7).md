---
title:  "Server lecture section1 [7/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.01 18:00:00
---

# Reader-Writer Lock

```cpp
/*----------------
    RW SpinLock
-----------------*/

/*--------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)
---------------------------------------------*/

// W -> R (O)
// R -> W (x)
class Lock
{
    enum : uint32
    {
        ACQUIRE_TIMEOUT_TICK = 10000,
        MAX_SPIN_COUNT = 5000,
        WRITE_THREAD_MASK = 0xFFFF'0000,
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };

public:
    void WriteLock();
    void WriteUnlock();
    void ReadLock();
    void ReadUnlock();

private:
    Atomic<uint32> _lockFlag = EMPTY_FLAG;
    uint16 _writeCount = 0;
};

/*----------------
    LockGuards
-----------------*/

class ReadLockGuard
{
	public:
		ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
		~ReadLockGuard() { _lock.ReadUnlock(); }

	private:
		Lock& _lock;
};

class WriteLockGuard
{
	public:
		WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
		~WriteLockGuard() { _lock.WriteUnlock(); }

	private:
		Lock& _lock;
};
```

```cpp
void Lock::WriteLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻음
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	// ReadLock 다 풀기 전에는 WriteUnlock 불가능
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올림
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
```

`Write`정보와 `Read`정보를 하나의 `int32`형 데이터에 비트플래그를 통해 묶어서 사용    


`WriteLock`이 걸려있는 상태에서 동일 쓰레드가 쓰거나 읽는 것은 허용    
다른 쓰레드가 접근시 경합, `MAX_SPIN_COUNT`만큼 시도 후 초과시 크래시

`ReadLock`이 걸려있는 상태에서 동일 쓰레드가 읽는 것은 허용하지만, 쓰는 것은 허용하지 않음    
`WriteLock`을 획득한 쓰레드가 없다면 다른 쓰레드들도 경합없이 `ReadLock`을 획득할 수 있음    

***

# DeadLock 탐지

그래프 구조를 이용하여 탐지    
* 그래프 사이클을 DFS를 사용하여 판별
* 순방향 간선 / 교차 간선 / 역방향 간선
* 역방향 간선이 발견될 경우 사이클이 발생


```cpp
// CoreGlobal.h
extern class DeadLockProfiler* GDeadLockProfiler;

// Lock.h
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
    const char* _name;
};

// Lock.cpp
void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	...
}

// CoreMacro.h
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());

```

```cpp
// DeadLockProfiler.h

#include <stack>
#include <map>
#include <vector>

/*---------------------
	DeadLockProfiler
---------------------*/

class DeadLockProfiler
{
	public:
		void PushLock(const char* name);
		void PopLock(const char* name);
		void CheckCycle();

	private:
		void Dfs(int32 index);

	private:
		unordered_map<const char*, int32>	_nameTold;
		unordered_map<int32, const char*>	_IdToName;
		stack<int32>						_lockStack;
		map<int32, set<int32>>				_lockHistory;

		Mutex _lock;

	private:
		vector<int32> _discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
		int32 _discoveredCount = 0; // 노드가 발견된 순서
		vector<bool> _finished; // Dfs()가 종료되었는지 여부
		vector<int32> _parent;
};
```

```cpp
// DeadLockProfiler.cpp

#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급
	int32 lockid = 0;

	auto findit = _nameTold.find(name);
	if (findit == _nameTold.end())
	{
		lockid = static_cast<int32>(_nameTold.size());
		_nameTold[name] = lockid;
		_IdToName[lockid] = name;
	}
	else
	{
		lockid = findit->second;
	}

	// 잡고있는 락이 있었다면
	if (LLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인
		const int32 previd = LLockStack.top();
		if (lockid != previd)
		{
			set<int32>& history = _lockHistory[previd];
			if (history.find(lockid) == history.end())
			{
				history.insert(lockid);
				CheckCycle();
			}
		}
	}

	LLockStack.push(lockid);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockid = _nameTold[name];
	if (_lockStack.top() != lockid)
		CRASH("INVALID_UNLOCK")

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameTold.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockid = 0; lockid < lockCount; lockid++)
		Dfs(lockid);

	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회
	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}
	
	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면 there은 here의 후손 (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)이 아직 종료되지 않았다면 there은 here의 선조 (역방향 간선)
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _IdToName[here], _IdToName[there]);

			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _IdToName[_parent[now]], _IdToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}
			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}
```

`unordered_map`을 사용하여 `_nameToId`, `IdToName`에 쓰레드 이름과 번호를 쌍으로 저장    
`#if _DEBUG`로 디버그 모드에서만 데드락이 일어날 수 있는 상황인지를 확인    

`PushLock` 함수에 들어온 쓰레드가 `_nameToId`에 저장되어있는 쓰레드가 아닐경우 새로 저장    
`_lockStack` 스택을 확인하여 잡고있는 락이 존재하며 현재 쓰레드가 바로 이전에 락이 걸린 쓰레드가 아니고, `_lockHistory` 에도 저장되어있지 않을 경우 새롭게 넣어주면서 싸이클을 체크    

`CheckCycle` 함수에서는 `Dfs`를 사용하여 모든 정점들을 순회하며 그래프의 방향을 확인    

***

# 연습문제

```cpp
// 소수 구하기

bool IsPrime(int number)
{
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; i++)
	{
		if (number % i == 0)
			return false;
	}
	return true;
}

int CountPrime(int start, int end)
{
	int count = 0;

	for (int number = start; number <= end; number++)
	{
		if (IsPrime(number))
			count++;
	}
	return count;
}

int main()
{
	const int MAX_NUMBER = 100'0000;
	// 1~MAX_NUMBER까지의 소수 개수

	vector<thread> threads;

	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	atomic<int> primeCount = 0;

	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]()
			{
				primeCount += CountPrime(start, end);
			}));
	}

	for (thread& t : threads)
		t.join();

	cout << primeCount << endl;
}
```

서로 공유하는 자원끼리 영향을 끼치지 않으므로 자유롭게 병렬 처리가 가능    
