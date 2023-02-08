// conv_EPETserv.cpp
// 클라이언트는 ch18의 chat_clnt.cpp를 사용

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 256
#define EPOLL_SIZE 50
void	error_handling(char *buf);
void	setnonblockingmode(int fd);

int		clnt_cnt = 0;
int		clnt_socks[MAX_CLNT];

int	main(int argc, char *argv[])
{
	int					serv_sock, clnt_sock;
	struct sockaddr_in	serv_adr, clnt_adr;
	socklen_t			adr_sz;
	int					str_len;
	char				buf[BUF_SIZE];

	struct epoll_event	*ep_events;
	struct epoll_event	event;
	int					epfd, event_cnt;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (::bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (::listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	setnonblockingmode(serv_sock);
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while (1)
	{
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if (event_cnt == -1)
		{
			error_handling("epoll_wait() error");
			break;
		}

		for (int i = 0; i < event_cnt; i++)
		{
			if (ep_events[i].data.fd == serv_sock)
			{
				adr_sz = sizeof(clnt_adr);
				clnt_sock == accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				clnt_socks[clnt_cnt++] = clnt_sock;
				setnonblockingmode(clnt_sock);

				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				cout << "connected client : " << clnt_sock << endl;
			}
			else
			{
				while (1)
				{
					str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
					if (str_len == 0)
					{
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);

						for (int i = 0; i < clnt_cnt; i++)
						{
							if (ep_events[i].data.fd == clnt_socks[i])
							{
								while (i++ < clnt_cnt - 1)
									clnt_socks[i] = clnt_socks[i + 1];
								break;
							}
						}
						clnt_cnt--;
						cout << "closed client : " << ep_events[i].data.fd << endl;
						break;
					}
					else if (str_len < 0)
					{
						if (errno == EAGAIN)
							break;
					}
					else
					{
						for (int i = 0; i < clnt_cnt; i++)
							write(clnt_socks[i], buf, str_len);
					}
				}
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void	setnonblockingmode(int fd)
{
	int	flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}