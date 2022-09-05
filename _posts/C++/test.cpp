// arfupt.cpp

#include <iostream>
using namespace std;

double calculate(double x, double y, double (*pt)(double, double));
double add(double x, double y);
double sub(double x, double y);
double mul(double x, double y);
double div(double x, double y);

int main()
{
	double (*pf[4])(double, double) = {add, sub, mul, div};
	double x, y;
	cout << "두 수를 입력하세요 : \n";
	while (cin >> x >> y)
	{
		cout << "add : " << calculate(x, y, pf[0]) << endl;
		cout << "sub : " << calculate(x, y, pf[1]) << endl;
		cout << "mul : " << calculate(x, y, pf[2]) << endl;
		cout << "div : " << calculate(x, y, pf[3]) << endl;
		cout << "두 수를 입력하세요 : \n";
	}
	
	return 0;
}

double calculate(double x, double y, double (*pt)(double, double))
{
	return pt(x, y);
}

double add(double x, double y)
{
	return x + y;
}

double sub(double x, double y)
{
	return x - y;
}

double mul(double x, double y)
{
	return x * y;
}

double div(double x, double y)
{
	return x / y;
}