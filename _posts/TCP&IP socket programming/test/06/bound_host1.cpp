// bound_host1.cpp

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
	int					sock, str_len;
	char				message[BUF_SIZE];
	socklen_t			adr_sz;
	struct sockaddr_in	my_adr, your_adr;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	
	memset(&my_adr, 0, sizeof(my_adr));
	my_adr.sin_family = AF_INET;
	my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr)) == -1)
		error_handling("bind() error");

	for (int i = 0; i < 3; i++)
	{
		sleep(5);
		adr_sz = sizeof(your_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0,
						   (struct sockaddr*)&your_adr, &adr_sz);
		cout << "Message " << i + 1 << " : " << message << endl;
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