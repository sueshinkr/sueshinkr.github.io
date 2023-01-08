---
title:  "Server lecture section3 [2/5]"
excerpt: "네트워크 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.29 18:00:00
---

# TCP 서버 실습

클라이언트에서 데이터 송신 - 서버에서 수신 - 수신한 데이터를 다시 클라이언트에 송신 - 클라이언트에서 수신    

```cpp
// DummyClient.cpp

int main
{
	// socket() -> connect()
	...
	...

	// ----------------
	// 연결 성공! 데이터 송수신 가능

	cout << "Connected To Server!" << endl;

	while (true)
	{
		// Todo
		char sendBuffer[100] = "Hello World!";

		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Send ErrorCode : " << errCode << endl;
			return 0;
		}
		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;


		// 서버에서 echo한거 수신
		char recvBuffer[1000];

		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Recv ErrorCode : " << errCode << endl;
			return 0;
		}

		cout << "Recv Data! Data = " << recvBuffer << endl;
		cout << "Recv Data! Len = " << recvLen << endl;

		this_thread::sleep_for(1s);
	}

	// 소켓 리소스 반환
	::closesocket(clientSocket);
	// winsock 종료
	::WSACleanup();
}
```

`send()` 함수로 데이터를 송신    
* 첫 번째 매개변수로 사용할 소켓의 디스크립터 지정     
* 두 번째 매개변수로 보낼 데이터를 지정    
* 세 번째 매개변수에는 보낼 데이터의 크기를 지정    
* 네 번째 매개변수로 플래그 설정(보통은 0)    

`recv()` 함수로 데이터를 수신, 매개변수의 사용은 `send()`와 동일함    

[`send()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send)    
[`recv()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv)    

```cpp
// GameServer.cpp

int main()
{
	// socket() -> bind() -> listen() -> accept()
	...
	...

	while (true)
	{
		...
		...

		// 손님 입장
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected! IP = " << ipAddress << endl;

		// TODO
		while (true)
		{
			// 수신할 데이터의 크기를 알 수 없기 때문에 버퍼를 넉넉하게
			char recvBuffer[1000];

			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Recv ErrorCode : " << errCode << endl;
				return 0;
			}

			cout << "Recv Data! Data = " << recvBuffer << endl;
			cout << "Recv Data! Len = " << recvLen << endl;

			// echo
			int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Send ErrorCode : " << errCode << endl;
				return 0;
			}
		}
	}
}
```

`send` `recv` 함수는 `blocking` 함수    
클라이언트에서 데이터를 송신했으나 서버에서 수신하지 않은 경우, `send` 함수는 정상적으로 넘어간 후 `recv` 함수에서 블로킹됨    
* 이는 `send`로 보내는 데이터가 클라이언트측 커널레벨의 `SendBuffer`에 저장되기 때문
* `SendBuffer`에 저장된 데이터는 서버측 커널레벨의 `RecvBuffer`으로 보내짐
* 서버에서 `recv`를 하면 `RecvBuffer`의 데이터를 가져오며 수신 완료

즉, `send` 작업은 `SendBuffer`가 가득 차있는 경우 블로킹됨    
반면 `recv` 작업은 `RecvBuffer`가 비어있는 경우 블로킹됨    

`TCP`는`recv`할 때 버퍼에있는 데이터를 한번에 가져온다는 특징이 있음    
즉, 데이터에 바운더리(Boundary)가 없어 버퍼 크기에 맞춰서 송수신됨    

***

# TCP vs UDP

상품 - 배송정책 - 최종주소 - 단지경로 - 택배운송    
어플리케이션 - 트랜스포트 - 네트워크 - 데이터 링크 - 피지컬    

TCP : 안전한 트럭, 전화 연결 방식
* 연결형 서비스 - 연결을 위해 할당되는 논리적인 경로가 존재, 전송 순서 보장    
* 신뢰성은 높지만(분실시 재전송) 속도는 느림(흐름/혼잡제어)    
* 바운더리 개념이 없음    

UDP : 위험한 총알 배송, 이메일 전송 방식 
* 비연결형 서비스 - 연결 개념이 없으며 전송 순서가 보장되지 않음    
* 신뢰성은 낮지만(분실에 대한 책임이 없음) 속도는 빠름    
* 바운더리 개념이 있음    

***

# UDP 서버 실습

```cpp
// DummyClient.cpp
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

	// 소켓 생성
	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr; // IPv4용
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connected UDP : 클라이언트와 서버가 오랫동안 통신할 경우의 효율성을 위해 사용
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	// ----------------

	while (true)
	{
		// Todo
		char sendBuffer[100] = "Hello World!";

		// Unconnected UDP
		// sendto를 통해 자신의 IP주소 + 포트 번호가 설정됨
		int32 resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
			(SOCKADDR*)&serverAddr, sizeof(serverAddr));

		// Connected UDP
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		
		if (resultCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

		// 서버에서 echo한거 수신
		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		// Unconnected UDP
		int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
			(SOCKADDR*)&recvAddr, &addrLen);

		// Connected UDP
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Recv Data! Data = " << recvBuffer << endl;
		cout << "::Recv Data! Len = " << recvLen << endl;
		
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

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr; // IPv4용
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		HandleError("Bind");
		return 0;
	}
	
	while (true)
	{
		// todo
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		char recvBuffer[1000];
		int32 recvLen = ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0, 
			(SOCKADDR*)&clientAddr, &addrLen);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Recv Data! Data = " << recvBuffer << endl;
		cout << "Recv Data! Len = " << recvLen << endl;

		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
			(SOCKADDR*)&clientAddr, sizeof(clientAddr));

		if (errorCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data! Len = " << recvLen << endl;
	}

	// winsokc 종료
	::WSACleanup();
}
```

UDP 소켓은 기본적으로 `unconnected UDP`로 생성됨   
`unconnected UDP`는 데이터를 송신할 때마다 아래의 과정을 수행
* UDP 소켓에 목적지 IP, 포트 정보 등록
* 데이터 전송
* 소켓에 등록된 정보 삭제    

따라서 해당 과정을 수행할 수 있는 `sendto()`, `recvfrom()` 함수를 사용해야함    

[`snedto()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-sendto)    
[`recvfrom()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recvfrom)    

<br/>

하나의 서버와 지속적으로 자주 통신해야 하는 경우 `unconnected UDP` 소켓을 사용하는건 비효율적이기 때문에 `connected UDP` 소켓을 사용    
간단하게 `connect()` 함수를 호출하여 `connected UDP`로 변경할 수 있음    
이 때는 목적지 정보가 저장되어있으므로 `send()`, `recv()` 함수도 사용 가능    
