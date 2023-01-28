// uecho_client.cpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 30
void	error_handling(char *message);

int main(int argc, char *argv[])
{
	int			sock, str_len;
	char		message[BUF_SIZE];
	socklen_t	adr_sz;

	struct	sockaddr_in serv_adr, from_adr;
	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		cout << "Input message(Q to quit) : ";
		cin.getline(message, BUF_SIZE);

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		sendto(sock, message, strlen(message), 0,
			   (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0,
						   (struct sockaddr*)&from_adr, &adr_sz);
		message[str_len] = 0;
		cout << "Message from server : " << message << endl;
	}

	close (sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
