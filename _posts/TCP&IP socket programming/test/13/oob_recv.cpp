// oob_recv.cpp

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

using namespace std;

#define	BUF_SIZE 30
void	error_handling(char *message);
void	urg_handler(int signo);

int		acpt_sock;
int		recv_sock;

int		main(int argc, char *argv[])
{
	int					str_len, state;
	struct sockaddr_in	recv_adr, serv_adr;
	socklen_t			serv_adr_sz;
	struct sigaction	act;
	char				buf[BUF_SIZE];

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
		error_handling("bind() error");
	::listen(acpt_sock, 5);
	
	serv_adr_sz = sizeof(serv_adr);
	recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

	fcntl(recv_sock, F_SETOWN, getpid());
	state = sigaction(SIGURG, &act, 0);

	while ((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0)
	{
		if (str_len == -1)
			continue;
		buf[str_len] = 0;
		cout << buf << endl;
	}
	close(recv_sock);
	close(acpt_sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void	urg_handler(int signo)
{
	int		str_len;
	char	buf[BUF_SIZE];

	str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
	cout << "recvlen : " << str_len << endl;
	buf[str_len] = 0;
	cout << "Urgent message : " << buf << endl;
}
