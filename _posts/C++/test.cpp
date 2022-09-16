// stock00.cpp - version 00

#include <iostream>
#include "stock00.h"

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

Acount::Acount(const std::string & client, const std::string & num, double bal = 0.0)
{
	name = client;
	acount_num = num;
	balance = bal;
}

void Acount::show() const
{
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



void Stock::acquire(const std::string & co, long n, double pr)
{
	company = co;
	if (n < 0)
	{
		std::cout << "주식 수는 음수가 될 수 없으므로, "
				  << company << " shares를 0으로 설정합니다.\n";
		shares = 0;
	}
	else
		shares = n;
	share_val = pr;
	set_tot();
}

void Stock::buy(long num, double price)
{
	if (num < 0)
	{
		std::cout << "매입 주식 수는 음수가 될 수 없으므로, "
				  << "거래가 취소되었습니다.\n";
	}
	else
	{
		shares += num;
		share_val = price;
		set_tot();
	}
}

void Stock::sell(long num, double price)
{
	using std::cout;
	if (num < 0)
	{
		cout << "매입 주식 수는 음수가 될 수 없으므로, "
			 << "거래가 취소되었습니다.\n";
	}
	else if (num > shares)
	{
		cout << "보유 주식보다 많은 주식을 매도할 수 없으므로, "
			 << "거래가 취소되었습니다.\n";
	}
	else
	{
		shares -= num;
		share_val = price;
		set_tot();
	}
}

void Stock::update(double price)
{
	share_val = price;
	set_tot();
}

void Stock::show()
{
	std::cout << "회사명 : " << company
			  << " 주식 수 : " << shares << '\n'
			  << " 주가 : $" << share_val
			  << " 주식 총 가치 : $" << total_val << '\n';
}
