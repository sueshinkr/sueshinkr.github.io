#pragma once
#include "Player.h"

class Archer : public Player
{
public:
	Archer(class Pet* pet);
	Archer(int hp);
	~Archer();

	virtual void AddHp(int value);

public:
	class Pet* _pet;
};

