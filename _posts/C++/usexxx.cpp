// usedma.cpp

#include <iostream>
#include "xxx.h"

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;

	int num;
	AbcDMA *dma;
	AbcDMA *temp[3];

	temp[0] = new baseDMA("bbbb", 1);
	temp[1] = new lacksDMA("black", "llll", 2);
	temp[2] = new hasDMA("warm", "hhhh", 3);

	cout << "base=1, lacks=2, has=3\n";
	cout << "번호를 선택해주십시오 : ";
	if (cin >> num)
	{
		switch(num)
		{
			case 1:
				dma = new baseDMA("base", 1111);
				dma->View();
				cout << endl;
				dma = temp[0];
				cout << *dma;
				break;
			case 2:
				dma = new lacksDMA("white", "lacks", 2222);
				dma->View();
				cout << endl;
				dma = temp[1];
				cout << (const lacksDMA &)*dma;
				break;
			case 3:
				dma = new hasDMA("cool", "has", 3333);
				dma->View();
				cout << endl;
				dma = temp[2];
				cout << (const hasDMA &)*dma;
				break;
		}
	}
	else
		cout << "잘못 입력하였습니다. 프로그램을 종료합니다.\n";
	
	return 0;
}