// todes.cpp

#include <iostream>
#include <fcntl.h>

using namespace std;

int	main(void)
{
	FILE	*fp;
	int		fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
	{
		fputs("file open error", stdout);
		return -1;
	}

	cout << "First file descriptor : " << fd << endl;
	fp = fdopen(fd, "w");
	fputs("TCP/IP SOCKET PROGRAMMING\n", fp);
	cout << "Second file descriptor : " << fileno(fp) << endl;
	fclose(fp);
	return 0;
}