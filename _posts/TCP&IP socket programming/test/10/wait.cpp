// wait.cpp

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int	main(int argc, char *argv[])
{
	int		status;
	pid_t	pid = fork();

	if (pid == 0)
		return 3;
	else
	{
		cout << "Child PID : " << pid << endl;
		pid = fork();
		if (pid == 0)
			exit(7);
		else
		{
			cout << "Child PID : " << pid << endl;
			wait(&status);
			if (WIFEXITED(status))
				cout << "Child send one : " << WEXITSTATUS(status) << endl;

			wait(&status);
			if (WIFEXITED(status))
				cout << "Child send two : " << WEXITSTATUS(status) << endl;
			sleep(30);
		}
	}
	return 0;
}