---
title:  "C++ lecture section8 [1/1]"
excerpt: "실습"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.06 14:30:00
---

# TextRPG(OOP) #1 / #2 / #3

```cpp
// Game.h

class Player; // 전방선언
class Field;

class Game
{
	public:
		Game();
		~Game();
		void Init();
		void Update();
		void CreatePlayer();
	private:
		Player* _player;
		Field* _field;
}; 
```

```cpp
// Game.cpp

#include "Game.h"
#include "Player.h"
#include "Field.h"
#include <iostream>
using namespace std;

Game::Game() : _player(nullptr), _field(nullptr)
{

}

Game::~Game()
{
	if (_player != nullptr)
		delete _player;
	if (_field != nullptr)
		delete _field;
}

void Game::Init()
{
	_field = new Field();
}

void Game::Update()
{
	if (_player == nullptr)
		CreatePlayer();
	
	if (_player->IsDead())
	{
		delete _player;
		_player = nullptr;
		CreatePlayer();
	}

	_field->Update(_player);
	
}

void Game::CreatePlayer()
{
	while (_player == nullptr)
	{
		cout << "-------------------------" << endl;
		cout << "캐릭터를 생성하세요!" << endl;
		cout << "1)기사 2)궁수 3)법사" << endl;
		cout << "-------------------------" << endl;
		cout << "> ";

		int input = 0;
		cin >> input;

		if (input == PT_Knight)
			_player = new Knight();
		else if (input == PT_Archer)
			_player = new Archer();
		else if (input == PT_Mage)
			_player = new Mage();
	}
}
```

```cpp
// Creature.h

#ifndef CREATURE_H
#define CREATURE_H

enum CreatureType
{
	CT_PLAYER = 0,
	CT_MONSTER = 1
};

class Creature
{
	public:
		Creature(int creatureType) 
			: _creatureType(creatureType), _hp(0), _att(0), _def(0) {}
		virtual ~Creature() {}
		virtual void Printinfo() = 0;
		void OnAttacked(Creature* attacker);
		bool IsDead() { return _hp <= 0; };
	protected:
		int _creatureType;
		int _hp;
		int _att;
		int _def;
};

#endif
```

```cpp
// Creature.cpp

#include "Creature.h"

void Creature::OnAttacked(Creature* attacker)
{
	int damage = attacker->_att - _def;
	if (damage < 0)
		damage = 0;
	
	_hp -= damage;
	if (_hp < 0)
		_hp = 0;
}
```

```cpp
// Player.h

#include "Creature.h"

enum PlayerType
{
	PT_Knight = 1,
	PT_Archer = 2,
	PT_Mage = 3
};

class Player : public Creature
{
	public:
		Player(int playerType) : Creature(CT_PLAYER), _playerType(playerType) {}
		virtual ~Player() {}
		void Printinfo();
	protected:
		int _playerType;
};

class Knight : public Player
{
	public:
		Knight() : Player(PT_Knight) 
		{
			_hp = 150;
			_att = 10;
			_def = 5;
		}

};

class Archer : public Player
{
	public:
		Archer() : Player(PT_Archer) 
		{
			_hp = 80;
			_att = 15;
			_def = 3;
		}
};

class Mage : public Player
{
	public:
		Mage() : Player(PT_Mage) 
		{
			_hp = 50;
			_att = 25;
			_def = 0;
		}
};
```

```cpp
// Player.cpp

#include "Player.h"
#include <iostream>
using namespace std;

void Player::Printinfo()
{
	cout << "-------------------" << endl;
	cout << "[플레이어 정보] " << "HP : " << _hp
						  << " ATT : " << _att
						  << " DEF : " << _def << endl;
	cout << "-------------------" << endl;
}
```

```cpp
// Field.h

class Player;
class Monster;

class Field
{
	public:
		Field();
		~Field();
		void Update(Player* player);
		void CreateMonster();
		void StartBattle(Player* player);
	private:
		Monster* _monster;
};
```

```cpp
// Field.cpp

#include "Field.h"
#include "Monster.h"
#include "Player.h"
#include <stdlib.h>

Field::Field() : _monster(NULL)
{
}

Field::~Field()
{
	if (_monster != NULL)
		delete _monster;
}

void Field::Update(Player* player)
{
	if (_monster == NULL)
		CreateMonster();
	StartBattle(player);
}

void Field::CreateMonster()
{
	int randValue = 1 + rand() % 3;

	switch (randValue)
	{
		case MT_SLIME:
			_monster = new Slime();
			break;
		case MT_ORC:
			_monster = new Orc();
			break;
		case MT_SKELETON:
			_monster = new Skeleton();
			break;
	}
}

void Field::StartBattle(Player* player)
{
	while (true)
	{
		player->Printinfo();
		_monster->Printinfo();

		_monster->OnAttacked(player);

		if (_monster->IsDead())
		{
			_monster->Printinfo();
			delete _monster;
			_monster = NULL;
			break;
		}

		player->OnAttacked(_monster);

		if (player->IsDead())
		{
			player->Printinfo();
			break;
		}

	}
}
```

```cpp
// Monster.h

#include "Creature.h"

enum MonsterType
{
	MT_SLIME = 1,
	MT_ORC = 2,
	MT_SKELETON = 3
};

class Monster : public Creature
{
	public:
		Monster(int monsterType) : Creature(CT_MONSTER), _monsterType(monsterType) {}
		void Printinfo();
	public:
		int _monsterType;
};

class Slime : public Monster
{
	public:
		Slime() : Monster(MT_SLIME)
		{
			_hp = 50;
			_att = 5;
			_def = 2;
		}
};

class Orc : public Monster
{
	public:
		Orc() : Monster(MT_ORC)
		{
			_hp = 80;
			_att = 8;
			_def = 3;
		}
};

class Skeleton : public Monster
{
	public:
		Skeleton() : Monster(MT_SKELETON)
		{
			_hp = 100;
			_att = 15;
			_def = 4;
		}
};
```

```cpp
// Monster.cpp

#include "Monster.h"
#include <iostream>
using namespace std;

void Monster::Printinfo()
{
	cout << "-------------------" << endl;
	cout << "[몬스터 정보] " << "HP : " << _hp
						  << " ATT : " << _att
						  << " DEF : " << _def << endl;
	cout << "-------------------" << endl;
}
```

```cpp
// main.cpp

#include <iostream>
using namespace std;
#include "Game.h"

int main()
{
	srand((unsigned int)time(nullptr));
	Game game;
	game.Init();

	while (true)
	{
		game.Update();
		
	}
	return 0;
}
```

***

# 전방선언

```cpp
// Player.h

#include "Monster.h" // 1

class Monster; // 2

class Player
{
	public:
		void KillMonster();
	public:
		int _hp;
		int _att;
		Monster _target1; // 1
		Monster* _target2; // 2
};
```

1번의 경우 `Monster`와 `Player`는 종속적인 관계가 됨    
따라서 `#include "Monster.h"`를 통해 `Monster` 클래스의 정보를 확인할 수 있어야만 `Player` 클래스가 정상적으로 작동함    

2번의 경우 `Monster`의 주소를 담을 포인터 변수를 지정한 것이므로 `Player` 클래스에서 직접적으로 `Monster` 클래스에 대한 정보를 알 필요가 없음    
단, `Monster`가 다른 클래스라는 정보가 필요하므로 `class Monster;`라는 전방선언을 해주어야함    
`class Monster* target;`과 같이 전방선언을 변수 선언과 동시에 할 수도 있음     

```cpp
// Player.cpp

#include "Player.h"
#include "Monster.h"

void Player::KillMonster()
{
	_target->_hp = 0;
}
```

`KillMonster()` 함수에서는 `Monster` 클래스의 멤버에 직접적으로 접근하므로 `Monster.h`를 인클루드 해주어야함    