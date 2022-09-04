// arfupt.cpp

#include <iostream>
long double factorial(long double x);

int main()
{
	using namespace std;

	cout << "수를 입력하세요 (음수 입력시 종료) : ";
	long double num;
	while (cin >> num && num > 0)
	{
		cout << num << "! = " << factorial(num) << endl;
		cout << "수를 입력하세요 (음수 입력시 종료) : ";
	}
	cout << "프로그램을 종료합니다.\n";
	return 0;
}

long double factorial(long double x)
{
	if (x == 0)
		return 1;
	return x * (factorial(x - 1));
}

