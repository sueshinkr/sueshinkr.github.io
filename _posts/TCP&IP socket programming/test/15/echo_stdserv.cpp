// echo_stdserv.c

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 1024
void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int					serv_sock, clnt_sock;
	struct sockaddr_in	serv_addr, clnt_addr;
	int					str_len, i;
	char				message[BUF_SIZE];
	socklen_t			clnt_addr_size;
	FILE				*readfp, *writefp;

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

	for (i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			cout << "Connected client " << i + 1 << endl;

		readfp = fdopen(clnt_sock, "r");
		writefp = fdopen(clnt_sock, "w");
		while (!feof(readfp))
		{
			fgets(message, BUF_SIZE, readfp);
			fputs(message, writefp);
			fflush(writefp);
		}

		fclose(readfp);
		fclose(writefp);
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