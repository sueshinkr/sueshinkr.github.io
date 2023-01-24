// op_client.cpp

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 1024
void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int	sock;

	struct sockaddr_in	serv_addr;

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (::connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else
		cout << "Connected.........\n";

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

	write(sock, opset, sizeof(int) * (num + 2));

	int result;
	read(sock, &result, sizeof(int));

	cout << "Operation result : " << result << endl;

	close(sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}