#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

using namespace std;

void	error_handling(char* message);

int		main()
{
	int		fd;
	char	buf[] = "HELOOOOOOO";
	char	buf2[BUF_SIZE];

	fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (fd == -1)
		error_handling("open() error");
	cout << "file descriptor : " << fd << endl;

	if (write(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error");

	close(fd);

	fd = open("data.txt", O_RDONLY);

	if (read(fd, buf2, sizeof(buf)) == -1)
		error_handling("read() error");
	cout << "file data : " << buf2 << endl;

	close(fd);

	return 0;
}

void	error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}