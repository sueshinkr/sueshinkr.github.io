// file_server_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 30
void	ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock, hClntSock;
	FILE		*fp;
	int			readCnt, clntAdrSz;
	char		buf[BUF_SIZE];
	SOCKADDR_IN	servAdr, clntAdr;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	fp = fopen("file_server_win.cpp", "rb");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAdr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	::bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	::listen(hServSock, 5);

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock), (SOCKADDR*)&clntAdr, &clntAdrSz;

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSOck, (char*)buf, BUF_SIZE, 0);
	cout << "Message from client : " << buf << endl;

	fclose(fp);
	closesocket(hClntSock);
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