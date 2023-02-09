// Asynchat_serv_win.cpp

#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <process.h>

using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 256


void	CompressSockets(SOCKET hSockArr[], int idx, int total);
void	CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void	SendMsg(SOCKET clntSocks[], int clntCnt, char *msg, int len);
void	ErrorHandling(char* message);

int	main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock, hClntSock;
	SOCKADDR_IN	servAdr, clntAdr;

	SOCKET				hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			newEvent;
	WSANETWORKEVENTS	netEvents;

	int		numOfClntSock = 0;
	int		strLen, posInfo, startIdx;
	int		clntAdrLen;
	char	msg[BUF_SIZE];

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (::bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (::listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	newEvent = WSACreateEvent();
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");
	
	hSockArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent;
	numOfClntSock++;

	while (1)
	{
		posInfo = WSAWaitForMultipleEvents(numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (int i = startIdx; i < numOfClntSock; i++)
		{
			int	sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
				continue;
			else
			{
				sigEventIdx = i;
				WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);
				if (netEvents.lNetworkEvents & FD_ACCEPT)	// 연결요청시
				{
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						cout << "Accept Error" << endl;
						break;
					}
					clntAdrLen = sizeof(clntAdr);
					hClntSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
					newEvent = WSACreateEvent();
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;
					hSockArr[numOfClntSock] = hClntSock;
					numOfClntSock++;

					cout << "connected new client...\n";
				}

				if (netEvents.lNetworkEvents & FD_READ)	// 데이터 수신시
				{
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						cout << "Read Error\n";
						break;
					}
					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					SendMsg(hSockArr, numOfClntSock, msg, strLen);
				}

				if (netEvents.lNetworkEvents & FD_CLOSE)	// 종료요청시
				{
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						cout << "Read Error\n";
						break;
					}
					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);

					numOfClntSock--;
					CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
					CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void	SendMsg(SOCKET clntSocks[], int clntCnt, char *msg, int len)	// send to all
{
	for (int i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
}

void	CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}

void	CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}