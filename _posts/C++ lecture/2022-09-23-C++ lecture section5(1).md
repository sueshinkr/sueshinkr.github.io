---
title:  "C++ lecture section5 [1/4]"
excerpt: "포인터"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.23 13:00:00
---

# 포인터 기초 #1

일반 변수는 함수 호출시 원본 대신 복사본이 사용됨    
따라서 원본 변수를 조작할 수 있는 포인터가 필요함    

`TYPE* 변수명`의 형태로 사용    
* 포인터 변수에는 값이 아닌 주소값이 저장됨    
* 포인터 변수의 크기는 항상 4바이트(32비트) 또는 8바이트(64비트)로 고정
* 일반 변수의 주소값은 변수 앞에 `&`연산자를 사용함    
* `int* ptr = &number;` : `ptr` 포인터 변수에 `number` 변수의 주소를 대입

`*포인터변수`의 형태로 사용할 경우 해당 포인터 변수가 가리키는 주소에 저장된 값을 지칭함
* `int value = *ptr;` : `value` 변수에 `ptr` 포인터 변수가 가리키는 주소에 저장된 값을 대입
* `*ptr = 2`: `ptr` 포인터 변수가 가리키는 주소에 저장된 값을 2로 변경

***

# 포인터 기초 #2

포인터의 타입을 지정하는 것은 해당 포인터가 가리키는 주소에 어떤 형식의 데이터가 있는지를 나타내기 위함    
해당 데이터가 어떤 타입으로 인식되는지에 따라 컴파일러의 작동이 달라질 수 있음    

포인터 타입과 데이터 타입이 불일치할경우 에러 발생    
강제 형변환 등으로 메모리 영역을 침범할시 상당한 문제가 발생할 수 있음    

```cpp
void SetHP(int *hp)
{
	*hp = 100;
}

int main()
{
	int hp = 1;
	SetHP(&hp);
}
```

***

# 포인터 

주소 연산자 `&`
* 해당 변수의 주소를 나타내는 연산자
* `int* pointer = &number;`에서 `&number`는 `number` 변수의 주소값을 의미
* 해당 변수 타입에 따라서 `TYPE*`형 반환

산술 연산자 `+`, `-`
* 포인터 변수도 덧셈, 뺄셈 연산이 가능
* 덧셈, 뺄셈 연산시 해당 포인터 변수의 자료형 크기만큼 움직임
* 즉, `int`형이라면 4바이트, `char`형이면 1바이트 ...

간접 연산자 `*`
* 포인터 변수가 가리키는 주소에 저장되어있는 값에 접근
* `*pointer = 3;`

간접 멤버 연산자 `->`
* 구조체의 특정 멤버를 다룰 때 사용
* `*`와 `.`을 결합하여 한번에 사용하는 연산자
* 어셈블리 언어로 동작을 살펴보면 사실상 구조체 멤버의 자료형 크기만큼 메모리를 이동하는 덧셈연산
```cpp
struct Player
{
	int hp;
	int damage;
}

int main()
{
	Player player;
	player.hp = 100;
	player.damage = 10;
	
	Player* playerPtr = &player;
	(*playerPtr).hp = 150;
	(*playerPtr).damage = 15;
	playerPtr->hp = 200;
	playerPtr->damage = 20;
}
```

***

# 포인터 실습

```cpp
// ptr_exercise

#include <iostream>
using namespace std;

struct StatInfo
{
	int hp;
	int att;
	int def;
};

void EnterLobby();
StatInfo CreatePlayer();
void CreateMonster(StatInfo* info);
bool StartBattle(StatInfo* player, StatInfo* monster);

int main()
{
	EnterLobby();
	return 0;
}

void EnterLobby()
{
	cout << "로비에 입장했습니다." << endl;
	
	StatInfo player;
	player = CreatePlayer();

	StatInfo monster;
	CreateMonster(&monster);

	bool victory = StartBattle(&player, &monster);
	if (victory)
		cout << "승리!" << endl;
	else
		cout << "패배!" << endl;
}

StatInfo CreatePlayer()
{
	StatInfo pl;

	cout << "플레이어 생성" << endl;
	pl.hp = 100;
	pl.att = 10;
	pl.def = 2;

	return pl;
}

void CreateMonster(StatInfo* info)
{
	cout << "몬스터 생성" << endl;
	info->hp = 40;
	info->att = 8;
	info->def = 1;
}

bool StartBattle(StatInfo* player, StatInfo* monster)
{
	while (true)
	{
		int damage = player->att - monster->def;
		if (damage < 0)
			damage = 0;
		
		monster->hp -= damage;
		if (monster->hp < 0)
			monster->hp = 0;
		
		cout << "몬스터 HP : " << monster->hp << endl;
		if (monster->hp == 0)
			return true;

		damage = monster->att - player->def;
		if (damage < 0)
			damage = 0;
		
		player->hp -= damage;
		if (player->hp < 0)
			player->hp = 0;

		cout << "플레이어 HP : " << player->hp << endl;
		if (player->hp == 0)
			return false;
	}
}
```
 
값에 의한 전달보다 포인터를 사용한 주소에 의한 전달이 훨씬 효율적으로 전달함
* 값에 의한 전달은 임시 구조체를 생성한 후 값을 대입하고, 이를 변경하고자하는 구조체에 복사함으로써 변경
* 주소에 의한 전달은 변경하고자하는 구조체의 값을 곧바로 변경

