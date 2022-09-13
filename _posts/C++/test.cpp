// static.cpp

#include <iostream>

struct chaff
{
	char dross[20];
	int slag;
};

const int N = 2;
const int BUF = 512;
char buffer[BUF];

int main()
{
	using namespace std;

	chaff *arr;

	arr = new (buffer) chaff[N];

	char temp[20];
	for (int i = 0; i < N; i++)
	{
		cout << "##" << i + 1 << "번째##\n";
		cout << "dross 입력 : ";
		cin.getline(temp, 19);
		strcpy(arr[i].dross, temp);

		cout << "slag 입력 : ";
		cin >> arr[i].slag;
		cin.get();
	}

	cout << endl;

	for (int i = 0; i < N; i++)
	{
		cout << "##" << i + 1 << "번째##\n";
		cout << "dross : " << arr[i].dross << endl;
		cout << "slag : " << arr[i].slag << endl;
	}
	return 0;
}
