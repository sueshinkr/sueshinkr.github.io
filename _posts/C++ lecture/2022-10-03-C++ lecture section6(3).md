---
title:  "C++ lecture section6 [3/3]"
excerpt: "객체지향 여행"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.03 11:00:00
---

# 연산자 오버로딩 #1

```cpp
class Position
{
	public:
		Position operator+(const Position& arg)
		{
			Position pos;
			pos._x = _x + arg._x;
			pos._y = _y + arg._y;
			return pos;
		}
		
	public:
		int _x;
		int _y;
};

Position operator+(int arg, const Position& src)
{
	Position pos;
	pos._x = src._x + arg;
	pos._y = src._y + arg;
	return pos;
}

int main()
{
	Position pos;
	pos._x = 0;
	pos._y = 0;

	Position pos2;
	pos2._x = 1;
	pos2._y = 1;

	Position pos3 = pos + pos2;
	Position pos4 = 1 + pos3;
}
```

연산자는 피연산자의 개수/타입이 고정되어있음    
연산자 오버로딩을 이용하여 연산자가 사용자가 정의한 타입의 피연산자를 처리하도록 만들 수 있음    
* 연산자 오버로딩을 위해선 연산자 함수를 정의해야함
* 연산자 함수는 멤버 함수와 전역 함수 두가지 방법으로 만들 수 있음
	* 멤버 함수로 사용시 `a op b` 형태로 사용, 왼쪽을 기준으로 실행되며 `a`는 기준 피연산자로 반드시 클래스여야 함
	* 전역 함수로 사용시 `a op b`에서 `a`와 `b` 모두를 피연산자로 간주함    
	* 단, 대입 연산자같은 경우 전역 함수로 사용할 수 없음

***

# 연산자 오버로딩 #2

```cpp
Position& operator=(int arg)
{
	_x = arg;
	_y = arg;
	return *this;
}
```
위와 같은 형식으로 객체가 자기 자신을 반환하도록 대입 연산자를 오버로딩할 수 있음    

```cpp
Position& operator=(Position& arg)
{
	_x = arg._x;
	_y = arg._y;
	return *this;
}
```
복사 대입 연산자란 위와같이 대입 연산자 중 자기 자신의 참조 타입을 인자로 받는 것을 뜻함    
객체가 복사되길 원하는 특성 때문에 사용됨    

연산자 중에는 `::`, `.`, `.*` 등 오버로딩할 수 없는 것들도 존재함    
`++`, `--`는 전위형(++a)의 경우 `operator++()`, 후위형(a++)의 경우 `operator++(int)`로 사용함    
* 전위형은 중첩하여 사용할 수 있으나 후위형은 중첩하여 사용할 수 없음
* 따라서 전위형은 자기 자신을 리턴하도록 하는 것이 유용함


***

# 객체지향 마무리

C++에서 `struct`와 `class`는 거의 차이가 없음    
`struct`는 기본 접근 지정자가 `public`, `class`는 `private`임    
일반적으로 `struct`는 구조체(데이터 묶음)을 표현하는 용도로만 사용    
`class`는 객체 지향 프로그래밍의 특징을 나타내는 용도로 사용    

```cpp
class Marine
{
	public:
		int _hp;
		int _att;
}
int main()
{
	Marine m1;
	m1._hp = 40;
	m1._att = 6;

	Marine m2;
	m2._hp = 40;
	m2._att = 6;

	m1._att = 7;
	m2._att = 7;
}
```
위와 같은 경우 마린의 공격력 업그레이드를 한 경우 모든 마린 객체를 찾아 공격력을 올려주어야 함    

```cpp
class Marine
{
	public:
		int _hp;
		static int s_att;
		static void Setattack()
		{
			s_att += 1;
		}
};

int Marine::s_att = 0;

int main()
{
	Marine::s_attack = 6;

	Marine m1;
	m1._hp = 35;
	
	Marine m2;
	m2._hp = 14;

	Marine::Setattack();
}
```
이런 불편함을 해소하기 위해 위와 같이 `static` 변수를 사용할 수 있음    
`static`으로 선언된 변수는 특정 개체의 변화에 무관하며, 클래스 자체와 연관되어있음    
함수도 `static`으로 지정하는 것이 가능하지만, `static` 함수 내에서는 `static` 변수만 조작할 수 있음    
`static` 변수는 데이터 영역에 저장됨    
* 프로그램의 시작시 메모리에 올려지며, 종료시 해제됨
* 단, 해당 변수가 선언된 함수 내에서만 사용할 수 있음