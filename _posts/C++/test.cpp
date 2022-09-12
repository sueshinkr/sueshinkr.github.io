// newplace.cpp

#include <iostream>
#include <new>

const int BUF = 512;
const int N = 5;
char buffer[BUF];

int main()
{
	using namespace std;

	double *pd1, *pd2;
	int i;
	cout << "new와 위치 지정 new의 첫번째 호출 : \n";
	pd1 = new double[N];
	pd2 = new (buffer) double[N];

	for (i = 0; i < N; i++)
		pd2[i] = pd1[i] = 1000 + 20.0 * i;
	cout << "메모리 주소 : \n" << pd1 << " : 힙;	"
		 << (void *) buffer << " : 정적" << endl;
	cout << "메모리 내용 : \n";
	for (i = 0; i < N; i++)
	{
		cout << &pd1[i] << "에 " << pd1[i] << ";	";
		cout << &pd2[i] << "에 " << pd2[i] << endl;
	}

	cout << "\nnew와 위치 지정 new의 두번째 호출 : \n";
	double *pd3, *pd4;
	pd3 = new double[N];
	pd4 = new (buffer) double[N];
	for (i = 0; i < N; i++)
		pd4[i] = pd3[i] = 1000 + 40.0 * i;
	cout << "메모리 내용 : \n";
	for (i = 0; i < N; i++)
	{
		cout << &pd3[i] << "에 " << pd3[i] << ";	";
		cout << &pd4[i] << "에 " << pd4[i] << endl;
	}

	cout << "\nnew와 위치 지정 new의 세번째 호출 : \n";
	delete [] pd1;
	pd1 = new double[N];
	pd2 = new (buffer + N * sizeof(double)) double[N];
	for (i = 0; i < N; i++)
		pd2[i] = pd1[i] = 1000 + 60.0 * i;
	cout << "메모리 내용 : \n";
	for (i = 0; i < N; i++)
	{
		cout << &pd1[i] << "에 " << pd1[i] << ";	";
		cout << &pd2[i] << "에 " << pd2[i] << endl;
	}
	delete [] pd1;
	delete [] pd3;
	return 0;
}
