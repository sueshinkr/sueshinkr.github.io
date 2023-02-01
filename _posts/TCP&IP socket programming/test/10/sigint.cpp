// sigint.cpp

#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

#define BUF_SIZE 30

void	timeout(int sig)
{
	if (sig == SIGALRM)
		cout << "Time out!" << endl;
	alarm(2);
}

void	keycontrol(int sig)
{
	char	buf[BUF_SIZE];

	if (sig == SIGINT)
	{
		cout << "Quit Program?" << endl;
		cin.getline(buf, BUF_SIZE);

		if (!strcmp(buf, "y") || !strcmp(buf, "Y"))
		{
			exit(1);
		}
	}
}

int		main(int argc, char *argv[])
{
	struct sigaction	act;
	
	act.sa_handler = keycontrol;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);

	while (1)
	{
		cout << "Wait..." << endl;
		sleep(1);
	}
	return 0;
}