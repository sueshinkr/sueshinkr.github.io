// file_server.cpp

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
	int					serv_sd, clnt_sd, read_cnt;
	char				buf[BUF_SIZE];
	FILE				*fp;
	socklen_t			clnt_adr_sz;
	struct sockaddr_in	serv_adr, clnt_adr;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}
	
	fp = fopen("file_server.cpp", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	while (1)
	{
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (read_cnt < BUF_SIZE)
		{
			write(clnt_sd, buf, read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}

	shutdown(clnt_sd, SHUT_WR);
	read(clnt_sd, buf, BUF_SIZE);
	cout << "Message from client : " << buf << endl;

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
