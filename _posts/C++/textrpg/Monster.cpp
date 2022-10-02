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