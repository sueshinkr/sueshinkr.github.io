// SyncEvent_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

#define STR_LEN 100
unsigned WINAPI		NumberOfA(void *arg);
unsigned WINAPI		NumberOfOthers(void *arg);
static char			str[STR_LEN];
static HANDLE		hEvent;

int	main(int argc, char *argv[])
{
	HANDLE hThread1, hThread2;
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	cout << "Input string : ";
	cin.getline(str, STR_LEN);
	SetEvent(hEvent);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);
	ResetEvent(hEvent);
	CloseHandle(hEvent);
	return 0;
}

unsigned WINAPI	NumberOfA(void *arg)
{
	int cnt = 0;
	WaitForSingleObject(hEvent, INFINITE);
	for (int i = 0; str[i] != 0; i++)
	{
		if (str[i] == 'A')
			cnt++;
	}
	cout << "Num of A : " << cnt << endl;
	return 0;
}

unsigned WINAPI	NumberOfOthers(void *arg)
{
	int cnt = 0;
	WaitForSingleObject(hEvent, INFINITE);
	for (int i = 0; str[i] != 0; i++)
	{
		if (str[i] != 'A')
			cnt++;
	}
	cout << "Num of others : " << cnt - 1 << endl;
	return 0;
}
