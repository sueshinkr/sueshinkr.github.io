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