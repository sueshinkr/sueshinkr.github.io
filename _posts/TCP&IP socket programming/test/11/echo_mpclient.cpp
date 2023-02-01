// echo_mpclient.cpp

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 30
void	error_handling(char *message);
void	read_routine(int sock, char *buf);
void	write_routine(int sock, char *buf);

int		main(int argc, char *argv[])
{
	int					sock;
	pid_t				pid;
	char				buf[BUF_SIZE];
	struct sockaddr_in	serv_adr;

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (::connect(sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		cout << "Connected.........\n";

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf);
	else
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void	read_routine(int sock, char *buf)
{
	while (1)
	{
		int	str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)
			return ;

		buf[str_len] = 0;
		cout << "Message from server : " << buf << endl;
	}
}

void	write_routine(int sock, char *buf)
{
	while (1)
	{
		cin.getline(buf, BUF_SIZE);

		if (!strcmp(buf, "q") || !strcmp(buf, "Q"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}