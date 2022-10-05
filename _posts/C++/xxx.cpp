// baddude.cpp

#include "xxx.h"
using std::cout;
using std::cin;
using std::endl;

abstr_emp::abstr_emp() : fname(""), lname(""), job("")
{
}

abstr_emp::abstr_emp(const std::string & fn, const std::string & ln, const std::string & j)
	: fname(fn), lname(ln), job(j)
{
}

void abstr_emp::ShowAll() const
{
	cout << "이름 : " << fname << " " << lname << endl;
	cout << "직업 : " << job << endl;
}

void abstr_emp::SetAll()
{
	cout << "성을 입력하십시오 : ";
	cin >> fname;
	cout << "이름을 입력하십시오 : ";
	cin >> lname;
	cout << "직업을 입력하십시오 : ";
	cin >> job;
}

abstr_emp::~abstr_emp()
{
}

std::ostream & operator<<(std::ostream & os, const abstr_emp & e)
{
	return os << e.fname << " " << e.lname << endl;
}

employee::employee() : abstr_emp()
{
}

employee::employee(const std::string & fn, const std::string & ln, const std::string & j)
	: abstr_emp(fn, ln, j)
{
}

void employee::ShowAll() const
{
	abstr_emp::ShowAll();
}

void employee::SetAll()
{
	abstr_emp::SetAll();
}

manager::manager() : abstr_emp(), inchargeof(0)
{
};

manager::manager(const std::string & fn, const std::string & ln, const std::string & j, int ico)
	: abstr_emp(fn, ln, j), inchargeof(ico)
{
}

manager::manager(const abstr_emp & e, int ico)
	: abstr_emp(e), inchargeof(ico)
{
}

manager::manager(const manager & m) : abstr_emp(m), inchargeof(m.inchargeof)
{
}

void manager::ShowAll() const
{
	abstr_emp::ShowAll();
	cout << "관리 수 : " << inchargeof << endl;
}

void manager::SetAll()
{
	abstr_emp::SetAll();
	cout << "관리 수를 입력하십시오 : ";
	cin >> inchargeof;
}

fink::fink() : abstr_emp(), reportsto("")
{
}

fink::fink(const std::string & fn, const std::string & ln, const std::string & j, const std::string & rpo)
	: abstr_emp(fn, ln, j), reportsto(rpo)
{
}

fink::fink(const abstr_emp & e, const std::string & rpo)
	: abstr_emp(e), reportsto(rpo)
{
}

fink::fink(const fink & e) : abstr_emp(e), reportsto(e.reportsto)
{
}

void fink::ShowAll() const
{
	abstr_emp::ShowAll();
	cout << "보고 대상 : " << reportsto << endl;
}

void fink::SetAll()
{
	abstr_emp::SetAll();
	cout << "보고 대상을 입력하십시오 : ";
	cin >> reportsto;
}

highfink::highfink() : abstr_emp(), manager(), fink()
{
}

highfink::highfink(const std::string & fn, const std::string & ln, const std::string & j, const std::string & rpo, int ico)
	: abstr_emp(fn, ln, j), manager(fn, ln, j, ico), fink(fn, ln, j, rpo)
{	
}

highfink::highfink(const abstr_emp & e, const std::string & rpo, int ico)
	: abstr_emp(e), manager(e, ico), fink(e, rpo)
{
}

highfink::highfink(const fink & f, int ico)
	: abstr_emp(f), manager(f, ico), fink()
{
}

highfink::highfink(const manager & m, const std::string & rpo)
	: abstr_emp(m), manager(m), fink(m, rpo)
{
}

highfink::highfink(const highfink & h)
	: abstr_emp(h), manager(h), fink(h)
{
}

void highfink::ShowAll() const
{
	abstr_emp::ShowAll();
	cout << "관리 수 : " << InChargeOf() << endl;
	cout << "보고 대상 : " << ReportsTo() << endl;
}

void highfink::SetAll()
{
	abstr_emp::SetAll();
	cout << "관리 수를 입력하십시오 : ";
	cin >> InChargeOf();
	cout << "보고 대상을 입력하십시오 : ";
	cin >> ReportsTo();
}