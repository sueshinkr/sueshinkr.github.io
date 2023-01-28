// file_transfer_server.cpp

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
	int	serv_sock;
	int	clnt_sock;

	struct sockaddr_in	serv_addr;
	struct sockaddr_in	clnt_addr;
	socklen_t			clnt_addr_size;
	
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
	clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	char	filename[BUFFER_SIZE];
	int		filename_len;
	if (read(clnt_sock, &filename_len, sizeof(int)))
	{
		read(clnt_sock, filename, filename_len);

		cout << "filename : " << filename << endl;
		if (access(filename, F_OK) == -1)
			error_handling("file doesn't exist");
		else
		{
			FILE	*sendfile = fopen(filename, "r");
			char	buf[1];

			while (fread(buf, sizeof(buf), 1, sendfile))
				write(clnt_sock, buf, 1);
		}
	}

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}