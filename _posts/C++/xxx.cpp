// stonewt.cpp

#include <iostream>
using std::cout;
#include "xxx.h"

Complex::Complex(double r, double i)
{
	real = r;
	imagine = i;
}

Complex::Complex()
{
	real = 0.0;
	imagine = 0.0;
}

Complex::~Complex()
{
}

Complex operator+(const Complex & a, const Complex & b)
{
	return Complex(a.real + b.real, a.imagine + b.imagine);
}

Complex operator-(const Complex & a, const Complex & b)
{
	return Complex(a.real - b.real, a.imagine - b.imagine);
}

Complex operator*(const Complex & a, const Complex & b)
{
	return Complex(a.real * b.real - a.imagine * b.imagine, 
		a.real * b.imagine + a.imagine * b.real);
}

Complex operator*(double mul, const Complex & a)
{
	return Complex(mul * a.real, mul * a.imagine);
}

Complex Complex::operator~()
{
	return Complex(real, -imagine);
}

std::ostream & operator<<(std::ostream & os, const Complex & c)
{
	os << "(" << c.real << ", " << c.imagine << "i)";
	return os;
}

std::istream & operator>>(std::istream & is, Complex & c)
{
	std::cout << "실수부를 입력해주세요 : ";
	if (!(is >> c.real))
		return is;
	std::cout << "허수부를 입력해주세요 : ";
	is >> c.imagine;
	return is;
}