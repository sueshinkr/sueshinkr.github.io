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