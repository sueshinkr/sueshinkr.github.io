// uecho_client_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 30
void	ErrorHandling(char *message);

int		main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		sock;
	char		message[BUF_SIZE];
	int			strLen;
	SOCKADDR_IN	servAdr;

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAdr, 0, sizeof(servAdr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		cout << "Input message(Q to quit) : ";
		cin.getline(message, BUF_SIZE);

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		send(sock, message, strlen(message), 0);
		strLen = recv(sock, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		cout << "Message from server : " << message << endl;
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
