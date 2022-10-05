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