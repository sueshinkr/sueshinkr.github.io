---
title:  "C++ lecture section3 [1/1]"
excerpt: "코드의 흐름 제어"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.20 13:00:00
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

```cpp
// rokc-scissors-paper

#include <iostream>
using namespace std;

void pr(int n);
void win_p(int win, int total);

int main()
{
	int you, com;
	int win = 0, total = 0;

	while(1)
	{
		srand(time(0));

		cout << "가위(1) 바위(2) 보(3) 골라주세요!\n";
		cout << "현재 승률 : ";
		win_p(win, total);

		cin >> you;
		if (!(you >= 1 && you <= 3))
		{
			cout << "잘못된 값을 입력하였습니다. 프로그램을 종료합니다.\n";
			break;
		}

		com = 1 + rand() % 3;
		if (you == com)
		{
			pr(you);
			cout << "(님) vs ";
			pr(com);
			cout << "(컴퓨터)" << " 비겼습니다.\n";
		}
		else if (you == com % 3 + 1)
		{
			pr(you);
			cout << "(님) vs ";
			pr(com);
			cout << "(컴퓨터)" << " 이겼습니다.\n";
			win++;
		}
		else
		{
			pr(you);
			cout << "(님) vs ";
			pr(com);
			cout << "(컴퓨터)" << " 졌습니다.\n";
		}
		total++;
	}
}

void pr(int n)
{
	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;
	switch(n)
	{
		case SCISSORS:
			cout << "가위";
			break;
		case ROCK:
			cout << "바위";
			break;
		case PAPER:
			cout << "보";
			break;
	}
}

void win_p(int win, int total)
{
	if (total == 0)
		cout << "없음\n";
	else
		cout << win * 100 / total << endl;
}
```

***

# 열거형

```cpp
enum
{
	ENUM_SCISSORS,
	ENUM_ROCK,
	ENUM_PAPER
};
```
열거형으로 여러 상수들을 한번에 정의할 수 있음    
`const`와는 달리 초기값 설정이 필요하지 않으며, 초기값이 지정되지 않았을 시 첫 값을 0으로 둠    
두번째 값부터는 이전 값 + 1으로 정의됨    

`const`로 상수를 정의할 경우, 경우에 따라 해당 상수들이 메모리에 저장되어 메모리 공간이 낭비될 수 있음    
열거형으로 상수를 정의한 경우 메모리 공간을 차지하지 않음    

```cpp
#define DEFINE_SCISSORS 1
#define DEFINE_TEST cout << "Hello World" << endl;
```
`#`은 전처리 지시문을 나타냄    
`#define`은 매크로를 만드는 키워드로, 선언한 매크로를 전처리 과정에서 통째로 대체함    
사용 우선순위는 가장 낮으며 권장되지 않음    
