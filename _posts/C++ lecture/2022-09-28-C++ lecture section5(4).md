---
title:  "C++ lecture section5 [4/4]"
excerpt: "포인터"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.28 13:00:00
---

# TextRPG #3

```cpp
// TextRPG3.cpp

#include <iostream>
using namespace std;

enum PlayerType
{
	PT_Knight = 1,
	PT_Archer = 2,
	PT_Mage = 3
};

enum MonsterType
{
	MT_Slime = 1,
	MT_Orc = 2,
	MT_Skeleton = 3
};

struct StatInfo
{
	int hp = 0;
	int att = 0;
	int def = 0;
};

void PrintMessage(const char* msg);
void EnterLobby();
void CreatePlayer(StatInfo* playerinfo);
void PrintStatInfo(const char* name, const StatInfo& info);
void Entergame(StatInfo* playerinfo);
void CreateMonsters(StatInfo monsterinfo[], int count);
bool EnterBattle(StatInfo* playerinfo, StatInfo* monsterinfo);

int main()
{
	srand((unsigned)time(nullptr));
	EnterLobby();
	return 0;
}

void PrintMessage(const char* msg)
{
	cout << "********************" << endl;
	cout << msg << endl;
	cout << "********************" << endl;	
}

void EnterLobby()
{
	while (true)
	{
		PrintMessage("로비에 입장했습니다.");
		StatInfo playerinfo;
		CreatePlayer(&playerinfo);
		PrintStatInfo("Player", playerinfo);
		Entergame(&playerinfo);
	}
}

void CreatePlayer(StatInfo* playerinfo)
{
	bool ready = false;

	while (ready == false)
	{
		PrintMessage("캐릭터 생성창");
		PrintMessage("[1]기사 [2]궁수 [3]법사");
		cout << "> ";

		int input;
		cin >> input;

		switch(input)
		{
			case PT_Knight:
				playerinfo->hp = 100;
				playerinfo->att = 10;
				playerinfo->def = 5;
				ready = true;
				break;
			case PT_Archer:
				playerinfo->hp = 80;
				playerinfo->att = 15;
				playerinfo->def = 3;
				ready = true;
				break;
			case PT_Mage:
				playerinfo->hp = 50;
				 playerinfo->att = 25;
				playerinfo->def = 1;
				ready = true;
				break;
		}
	}
}

void PrintStatInfo(const char* name, const StatInfo& info)
{
	cout << "********************" << endl;
	cout << name << " : HP = " << info.hp
				 << " ATT = " << info.att
				 << " DEF = " << info.def << endl;
	cout << "********************" << endl;
}

void Entergame(StatInfo* playerinfo)
{
	const int MONSTER_COUNT = 2;
	PrintMessage("게임에 입장했습니다.");

	while (true)
	{
		StatInfo monsterinfo[MONSTER_COUNT];
		CreateMonsters(monsterinfo, MONSTER_COUNT);

		for (int i = 0; i < MONSTER_COUNT; i++)
			PrintStatInfo("Monster", monsterinfo[i]);
		
		PrintStatInfo("Player", *playerinfo);
		PrintMessage("[1]전투 [2]전투 [3]도망");

		int input;
		cin >> input;

		if (input == 1 || input == 2)
		{
			int index = input - 1;
			bool victory = EnterBattle(playerinfo, &monsterinfo[index]);
			if (victory == false)
				break;
		}
	}
}

void CreateMonsters(StatInfo monsterinfo[], int count)
{
	for (int i = 0; i < count; i++)
	{
		int randValue = 1 + rand() % 3;
		
		switch(randValue)
		{
			case MT_Slime:
				monsterinfo[i].hp = 30;
				monsterinfo[i].att = 5;
				monsterinfo[i].def = 1;
				break;
			case MT_Orc:
				monsterinfo[i].hp = 40;
				monsterinfo[i].att = 8;
				monsterinfo[i].def = 2;
				break;
			case MT_Skeleton:
				monsterinfo[i].hp = 50;
				monsterinfo[i].att = 15;
				monsterinfo[i].def = 3;
				break;
		}
	}
}

bool EnterBattle(StatInfo* playerinfo, StatInfo* monsterinfo)
{
	while (true)
	{
		int damage = playerinfo->att - monsterinfo->def;
		if (damage < 0)
			damage = 0;
		monsterinfo->hp -= damage;
		if (monsterinfo->hp < 0)
			monsterinfo->hp = 0;
		PrintStatInfo("Monster", *monsterinfo);
		if (monsterinfo->hp == 0)
		{
			PrintMessage("몬스터를 처치했습니다.");
			return true;
		}

		damage = monsterinfo->att - playerinfo->def;
		if (damage < 0)
			damage = 0;
		playerinfo->hp -= damage;
		if (playerinfo->hp < 0)
			playerinfo->hp = 0;
		PrintStatInfo("Player", *playerinfo);
		if (playerinfo->hp == 0)
		{
			PrintMessage("Game Over");
			return false; 
		}
	}
}
```

***

# 연습 문제 (문자열) #1

```cpp
// strPractice1.cpp

#include <iostream>
using namespace std;

int StrLen(const char* str)
{
	int count;

	for (count = 0; str[count]; count++)
		;

	return count;
}

char* StrCpy(char* dest, const char* src)
{
	int i;

	for (i = 0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = '\0';

	return dest;
}

int main()
{
	const int BUF_SIZE = 100;
	char a[BUF_SIZE] = "Hello";
	char b[BUF_SIZE];
	char c[BUF_SIZE];

	cout << "strlen : " << strlen(a) << endl;
	cout << "StrLen : " << StrLen(a) << endl;

	strcpy(b, a);
	cout << "strcpy : " << b << endl;
	StrCpy(c, a);
	cout << "StrCpy : " << c << endl;
	return 0;
}
```

***

# 연습 문제 (문자열) #2

```cpp
// strPractice2.cpp

#include <iostream>
using namespace std;

char* StrCat(char* dest, const char* src)
{
	char* ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';

	return ret;
}

int StrCmp(const char* a, const char* b)
{
	int i = 0;
	while (a[i] && b[i])
	{
		if (a[i] > b[i] || a[i] < b[i])
			return a[i] - b[i];
		i++;
	}
	return 0;
}

void ReverseStr(char* str)
{
	int len = strlen(str) - 1;
	int i = 0;
	char temp;

	while(i <= len / 2)
	{
		temp = str[len - i];
		str[len - i] = str[i];
		str[i] = temp;
		i++;
	}
}

int main()
{
	const int BUF_SIZE = 100;
	char a[BUF_SIZE] = "Hello";
	char b[BUF_SIZE] = "Goodd";
	char c[BUF_SIZE] = "Goodd";

	cout << "strcat : " << strcat(b, a) << endl;
	cout << "StrCat : " << StrCat(c, a) << endl;

	cout << "strcmp : " << strcmp(b, c) << endl;
	cout << "StrCmp : " << StrCmp(b, c) << endl;

	ReverseStr(b);
	cout << "rev : " << b << endl;
	return 0;
}
```

***

# 연습 문제 (달팽이)

```cpp
// snail.cpp

#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 100;
int arr[MAX][MAX] = {0, };

void print_snail(int num)
{
	cout << "****************\n";
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			//if (arr[i][j] < 10)
			//	cout << "0";
			//cout << arr[i][j] << " ";
			cout << setfill('0') << setw(2) << arr[i][j] << " ";
		}
		cout << endl;
	}
	cout << "****************\n";
}

void make_snail(int num)
{
	int i = 1, row = 0, col = 0;
	enum {RIGHT, DOWN, LEFT, UP};
	int dir = RIGHT;

	while (i <= num * num)
	{
		arr[row][col] = i;
		if (dir == RIGHT)
		{
			if (col == num - 1 || arr[row][col + 1] != 0)
			{
				dir = DOWN;
				row++;
			}
			else
				col++;
		}
		else if (dir == DOWN)
		{
			if (row == num - 1 || arr[row + 1][col] != 0)
			{
				dir = LEFT;
				col--;
			}
			else
				row++;
		}
		else if (dir == LEFT)
		{
			if (col == 0 || arr[row][col - 1] != 0)
			{
				dir = UP;
				row--;
			}
			else
				col--;
		}
		else if (dir == UP)
		{
			if (row == 0 || arr[row - 1][col] != 0)
			{
				dir = RIGHT;
				col++;
			}
			else
				row--;
		}
		i++;
	}
}

int main()
{
	int num;
	cin >> num;

	make_snail(num);
	print_snail(num);
}
```

***

# 파일 분할 관리

헤더 파일과 소스 코드 파일을 분리하여 작업하는 것이 효율이 높음    
헤더 파일에는 함수의 선언, 소스 코드 파일에는 함수의 정의를 구현    
소스 코드 파일에는 `#include`를 이용하여 헤더 파일을 삽입함    
헤더 파일에 구현부를 작성한 경우 함수의 중복에 주의해야함    
헤더 파일은 최대한 간단하게 작성하는 것이 권장됨    

`#pragma once`를 사용하면 중복 인클루드를 방지할 수 있음    
공식 방법으로는 `#ifndef #define #endif`를 사용함    
