// Pat&Mat.cpp

#include <iostream>
#include <list>
using namespace std;

int main()
{
	list<string> Pat;
	list<string> Mat;
	list<string> PatandMat;

	cout << "Mat의 친구들을 입력하세요 (끝내려면 quit) : \n";
	string temp;
	while (cin >> temp && temp != "quit")
		Mat.push_back(temp);
	Mat.sort();
	cout << "Mat의 친구들 목록입니다.\n";
	for (list<string>::iterator i = Mat.begin(); i != Mat.end(); ++i)
		cout << (*i) << endl;

	cout << "Pat의 친구들을 입력하세요 (끝내려면 quit) : \n";
	while (cin >> temp && temp != "quit")
		Pat.push_back(temp);
	Pat.sort();
	cout << "Pat의 친구들 목록입니다.\n";
	for (list<string>::iterator i = Pat.begin(); i != Pat.end(); ++i)
		cout << (*i) << endl;

	PatandMat = Mat;
	PatandMat.merge(Pat);
	PatandMat.unique();

	cout << "Pat&Mat의 친구들 목록입니다.\n";
	for (list<string>::iterator i = PatandMat.begin(); i != PatandMat.end(); ++i)
		cout << (*i) << endl;

	return 0;
}