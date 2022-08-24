// typecast.cpp

#include <iostream>

int main()
{
	using namespace std;
	int auks, bats, coots;

	auks = 19.99 + 11.99;

	bats = (int) 19.00 + (int) 11.99;
	coots = int (19.99) + int (11.99);
	cout << "auks = " << auks << ", bats = " << bats;
	cout << ", coots = " << coots << endl;

	char ch = 'Z';
	cout << "코드 " << ch << " 의 값은 ";
	cout << int(ch) << endl;
	cout << "네, 코드 Z의 값은 ";
	cout << static_cast<int>(ch) << endl;
	return 0;
}
