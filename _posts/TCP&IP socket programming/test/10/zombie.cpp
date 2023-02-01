// zombie.cpp

#include <iostream>
#include <unistd.h>

using namespace std;

int	main(int argc, char *argv[])
{
	pid_t	pid = fork();

	if (pid == 0)	// child
		cout << "Hi, I'm a child process\n";
	else
	{
		cout << "Child Process ID : " << pid << endl;
		sleep(30);
	}

	if (pid == 0)
		cout << "End Child process\n";
	else
		cout << "End parent process\n";
	
	return 0;
}