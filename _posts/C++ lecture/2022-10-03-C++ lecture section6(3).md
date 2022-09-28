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