// echo_mtserv.cpp
// 클라이언트는 이전의 echo.client.cpp를 사용

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 256

void*	handle_clnt(void *arg);
void	send_msg(char *msg, int len);
void	error_handling(char *msg);

char	msg[BUF_SIZE];
pthread_mutex_t	mutx;

int	main(int argc, char *argv[])
{
	int					serv_sock, clnt_sock;
	struct sockaddr_in	serv_adr, clnt_adr;
	socklen_t			clnt_adr_sz;
	pthread_t			t_id;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
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
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		cout << "Connected client IP : " << inet_ntoa(clnt_adr.sin_addr) << endl;
	}
	close(serv_sock);
	return 0;
}

void*	handle_clnt(void *arg)
{
	int		clnt_sock = *((int*)arg);
	int		str_len = 0;

	while (1)
	{
		pthread_mutex_lock(&mutx);
		str_len = read(clnt_sock, msg, sizeof(msg));
		if (str_len <= 0)
			break;
		else
			write(clnt_sock, msg, str_len);
		pthread_mutex_unlock(&mutx);
	}

	close(clnt_sock);
	return NULL;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}