// vintageport.cpp

#include "xxx.h"
#include <cstring>

VintagePort::VintagePort()
{
}

VintagePort::VintagePort(const char * br, int b, const char * nn, int y)
	: Port(br, "Vintage", b)
{
	nickname = new char[strlen(nn) + 1];
	strcpy(nickname, nn);
	year = y;
}

VintagePort::VintagePort(const VintagePort & vp)
	: Port(vp)
{
	nickname = new char[strlen(vp.nickname) + 1];
	strcpy(nickname, vp.nickname);
	year = vp.year;
}

VintagePort & VintagePort::operator=(const VintagePort & vp)
{
	if (this == &vp)
		return *this;

	Port::operator=(vp);
	delete [] nickname;
	nickname = new char[strlen(vp.nickname) + 1];
	strcpy(nickname, vp.nickname);
	year = vp.year;
	return *this;
}

void VintagePort::Show() const
{
	Port::Show();
	cout << "별명 : " << nickname << endl;
	cout << "년도 : " << year << endl;
}

ostream & operator<<(ostream & os, const VintagePort & vp)
{
	os << (const Port &)vp;
	os << ", " << vp.nickname << ", " << vp.year << endl;
}
