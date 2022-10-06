---
title:  "C++ lecture section10 [1/2]"
excerpt: "콜백 함수"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.10 18:00:00
---

# 함수 포인터 #1

`int* pointer = &a`
* `int` : 데이터 타입
* `pointer` : 변수 이름

```cpp
int Add(int a, int b)
{
	return a + b;
}

int main()
{
	typedef int(FUNC_TYPE)(int a, int b);
	using FUNC_TYPE = int(int a, int b); // modern C++

	FUNC_TYPE* fn = ADD;
	int result = fn(1, 2);

	int result2 = (*fn)(1, 2);
}
```

`FUNC_TYPE* fn = ADD()`
* `FUNC_TYPE` : 데이터 타입
* `fn` : 변수 이름
* `Add` : 주소를 저장할 대상 함수

함수의 이름은 함수의 시작 주소를 가짐(배열과 유사)    
함수 포인터에는 접근 연산자 `*`를 붙여도 함수 주소를 가리킴    

```cpp
class Item
{
	public:
		Item() : _itemid(0), _rarity(0), _ownerid(0) {}
	public:
		int _itemid;
		int _rarity;
		int _ownerid;
}

Item* FindItem(Item items[], int itemCount, bool(*selector)(Item* item))
{
	for (int i = 0; i < itemCount; i++)
	{
		Item* item = &items[i];
		if (selector(item))
			return item;
	}

	return nullptr;
}

bool IsRareItem(Item* item)
{
	return item->_rerity >= 2;
}

int main()
{
	Item item[10];
	items[3]._rarity - 2;
	Item* rareItem = FIndItem(items, 10, isRareItem);
}
```

***

# 함수 포인터 #2

`typedef`는 선언 문법의 앞에 붙이는 형식으로 사용    
보통은 `int (*PFUNC)(int, int);`와 같은 형식으로 함수 포인터 시그니처를 만들어 사용함    

```cpp
class Knight
{
	public:
		int GetHP(int, int)
		{
			...
		}
	public:
		int _hp = 100;
}

typedef int(Knight::*PMEMFUNC)(int, int);


int main()
{
	Knight k1;
	Knight* k2 = new KNight();
	PMEMFUNC mfn;

	mfn = &Knight::GetHP;
	(k1.*mfn)(1, 2);

	(k2->*mfn)(1, 2); 
	delete k2;

	return 0;
}
```

멤버 함수는 특정 객체를 기반으로 호출됨    
함수 포인터에 함수의 주소를 대입할시 `&`는 생략이 가능하나, 붙이는 것이 권장됨    
특히 멤버 함수를 함수 포인터에 대입할 때는 반드시 `&`를 붙어야함    
멤버 함수를 함수 포인터로 사용시 객체가 필요하며, `(k1.*mfn)(1, 2);`와 같은 형식으로 사용하여 해당 객체의 멤버가 아닌 함수 포인터라는 것을 지정해야함    

***

# 함수 객체

함수 포인터 단점
* 함수 시그니처가 맞지 않으면 사용할 수 없음
* 상태를 가질 수 없음

함수 객체(Functor) : 함수처럼 동작하는 객체

```cpp
class Functor
{
	public:
		void operator()()
		{
			cout << "Functor Test" << endl;
			cout << _value << endl;
		}
		boll operator()(int num)
		{
			cout << "Functor Test" << endl;
			_value += num;
			cout << _value << endl;
			return true;
		}
	private:
		int _value = 0;
}

int main()
{
	Functor funcotr;

	functor();
	bool ret = functor(3);

	return 0;
}
```

함수 객체를 생성하는 시점과 실행하는 시점을 분리할 수 있음

```cpp
class MoveTask
{
	public:
		void operator()()
		{
			cout << "해당 좌표로 플레이어 이동" << endl;
		}
	public:
		int _playerid;
		int _posX;
		int _posY;
}

int main()
{
	MoveTask task;
	task._plyaerid = 100;
	task._posX = 5;
	task._posY = 0;
	// 이동 요청을 받아서 함수 객체에 저장

	// 요청을 곧바로 실행하지 않고, 얼마든지 나중에 task를 실행할 수 있음
	task();
}
```