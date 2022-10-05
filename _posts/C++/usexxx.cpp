// pe14-5.cpp

#include "xxx.h"
#include <iostream>
using namespace std;

int main()
{
	employee em("Trip", "Harris", "Thumper");
	cout << em << endl;
	em.ShowAll();

	manager ma("Amorphia", "Spindragon", "Nuancer", 5);
	cout << ma << endl;
	ma.ShowAll();

	fink fi("Matt", "Oggs", "Oiler", "Juno Barr");
	cout << fi << endl;
	fi.ShowAll();

	highfink hf(ma, "Curly Kew");
	hf.ShowAll();
	cout << "다음 출력을 위해 아무 키나 누르십시오 : \n";
	cin.get();
	highfink hf2;
	hf2.SetAll();

	cout << "abstr_emp * 포인터의 사용 : \n";

	abstr_emp tri[4] = {em, fi, hf, hf2};
	for (int i = 0; i < 4; i++)
		tri[i].ShowAll();
/*
	abstr_emp * tri[4] = { &em, &fi, &hf, &hf2 };
	for (int i = 0; i < 4; i++)
	{
		tri[i]->ShowAll();
		cout << endl;
	}
*/
	return 0;
}