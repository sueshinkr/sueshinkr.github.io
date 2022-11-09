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
