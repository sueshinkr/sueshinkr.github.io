// workmi.cpp

#include <iostream>
#include <cstring>
#include "xxx.h"

const int SIZE = 5;

int main()
{
	using std::cin;
	using std::cout;
	using std::endl;
	using std::strchr;

	Worker * lolas[SIZE];

	int ct;
	for (ct = 0; ct < SIZE; ct++)
	{
		char choice;
		cout << "직종을 입력하십시오 : \n"
			 << "w : 웨이터		s : 가수		"
			 << "t : 가수 겸 웨이터		q : 종료\n";
		cin >> choice;
		while (strchr("wstq", choice) == NULL)
		{
			cout << "w, s, t, q 중에서 하나를 선택하십시오 : ";
			cin >> choice;
		}
		if (choice == 'q')
			break;
		switch(choice)
		{
			case 'w': lolas[ct] = new Waiter;
				break;
			case 's': lolas[ct] = new Singer;
				break;
			case 't': lolas[ct] = new SingingWaiter;
				break;
		}
		cin.get();
		lolas[ct]->Set();
	}
	
	cout << "\n사원 현황은 다음과 같습니다 : \n";
	int i;
	for (i = 0; i < ct; i++)
	{
		cout << endl;
		lolas[i]->Show();
	}
	for (i = 0; i < ct; i++)
		delete lolas[i];
	cout << "프로그램을 종료합니다.\n";
	return 0;
}