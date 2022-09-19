// stonewt.h

#ifndef XXX_H_
#define XXX_H_

class Complex
{
	private:
		double real;
		double imagine;
	public:
		Complex(double r, double i);
		Complex();
		~Complex();
		friend Complex operator+(const Complex & a, const Complex & b);
		friend Complex operator-(const Complex & a, const Complex & b);
		friend Complex operator*(const Complex & a, const Complex & b);
		friend Complex operator*(double mul, const Complex & a);
		Complex operator~();
		friend std::ostream & operator<<(std::ostream & os, const Complex & c);
		friend std::istream & operator>>(std::istream & is, Complex & c);
};

#endif