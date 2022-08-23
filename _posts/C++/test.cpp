// hexoct2.cpp

#include <iostream>

int main()
{
	using namespace std;
	int chest = 42;
	int waist = 42;
	int inseam = 42;

	cout << "손님 몸매는 한마디로 끝내줍니다!\n";
	cout << "가슴둘레 " << chest << " (10진수)\n";
	cout << hex;
	cout << "허리둘레 " << waist << " (16진수)\n";
	cout << oct;
	cout << "인심길이 " << inseam << " (8진수)\n";
	return 0;
}
