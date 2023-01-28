// conversation_server.cpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 256
void	error_handling(char *message);

int main(int argc, char *argv[])
{
	int			serv_sock, str_len;
	char		message[BUF_SIZE];
	socklen_t	clnt_adr_sz;

	struct	sockaddr_in serv_adr, clnt_adr;
	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
						   (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		message[str_len] = 0;
		cout << "Message from Client: " << message << endl;

		cout << "Message to Client(Q to quit) : ";
		cin.getline(message, BUF_SIZE);

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		sendto(serv_sock, message, strlen(message), 0,
			   (struct sockaddr*)&clnt_adr, clnt_adr_sz);
	}

	close(serv_sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}