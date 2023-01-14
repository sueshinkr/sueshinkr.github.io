#undef UNICODE
#undef _UNICODE
#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[])
{
	char* strAddr = "203.211.218.102:9190";

	char		strAddrBuf[50];
	sockaddr_in	servAddr;
	int			size;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	size = sizeof(servAddr);
	WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);
	
	size = sizeof(strAddrBuf);
	WSAAddressToString((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);

	cout << "Second conv result : " << strAddrBuf << endl;
	WSACleanup();
	return 0;
}