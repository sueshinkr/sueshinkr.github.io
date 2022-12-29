---
title:  "Server lecture section3 [4/5]"
excerpt: "네트워크 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.12.01 15:00:00
---

# WSAEventSelect 모델

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

	// WSAEventSelect : WSAEventSelect 함수가 핵심
	// 소켓과 관련된 네트워크 이벤트를 이벤트 객체를 통해 감지
	// Select는 동기, EventSelect는 비동기

	// 이벤트 객체 관련 함수
	// 생성 : WSACreateEvent - 수동 리셋(Manual-Reset) + Non-Signaled 상태 시작
	// 삭제 : WSACloseEvent
	// 신호 상태 감지 : WSAWaitForMultipleEvents
	// 구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents

	// 소켓 <-> 이벤트 객체 연동
	// WSAEventSelect(socket, event, networkEvents);

	// 관찰할 네트워크 이벤트
	// FD_ACCEPT : 접속한 클라가 있는지 여부 - accept
	// FD_READ : 데이터 수신 가능 여부 - recv, recvfrom
	// FD_WRITE : 데이터 송신 가능 여부 - send, sendto
	// FD_CLOSE : 상대방 접속 종료 여부 
	// FD_CONNECT : 통신을 위한 연결 절차 완료 여부
	// FD_OOB :

	// 주의사항
	// WSAEventSelect 함수 호출시 해당 소켓은 자동으로 논블로킹 모드 전환
	// accept() 함수가 리턴하는 소켓은 listenSocket과 동일한 속성을 가짐
	// 따라서 clientSocket은 FD_READ, FD_WRITE 등을 다시 등록해야함
	// 드물게 WSAEWOULDBLOCK 오류가 발생할 수 있으니 예외 처리 필요
	// 중요!!) 이벤트 발생시 적절한 소켓 함수를 호출해야함
	//		   그렇지 않으면 다음 번에는 동일 네트워크 이벤트가 발생하지 않음
	//		   ex) FD_READ 이벤트 발생시 recv()를 호출해야하고, 호출하지 않으면 FD_READ는 다시 발생하지 않음

	// WSAWaitForMultipleEvent(count, event, waitAll, timeout, fAlertable)
	// 1) count, event
	// 2) waitAll : 모두 기다릴지, 혹은 하나만 완료 되어도 OK일지
	// 3) timeout
	// 4) 지금은 false
	// return : 완료된 첫번째 인덱스

	// WSAEnumNetworkEvents(socket, eventObject, networkEvent)
	// eventObject : socket과 연동된 이벤트 객체 핸들을 넘겨줄시 이벤트 객체를 non-signaled 상태로 변경
	// networkEvent : 네트워크 이벤트 / 오류 정보가 저장
	

	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);

	WSAEVENT listenEvent = ::WSACreateEvent();
	wsaEvents.push_back(listenEvent);
	sessions.push_back(Session{ listenSocket });
	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
		return 0;

	while (true)
	{
		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED)
			continue;
		index -= WSA_WAIT_EVENT_0;

		//::WSAResetEvent(wsaEvents[index]);
		// reset기능 자체가 WSAEnumNetworkEvents 함수에 포함되어있음

		WSANETWORKEVENTS networkEvents;
		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
			continue;

		// Listener 소켓 체크
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			// Error-Check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);

			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;

				WSAEVENT clientEvent = ::WSACreateEvent();
				wsaEvents.push_back(clientEvent);
				sessions.push_back(Session{ clientSocket });
				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					return 0;
			}
		}

		// Client Session 소켓 체크
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			// Error-Check
			if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT] != 0))
				continue;
			if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT] != 0))
				continue;

			Session& s = sessions[index];

			//Read
			if (s.recvBytes == 0)
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// todo : remove session
					continue;
				}

				s.recvBytes = recvLen;
				cout << "Recv Data = " << recvLen << endl;
			}

			// Write
			if (s.recvBytes > s.sendBytes)
			{
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// todo : remove session
					continue;
				}

				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
				cout << "Send Data = " << sendLen << endl;
			}
		}

		// FD_CLOSE 처리
		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{
			// todo : remove Socket
		}
	}

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

`WSACreateEvent()` 함수로 `WSAEVENT` 객체를 생성    
`WSACloseEvent()` 함수로 객체 삭제    
* 생성된 객체는 `non-signaled` 상태로 `manual-reset`되어있음    

`WSAEventSelect()` 함수로 소켓과 이벤트 객체를 연동, 소켓과 관련된 네트워크 이벤트를 이벤트 객체를 통해 감지    
* `select()`와 달리 비동기로 진행     
* `select()`와 달리 매번마다 소켓 set을 리셋할 필요가 없음    
* 첫 번째 매개변수로 소켓을 지정    
* 두 번째 매개변수로 이벤트 객체를 지정    
* 세 번째 매개변수로 네트워크 이벤트 비트마스크를 설정    
	* `FD_ACCEPT` / `FD_READ` / `FD_WRITE` / `FD_CONNECT` / `FD_CLOSE` 등이 존재    

`WSAWaitForMultipleEvents()` 함수로 신호 상태를 감지    
* 첫 번째 매개변수로 이벤트 객체 핸들의 개수를 설정    
* 두 번째 매개변수로 이벤트 객체 핸들을 지정    
* 세 번째 매개변수로 대기시 동작을 결정
	* `TRUE` : 모든 이벤트 핸들이 `signaled`일 경우 리턴
	* `FALSE` : 최소한 하나의 이벤트 객체가 `signaled`일 경우 리턴
		* 이 때(리턴값 - WSA_WAIT_EVENT_0) 값이 함수를 리턴시킨 이벤트 객체의 인덱스를 나타냄     
		* `signaled`인 이벤트 객체가 여러개일 경우 인덱스 값이 가장 작은 이벤트 객체에 대한 값이 리턴됨    
* 네 번째 매개변수로 타임아웃 시간을 설정    
* 다섯 번째 매개변수로 입출력 완료 루틴을 설정, `WSAEventSelect` 모델에서는 항상 `FALSE`로 설정    

`WSAEnumNetworkEvents()` 함수로 구체적인 네트워크 이벤트를 확인    
* 첫 번째 매개변수로 소켓을 지정
* 두 번째 매개변수로 소켓과 연동된 이벤트 객체 핸들을 넘겨주어 `non-signaled` 상태로 변경
* 세 번째 매개변수에는 네트워크 이벤트 / 오류 정보가 저장됨    

`WSAWaitForMultipleEvents`에도 전체 이벤트 개수에 한계가 존재한다는 단점이 있음    

[`WSACreateEvent()`에 대한 자세한 설명](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsacreateevent)    
[`WSACloseEvent()`에 대한 자세한 설명](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsacloseevent)    
[`WSAWaitForMultipleEvents()`에 대한 자세한 설명](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsawaitformultipleevents)    
[`WSAEnumNetworkEvents()`에 대한 자세한 설명](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaenumnetworkevents)    

***

# Overlapped 모델(이벤트 기반)

블로킹(BLOCKING) / 논블로킹(NONBLOCKING) / 동기(SYNCHRONOUS) / 비동기(ASYNCHRONOUS)

블로킹 vs 논블로킹
* 블로킹 : 함수 호출시 다른 일 못하고 호출한 함수가 반환될때까지 대기
* 논블로킹 : 함수 호출시 거의 바로 반환되어 동시에 다른 일 수행 가능

동기 vs 비동기
* 전화로 질문 - 동기 / 메일로 질문 - 비동기
* 비동기는 함수 호출을 Callback으로 함
* `recv`, `send`, `accept`와 같은 기본 함수들은 모두 동기 함수    

[블록/논블록, 동기/비동기`에 대해 자세히 정리되어있는 글](https://inpa.tistory.com/entry/%F0%9F%91%A9%E2%80%8D%F0%9F%92%BB-%EB%8F%99%EA%B8%B0%EB%B9%84%EB%8F%99%EA%B8%B0-%EB%B8%94%EB%A1%9C%ED%82%B9%EB%85%BC%EB%B8%94%EB%A1%9C%ED%82%B9-%EA%B0%9C%EB%85%90-%EC%A0%95%EB%A6%AC)    

<br/>

`select` 모델은 동기/블록    
입출력 함수를 안전하게 호출할 수 있는 시점을 운영체제로부터 전달받는 `통지(Notification)` 또한 동기적으로 이루어짐    
즉, 사건을 등록하는 함수 호출/반환 시점과 사건 발생을 통지하는 시점이 일치하며 `select()` 함수의 반복을 통해 입출력이 완료 가능한 상태인지를 계속 체크함     

`WSAEventSelect` 모델은 동기/논블록 방식    
`WSAEventSelect()` 함수는 입출력 완료 여부에 상관없이 바로 반환되기 때문에 입출 력 통지가 비동기적으로 이루어짐    
즉, 사건을 등록하는 함수 호출/반환 시점과 사건 발생을 통지하는 시점이 일치하지 않음    
이를 `비동기 통지(asynchronous notification)`이라고 부르며, 입출력 방식과 통지 방식의 동기/비동기 여부는 별개이기 때문에 `WSAEventSelect()`는 동기 입출력 방식과 비동기 통지 방식을 가짐    

동기 입출력 방식인 `select`와 `WSAEventSelect` 모델은 모두 입출력 완료 시점과 함수 리턴 시점이 일치함    
다만 통지를 받기 때문에 단순한 동기 입출력 방식보다는 소켓들을 편리하게 처리할 수 있음    
반면 `Overlapped`와 `Completion Port` 모델은 비동기 입출력 방식이며, 비동기 통지가 결합되어있음    
따라서 해당 모델들은 입출력 완료 시점과 함수 리턴 시점이 일치하지 않음    

[각 모델에 따른 동기/비동기 차이점에 대해 정리되어있는 글](https://dbehdrhs.tistory.com/85)    

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
	//int32 sendBytes = 0;
	WSAOVERLAPPED overlapped = {};
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

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

	// Overlapped IO (비동기 + 논블로킹)
	// - Overlapped 함수를 호출 (WSARecv, WSASend 등)
	// - Overlapped 함수가 성공했는지 확인
	// 성공시 결과 얻어서 처리, 실패시 사유 확인

	// WSASend
	// WSARecv
	// 1) 비동기 입출력 소켓
	// 2) WSABUF 배열의 시작 주소 + 개수 
	//	  Scatter-Gather 기능을 위해 배열을 사용
	// 3) 보내거나 받은 바이트 수
	// 4) 상세 옵션, 기본으로 0 사용
	// 5) WSAOVERLAPPED 구조체 주소값
	// 6) 입출력이 완료되면 OS가 호출할 콜백 함수(이벤트 사용시 사용 안함)

	// Overlapped 모델 (이벤트 기반)
	// - 비동기 입출력 지원하는 소켓 생성 + 통지받기 위한 이벤트 객체 생성
	// - 비동기 입출력 함수 호출 (위에서 만든 이벤트 객체를 같이 넘겨줌)
	// - 비동기 작업이 바로 완료되지 않으면 WSA_IO_PENDING 오류 코드
	// - 운영체제는 이벤트 객체를 signaled 상태로 만들어 완료 상태 알려줌
	// - WSAWaitForMultipleEvents 함수를 호출해서 이벤트 객체의 signal 판별
	// - WSAGetOverlappedResult 함수 호출해서 비동기 입출력 결과 확인 및 데이터 처리
	//	 1) 비동기 소켓
	//	 2) 넘겨준 overlapped 구조체
	//	 3) 전송된 바이트 수
	//   4) 비동기 입출력 작업이 끝날때까지 대기할지 여부
	//		WSAWaitForMultipleEvents 함수 사용시 이미 비동기 입출력 작업이 끝난것이므로 False로 설정
	//   5) 비동기 입출력 작업 관련 부가 정보, 거의 사용 안함

	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket;
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
				break;
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 문제있는 상황
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session.overlapped.hEvent = wsaEvent;

		cout << "Client Connected !" << endl;

		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
				}
				else
				{
					// todo : 문제상황
					break;
				}
			}
			cout << "Data Recv Len = " << recvLen << endl;
		}

		::closesocket(session.socket);
		::WSACloseEvent(wsaEvent);
	}

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

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
	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	// Send
	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = sendBuffer;
		wsaBuf.len = 100;

		DWORD sendLen = 0;
		DWORD flags = 0;
		if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				// Pending
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, FALSE, &flags);
			}
			else
			{
				// 진짜 문제가 있는 상황
				break;
			}
		}

		cout << "Send Data Len = " << sizeof(sendBuffer) << endl;

		this_thread::sleep_for(1s);
	}
	
	// 소켓 리소스 반환
	::closesocket(clientSocket);
	// winsock 종료
	::WSACleanup();
}
```

`WSARecv` 도중 `overlapped`나 `recvBuffer`의 값을 변경하지 않도록 주의해야함    
