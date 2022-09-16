---
title:  "C++ Primer 10 프로그래밍 연습"
excerpt: "10챕터 programing exercise"

categories:
  - Cpp
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.16 18:00:00
---

# 01
```cpp
// acount.h

#ifndef ACOUNT_H_
#define ACOUNT_H_

#include <string>

class Acount
{
	private :
		std::string name;
		std::string acount_num;
		double balance;

	public :
		Acount(const std::string & client, const std::string & num, double bal = 0.0);
		void show(void) const;
		void deposit(double cash);
		void withdraw(double cash);
};

#endif
```

```cpp
// acount.cpp

#include <iostream>
#include "acount.h"

Acount::Acount(const std::string & client, const std::string & num, double bal)
{
	name = client;
	acount_num = num;
	balance = bal;
}

void Acount::show() const
{
	using std::cout;
	using std::endl;

	cout << "계좌 정보를 출력합니다.\n";
	cout << "name : " << name << endl;
	cout << "acount : " << acount_num << endl;
	cout << "balance : " << balance << endl;
}

void Acount::deposit(double cash)
{
	balance += cash;
}

void Acount::withdraw(double cash)
{
	balance -= cash;
}
```

```cpp
// useaccnt.cpp

#include <iostream>
#include <cctype>
#include "acount.h"

int main()
{
	using namespace std;

	Acount ac("ABC", "1234-5678", 100);
	ac.show();

	ac.deposit(1000);
	ac.show();
	
	ac.withdraw(500);
	ac.show();

	return 0;
}
```

# 02

```cpp
// name.h

#ifndef NAME_H_
#define NAME_H_

#include <string>

using std::string;

class Person
{
	private:
		static const int LIMIT = 25;
		string lname;
		char fname[LIMIT];
	public:
		Person() {lname = ""; fname[0] = '\0';};
		Person(const string & ln, const char * fn = "Heyyou");
		void Show() const;
		void FormalShow() const;
};

#endif
```

```cpp
// name.cpp

#include <iostream>
#include "name.h"

Person::Person(const std::string & ln, const char * fn)
{
	lname = ln;
	strcpy(fname, fn);
}

void Person::Show() const
{
	std::cout << fname << " " << lname << std::endl;
}

void Person::FormalShow() const
{
	std::cout << lname << ", " << fname << std::endl;
}
```

```cpp
// usename.cpp

#include <iostream>
#include "name.h"

int main()
{
	using namespace std;

	Person one;
	Person two("Smythecraft");
	Person three("Dimwiddy", "Sam");

	one.Show();
	one.FormalShow();
	cout << endl;

	two.Show();
	two.FormalShow();
	cout << endl;

	three.Show();
	three.FormalShow();
	cout << endl;

	return 0;
}
```

# 03

```cpp
// golf.h

#ifndef GOLF_H_
#define GOLF_H_

class Golf
{
	private:
		static const int Len = 40;
		char fullname[Len];
		int handicap;
	public:
		Golf();
		Golf(const char * fn, const int hc);
		int setgolf();
		void sethandicap(int hc);
		void showgolf() const;
};

#endif
```

```cpp
// golf.cpp

#include <iostream>
#include "golf.h"

Golf::Golf()
{
}

Golf::Golf(const char * fn, const int hc)
{
	strcpy(fullname, fn);
	handicap = hc;
}

int Golf::setgolf()
{
	using std::cout;
	using std::cin;

	char name[50];
	cout << "이름을 입력하세요 : ";
	cin.getline(name, 49);

	if (name[0] == '\0')
	{
		cout << "입력을 종료합니다.\n";
		std::cin.clear();
		return 0;
	}

	int hc;
	cout << "핸디캡을 입력하세요 : ";
	cin >> hc;
	cin.get();
	
	Golf temp(name, hc);
	*this = temp;

	return 1;
}

void Golf::sethandicap(int hc)
{
	handicap = hc;
}

void Golf::showgolf() const
{
	using std::cout;
	using std::endl;

	cout << "이름 : " << fullname << endl;
	cout << "핸디캡 : " << handicap << endl;
}
```

```cpp
// usegolf.cpp

#include <iostream>
#include "golf.h"

int main()
{
	Golf arr[4];
	int i;

	for (i = 0; i < 3; i++)
	{
		if (arr[i].setgolf() == 0)
			break;
	}

	arr[0].sethandicap(100);

	for (int j = 0; j < i; j++)
		arr[j].showgolf();

	return 0;
}
```

# 04

```cpp
// sales.h

#ifndef SALES_H_
#define SALES_H_

namespace SALES
{
	class Sales
	{
		private:
			static const int QUARTERS = 4;
			double sales[QUARTERS];
			double average;
			double max;
			double min;
		public:
			Sales();
			Sales(const double ar[], int n);
			void setSales();
			void showSales() const;
	};
}

#endif
```

```cpp
// sales.cpp

#include <iostream>
#include "sales.h"

double cal_mean(const double ar[], int n);
double cal_max(const double ar[], int n);
double cal_min(const double ar[], int n);

namespace SALES
{
	Sales::Sales()
	{
	}

	Sales::Sales(const double ar[], int n)
	{
		int small = n < 4 ? n : 4;
		int big = n > 4 ? n : 4;

		for (int j = 0; j < big; j++)
		{
			if (j < small)
				sales[j] = ar[j];
			else
				sales[j] = 0;
		}
		average = cal_mean(sales, small);
		max = cal_max(sales, small);
		min = cal_min(sales, small);
	}

	void Sales::setSales()
	{
		using std::cout;
		using std::cin;

		double temp_sales[QUARTERS];

		for (int i = 0; i < 4; i++)
		{
			cout << i + 1 << "분기 판매액을 입력하세요 : ";
			cin >> temp_sales[i];
		}
		average = cal_mean(temp_sales, 4);
		max = cal_max(temp_sales, 4);
		min = cal_min(temp_sales, 4);

		Sales temp(temp_sales, QUARTERS);
		*this = temp;
	}

	void Sales::showSales() const
	{
		using std::cout;

		cout << std::endl;
		for (int i = 0; i < 4; i++)
			cout << i + 1 << "분기 판매액은 " << sales[i] << "$ 입니다.\n";
		cout << "평균은 " << average << "$ 입니다.\n";
		cout << "최대값은 " << max << "$ 입니다.\n";
		cout << "최소값은" << min << "$ 입니다.\n";
	}
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
```

```cpp
// sales_main.cpp

#include <iostream>
#include "sales.h"

int main(void)
{
	SALES::Sales arr1;
	arr1.setSales();
	arr1.showSales();

	double ar[3] = {1.1, 2.2, 3.3};

	SALES::Sales arr2(ar, 3);
	arr2.showSales();

	return 0;
}
```

# 05

```cpp
// stack.h

#ifndef STACK_H_
#define STACK_H_

struct customer{
	char fullname[35];
	double payment;
};

typedef customer Item;

class Stack
{
	private:
		enum {MAX = 10};
		Item items[MAX];
		int top;

	public:
		Stack();
		bool isempty() const;
		bool isfull() const;
		bool push(const Item & item);
		bool pop(Item & item);
		void total_sum(int & total) const;
};

#endif
```

```cpp
// stack.cpp

#include <cstring>
#include "stack.h"

Stack::Stack()
{
	top = 0;
}

bool Stack::isempty() const
{
	return top == 0;
}

bool Stack::isfull() const
{
	return top == MAX;
}

bool Stack::push(const Item & item)
{
	if (top < MAX)
	{
		strcpy(items[top].fullname, item.fullname);
		items[top++].payment = item.payment;
		return true;
	}
	else
		return false;
}

bool Stack::pop(Item & item)
{
	if (top > 0)
	{
		item = items[--top];
		return true;
	}
	else
		return false;
}

void Stack::total_sum(int & total) const
{
	total += items[top].payment;
}
```

```cpp
// usestack.cpp

#include <iostream>
#include "stack.h"

int main(void)
{
	using namespace std;
	Stack st;
	Item temp;
	char ch;
	int total = 0;

	cout << "지불 내역을 추가하려면 A, 상환 처리하려면 P, \n"
		 << "종료하려면 Q를 입력하십시오.\n";
	while (cin >> ch && toupper(ch) != 'Q')
	{
		while (cin.get() != '\n')
			continue;
		if (!isalpha(ch))
		{
			cout << '\a';
			continue;
		}
		switch (ch)
		{
			case 'A':
			case 'a': cout << "지불할 고객의 이름을 입력하십시오 : ";
					  cin.getline(temp.fullname, 35);
					  cout << "지불할 금액을 입력하십시오 : ";
					  cin >> temp.payment;
					  if (st.isfull())
					  	cout << "스택이 가득 차 있습니다.\n";
					  else
					  	st.push(temp);
					  break;
			case 'P':
			case 'p': if (st.isempty())
					  	cout << "스택이 비어있습니다.\n";
					  else {
						st.pop(temp);
						st.total_sum(total);
						cout << "#" << temp.fullname << " 고객의 상환 처리가 완료되었습니다.\n";
						cout << "현재까지의 수입은 " << total << "원입니다.\n";
					  }
					  break;
		}
		cout << "지불 내역을 추가하려면 A, 상환 처리하려면 P, \n"
			 << "종료하려면 Q를 입력하십시오.\n";
	}
	cout << "총 수입은 " << total << "원입니다.\n";
	cout << "프로그램을 종료합니다.\n";
	return 0;
}
```

# 06

```cpp
// move.h

#ifndef MOVE_H_
#define MOVE_H_

class Move
{
	private:
		double x;
		double y;
	public:
		Move(double a = 0, double b = 0);
		void showmove() const;
		Move add(const Move & m) const;
		void reset(double a = 0, double b = 0);
};

#endif
```

```cpp
// move.cpp

#include <iostream>
#include "move.h"

Move::Move(double a, double b)
{
	x = a;
	y = b;
}

void Move::showmove() const
{
	std::cout << "x : " << x << std::endl;
	std::cout << "y : " << y << std::endl;
}

Move Move::add(const Move & m) const
{
	double newx;
	double newy;

	newx = x + m.x;
	newy = y + m.y;

	Move newmove(newx, newy);
	return newmove;
}

void Move::reset(double a, double b)
{
	x = a;
	y = b;
}
```

```cpp
// usemove.cpp

#include <iostream>
#include "move.h"

int main(void)
{
	using namespace std;
	Move a;
	Move b(11.11, 22.22);

	a.showmove();
	b.showmove();
	Move c = a.add(b);

	c.showmove();
	c.reset();
	c.showmove();
	
	return 0;
}
```

# 07

```cpp
// plorg.h

#ifndef PLORG_H_
#define PLORG_H_

class Plorg
{
	private:
		static const int LIMIT = 20;
		char name[LIMIT];
		int ci;
	public:
		Plorg();
		Plorg(const char * newname);
		void change_ci(int num);
		void show() const;
};

#endif
```

```cpp
// plorg.cpp

#include <iostream>
#include "plorg.h"

Plorg::Plorg()
{
	strcpy(name, "Plorga");
	ci = 50;
}

Plorg::Plorg(const char * newname)
{
	strcpy(name, newname);
	ci = 50;
}

void Plorg::change_ci(int num)
{
	ci = num;
}

void Plorg::show() const
{
	std::cout << "name : " << name << std::endl;
	std::cout << "CI : " << ci << std::endl;
}
```

```cpp
// useplorg.cpp

#include <iostream>
#include "plorg.h"

int main(void)
{
	using namespace std;
	Plorg a;
	Plorg b("newplorg");

	a.show();
	b.show();

	a.change_ci(100);
	b.change_ci(1000);

	a.show();
	b.show();
	
	return 0;
}
```

# 08

```cpp
// list.h

#ifndef LIST_H_
#define LIST_H_

typedef int Item;

class List
{
	private:
		static const int MAX = 10;
		Item items[MAX];
		int top;
	public:
		List();
		bool isempty() const;
		bool isfull() const;
		void add(const Item &);
		void show() const;
		void visit(void (*pf)(Item &));
};

#endif
```

```cpp
// list.cpp

#include <iostream>
#include "list.h"

List::List()
{
	top = 0;
}

bool List::isempty() const
{
	return top == 0;
}

bool List::isfull() const
{
	return top == MAX;
}

void List::add(const Item & item)
{
	if(top < MAX)
		items[top++] = item;
}

void List::visit(void (*pf)(Item &))
{
	for(int i = 0; i < top; i++)
		pf(items[i]);
}

void List::show() const
{
	if(isempty())
		std::cout << "리스트가 비어있습니다.\n";
	else
	{
		for(int i = 0; i < top; i++)
			std::cout << "Item #" << i + 1 << ": " << items[i] << "\n";
	}
}
```

```cpp
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
```