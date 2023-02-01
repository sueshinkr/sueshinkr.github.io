// echo_mpserver.c

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 30
void	error_handling(char *message);
void	read_childproc(int sig);

int		main(int argc, char *argv[])
{
	int					serv_sock, clnt_sock;
	struct sockaddr_in	serv_adr, clnt_adr;

	pid_t				pid;
	struct sigaction	act;
	socklen_t			adr_sz;
	int					str_len, state;
	char				buf[BUF_SIZE];

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (::listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			cout << "new client connected..." << endl;
		
		pid = fork();
		if (pid == -1)
		{
			close(clnt_sock);
			continue;
		}
		else if (pid == 0)
		{
			close(serv_sock);
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);
			
			close(clnt_sock);
			cout << "client disconnected..." << endl;
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void	read_childproc(int sig)
{
	int		status;
	pid_t	pid = waitpid(-1, &status, WNOHANG);

	if (WIFEXITED(status))
		cout << "Removed proc id : " << pid << endl;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}