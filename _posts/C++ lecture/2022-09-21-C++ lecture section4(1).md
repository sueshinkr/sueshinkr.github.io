---
title:  "C++ lecture section4 [1/1]"
excerpt: "함수"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.21 13:00:00
---

# 함수 기초

```cpp
반환타입 함수이름(인자타입 매개변수)
{
	함수 내용
	return ---;
}

void PrintHelloWorld()
{
	cout << "Hello World" << endl;
}
```
input으로 무엇을 받고, output으로 무엇을 반환할지 정해줘야함    
입력 혹은 출력이 없는 경우 타입을 `void`형으로 지정    
`return`을 만나는 순간 함수를 완전히 빠져나오게됨    

```cpp
void PrintNumber(int n)
{
	cout << "넘겨주신 숫자는 " << n << "입니다" << endl;
}

int MultiplyBy2(int a)
{
	return a * 2;
}

int MultiplyBy(int a, int b)
{
	return a * b;
}
```
매개변수는 해당 함수 내에서만 유효함    
매개변수는 여러개 있을 수 있음    

***

# 스택 프레임

디버깅시 프로시저 단위 실행과 단계별 코드 실행을 선택할 수 있음    

스택은 높은 메모리 주소에서 낮은 메모리 주소 순서로 채워짐    
함수 호출시 매개변수, 반환 주소값, 지역변수가 메모리에 올라감    
스택은 결국 함수들끼리 돌려 사용하는 메모장같은 느낌이라고 볼 수 있음    

***

# 지역 변수와 값 전달

전역 변수는 데이터 영역, 지역 변수는 스택 영역에 속함    
지역 변수는 함수 내에 정의하며, 해당 함수 내에서만 사용할 수 있음    

전역 변수는 관리가 어렵기 때문에 사용을 지양해야함    

```cpp
void increase(int hp)
{
	hp = hp + 1;
}

int main()
{
	int hp = 1;
	cout << "increase 호출 전 : " << hp << endl;
	increase(hp);
	cout << "increase 호출 후 : " << hp << endl;
	
	return 0;
}
```

매개변수로 전달된 지역변수는 복사본으로 사용되어 해당 함수 내에서만 변경됨    
이를 값에 의한 전달이라고 함    

***

# 호출 스택

```cpp
void Func1();
void Func2(int a, int b);
void Func3(float a);

void Func1()
{
	cout << "Func1" << endl;
	Func2(1, 2);
}

void Func2(int a, int b)
{
	cout << "Func1" << endl;
	Func3(10);
}

void Func3(float a)
{
	cout << "Func3" << endl;
}

int main()
{
	cout << "main" << endl;
	Func1();
	return 0;
}
```

함수를 정의 순서에 상관없이 사용하기 위해서는 함수 선언이 필요함    
함수 선언시 매개변수에는 이름을 붙이지 않아도 되고, 정의에서의 이름과 달라도 됨    
함수가 어떤 경로로 호출되었는지를 호출 스택에서 확인할 수 있음    

***

# 함수 마무리

```cpp
int Add(int a, int b)
{
	return a + b;
}

int Add(float a, float b)
{
	return a + b;
}

int main()
{
	int result = Add(1.5f, 2.1f);
	return 0;
}
```

오버로딩 : 함수 이름의 재사용    
함수의 매개변수 개수 혹은 타입이 다른 경우(순서가 다른 경우도 포함) 동일한 이름의 함수를 중복 정의할 수 있음    
단, 반환형식만 다른 경우에는 오버로딩할 수 없음    

```cpp
void SetPlayerinfo(int hp, int mp, int attack, int guildid = 0)
{
	...
}

int main()
{
	SetPlayerinfo(100, 40, 10);
	SetPlayerinfo(111, 40, 10, 1);
	SetPlayerinfo(120, 40, 10);
	...
}
```

매개변수의 기본값 설정시 함수를 호출할 때 해당 인수를 지정하지 않아면 기본값으로 설정됨    
단, 기본값을 설정할 매개변수는 반드시 끝쪽에 있어야 함    

```cpp
int Factorial(int n)
{
	if (n <= 1)
		return 1;
	return n * Factorial(n - 1);
}

int main()
{
	int result = Factorial(1000000);
	cout << result << endl;
}
```

함수가 지나치게 호출될경우 스택 오버플로우가 발생할 수 있음    

***

# TextRPG

```cpp
// textRPG

#include <iostream>
using namespace std;

struct ObjectINfo
{
	int type;
	int hp;
	int att;
	int def;
};

ObjectINfo player;
ObjectINfo monster;

void select_job();
void select_do();
void select_mon();
void select_battle();

int main()
{
	srand(time(0)); 
	while (true)
	{
		cout << "---------------\n";
		cout << "로비에 입장했습니다!\n";
		cout << "---------------\n";
		
		select_job();

		select_do();
	}
	return 0;
}

void select_job()
{
	const int KNIGHT = 1;
	const int ARCHER = 2;
	const int MAGE = 3;

	while (true)
	{
		cout << "직업을 골라주세요!\n";
		cout << "(1) 기사 (2) 궁수 (3) 법사\n";
		cout << "> ";

		cin >> player.type;

		switch(player.type)
		{
			case KNIGHT:
			{
				cout << "기사 생성중... !\n";
				player.hp = 150;
				player.att = 10;
				player.def = 5;
				return ;
			}
			case ARCHER:
			{
				cout << "궁수 생성중... !\n";
				player.hp = 100;
				player.att = 15;
				player.def = 3;
				return ;
			}
			case MAGE:
			{
				cout << "법사 생성중... !\n";
				player.hp = 80;
				player.att = 25;
				player.def = 0;
				return ;
			}
		}
	}
}

void select_do()
{
	cout << "---------------\n";
	cout << "(1) 필드 입장 (2) 게임 종료 \n";
	cout << "> ";

	int input;
	cin >> input;

	if (input == 1)
	{
		while (true)
		{
			cout << "---------------\n";
			cout << "필드에 입장했습니다!\n";
			cout << "---------------\n";
			cout << "[PLAYER] HP : " << player.hp << " / ATT : " << player.att << " / DEF : " << player.def << endl;

			select_mon();
			select_battle();
			if (player.hp == 0)
				return;
		}
	}
	else
		return;
}

void select_mon()
{
	enum MonsterType
	{
		SKELETON = 0,
		SLIME = 1,
		ORC = 2
	};

	monster.type = rand() % 3;

	switch(monster.type)
	{
		case SKELETON:
		{
			cout << "해골 생성중...! (HP : 80 / ATT : 15 / DEF : 5)\n";
			monster.hp = 80;
			monster.att = 15;
			monster.def = 5;
			break;
		}
		case SLIME:
		{
			cout << "슬라임 생성중...! (HP : 15 / ATT : 5 / DEF : 0)\n";
			monster.hp = 15;
			monster.att = 5;
			monster.def = 0;
			break;
		}
		case ORC:
		{
			cout << "오크 생성중...! (HP : 40 / ATT : 10 / DEF : 3)\n";
			monster.hp = 40;
			monster.att = 10;
			monster.def = 5;
		}
	}
}

void select_battle()
{
	cout << "---------------\n";
	cout << "(1) 전투 (2) 도주\n";
	cout << "> ";

	int input;
	cin >> input;
	if (input == 1)
	{
		while (monster.hp > 0 && player.hp > 0)
		{
			int damage = player.att - monster.def;
			if (damage < 0)
				damage = 0;
			monster.hp = monster.hp - damage;
			if (monster.hp < 0)
				monster.hp = 0;
			cout << "몬스터 남은 체력 : " << monster.hp << endl;
			if (monster.hp == 0)
			{
				cout << "몬스터를 처치했습니다!" << endl;
				return ;
			}

			damage = monster.att - player.def;
			if (damage < 0)
				damage = 0;
			player.hp = player.hp - damage;
			if (player.hp < 0)
				player.hp = 0;
			cout << "플레이어 남은 체력 : " << player.hp << endl;
			if (player.hp == 0)
			{
				cout << "당신은 사망했습니다... GAME OVER" << endl;
				return ;
			}
		}
	}
	else
		return ;
}
```

구조체가 유용하게 쓰임    
구조체의 멤버들이 단일 타입이 아닐 경우 패딩으로 인해 크기가 조정됨    	