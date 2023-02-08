// SyncSema_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

unsigned WINAPI		Read(void *arg);
unsigned WINAPI		Accu(void *arg);
static HANDLE		semOne;
static HANDLE		semTwo;
static int			num;

int	main(int argc, char *argv[])
{
	HANDLE hThread1, hThread2;
	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne);
	CloseHandle(semTwo);
	return 0;
}

unsigned WINAPI	Read(void *arg)
{
	for (int i = 0; i < 50000000; i++)
	{
		cout << "Input num : ";
		WaitForSingleObject(semTwo, INFINITE);
		cin >> num;
		ReleaseSemaphore(semOne, 1, NULL);
	}
	return 0;
}

unsigned WINAPI	threadDes(void *arg)
{
	for (int i = 0; i < 50000000; i++)
	{
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		ReleaseSemaphore(semTwo, 1, NULL);
	}
	cout << "Result : " << sum << endl;
	return 0;
}
