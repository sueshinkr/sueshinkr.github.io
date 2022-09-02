// strgback.cpp

#include <iostream>
char * buildstr(char c, int n);

int main()
{
	using namespace std;
	int times;
	char ch;

	cout << "문자 하나를 입력하십시오 : ";
	cin >> ch;
	cout << "정수 하나를 입력하십시오 : ";
	cin >> times;
	char *ps = buildstr(ch, times);
	cout << ps << endl;
	delete [] ps;
	ps = buildstr('+', 20);
	cout << ps << "-DONE-" << ps << endl;
	delete [] ps;
	return 0;
}

char * buildstr(char c, int n)
{
	char * pstr = new char[n + 1];
	pstr[n] = '\0';
	while (n-- > 0)
		pstr[n] = c;
	return pstr;
}
