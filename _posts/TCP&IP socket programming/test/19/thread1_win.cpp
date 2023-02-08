// thread1_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

void			ErrorHandling(char* message);
unsigned WINAPI	ThreadFunc(void *arg);

int	main(int argc, char *argv[])
{
	HANDLE		hThread;
	unsigned	threadID;
	int			param = 5;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	if (hThread == 0)
	{
		ErrorHandling("_beginthreadex() error");
		return -1;
	}
	Sleep(3000);
	cout << "end of main\n";
	return 0;
}

unsigned WINAPI	ThreadFunc(void *arg)
{
	int	cnt = *((int*)arg);
	for (int i = 0; i < cnt; i++)
	{
		Sleep(1000);
		cout << "running thread\n";
	}
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}