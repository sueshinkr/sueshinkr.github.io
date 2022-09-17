// stock00.cpp - version 00

#include <iostream>

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
