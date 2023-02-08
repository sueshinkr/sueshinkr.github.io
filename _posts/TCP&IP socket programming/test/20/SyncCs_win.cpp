// SyncCs_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

#define NUM_THREAD 50
unsigned WINAPI		threadInc(void *arg);
unsigned WINAPI		threadDes(void *arg);
long long			num = 0;
CRITICAL_SECTION	cs;

int	main(int argc, char *argv[])
{
	HANDLE tHandles[NUM_THREAD];

	InitializerCriticalSection(&cs);
	for (int i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	DeleteCriticalSection(&cs);
	cout << "result : " << num << endl;
	return 0;
}

unsigned WINAPI	threadInc(void *arg)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < 50000000; i++)
		num += 1;
	LeaveCritialSection(&cs);
	return 0;
}

unsigned WINAPI	threadDes(void *arg)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < 50000000; i++)
		num -= 1;
	LeaveCritialSection(&cs);
	return 0;
}
