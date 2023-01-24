// op_client_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 1024
void	ErrorHandling(char *message);

int		main(int argc, char *argv[])
{
	WSADATA		wsaData;
	SOCKET		hSocket;
	SOCKADDR_IN	servAddr;

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
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (::connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		error_handling("connect() error");
	else
		cout << "Connected..........\n";

	int *opset = new int[BUF_SIZE];
	int num;

	cout << "Operand count : ";
	cin >> num;

	opset[0] = static_cast<char>(num);
	cout << "num : " << num << endl;
	for (int i = 1; i < num + 1; i++)
	{
		cout << "Operand " << i << " : ";
		cin >> opset[i];
	}

	cout << "Operator : ";
	char op;
	cin >> op;
	opset[num + 1] = static_cast<char>(op);

	send(hSocket, opset, sizeof(int) * (num + 2), 0);

	int result;
	recv(hSocket, &result, sizeof(int), 0);

	cout << "Operation result : " << result << endl;

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
