// SyncEventtoSema_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

#define STR_LEN 100
unsigned WINAPI		NumberOfA(void *arg);
unsigned WINAPI		NumberOfOthers(void *arg);
static HANDLE		semOne;
static HANDLE		semTwo;
static char			str[STR_LEN];

int	main(int argc, char *argv[])
{
	HANDLE hThread1, hThread2;
	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	cout << "Input string : ";
	cin.getline(str, STR_LEN);
	ReleaseSemaphore(semTwo, 1, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne);
	CloseHandle(semTwo);
	return 0;
}

unsigned WINAPI	NumberOfA(void *arg)
{
	int cnt = 0;
	WaitForSingleObject(semTwo, INFINITE);
	for (int i = 0; str[i] != 0; i++)
	{
		if (str[i] == 'A')
			cnt++;
	}
	cout << "Num of A : " << cnt << endl;
	ReleaseSemaphore(semOne, 1, NULL);
	return 0;
}

unsigned WINAPI	NumberOfOthers(void *arg)
{
	int cnt = 0;
	WaitForSingleObject(semOne, INFINITE);
	for (int i = 0; str[i] != 0; i++)
	{
		if (str[i] != 'A')
			cnt++;
	}
	cout << "Num of others : " << cnt - 1 << endl;
	ReleaseSemaphore(semTwo, 1, NULL);
	return 0;
}
