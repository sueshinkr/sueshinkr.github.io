// sock_type.cpp

#include <iostream>
#include <sys/socket.h>

using namespace std;

void	error_handling(char *message);

int main(int argc, char *argv[])
{
	int			tcp_sock, udp_sock;
	int			sock_type;
	int			state;
	socklen_t	optlen;

	optlen = sizeof(sock_type);
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
	cout << "SOCK_STREAM : " << SOCK_STREAM << endl;
	cout << "SOCK_DGRAM : " << SOCK_DGRAM << endl;

	state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state)
		error_handling("getsockopt() error!");
	cout << "Socket type one : " << sock_type << endl;

	state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state)
		error_handling("getsockopt() error!");
	cout << "Socket type two : " << sock_type << endl;
	
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}