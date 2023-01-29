// gethostbyname.cpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

void	error_handling(char *message);

int main(int argc, char *argv[])
{
	struct hostent	*host;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <addr>\n";
		exit(1);
	}

	host = gethostbyname(argv[1]);
	if (!host)
		error_handling("gethost... error");

	cout << "Official name : " << host->h_name << endl;
	for (int i = 0; host->h_aliases[i]; i++)
		cout << "Aliases " << i + 1 << " : " << host->h_aliases[i] << endl;
	cout << "Address type : " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << endl;
	for (int i = 0; host->h_addr_list[i]; i++)
		cout << "IP addr " << i + 1 << " : " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << endl;
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}