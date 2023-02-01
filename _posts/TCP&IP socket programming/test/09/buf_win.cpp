// buf_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

void	ErrorHandling(char* message);
void	ShowSocketBufSize(SOCKET sock);

int main(int argc, char *argv[])
{
	WSADATA			wsaData;
	SOCKET			hSock;
	int				sndBuf, rcvBuf, state;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSock = socket(PF_INET, SOCK_STREAM, 0);
	ShowSocketBufSize(hSock);

	sndBuf = 1024*3, rcvBuf = 1024*3;
	state = setsockopt(hSock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf));
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error!");

	state = setsockopt(hSock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf));
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error!");

	ShowSocketBufSize(hSock);
	closesocket(hSock);
	WSACleanup();

	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void	ShowSocketBufSize(SOCKET sock)
{
	int	sndBuf, rcvBuf, state, len;

	len = sizeof(sndBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, &len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	len = sizeof(rcvBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, &len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	cout << "Input buffer size : " << rcvBuf << endl;
	cout << "Output buffer size : " << sndBuf << endl;
}