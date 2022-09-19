// usecomplex.cpp

#include <iostream>
using namespace std;
#include "xxx.h"

int main()
{
	Complex a(3.0, 4.0);
	Complex c;
	cout << "하나의 복소수를 입력하십시오 (끝내려면 q) : \n";
	while (cin >> c)
	{
		cout << "c = " << c << '\n';
		cout << "공액복소수 = " << ~c << '\n';
		cout << "a = " << a << '\n';
		cout << "a + c = " << a + c << '\n';
		cout << "a - c = " << a - c << '\n';
		cout << "a * c = " << a * c << '\n';
		cout << "2 * c = " << 2 * c << '\n';
		cout << "하나의 복소수를 입력하십시오 (끝내려면 q) : \n";
	}
	cout << "프로그램을 종료합니다.\n";
		return 0;
}