// oob_send_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 30
void	ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hAcptSock, hRecvSock;
	SOCKADDR_IN	sendAdr, recvAdr;
	int			sendAdrSize, strLen;
	char		buf[BUF_SIZE];
	int			result;
	
	fd_set			read, except, readCopy, exceptCopy;
	struct timeval	timeout;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recvAdr, 0, sizeof(recvAdr));
	recvAdr.sin_family = AF_INET;
	recvAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAdr.sin_port = htons(atoi(argv[1]));

	if (::bind(hAcptSock, (SOCKADDR*)&recvAdr, sizeof(recvAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	if (::listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	sendAdrSize = sizeof(sendAdr);
	hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAdr, &sendAdrSize);
	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(hRecvSock, &read);
	FD_SET(hRecvSock, &except);

	while (1)
	{
		readCopy = read;
		exceptCopy = except;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);
		if (result > 0)
		{
			if (FD_ISSET(hRecvSock, &exceptCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				cout << "Urgent message : " << buf << endl;
			}

			if (FD_ISSET(hRecvSock, &readCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, 0);
				if (strLen == 0)
				{
					break;
					closesocket(hRecvSock);
				}
				else
				{
					buf[strLen] = 0;
					cout << buf << endl;
				}
			}
		}
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}