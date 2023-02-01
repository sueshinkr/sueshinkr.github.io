// fork.cpp

#include <iostream>
#include <unistd.h>

using namespace std;

int gval = 10;

int	main(int argc, char *argv[])
{
	pid_t	pid;
	int		lval = 20;
	gval++, lval += 5;

	pid = fork();
	if (pid == 0)	// child
		gval += 2, lval += 2;
	else 			// parent
		gval -= 2, lval -= 2;
	
	if (pid == 0)
		cout << "Child Proc : [" << gval << ", " << lval << "]\n";
	else
		cout << "Parent Proc : [" << gval << ", " << lval << "]\n";

	return 0;
}