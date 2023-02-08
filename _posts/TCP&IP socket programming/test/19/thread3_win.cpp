// thread3_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

#define NUM_THREAD 50
void			ErrorHandling(char* message);
unsigned WINAPI	threadInc(void *arg);
unsigned WINAPI	threadDes(void *arg);
long long		num = 0;

int	main(int argc, char *argv[])
{
	HANDLE		tHandles[NUM_THREAD];

	cout << "sizeof long long : " << sizeof(long long) << endl;
	for (int i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	cout << "result : " << num << endl;
	return 0;
}

unsigned WINAPI	threadInc(void *arg)
{
	for (int i = 0; i < 50000000; i++)
		num += 1;
	return 0;
}

unsigned WINAPI	threadDes(void *arg)
{
	for (int i = 0; i < 50000000; i++)
		num -= 1;
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}