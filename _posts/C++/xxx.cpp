// port.cpp

#include "xxx.h"
#include <cstring>

Port::Port(const char * br, const char * st, int b)
{
	brand = new char[strlen(br) + 1];
	strcpy(brand, br);
	strcpy(style, st);
	bottles = b;
}

Port::Port(const Port & p)
{
	brand = new char[strlen(p.brand) + 1];
	strcpy(brand, p.brand);
	strcpy(style, p.style);
	bottles = p.bottles;
}

Port & Port::operator=(const Port & p)
{
	delete [] brand;
	brand = new char[strlen(p.brand) + 1];
	strcpy(brand, p.brand);
	strcpy(style, p.style);
	bottles = p.bottles;
}

Port & Port::operator+=(int b)
{
	bottles += b;
}

Port & Port::operator-=(int b)
{
	bottles -= b;
}

void Port::Show() const
{
	cout << "브랜드 : " << brand << endl;
	cout << "스타일 : " << style << endl;
	cout << "수량 : " << bottles << endl;
}

ostream & operator<<(ostream & os, const Port & p)
{
	os << p.brand << ", " << p.style << ", " << p.bottles;
}
