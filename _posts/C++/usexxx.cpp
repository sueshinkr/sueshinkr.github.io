// use_sales.cpp

#include "xxx.h"
#include <iostream>

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;

	double vals1[12] =
	{
		1220, 1100, 1122, 2212, 1232, 2334,
		2884, 2393, 3302, 2922, 3002, 3544
	};

	double vals2[12] =
	{
		12, 11, 22, 21, 32, 34,
		28, 29, 33, 29, 32, 35
	};

	Sales sales1(2011, vals1, 12);
	LabeledSales sales2("Blogstar", 2012, vals2, 12);

	cout << "첫 번째 try 블록 : \n";
	try
	{
		int i;
		cout << "Year = " << sales1.Year() << endl;
		for (i = 0; i < 12; ++i)
		{
			cout << sales1[i] << ' ';
			if (i % 6 == 5)
				cout << endl;
		}
		cout << "Year = " << sales2.Year() << endl;
		cout << "Label = " << sales2.Label() << endl;
		for (i = 0; i <= 12; ++i)
		{
			cout << sales2[i] << ' ';
			if (i % 6 == 5)
				cout << endl;
		}
	}
	catch(Sales::bad_index & bad)
	{
		cout << bad.what();
		if (typeid(LabeledSales::nbad_index) == typeid(bad))
		{
			LabeledSales::nbad_index nbad = dynamic_cast<LabeledSales::nbad_index &>(bad);
			cout << "Company : " << nbad.label_val() << endl;
		}
		cout << "잘못된 인덱스 : " << bad.bi_val() << endl;
	}

	cout << "\n다음 try 블록 : \n";
	try
	{
		sales2[2] = 37.5;
		sales1[20] = 23345;
		cout << "try 블록 2의 끝\n";
	}
	catch(Sales::bad_index & bad)
	{
		cout << bad.what();
		if (typeid(LabeledSales::nbad_index) == typeid(bad))
		{
			LabeledSales::nbad_index nbad = dynamic_cast<LabeledSales::nbad_index &>(bad);
			cout << "Company : " << nbad.label_val() << endl;
		}
		cout << "잘못된 인덱스 : " << bad.bi_val() << endl;
	}
	cout << "프로그램을 종료합니다.\n";

	return 0;
}