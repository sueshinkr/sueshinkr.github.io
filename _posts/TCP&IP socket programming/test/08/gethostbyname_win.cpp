// gethostbyname_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

void	ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	WSADATA			wsaData;
	struct hostent	*host;

	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <addr>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	host = gethostbyname(argv[1]);
	if (!host)
		ErrorHandling("gethost... error");

	cout << "Official name : " << host->h_name << endl;
	for (int i = 0; host->h_aliases[i]; i++)
		cout << "Aliases " << i + 1 << " : " << host->h_aliases[i] << endl;
	cout << "Address type : " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << endl;
	for (int i = 0; host->h_addr_list[i]; i++)
		cout << "IP addr " << i + 1 << " : " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << endl;
	WSACleanup();
	return 0;
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}