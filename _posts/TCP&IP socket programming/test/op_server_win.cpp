// op_server_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 1024
int		calculate(int num, int *opset);
void	ErrorHandling(char* message);

int		main(int argc, char* argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock, hClntSock;
	SOCKADDR_IN	servAddr, clntAddr;
	int			clntAddrSize;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");
	
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (::bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	
	if (::listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	
	clntAddrSize = sizeof(clntAddr);

	for (i = 0; i < 5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");
		else
			cout << "Connected client " << i + 1 << endl;

		int *opset = new int[BUF_SIZE];

		int num;
		recv(hClntSock, &num, sizeof(int), 0);

		int recvLen = 0;
		while (recvLen < num * sizeof(int) + 1)
			recvLen += recv(hClntSock, opset, BUF_SIZE - 1, 0);

		int result = calculate(num, opset);

		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

int		calculate(int num, int *opset)
{
	int result = opset[0];
	char op = static_cast<char>(opset[num]);

	switch(op)
	{
		case '+':
			for (int j = 1; j < num; j++)
				result += opset[j];
			break;
		case '-':
			for (int j = 1; j < num; j++)
				result -= opset[j];
			break;
		case '*':
			for (int j = 1; j < num; j++)
				result *= opset[j];
			break;
	}

	delete opset;
	return (result);
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}