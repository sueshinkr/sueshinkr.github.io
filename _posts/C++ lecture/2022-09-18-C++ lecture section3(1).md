---
title:  "C++ lecture section3 [1/2]"
excerpt: "코드의 흐름 제어"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.18 13:00:00
---

# 분기문

```cpp
int main()
{
	if (isDead)
		cout << "몬스터를 처치했습니다." << endl;

	if (isDead)
		cout << "몬스터를 처치했습니다." << endl;
	else
	{
		if (hp <= 20)
			cout << "몬스터가 도망가고있습니다." << endl;
		else
			cout << "몬스터가 반격했습니다." << endl;
	}

	if (isDead)
		cout << "몬스터를 처치했습니다." << endl;
	else if (hp <= 20)
		cout << "몬스터가 도망가고있습니다." << endl;
	else
		cout << "몬스터가 반격했습니다." << endl;
}
```
실행문이 여러개인 경우 중괄호로 묶어서 사용    
`if`문이 중첩될 경우 `if-elseif`문으로 가독성을 향상시킬 수 있음    

```cpp
int main()
{
	const int ROCK = 0;
	const int PAPER = 1;
	const int SCISSORS = 2;

	int input = 40;

	if (input == ROCK)
		cout << "바위를 냈습니다." << endl;
	else if (input == PAPER)
		cout << "보를 냈습니다." << endl;
	else if (input == SCISSORS)
		cout << "가위를 냈습니다." << endl;
	else
		cout << "뭘 낸겁니까?" << endl;

	switch(input)
	{
		case ROCK:
			cout << "바위를 냈습니다." << endl;
			break;
		case PAPER:
			cout << "보를 냈습니다." << endl;
			break;
		case SCISSORS:
			cout << "가위를 냈습니다." << endl;
			break;
		default:
			cout << "뭘 낸겁니까?" << endl;
	}
}

조건문이 여러개인 경우 `switch-case-break`문을 사용할 수도 있음    
``` 

***

# 반복문

```cpp
int main()
{
	int count = 0;

	while (count < 5)
	{
		cout << "Hello World" << endl;
		count++;
	}

	do
	{
		cout << "Hello World" << endl;
		count++;
	} while (count < 5);
}
```
무한루프 발생에 주의    
`do-while`문은 조건식이 참이 아니더라도 최소한 한번은 실행문을 실행함    

```cpp
for (int count = 0; count < 5; count++)
{
	cout << "Hello World" << endl;
}
```
`while`과 `for`문은 기능적으로 완벽하게 호환 가능함    

```cpp
int main()
{
	int round = 1;
	int hp = 100;
	int damage = 10;

	while (true)
	{
		hp -= damage;
		if (hp < 0)
			hp = 0;
		cout << "Round " << round << " 몬스터 체력" << hp << endl;

		if (hp == 0)
		{
			cout << "몬스터 처치!" << endl;
		}

		if (round == 5)
		{
			cout << "제한 라운드 종료" << endl;
			break;
		}
	}

	for (int count = 1; count <= 10; count++)
	{
		bool isEven = ((count % 2) == 0);
		if (isEven)
			continue;
		cout << count << endl;
	}
}
```
`break`와 `continue`로 반복문의 제어가 가능함    

***

# 연습문제 (별찍기와 구구단)

```cpp
// exercise1

#include <iostream>
using namespace std;

int main()
{
	int n;

	cout << "정수 입력 : ";
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << '*';
		cout << endl;
	}
}
```

```cpp
// exercise2

#include <iostream>
using namespace std;

int main()
{
	int n;

	cout << "정수 입력 : ";
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <= i; j++)
			cout << '*';
		cout << endl;
	}
}
```

```cpp
// exercise3

#include <iostream>
using namespace std;

int main()
{
	int n;

	cout << "정수 입력 : ";
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = n; j > i; j--)
			cout << '*';
		cout << endl;
	}
}
```

***

# 가위바위보

***

# 열거형