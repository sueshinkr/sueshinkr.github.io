// op_server.c

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 1024
int		calculate(int num, int *opset);
void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int	serv_sock;
	int	clnt_sock;

	struct sockaddr_in	serv_addr;
	struct sockaddr_in	clnt_addr;
	socklen_t			clnt_addr_size;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (::bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (::listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);

	for (int i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			cout << "Connected client " << i + 1 << endl;

		int *opset = new int[BUF_SIZE];

		int num;
		read(clnt_sock, &num, sizeof(int));

		int recv_len = 0;
		while (recv_len < num * sizeof(int) + 1)
			recv_len += read(clnt_sock, opset, BUF_SIZE - 1);

		int result = calculate(num, opset);
		write(clnt_sock, &result, sizeof(int));
		close(clnt_sock);
	}

	close(serv_sock);
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

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}