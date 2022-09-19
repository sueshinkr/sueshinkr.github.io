// strngbad.cpp

#include <cstring>
#include "xxx.h"
using std::cout;

int StringBad::num_strings = 0;

StringBad::StringBad(const char * s)
{
	len = std::strlen(s);
	str = new char[len + 1];
	std::strcpy(str, s);
	num_strings++;
	cout << num_strings << " : \"" << str
		 << "\" 객체 생성\n";
}

StringBad::StringBad()
{
	len = 4;
	str = new char[4];
	std::strcpy(str, "C++");
	num_strings++;
	cout << num_strings << " : \"" << str
		 << "\" 디폴트 객체 생성\n";
}

StringBad::~StringBad()
{
	cout << "\"" << str << "\" 객체 파괴, ";
	--num_strings;
	cout << "남은 객체 수 : " << num_strings << "\n";
	delete [] str;
}

std::ostream & operator<<(std::ostream & os, const StringBad & st)
{
	os << st.str;
	return os;
}