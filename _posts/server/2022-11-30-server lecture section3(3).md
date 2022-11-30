---
title:  "Server lecture section3 [3/5]"
excerpt: "네트워크 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.30 11:00:00
---

# 소켓 옵션

```cpp
// GameServer.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	// socket, level, option관련 
	// level : 옵션을 해석하고 처리할 주체
	//		   소켓 - SOL_SOCKET, IPv4 - IPPROTO_IP, TCP - IPPROTO_TCP

	// SO_KEEPALIVE : 주기적으로 연결 상태 확인 여부(TCP Only)
	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));

	// SO_LINGER : 지연
	// 소켓 리소스 반환 후 송신 버퍼에 있는 데이터를 보낼지 날릴지를 결정
	// onoff = 0이면 closesocket()이 바로 리턴, 아니면 linger초만큼 대기 (기본값은 0)
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

	// Half-Close : 소켓을 닫겠다는 선언을 해줌
	// SD_SEND : send를 막음
	// SD_RECEIVE : recv를 막음
	// SD_BOTH : 둘다 막음
	::shutdown(serverSocket, SD_SEND);

	// SO_SNDBUF : 송신 버퍼 크기
	// SO_RCVBUF : 수신 버퍼 크기
	int32 sendBufferSize;
	int32 optionLen = sizeof(sendBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen);
	cout << "송신 버퍼 크기 : " << sendBufferSize << endl;

	// SO_REUSEADDR : IP주소 및 포트 재사용 - 사용중인 주소 강제 점유
	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)enable, sizeof(enable));

	// TCP_NODELAY : Nagle 알고리즘 작동 여부
	// 데이터가 충분히 크면 보내고, 그렇지 않으면 데이터가 충분히 쌓일때까지 대기
	// 장점 - 작은 패킷이 불필요하게 많이 생성되는 일을 방지
	// 단점 - 반응 시간 손해 -> 따라서 게임에서는 일반적으로 꺼둠
	bool enable = true;
	::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));

	// 소켓 리소스 반환
	::closesocket(serverSocket);

	// winsock 종료
	::WSACleanup();
}
```

***

# 논블로킹 소켓

```cpp
// DummyClient.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	// 소켓 생성
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	// 논블로킹 소켓으로 전환
	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// 이미 연결된 상태라면 break
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			// Error
			break;
		}
	}

	cout << "Connected to Server!" << endl;

	char sendBuffer[100] = "Hello World";

	// Send
	while (true)
	{
		if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
		{
			// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// Error
			break;
		}

		cout << "Send Data Len = " << sizeof(sendBuffer) << endl;
			
		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
				// Error
				break;
			}
			else if (recvLen == 0)
			{
				// 연결 끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;
			break;
		}

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

	// 블로킹(Blocking) 소켓
	// accept - 접속한 클라가 있을 때 / connect - 서버 접속 성공했을 때
	// send, sendto - 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom - 수신 버퍼에 도착한 데이터가 있고, 이를 유지레벨 버퍼에 복사했을 때
	// 게임에서는 블로킹이 일어날경우 문제가 발생할 수 있음

	// 논블로킹(Non-Blocking) 소켓
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;
	
	// 논블로킹 소켓으로 전환
	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	SOCKADDR_IN clientAddr;
	int32 addrLen = sizeof(clientAddr);

	// Accept
	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// Error
			break;
		}

		cout << "Client Connected!" << endl;

		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
				// Error
				break;
			}
			else if (recvLen == 0)
			{
				// 연결 끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;

			// Send
			while (true)
			{
				if (::send(clientSocket, recvBuffer, recvLen, 0) == SOCKET_ERROR)
				{
					// 원래 블록했어야 하는데 논블로킹이기 때문에 넘어감
					if (::WSAGetLastError() == WSAEWOULDBLOCK)
						continue;
					// Error
					break;
				}

				cout << "Send Data Len = " << recvLen << endl;
				break;
			}
		}
	}

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

무한루프를 돌면서 확인하기 때문에 비효율이 발생할 수 있음    

***

# Select 모델

```cpp
// GameServer.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;
	
	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	// Select 모델 : select 함수가 핵심
	// 소켓 함수 호출이 성공할 시점을 미리 알 수 있음
	// 블로킹 소켓에 적용 - 조건이 만족되지 않아 블로킹되는 상황 예방
	// 논블로킹 소켓에 적용 - 조건이 만족되지 않아 불필요하게 반복 체크하는 상황을 예방

	// socket set
	// 1) 읽기[] 쓰기[] 예외(OOB, OutofBand)[] 관찰 대상 등록
	// OutofBand는 send() 마지막 인자 MSG_OOB로 보내는 특별한 데이터
	// 받는 쪽에서도 recv OOB 세팅을 해야 읽을 수 있음
	// 2) select(readSet, writeSet, exceptSet); - 관찰 시작
	// 3) 적어도 하나의 소켓이 준비되면 리턴 - 낙오자는 자동으로 제거됨
	// 4) 남은 소켓 체크해서 진행

	// fd_set read;
	// FD_ZERO(set) - 비움 / FD_SET(s, &set) - 소켓 s를 넣음
	// FD_CLR(s, &set) - 소켓 s를 제거
	// FD_ISSET(s, &set) - 소켓 s가 set에 들어있으면 0이 아닌 값을 리턴

	vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		// 소켓 셋 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		// ListenSocket 등록
		FD_SET(listenSocket, &reads);

		// 소켓 등록
		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}
		
		// 마지막 인자는 timeout
		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;

		// Listener 소켓 체크
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		// 나머지 소켓 체크
		for (Session& s : sessions)
		{
			// Read
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// todo : sessions 제거
					continue;
				}
				s.recvBytes = recvLen;
			}

			// Write
			if (FD_ISSET(s.socket, &writes))
			{
				// 블로킹 - 모든 데이터 다 보냄
				// 논블로킹 - 상대방 수신 버퍼 상황에 따라 일부만 보낼 수 있음
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR)
				{
					// todo : sessions 제거
					continue;
				}
				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
			}
		}
	}

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

FD_SETSIZE에 한계가 존재한다는 문제점이 있음    
단, FD_SET을 여러개 둠으로써 해결은 가능함    
