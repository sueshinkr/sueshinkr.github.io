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