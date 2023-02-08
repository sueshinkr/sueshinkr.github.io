// thread2_win.cpp

#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

void			ErrorHandling(char* message);
unsigned WINAPI	ThreadFunc(void *arg);

int	main(int argc, char *argv[])
{
	HANDLE		hThread;
	DWORD		wr;
	unsigned	threadID;
	int			param = 5;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	if (hThread == 0)
	{
		ErrorHandling("_beginthreadex() error");
		return -1;
	}

	if ((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED)
	{
		ErrorHandling("thread wait error");
		return -1;
	}

	cout << "wait result : " << (wr == WAIT_OBJECT_0) ? "signaled" : "time-out" << endl;
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

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}