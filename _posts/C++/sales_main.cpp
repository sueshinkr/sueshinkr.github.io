// sales_main.cpp

#include <iostream>
#include "sales.h"

int main(void)
{
	SALES::Sales arr[2];

	double ar[3] = {1.1, 2.2, 3.3};
	SALES::setSales(arr[0], ar, 3);
	SALES::setSales(arr[1]);

	SALES::showSales(arr[0]);
	SALES::showSales(arr[1]);
	return 0;
}