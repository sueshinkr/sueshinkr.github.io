// fork_checkfd.cpp

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int gval = 10;

int	main(int argc, char *argv[])
{
	pid_t	pid;
	int		sock;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	pid = fork();
	if (pid == 0)	// child
		cout << "in child socket fd : " << sock << endl;
	else 			// parent
		cout << "in parent socket fd : " << sock << endl;

	return 0;
}