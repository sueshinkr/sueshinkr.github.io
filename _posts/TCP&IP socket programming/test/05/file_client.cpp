// file_transfer_client.cpp

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
	int	sock;

	struct sockaddr_in	serv_addr;

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

	string filename;
	cout << "Write filename to recv : ";
	cin >> filename;

	char		fbuf[BUFFER_SIZE];
	
	if (access(("new" + filename).c_str(), F_OK) == 0)
		error_handling("file already exist");
	else
	{
		FILE	*recvfile = fopen(("new" + filename).c_str(), "w");

		int filename_len = filename.length();
		write(sock, &filename_len, sizeof(int));
		write(sock, filename.c_str(), filename.size());

		int	readbyte;
		while ((readbyte = read(sock, fbuf, BUFFER_SIZE)) != 0)
		{
			cout << "readbyte : " << readbyte << endl;
			fwrite(fbuf, sizeof(char), readbyte, recvfile);
		}
		cout << "File transfer success" << endl;
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