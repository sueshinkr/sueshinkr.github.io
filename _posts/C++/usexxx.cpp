// palindrome.cpp

#include <iostream>
#include <string>
using namespace std;

bool is_palindrome(string & str);

int main()
{
	string str;

	cout << "문자열을 입력하세요 : ";
	getline(cin, str);

	if (is_palindrome(str))
		cout << "화문입니다.\n";
	else
		cout << "화문이 아닙니다.\n";

	return 0;
}

bool is_palindrome(string & str)
{
	for (auto i = str.begin(); i != str.end(); ++i)
	{
		if (!isalpha(*i))
			str.erase(i--);
		else
			*i = tolower(*i);
	}
	for (auto f = str.begin(), r = str.end(); f < r; )
	{
		if (*f++ != *--r)
			return false;
	}
	return true;
}