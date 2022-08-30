// nested.cpp

#include <iostream>

int main()
{
	using namespace std;

	cout << "출력할 행 수를 입력하십시오 : ";
	int num;
	cin >> num;
	for (int i = 1; i <= num; i++)
	{
		for (int j = i; j < num; j++)
			cout << ".";
		for (int j = 0; j < i; j++)
			cout << "*";
		cout << endl;
	}
	return 0;
}

//cout << "문자들을 입력하시오; 끝내려면 #을 입력하시오 : \n";
