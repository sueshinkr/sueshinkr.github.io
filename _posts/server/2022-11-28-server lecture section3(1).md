---
title:  "Server lecture section3 [1/5]"
excerpt: "네트워크 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.28 18:00:00
---

# 소켓 프로그래밍 기초 #1

손님 : 핸드폰 준비 - 식당 번호로 문의 - 대리인을 통해 식당 측과 대화 가능
* 손님 = 클라이언트
* 핸드폰 = 소켓
* 식당 번호 = 서버 주소
* 소켓을 통해 서버와 패킷 송수신 가능

식당 : 안내원 고용 - 안내원 교육(대표 번호 배정) - 영업 시작 - 대리인에 안내
* 식당 = 서버
* 안내원 = Listener 소켓
* 안내원 교육 = Bind(서버주소/포트를 소켓에 연동)
* 영업 시작 = Listen
* 안내 = Accept(별도의 소켓에 연결)
* 클라이언트 세션을 통해 통신 가능

***

# 소켓 프로그래밍 기초 #2

```cpp
// DummyClient.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	// winsock 초기화(ws2_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	// 소켓 생성
	// socket(af, type, protocol)
	// af : Address Family(AF_INET = IPv4, AF_INET6 = IPv6
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0일경우 알아서 선택
	// return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	SOCKADDR_IN serverAddr; // IPv4용
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); << deprecated
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	// 연결할 목적지 : IP주소와 Port가 한쌍 - 아파트 동호수와 비슷
	// htons : host to network short, 네트워크에서는 Big-Endian을 사용하기 때문에 맞춰주는 역할 
	serverAddr.sin_port = ::htons(7777); // 일부 포트는 예약되어있음
	
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect ErrorCode : " << errCode << endl;
		return 0;
	}

	// ----------------
	// 연결 성공! 데이터 송수신 가능능

	cout << "Connected To Server!" << endl;

	while (true)
	{
		// Todo
		this_thread::sleep_for(1s);
	}

	// 소켓 리소스 반환
	::closesocket(clientSocket);
	// winsock 종료
	::WSACleanup();
}
```

`#pragma comment(lib, "xxxx.lib")`와 같이 사용하면 라이브러리를 명시적으로 링크할 수 있음    
기본 형식은 `#pragma comment(comment-type, ["comment string"])`
* comment type에는 `compiler`, `exestr`, `lib`, `linker`, `user`등이 올 수 있음
* `[]`안의 구문은 형식에 따라 필요한 것을 사용

<br/>

`WSAStartup` 함수는 `winsock` 동적 연결 라이브러리를 초기화하고 어플리케이션 요구사항을 충족하는지 확인함    
* 첫 번째 매개변수인 `wVersionRequested`에서 버전값을 받으며, `MAKEWORD` 매크로를 통해 편리하게 사용할 수 있음    
	* `WORD MAKEWORD(BYTE bLow, BYTE bHigh)`의 형식, 하위 8비트와 상위 8비트를 각각 인자로 전달    
* 두 번째 매개변수인 `IpWSAData`에 WSADATA 구조체 포인터 주소를 넣어주면 해당 구조체에 세부 정보들이 채워짐    
* `WSAStartup` 함수 호출시 반드시 `WSACleanup()` 함수를 호출하여 짝을 맞춰주어야함    

[`WSAStartup()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup)    

<br/>

`socket()` 함수를 통해 소켓 생성    
* 첫 번째 매개변수인 `af`에서 `Winsock2.h` 헤더파일에 정의되어잇는 `address family` 형식 중 하나를 받음    
	* `AF_INET` : IPv4 / `AF_INET6` : IPv6 등이 존재
* 두 번째 매개변수인 `type`에서 소켓 타입을 선택
	* `SOCK_STREAM` : TCP / `SOCK_DGRAM` : UDP
* 세 번째 매개변수에는 프로토콜을 입력
	* 0으로 입력시 자동으로 적절한 프로토콜을 선택    

[`socket()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket)    

<br/>

`SOCKADDR_IN` 구조체에 소켓에 사용할 정보들을 저장    
```cpp
typedef struct sockaddr_in {
  short          sin_family;
  u_short        sin_port;
  struct in_addr sin_addr;
  char           sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN, *LPSOCKADDR_IN;
```

* `sin_family`에 `address family` 형식을 지정
* `inet_pton()`함수를 사용해 연결할 목적지 IP 주소를 `sin_addr`에 지정
* `htons()` 함수를 사용해 연결할 목적지 포트를 `sin_port`에 지정
	* `htons` : host to network short   
* `sin_zero[8]`은 전체 크기를 16바이트로 맞추기 위한 더미    

[`SOCKADDR_IN`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-sockaddr_in)    
[`inet_pton()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_pton)    
[`htons()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-htons)    

<br/>

`connect()` 함수로 생성한 소켓을 서버의 주소에 연결 요청    
* 첫 번째 매개변수에는 연결할 소켓을 지정
* 두 번째 매개변수에는 `SOCKADDR` 구조체를 지정
	* `SOCKADDR` 구조체는 `SOCKADDR_IN`과 같은 데이터를 가짐
	* 단, IP주소와 포트 번호가 `data[14]`로 묶여있는 `SOCKADDR`보다 `SOCKADDR_IN`이 사용하기 편리함    

[`connect()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect)


```cpp
// GameServer.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	// 나의 주소
	SOCKADDR_IN serverAddr; // IPv4용
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // 알아서 골라달라
	serverAddr.sin_port = ::htons(7777); // 일부 포트는 예약되어있음

	// 대표 번호 배정
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Bind ErrorCode : " << errCode << endl;
		return 0;
	}

	// 영업 시작
	// 10 : 백로그, 대기열의 최대한도
	if (::listen(listenSocket, 10) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Listen ErrorCode : " << errCode << endl;
		return 0;
	}

	// ---------------------------

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);
		
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Accept ErrorCode : " << errCode << endl;
			return 0;
		}

		// 손님 입장
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected! IP = " << ipAddress << endl;
	}

	// ---------------------------

		::WSACleanup();
}
```

`bind()` 함수로 `listen`소켓을 바인딩 - 소켓에 IP주소와 포트를 할당    
매개변수는 `connect()` 함수와 동일    
[`bind()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-bind)    

<br/>

`listen()` 함수로 소켓을 통해 들어오는 연결을 대기열에 추가함    
* 첫 번째 매개변수로 `listen`소켓을 지정    
* 두 번째 매개변수로 대기열의 크기를 지정    

[`listen()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen)    

<br/>

`accept()` 함수로 수신 대기열에 있는 클라이언트의 연결 요청을 받아들이며 새 소켓을 생성    
* 첫 번째 매개변수로 `listen` 소켓을 지정    
* 두 번째 매개변수로 클라이언트의 주소 및 포트정보를 구조체에 복사하여 넘겨줌    
* 세 번째 매개변수로 구조체의 크기    

[`accept()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept)    

`inet_ntop()` 함수는 IPv4나 IPv6 주소를 binary 형태에서 사람이 알아보기 쉬운 텍스트로 전환해줌    
[`inet_ntop()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_ntop)    
