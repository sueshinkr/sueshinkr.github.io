// choices.cpp

#include <iostream>
#include <string>
using namespace std;

string change_upper(string & str);

int main()
{
	
	
	string str;
	cout << "문자열을 입력하시오 (끝내려면 q) : ";
	getline(cin, str);
	while (str != "q")
	{
		str = change_upper(str);
		cout << str << endl;
		cout << "다음 문자열을 입력하시오 (끝내려면 q) : ";
		getline(cin, str);
	}
	cout << "끝.\n";
	return 0;
}

string change_upper(string &str)
{
	for (int i = 0; str[i]; i++)
		str[i] = toupper(str[i]);
	return str;
}