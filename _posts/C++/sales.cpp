// sales.cpp

#include <iostream>
#include "sales.h"

double cal_mean(const double ar[], int n);
double cal_max(const double ar[], int n);
double cal_min(const double ar[], int n);

void SALES::setSales(Sales & s, const double ar[], int n)
{
	int small = n < 4 ? n : 4;
	int big = n > 4 ? n : 4;

	for (int j = 0; j < big; j++)
	{
		if (j < small)
			s.sales[j] = ar[j];
		else
			s.sales[j] = 0;
	}
	s.average = cal_mean(ar, small);
	s.max = cal_max(ar, small);
	s.min = cal_min(ar, small);
}

void SALES::setSales(Sales & s)
{
	using std::cout;
	using std::cin;

	for (int i = 0; i < 4; i++)
	{
		cout << i + 1 << "분기 판매액을 입력하세요 : ";
		cin >> s.sales[i];
	}
	s.average = cal_mean(s.sales, 4);
	s.max = cal_max(s.sales, 4);
	s.min = cal_min(s.sales, 4);
}

void SALES::showSales(const Sales & s)
{
	using std::cout;

	cout << std::endl;
	for (int i = 0; i < 4; i++)
		cout << i + 1 << "분기 판매액은 " << s.sales[i] << "$ 입니다.\n";
	cout << "평균은 " << s.average << "$ 입니다.\n";
	cout << "최대값은 " << s.max << "$ 입니다.\n";
	cout << "최소값은" << s.min << "$ 입니다.\n";
}

double cal_mean(const double ar[], int n)
{
	double sum = 0;
	for (int i = 0; i < n; i++)
		sum += ar[i];
	return sum / n;
}

double cal_max(const double ar[], int n)
{
	double max = 0;
	for (int i = 0; i < n; i++)
	{
		if (max < ar[i])
			max = ar[i];
	}
	return max;
}

double cal_min(const double ar[], int n)
{
	double min = INT_MAX;
	for (int i = 0; i < n; i++)
	{
		if (min > ar[i])
			min = ar[i];
	}
	return min;
}