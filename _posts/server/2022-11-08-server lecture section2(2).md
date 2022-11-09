---
title:  "Server lecture section2 [2/4]"
excerpt: "메모리 관리"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.08 16:30:00
---

# Allocator

기본 `new`와 `delete`를 사용하는 대신 메모리 풀링 기법을 사용하면 속도를 향상시킬 수 있음    

```cpp
class Knight
{
	public:
		Knight()
		{
			cout << "Knight()" << endl;
		}

		~Knight()
		{
			cout << "~Knight()" << endl;
		}

		static void* operator new(size_t size)
		{
			cout << "Knight new! " << size << endl;
			void* ptr = ::malloc(size);
			return ptr;
		}

		static void operator delete(void* ptr)
		{
			cout << "Knight delete!" << endl;
			::free(ptr);
		}

		int32 _hp = 100;
		int32 _mp = 10;
};

void* operator new[](size_t size)
{
	cout << "new[]! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete[](void* ptr)
{
	cout << "delete!" << endl;
	::free(ptr);
}

int main()
{
	Knight* knight = new Knight();

	delete knight;
}
```

위와같이 `new`와 `delete`도 오버로딩하여 사용자 정의대로 사용 가능    

```cpp
// Allocator.h

/*------------------
	BaseAllocator
-------------------*/

class BaseAllocator
{
	public:
		static void*	Alloc(int32 size);
		static void		Release(void* ptr);
};

// Allocator.cpp

#include "Allocator.h"

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}
```

```cpp
// CoreMacro.h

/*---------------
	  Memory
---------------*/

#ifdef _DEBUG
#define xalloc(size)	BaseAllocator::Alloc(size)
#define xrelease(ptr)	BaseAllocator::Release(ptr)
#endif
```

```cpp
// Memory.h

#include "Allocator.h"

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));

	//placement new
	new(memory)Type(forward<Args>(args)...);

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xrelease(obj);
}
```

```cpp
class Knight
{
	public:
		Knight()
		{
			cout << "Knight()" << endl;
		}

		Knight(int32 hp) : _hp(hp)
		{
			cout << "Knight(hp)" << endl;
		}

		~Knight()
		{
			cout << "~Knight()" << endl;
		}

		int32 _hp = 100;
		int32 _mp = 10;
};

int main()
{
	Knight* knight = xnew<Knight>(100);

	xdelete(knight);
}
```

`template<typename... Args>` 형식으로 템플릿에 가변인자를 사용할 수 있음    
`Type* memory = static_cast<Type*>(xalloc(sizeof(Type)));`로 메모리를 미리 확보    
`new(memory)Type(forward<Args>(args)...);` 형식의 `new`를 사용하여 
지정한 메모리 위치에 할당, 가변인자에 따른 생성자 호출    

***

# Stomp Allocator

```cpp
int main()
{
	Knight* k1 = new Knight();
	k1->_hp = 200;
	delete k1;
	k1->_hp = 100; 
}
```
위와 같은 경우 댕글링 포인터 현상(=Use-After-Free) 발생    
스마트 포인터로 해결할 수 있음    

```cpp
int main()
{
	vector<int32> v{1,2,3,4,5};
	for (int32 i = 0; i < 5; i++)
	{
		int32 value = v[i];
		...
		if (value == 3)
		{
			v.clear();
			// break;
		}
	}
}
```

위 경우에도 벡터의 `clear()` 이후에 계속 접근하여 메모리 오염이 발생    

```cpp
class Player
{
	public:
		Player() {}
		virtual ~Player() {}
};

class Knight : public Player
{
	...
}

int main()
{
	Player* p = new Player();
	Knight* k = static_cast<Knight*>(p);
	
	k->_hp = 200;
}
```

잘못된 캐스팅으로 인해 사용해야하는 영역을 초과하여 건드리는 메모리 오염 발생    

유저 레벨의 프로그램들은 가상 메모리를 사용    
가상 메모리는 실제 메모리에 매핑됨    
운영체제가 메모리를 관리할 때는 페이지 단위를 사용(페이징)    
`SYSTEM_INFO info; GetSystemInfo(&info);` 명령어로 페이지 크기 확인 가능    
* `info.dwPageSize` : 페이지 크기
* `info.dwAllocationGranularity` : 메모리 할당시 해당 변수의 배수로 메모리 주소를 반환    

`VirtualAlloc()`, `VirtualFree()` 함수를 운영체제에 직접 메모리를 요청하여 할당하거나 해제할 때 사용    
* `int* test = (int*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)`과 같은 형식으로 사용
* 즉, 옵션을 세부적으로 설정할 수 있음
* `VirtualFree()`시 메모리를 완전하게 해제하여 더이상 해당 주소를 사용할 수 없음 (Use-After-Free 문제 해결)

```cpp
// Allocator.h

/*------------------
	StompAllocator
-------------------*/

class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};
```

```cpp
// Allocator.cpp

/*------------------
	StompAllocator
-------------------*/

void* StompAllocator::Alloc(int32 size)
{
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	return ::VirtualAlloc(NULL, pageCount + PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void StompAllocator::Release(void* ptr)
{
	::VirtualFree(ptr, 0, MEM_RELEASE);
}
```

`StompAllocator` 사용시 개발 단계에서 메모리 오염을 찾아낼 수 있으나 메모리 할당 크기가 커짐    
단, 메모리 오버플로우 현상은 방지할 수 없기 때문에 아래와 같이 오버플로우를 방지하는 방식으로 변경할 수 있음    

```cpp
/*------------------
	StompAllocator
-------------------*/

void* StompAllocator::Alloc(int32 size)
{
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount + PAGE_SIZE - size;
	
	void* baseAddress = ::VirtualAlloc(NULL, pageCount + PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}
```

할당하는 메모리를 끝쪽에 배치하여 오버플로우가 일어나는 경우 크래시가 나도록 유도    
단, 이 경우 언더플로우는 방지할 수 없음    

***

# STL Allocator

`vector`, `map`과 같은 STL 자료구조는 기본적으로 `new`와 `delete`를 사용    
대신 사용자 정의 `allocator`를 사용하기 위해서는 `vector<int32, BaseAllocator> v;`와 같은 형식을 사용해 인자로 넣어주면 됨    
단, 이 때 인자로 받는 `allocator`에는 요구하는 형식이 있기 때문에 이를 맞춰주어야함    

```cpp
// Allocator.h

/*------------------
	STL Allocator
-------------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xAlloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		xRelease(ptr);
	}
};
```

이후 각각의 자료구조에 대해 사용자 정의 `allocator`를 사용한 버전을 아래와 같이 각각 선언하여 편하게 사용    

```cpp
// Container.h

#include "Types.h"
#include "Allocator.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename T>
using Vector = vector<T, StlAllocator<T>>;

template<typename T>
using List =  list<T, StlAllocator<T>>;

template<typename Key, typename T, typename Pred = less<Key>>
using Map = map<Key, T, Pred, StlAllocator<pair<const Key, T>>>;

template<typename Key, typename Pred = less<Key>>
using Set = set<Key, Pred, StlAllocator<Key>>;

template<typename T>
using Deque = deque<T, StlAllocator<T>>;

template<typename T, typename Container = Deque<T>>
using Queue = queue<T, Container>;

template<typename T, typename Container = Deque<T>>
using Stack = stack<T, Container>;

template<typename T, typename Container = Vector<T>, typename Pred = less<typename Container::value_type>>
using PriorityQueue = priority_queue<T, Container, Pred>;

using String = basic_string<char, char_traits<char>, StlAllocator<char>>;
using Wstring = basic_string<wchar_t, char_traits<wchar_t>, StlAllocator<wchar_t>>;

template<typename Key, typename T, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashMap = unordered_map<Key, T, Hasher, KeyEq, StlAllocator<pair<const Key, T>>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashSet = unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;
```




