// uselist.cpp

#include <iostream>
#include "list.h"

void sqr(int & x){ x *= x; };

int main(void)
{
	List lst;
	int tmp;
	std::cout << "숫자를 입력하십시오 (q 입력시 종료됩니다) : ";
	while(std::cin >> tmp)
	{
		lst.add(tmp);
		if(lst.isfull()) break;
		std::cout << "숫자를 입력하십시오 (q 입력시 종료됩니다) : ";
	}
	lst.show();
	lst.visit(sqr);
	lst.show();

	return 0;
}