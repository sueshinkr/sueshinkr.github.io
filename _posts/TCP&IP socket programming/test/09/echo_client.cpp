// echo_client.cpp

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

	char	message[BUF_SIZE];
	int		str_len;

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

	while (1)
	{
		cout << "Input message(Q to quit) : ";
		cin.getline(message, BUF_SIZE);

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		cout << "Message from server : " << message << endl;
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