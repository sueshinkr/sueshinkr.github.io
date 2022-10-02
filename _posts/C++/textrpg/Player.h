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