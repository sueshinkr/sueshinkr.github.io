// chat_serv_win.cpp

#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <process.h>

using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI	HandleClnt(void *arg);
void			SendMsg(char *msg, int len);
void			ErrorHandling(char* message);

int		clntCnt = 0;
SOCKET	clntSocks[MAX_CLNT];
HANDLE	hMutex;

int	main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock, hClntSock;
	SOCKADDR_IN	servAdr, clntAdr;
	int			clntAdrSz;
	HANDLE		hThread;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (::bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (::listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		cout << "Connected client IP : " << inet_ntoa(clntAdr.sin_addr) << endl;
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

unsigned WINAPI	HandleClnt(void *arg)
{
	SOCKET	hClntSock = *((SOCKET*)arg);
	int		strLen = 0;
	char	msg[BUF_SIZE];

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
		SendMsg(msg, strLen);

	WaitForSingleObject(hMutex, INFINITE);

	for (int i = 0; i < clntCnt; i++)	// remove disconnected client
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return NULL;
}

void	SendMsg(char *msg, int len)	// send to all
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
	ReleaseMutex(hMutex);
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}