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




***

# 연습문제
