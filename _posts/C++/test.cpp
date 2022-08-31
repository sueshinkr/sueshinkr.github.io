// sumafile.cpp

#include <iostream>
#include <string>
#include <fstream>
struct donation
{
	std::string name;
	double money;
};

int main()
{
	using namespace std;
	char filename[100];
	ifstream inFile;


	cout << "기부 정보를 파일로부터 가져옵니다. 파일 이름을 입력해주세요 : ";
	cin >> filename;
	inFile.open(filename);
	int person;
	inFile >> person;
	donation * dn = new donation [person];

	for (int i = 0; i < person; i++)
	{
		getline(inFile, dn[i].name);
		getline(inFile, dn[i].name);
		cout << dn[i].name << endl;
		inFile >> dn[i].money;
	}

	for (int i = 0; i < person; i++)
	{
		if (dn[i].money >= 10000)
			cout << "고액 기부자 " << dn[i].name << " : " << dn[i].money << "$ 기부\n";
	}
	for (int i = 0; i < person; i++)
	{
		if (dn[i].money < 10000)
			cout << "소액 기부자 " << dn[i].name << " : " << dn[i].money << "$ 기부\n";
	}
	if (person == 0)
		cout << "기부자가 없습니다.\n";
	return 0;
}
