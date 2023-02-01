// three_conversation.cpp

#include <iostream>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30

int	 main(int argc, char *argv[])
{
	int		fds1[2], fds2[2];
	char	buf[BUF_SIZE];
	pid_t	pid;

	pipe(fds1), pipe(fds2);
	pid = fork();
	if (pid == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			cout << "Child : ";
			cin.getline(buf, BUF_SIZE);
			write(fds1[1], buf, strlen(buf));
			memset(buf, 0, sizeof(buf));
			read(fds2[0], buf, BUF_SIZE);
			cout << "from Parent : " << buf << endl;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			memset(buf, 0, sizeof(buf));
			read(fds1[0], buf, BUF_SIZE);
			cout << "from Child : " << buf << endl;
			cout << "Parent : ";
			cin.getline(buf, BUF_SIZE);
			write(fds2[1], buf, strlen(buf));
		}
	}
	return 0;
}