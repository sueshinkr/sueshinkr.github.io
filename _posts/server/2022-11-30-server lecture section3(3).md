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

`setsockopt()` 함수로 소켓 세부 설정이 가능    
* 첫 번째 매개변수로 설정을 변경할 소켓을 지정    
* 두 번째 매개변수로 옵션을 설정할 프로토콜의 레벨을 전달
* 세 번째 매개변수로 설정할 옵션 지정
* 네 번째 매개변수로 옵션 요청에 대한 결과를 확인할 버퍼 지정
* 다섯 번째 매개변수로 버퍼의 크기 지정    

공식 문서에서 볼 수 있듯 적용할 수 있는 옵션의 갯수가 상당히 많음    
* `SO_KEEPALIVE` : 주기적으로 연결 상태를 확인 (TCP 연결만 가능)
* `SO_LINGER` : 소켓 리소스 반환 후 (`closesocket()` 호출 후) 전송되지 않은 데이터가 있는 경우 (송신 버퍼에 데이터가 남아있는 경우) 소켓을 즉시 닫을 것인지 아닐지를 결정    
	* `LINGER` : 데이터가 전송되기 위해 대기중이거나 `closesocket()` 함수가 호출되었을 때 소켓의 동작방식에 대한 정보를 저장해놓은 구조체
	* ```cpp
	  typedef struct linger {
		u_short l_onoff;
		u_short l_linger;
		} LINGER, *PLINGER, *LPLINGER;
	  ```
	* `l_onoff` : `closesocket()` 호출시 대기중인 데이터를 전송하기 위해 소켓을 열어둘 것인지를 설정    
	* `l_linger` : `l_onoff`가 `nonzero` 값으로 설정되어있을 경우, 얼마만큼의 시간동안 소켓을 열어둘 것인지를 설정    
* `SO_REUSEADDR` : 이미 사용중인 주소나 포트에 대해서도 바인드 허용    
* `TCP_NODELAY` : TCP 소켓의 NAGLE 알고리즘 작동 여부를 설정, 기본값은 작동으로 설정되어있음    
	* NAGLE 알고리즘 - 데이터가 충분히 크면 전송하고, 그렇지 않으면 충분히 쌓일 때까지 대기    
	* 게임에서는 트래픽 효율보다 빠른 반응성이 유리하기 때문에 잘 쓰이지 않음    

`getsockopt()` 함수로 소켓 세부설정 사항을 확인 가능    

[`setsockopt()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt)    
[`LINGER`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-linger)    
[`getsockopt()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt)    

<br/>

`shutdown()` 함수로 소켓을 닫을 수 있음    
두 호스트간의 연결을 완전 종료시키는 `closesocket()`과는 달리 `half-close`가 가능    
즉, 송신은 가능하지만 수신은 불가능하거나, 그 반대의 상황을 만들 수 있음    
따라서 마지막 데이터의 송신 시점을 확인하는데 사용

[`shutdown()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-shutdown)    

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

`accept()`, `connect()`, `recv()`, `send()` 등은 모두 블로킹 함수    
호출 쓰레드를 블로킹 상태로 만들기 때문에 조건 만족 혹은 타임아웃시까지 블로킹 상태가 유지됨    
그렇기 때문에 클라이언트가 여러개일 때 일부가 블로킹될경우, 서버가 블로킹된 클라이언트의 패킷을 기다리며 대기하게 되기 때문에 문제가 발생할 수 있음    

`ioctlsocket()` 함수로 소켓을 논블로킹(nonblocking)으로 전환 가능    
* 첫 번째 매개변수로 설정할 소켓을 지정    
* 두 번째 매개변수인 커맨드에 `FIONBIO`, 세 번째 매개변수인 설정값을 1로 놓으면 노블로킹 소켓으로 설정됨    

[`ioctlsocket()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-ioctlsocket)    
[`ioctl` 커맨드에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/winsock/winsock-ioctls)    


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

논블로킹 소켓에서는 원래 블록되어야 할 경우 대신 -1을 반환하고 `WSAGetLastError()` 값을 `WSAEWOULDBLOCK`으로 설정함    
따라서 while문 내에서 `connect()`, `send()` 등을 수행하며 `WSAEWOULDBLOCK`인 경우 계속 루프를 돌도록 하면 논블로킹 소켓을 활용할 수 있음    
단, 논블로킹 소켓의 경우 블로킹 상황 발생 여부를  무한루프를 돌면서 확인하기 때문에 비효율이 발생할 수 있음    

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

`fd_set` : 소켓을 FD로 그룹짓기 위해 사용되는 구조체    
```cpp
typedef struct fd_set {
  u_int  fd_count;
  SOCKET fd_array[FD_SETSIZE];
} fd_set, FD_SET, *PFD_SET, *LPFD_SET;
```
* `fd_count` : 집합에 존재하는 소켓 수    
* `fd_array[FD_SETSIZE]` : 집합에 존재하는 소켓 배열    

`fd_set`을 조작하는 매크로가 존재    
* `FD_ZERO(*set)` : set을 empty로 초기화    
* `FD_CLR(s, *set)` : set에서 소켓 s를 제거    
* `FD_ISSET(s, *set)` : 소켓 s가 set에 존재하는지 확인    
* `FD_SET(s, *set)` : 소켓 s를 set에 추가    

<br/>

`select()` 함수는 `send()`, `recv()` 함수 등의 호출이 성공할 수 있는 시점을 알려줌    
소켓이 블로킹이든 논블로킹이든 관계없이 여러 소켓을 한 쓰레드로 처리할 수 있음    
* 블로킹 소켓 : 함수 호출의 조건이 만족되지 않아 블로킹되는 상황 예방     
* 논블로킹 소켓 : 함수 호출의 조건이 만족되지 않아 불필요하게 반복 체크하는 상황을 예방
* 첫 번째 매개변수는 가장 큰 fd의 값으로 일반적으로 무시됨
* 두 번째 매개변수에는 `read set`을 지정
* 세 번째 매개변수에는 `write set`을 지정
* 네 번째 매개변수에는 `exception set`을 지정
* 다섯 번째 매개변수에는 타임아웃 시간을 지정, 타임아웃이 되면 `select()` 함수는 무조건 리턴
	* 타임아웃 값이 `NULL`일 경우 : 최소한 한 소켓이 조건을 만족할 때까지 무한히 대기하고, 조건을 만족하는 소켓 개수를 리턴함
	* 타임아웃 값이 {0, 0}일 경우 : 소켓 셋에 포함된 모든 소켓을 검사한 후, 조건을 만족하는 소켓의 개수를 리턴    
	* 타임아웃 값이 양수 : 최소한 한 소켓이 조건을 만족하거나 타임아웃 시간이 지나면 조건을 만족하는 소켓 개수를 리턴    

`select()` 함수 호출시 각 소켓 set에서 함수 호출이 성공할 수 있는 소켓만 집합에 남겨두고 나머지는 제거    
따라서 다음에는 다시 `fd_set`에 소켓들을 추가해주어야함    

[`select()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select)    

`FD_SETSIZE`에 한계가 존재한다는 문제점이 있음    
단, `fd_set`을 여러개 둠으로써 해결은 가능함    
