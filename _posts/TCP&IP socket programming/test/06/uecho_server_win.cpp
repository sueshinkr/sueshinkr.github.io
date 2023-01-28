// uecho_server_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 30
void	ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		servSock;
	char		message[BUF_SIZE];
	int			strLen, clntAdrSz;
	SOCKADDR_IN	servAdr, clntAdr;
	
	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");
	
	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAdr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (::bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0,
						  (SOCKADDR*)&clntAdr, &clntAdrSz);
		sendto(servSock, message, strLen, 0,
			   (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}

	closesocket(servSock);
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}