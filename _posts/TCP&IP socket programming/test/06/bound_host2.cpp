// bound_host2.cpp

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
	int					sock;
	char				msg1[] = "Hi!";
	char				msg2[] = "I'm another UDP host!";
	char				msg3[] = "Nice to meet you";
	//socklen_t			your_adr_sz;
	struct sockaddr_in	your_adr;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	
	memset(&your_adr, 0, sizeof(your_adr));
	your_adr.sin_family = AF_INET;
	your_adr.sin_addr.s_addr = inet_addr(argv[1]);
	your_adr.sin_port = htons(atoi(argv[2]));

	sendto(sock, msg1, sizeof(msg1), 0,
		   (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg2, sizeof(msg2), 0,
		   (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg3, sizeof(msg3), 0,
		   (struct sockaddr*)&your_adr, sizeof(your_adr));
	cout << "finish\n";

	close(sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}