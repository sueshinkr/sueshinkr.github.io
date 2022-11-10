---
title:  "Server lecture section2 [3/4]"
excerpt: "메모리 관리"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.09 19:00:00
---

# Memory Pool #1

메모리 풀링 : 메모리의 재사용 개념    
* 메모리 해제, 할당 반복시 컨텍스트 스위칭이 일어나기 때문에 자원의 낭비가 발생
* 메모리 해제, 할당 반복시 메모리 파편화가 발생할 수 있음
* 이를 메모리 풀링을 통해 방지할 수 있으나, 최근의 할당기는 성능이 뛰어나기 때문에 선택의 영역이 됨    

```cpp
// MemoryPool.h

/*-----------------
	MemoryHeader
------------------*/

// 할당된 객체의 정보가 담긴 구조체, 데이터 앞에 들어감
// [MemoryHeader][Data]
struct MemoryHeader
{
	MemoryHeader(int32 size) : allocSize(size) {}

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
};

/*-----------------
	MemoryPool
------------------*/

class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();

private:
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

	USE_LOCK; 
	queue<MemoryHeader*> _queue;

};
```

```cpp
// MemoryPool.cpp

#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}


void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	
	// Pool에 메모리 반납
	ptr->allocSize = 0;

	_queue.push(ptr);
	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	{
		WRITE_LOCK;

		// Pool에 여분이 있는지 확인
		if (_queue.empty() == false)
		{
			// 있으면 하나 꺼내옴
			header = _queue.front();
			_queue.pop();
		}
	}

	// 없으면 새로 만듬
	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}
	else
	{
		ASSERT_CRASH(header->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return header;
}
```

```cpp
// Memory.h

/*-------------
	Memory
--------------*/

class Memory
{
	enum
	{
		// ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기에 따라 메모리 풀을 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};
```

```cpp
// Allocator.h

/*------------------
	PoolAllocator
-------------------*/

class PoolAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};
```

동일한 데이터들을 묶어서 관리하는 것이 편리하기 때문에 메모리 크기별로 탐색하는 테이블인 `_poolTable`을 만들어 사용    

메모리 풀링과 `StompAllocator`는 같이 사용하기에 적합하지 않음
* `StompAllocator` : 메모리가 필요없어지면 운영체제에 해당 메모리를 확실하게 지울 것을 요구
* 메모리 풀링 : 메모리가 필요없어져도 재사용을 위해 보관

***

# Memory Pool #2

`Lock`을 사용하여 멀티쓰레드 환경에서는 경합이 발생    
메모리를 동적 배열인 큐에 저장    

`LockFreeStack` 설계시 `Node`를 따로 만들지 않고 사용하고자하는 데이터 내에 포함시키는 방법을 사용할 수도 있음    
이 경우 노드를 따로 할당하지 않아도 되지만, 데이터 자체의 설계가 필요하므로 외부 라이브러리에는 적용할 수 없음    

```cpp
// LockFreeStack.h

/*--------------
	1차 시도
---------------*/

struct SListEntry
{
	SListEntry* next;

};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void initializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);

// LockFreeStack.cpp

/*--------------
	1차 시도
---------------*/

void initializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;

	if (first != nullptr)
		header->next = first->next;

	return first;
}

```

1차 시도와 같이 설계시 싱글쓰레드에서만 정상작동함



```cpp
// LockFreeStack.h

/*--------------
	2차 시도
---------------*/

struct SListEntry
{
	SListEntry* next;

};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void initializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);

// LockFreeStack.cpp

/*--------------
	2차 시도
---------------*/

void initializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;

	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) == 0)
	{

	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* expected = header->next;

	// ABA Problem
	// 도중에 상태가 변했지만 header->next값은 그대로일 경우, 아래 코드는 정상적으로 실행됨
	// 따라서 단일 포인터는 CAS를 할 수 없음

	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected == 0))
	{

	}

	return expected;
}
```

2차 시도와 같이 설계씨 ABA Problem이 발생    

```cpp
// LockFreeStack.h

/*--------------
	3차 시도
---------------*/

DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;

};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	union
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		struct
		{
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};

};

void initializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);

// LockFreeStack.cpp

/*--------------
	3차 시도
---------------*/

void initializeHead(SListHeader* header)
{
	header->alignment = 0;
	header->region = 0;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};

	// 16바이트 정렬
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// 이 사이에 데이터가 변경될 수 있음

		entry->next = (SListEntry*)(((int64)expected.HeaderX64.next) << 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (true)
	{
		expected = *header;

		entry = (SListEntry*)(((int64)expected.HeaderX64.next) << 4);
		if (entry == nullptr)
			break;

		// 멀티 쓰레드에서 Pop을 할시 Use-After-Free 문제는 여전히 발생할 수 있음
		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return entry;
}
```

```cpp
#include "LockFreeStack.h"

DECLSPEC_ALIGN(16)
class Data // : public SListEntry
{
public:
	SListEntry _entry;
	int64 _rand = rand() % 1000;
};

SListHeader* GHeader;

int main()
{
	GHeader = new SListHeader();
	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
	initializeHead(GHeader);

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* data = new Data();
					ASSERT_CRASH(((uint64)data % 16) == 0);

					PushEntrySList(GHeader, (SListEntry*)data);
					this_thread::sleep_for(10ms);
				}
			});
	}

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* pop = nullptr;
					pop = (Data*)PopEntrySList(GHeader);

					if (pop)
					{
						cout << pop->_rand << endl;
						delete pop;
					}
					else
					{
						cout << "NONE" << endl;
					}
				}
			});
	}
}
```

ABA 문제를 해결하기 위해 카운터를 둠    
단, 멀티쓰레드 환경에서 동시다발적인 `Pop` 발생시 Use-After-Free 문제는 여전히 발생할 수 있음    
16바이트 정렬을 하지 않는 경우 문제가 발생    

***

# Memory Pool #3

```cpp
DECLSPEC_ALIGN(16)
class Data // : public SListEntry
{
public:
	SLIST_ENTRY _entry;
	int64 _rand = rand() % 1000;
};

SLIST_HEADER* GHeader;

int main()
{
	GHeader = new SLIST_HEADER();
	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
	InitializeSListHead(GHeader);

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* data = new Data();
					ASSERT_CRASH(((uint64)data % 16) == 0);

					InterlockedPushEntrySList(GHeader, (SLIST_ENTRY*)data);
					this_thread::sleep_for(10ms);
				}
			});
	}

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* pop = nullptr;
					pop = (Data*)InterlockedPopEntrySList(GHeader);

					if (pop)
					{
						cout << pop->_rand << endl;
						delete pop;
					}
					else
					{
						cout << "NONE" << endl;
					}
				}
			});
	}
}
```

Window에서 기본으로 제공하는 `SLIST` 타입이 존재    
Memory Pool #2에서 구현한 것과 거의 동일하게 작동함    

```cpp
// MemoryPool.h

enum
{
	SLIST_ALIGNMENT = 16
};

/*-----------------
	MemoryHeader
------------------*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size) : allocSize(size) {}

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
};

/*-----------------
	MemoryPool
------------------*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
	public:
		MemoryPool(int32 allocSize);
		~MemoryPool();

		void			Push(MemoryHeader* ptr);
		MemoryHeader*	Pop();

	private:
		SLIST_HEADER	_header;
		int32			_allocSize = 0;
		atomic<int32>	_allocCount = 0;
};
```

```cpp
// MemoryPool.cpp

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	// Pool에 메모리 반납

	ptr->allocSize = 0;
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// 없으면 새로 만듬
	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return memory;
}
```

```cpp
class Knight
{
public:
	int32 _hp = rand() % 1000;
};

int main()
{

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Knight* knight = xnew<Knight>();

					cout << knight->_hp << endl;

					this_thread::sleep_for(10ms);

					xdelete(knight);
				}
			});
	}
	
	GThreadManager->Join();
}
```

기존 `MemoryPool`에서 `queue`를 사용하는 구조 대신 `SLIST`를 사용한 `LockFree` 구조로 변경    

