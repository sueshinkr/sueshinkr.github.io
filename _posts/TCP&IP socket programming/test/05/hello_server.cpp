// q5_tcp_server.cpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUFFER_SIZE 1024
void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int	serv_sock;
	int	clnt_sock;

	struct sockaddr_in	serv_addr;
	struct sockaddr_in	clnt_addr;
	socklen_t			clnt_addr_size;
	
	if (argc != 2)
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
	clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	for (int i = 0; i < 3; i++)
	{
		char	str[BUFFER_SIZE];
		int		write_len, read_len;

		cout << "Write message to client : ";
		cin.getline(str, BUFFER_SIZE);
		write_len = strlen(str);
		write(clnt_sock, &write_len, sizeof(int));
		if (write(clnt_sock, str, write_len) != write_len)
			error_handling("write() error!");
		memset(str, 0, write_len);

		cout << "Read message from client : ";
		read(clnt_sock, &read_len, sizeof(int));
		if (read(clnt_sock, str, read_len) != read_len)
			error_handling("read() error!");
		cout << str << endl;
	}

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}