---
title:  "Server lecture section2 [4/4]"
excerpt: "메모리 관리"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.11 10:00:00
---

# Object Pool

기존 메모리 풀은 비슷한 크기의 메모리들을 같은 메모리 풀에 넣어 공용으로 사용함    
이 때 메모리가 오염된 경우 원인을 찾기 힘들다는 단점이 존재    
따라서 동일한 크기가 아닌 동일한 클래스끼리 묶어서 관리하는 오브젝트 풀을 사용할 수 있음    

```cpp
// ObjectPool.h

#include "Types.h"
#include "MemoryPool.h"

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif		
		new(memory)Type(forward<Args>(args)...); // placement new
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}

	static shared_ptr<Type> MakeShared()
	{
		shared_ptr<Type> ptr = { Pop(), Push };
		return ptr;
	}

private:
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };
```

```cpp
// Memory.h

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>{ xnew<Type>(), xdelete<Type> };
}

// Memory.cpp

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;

	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// 메모리 풀에서 꺼내옴
		header = _poolTable[allocSize]->Pop();
	}
#endif

	// MemoryHeader에 정보 기입
	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀에 반납
		_poolTable[allocSize]->Push(header);
	}
#endif
}
```

```cpp
class Knight
{
public:
	int32 _hp = rand() % 1000;
};

class Monster
{
public:
	int64 _id = 0;
};

int main()
{
	Knight* knights[100];

	for (int32 i = 0; i < 100; i++)
		knights[i] = ObjectPool<Knight>::Pop();

	for (int32 i = 0; i < 100; i++)
	{
		ObjectPool<Knight>::Push(knights[i]);
		knights[i] = nullptr;
	}

	// 수동으로 관리
	//Knight*k = ObjectPool<Knight>::Pop();
	//ObjectPool<Knight>::Push(k);

	// 스마트 포인터를 사용하여 자동으로 관리
	// 기본 new delete 사용
	//shared_ptr<Knight> sptr = make_shared<Knight>(); 
	
	// 오브젝트 풀링 사용
	// shared_ptr<Knight> sptr = { ObjectPool<Knight>::Pop(), ObjectPool<Knight>::Push(k); };
	shared_ptr<Knight> sptr = ObjectPool<Knight>::MakeShared();
	
	// 메모리 풀링 사용
	shared_ptr<Knight> sptr2 = MakeShared<Knight>();
}
```

***

# TypeCast

`static_cast`는 잘못 사용시 메모리 오염이 발생    
`dynamic_cast`는 잘못된 사용은 방지할 수 있으나 성능이 좋지 않음    
따라서 주로 `static_cast`를 이용하는데, 안전하게 하기 위해서는 타입을 확인하는 작업이 필요    


`TypeList`는 컴파일시 크기가 정해짐    

`Conversion` : `From`에서 `To`로 변환이 가능한지를 판별    
`TypeCast` : 타입끼리 변환이 가능한지를 테이블을 만들어 판별    
* 테이블은 미리 만들어지기 때문에 추후 계속해서 편리하게 사용할 수 있음    
* `shared_ptr`를 사용하는 버전도 간단하게 만들 수 있음    