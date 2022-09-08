// autoscp.cpp

#include <iostream>
void oil(int x);

int main()
{
	using namespace std;

	int texas = 31;
	int year = 2011;
	cout << "main()에서, texas = " << texas << ", &texas = ";
	cout << &texas << endl;
	cout << "main()에서, year = " << year << ", &year = ";
	cout << &year << endl;
	oil(texas);
	cout << "main()에서, texas = " << texas << ", &texas = ";
	cout << &texas << endl;
	cout << "main()에서, year = " << year << ", &year = ";
	cout << &year << endl;
	return 0;
}

void oil(int x)
{
	using namespace std;
	int texas = 5;

	cout << "oil()에서, texas = " << texas << ", &texas = ";
	cout << &texas << endl;
	cout << "oil()에서, x = " << x << ", &x = ";
	cout << &x << endl;
	{
		int texas = 113;
		cout << "블록에서, texas = " << texas << ", &texas = ";
		cout << &texas << endl;
		cout << "블록에서, x = " << x << ", &x = ";
		cout << &x << endl;
	}
	cout << "블록을 통과한 후, texas = " << texas;
	cout << ", &texas = " << &texas << endl;
}