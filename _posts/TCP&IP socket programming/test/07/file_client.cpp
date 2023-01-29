// file_client.cpp

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
	int					sd, read_cnt;
	char				buf[BUF_SIZE];
	FILE				*fp;
	struct sockaddr_in	serv_adr;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);

	cout << "Received file data";
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}