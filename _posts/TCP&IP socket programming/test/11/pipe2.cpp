// pipe2.cpp

#include <iostream>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30

int	 main(int argc, char *argv[])
{
	int		fds[2];
	char	str1[] = "Who are you?";
	char	str2[] = "Thank you for your message";
	char	buf[BUF_SIZE];
	pid_t	pid;

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		write(fds[1], str1, sizeof(str1));
		//sleep(2);
		read(fds[0], buf, BUF_SIZE);
		cout << "Child proc output : " << buf << endl;
	}
	else
	{
		read(fds[0], buf, BUF_SIZE);
		cout << "Parent proc output : " << buf << endl;
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}
	return 0;
}