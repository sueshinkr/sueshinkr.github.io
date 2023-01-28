// q5_tcp_client.cpp

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

	for (int i = 0; i < 3; i++)
	{
		char	str[BUFFER_SIZE];
		int		write_len, read_len;

		cout << "Read message from server : ";
		read(sock, &read_len, sizeof(int));
		if (read(sock, str, read_len) != read_len)
			error_handling("read() error!");
		cout << str << endl;
		memset(str, 0, read_len);

		cout << "Write message to server : ";
		cin.getline(str, BUFFER_SIZE);
		write_len = strlen(str);
		write(sock, &write_len, sizeof(int));
		if (write(sock, str, write_len) != write_len)
			error_handling("write() error!");
	}

	close(sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}