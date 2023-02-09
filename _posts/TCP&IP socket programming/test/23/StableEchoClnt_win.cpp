// StableEchoClnt_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 1024
void	ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hSocket;
	SOCKADDR_IN	servAdr;
	char		message[BUF_SIZE];
	int			strLen, readLen;

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (::connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");
	else
		cout << "Connected.................\n";

	while (1)
	{
		cout << "Input message(Q to quit) : ";
		cin.getline(message, BUF_SIZE);

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		strLen = strlen(message);
		send(hSocket, message, strLen, 0);
		readLen = 0;
		while (1)
		{
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen)
				break;
		}
		message[strLen] = 0;
		cout << "Message from server : " << message << endl;
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
