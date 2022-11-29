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
	// socket(ad, type, protocol)
	// ad : Address Family(AF_INET = IPv4, AF_INET6 = IPv6
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


