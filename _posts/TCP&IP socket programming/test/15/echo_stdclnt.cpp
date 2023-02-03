// echo_stdclnt.cpp

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 1024
void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int					sock;
	int					str_len;
	char				message[BUF_SIZE];
	struct sockaddr_in	serv_addr;
	FILE				*readfp, *writefp;

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

	readfp = fdopen(sock, "r");
	writefp = fdopen(sock, "w");

	while (1)
	{
		cout << "Input message(Q to quit) : ";
		fgets(message, BUF_SIZE, stdin);
		
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		fputs(message, writefp);
		fflush(writefp);
		fgets(message, BUF_SIZE, readfp);
		cout << "Message from server : " << message;
	}

	fclose(writefp);
	fclose(readfp);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}