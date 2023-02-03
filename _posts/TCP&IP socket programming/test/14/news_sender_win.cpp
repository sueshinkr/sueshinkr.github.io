// news_sender_win.cpp

#include <iostream>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define TTL 64
#define BUF_SIZE 30
void	ErrorHandling(char* message);

int		main(int argc, char *argv[])
{
	WSADATA				wsaData;
	SOCKET				hSendSock;
	SOCKADDR_IN			mulAdr;
	int					time_live = TTL;
	FILE				*fp;
	char				buf[BUF_SIZE];

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <GroupIP> <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSendSock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&mulAdr, 0, sizeof(mulAdr));
	mulAdr.sin_family = AF_INET;
	mulAdr.sin_addr.s_addr = inet_addr(argv[1]);
	mulAdr.sin_port = htons(atoi(argv[2]));

	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&timeLive, sizeof(timeLive));
	if ((fp = fopen("news.txt", "r")) == NULL)
		error_handling("fopen() error");

	while (!feof(fp))	// Broadcasting
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(hSendSock, buf, strlen(buf), 0, (struct sockaddr*)&mulAdr, sizeof(mulAdr));
		sleep(2);
	}

	fclose(fp);
	closesocket(hSendSock);
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}