// choices.cpp

#include <iostream>
#include <cstring>
using namespace std;
struct stringy
{
	char * str;
	int ct;
};

void set(stringy & beany, const char testing[]);
void show(const stringy & beany, int count = 1);
void show(const char testing[], int count = 1);

void show(const stringy & beany, int count)
{
	for (int i = count; i > 0; i--)
		cout << beany.str << endl;
}

void show(const char testing[], int count)
{
	for (int i = count; i > 0; i--)
		cout << testing << endl;
}

int main()
{
	stringy beany;
	char testing[] = "Reality isn't what it used to be.";

	set(beany, testing);
	show(beany);
	show(beany, 2);
	testing[0] = 'D';
	testing[1] = 'u';
	show(testing);
	show(testing, 3);
	show("Done!");
	return 0;
}

void set(stringy & beany, const char testing[])
{
	char * new_str = new char;
	beany.str = new_str;
	strcpy(new_str, testing);
	beany.ct = strlen(new_str);
}
