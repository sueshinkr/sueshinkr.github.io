// echo_selectserv_win.cpp
// 클라이언트는 이전의 echo_client_win.cpp를 사용

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 1024
void	ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock, hClntSock;
	SOCKADDR_IN	servAdr, clntAdr;
	timeval		timeout;
	fd_set		reads, cpyReads;

	int		adrSz;
	int		strLen, fdNum;
	char	buf[BUF_SIZE];

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAddr, 0, sizeof(servAdr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (::bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (::listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	while (1)
	{
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;
		
		if (fdNum == 0)
			continue;
		
		for (int i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock)	// conncection request
				{
					adrSz = sizeof(clntAdr);
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSOck, &reads);
					cout << "connected clinet : " << hClntSock << endl;
				}
				else	// read message
				{
					strLen = recv(reads.fd_array[i], &reads);
					if (strLen == 0)	// close request
					{
						FD_CLR(reads.fd_aaray[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						cout << "closed client : " << cpyReads.fds_array[i] << endl;
					}
					else
						send(reads.fd_array[i], buf, strLen, 0);	// echo
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}