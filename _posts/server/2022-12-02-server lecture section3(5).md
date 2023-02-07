---
title:  "Server lecture section3 [5/5]"
excerpt: "네트워크 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.12.02 18:00:00
---

# Overlapped 모델(콜백 기반)

```cpp
// GameServer.cpp

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

const int32 BUFSIZE = 1000;

struct Session
{
	WSAOVERLAPPED overlapped = {};
	SOCKET socket = INVALID_SOCKET; 
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
};

//여기서 CALLBACk은 _stdcall로 함수의 호출 규약, 없어도 상관없음
void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback = " << recvLen << endl;
	// TODO - 에코 서버를 만든다면 WSASend()

	Session* session = (Session*)overlapped;
}

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

	// Overlapped 모델 (Competion Routine 콜백 기반)
	// - 비동기 입출력 지원하는 소켓 생성
	// - 비동기 입출력 함수 호출 (완료 루틴의 시작 주소를 넘겨줌)
	// - 비동기 작업이 바로 완료되지 않으면 WSA_IO_PENDING 오류 코드
	// - 비동기 입출력 함수를 호출한 쓰레드를 Alertable wait 상태로 만듬
	//	 ex) WaitForSingleObjectEx, WaitForMultipleObjectsEx, SleepEx, WSAWaitForMultipleEvents...
	// - 비동기 IO 완료되면 운영체제는 완료 루틴 호출
	// - 완료 루틴 호출이 모두 끝나면 쓰레드는 Alertable Wait 상태에서 빠져나옴 

	// void CompletionRoutine()
	// 1) 오류 발생시 0 아닌 값
	// 2) 전송 바이트 수
	// 3) 비동기 입출력 함수 호출시 넘겨준 WSAOVERLAPPED 구조체의 주소값
	// 4) 0

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

		cout << "Client Connected !" << endl;

		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					// Alertable Wait - APC queue 사용
					::SleepEx(INFINITE, TRUE);
					//::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, TRUE);
				}
				else
				{
					// todo : 문제상황
					break;
				}
			}
			else
			{
				cout << "Data Recv Len = " << recvLen << endl;
			}
		}
		 
		::closesocket(session.socket);
		//::WSACloseEvent(wsaEvent);
	}

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

`_stdcall`은 함수 호출 규약(Calling Convention)의 방식 중 하나    
호출자(Caller)와 피호출자(Callee)간 패러미터 전달방식 및 사용 후 스택 정리에 대한 규칙을 지정    

[함수 호출 규약에 대한 정보가 담긴 글](https://moc0.tistory.com/36)

<br/>

`Competion Routine 콜백` 기반의 `Overlapped` 모델은 이벤트를 사용하는 대신 콜백함수를 사용    
비동기 입출력 함수에서 완료 루틴의 시작 주소를 넘겨주고, 해당 함수를 호출한 쓰레드를 `Alertable wait` 상태로 만듬    
* `Alertable wait` : 비동기 입출력을 위한 특별한 대기 상태, 비동기 입출력 함수를 요청한 쓰레드가 이 상태일 때만 완료 루틴이 호출될 수 있음    
* `WaitForSingleObjectEx()`, `WaitForMultipleObjectsEx()`, `SleepEx()`, `WSAWaitForMultipleEvents()` 등의 함수 사용    
비동기 입출력 작업 결과는 `APC Queue(Asynchronous Procedure Call Queue)`에 저장됨    
* `APC Queue` : 비동기 입출력 결과 저장을 위해 운영체제가 각 쓰레드마다 할당하는 메모리 영역, 쓰레드별로 독립적    
 `APC Queue`를 참조하여 입출력이 완료되면 완료 루틴을 호출하고, 호출이 끝나면 쓰레드가 `Alertable Wait` 상태에서 빠져나옴    

<br/>

루틴으로 사용되는 함수에는 요구되는 형식이 존재    
* 첫 번째 인자로 입출력의 완료 상태를 지정    
* 두 번째 인자로 전송된 데이터의 바이트를 지정, 오류시 0    
* 세 번째 인자로 `WSAOVERLAPPED` 구조체를 지정    
* 네 번째 인자로 플래그를 지정    

[`OVERLAPPED_COMPLETION_ROUTINE`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nc-winsock2-lpwsaoverlapped_completion_routine)    


<br/>

이벤트 사용시
* 소켓 하나당 이벤트 하나 연동
* 다수의 이벤트 체크시의 한계 존재

콜백 사용시
* `WaitForSingleObjectEx`, `WaitForMultipleObjectsEx`, `SleepEx` 등을 통해 APC queue로 진입하는 순간 예약된 모든 콜백함수들을 실행
* 콜백 함수는 `WSAOVERLAPPED` 구조체의 주소를 매개변수로 받는데, 이를 상위 구조체로 묶어둔 `session`의 주소로 캐스팅하여 사용할 수 있음(단, `overlapped`가 `session` 구조체의 처음에 위치하도록 구조가 짜여있어야 함)    

<br/>

모델 정리
* Select 모델
	* 장점 : 윈도우 / 리눅스 공통
	* 단점 : 성능 최하(소켓 셋을 매번 등록), 64개 제한
* WSAAsyncSelect 모델
	* 소켓 이벤트를 윈도우 메세지 형태로 처리
	* 일반 윈도우 메세지와 같이 처리하기때문에 성능이 좋지 않음
* WSAEventSelect 모델
	* 장점 : 비교적 뛰어난 성능
	* 단점 : 64개 제한
* Overlapped (이벤트 기반)
	* 장점 : 성능 뛰어남
	* 단점 : 64개 제한
* Overlapped (콜백 기반)
	* 장점 : 성능 뛰어남
	* 단점 : 모든 비동기 소켓 함수에서 사용 가능하지는 않음 (accept 등), 빈번한 Alertable wait로 인한 성능 저하
* IOCP : 끝판왕

Reactor Pattern : 소켓 상태 확인 후 뒤늦게 `recv`, `send` 호출    
Proactor Pattern : `Overlapped WSA-` 기능을 이용하여 미리 `recv`, `send`를 걸어놓음    

***

# Completion Port 모델

```cpp
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

const int32 BUFSIZE = 1000;

struct Session
{
	WSAOVERLAPPED overlapped = {};
	SOCKET socket = INVALID_SOCKET; 
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
};

enum IO_TYPE
{
	READ,
	WRITE,
	ACCEPT,
	CONNECT,
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	int32 type = 0; // read, write, accept, connect
};

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);

		if (ret == FALSE || bytesTransferred == 0)
		{
			// TODO : 연결 끊김
			continue;
		}

		ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);
		
		cout << "Recv Data IOCP = " << bytesTransferred << endl;

		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		DWORD recvLen = 0;
		DWORD flags = 0;
		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
	}
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
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

	// Overlapped 모델 (Competion Routine 콜백 기반)
	// - 비동기 입출력 함수가 완료되면 쓰레드마다 있는 APC 큐에 쌓임
	// - Alertable Wait 상태로 들어가서 APC 큐 비우기(콜백 함수)
	// 단, APC큐가 쓰레드마다 있다는 단점이 존재

	// IOCP (Completion Port) 모델
	// - APC X -> Completion Port(쓰레드마다 존재하지 않음, 중앙에서 관리하는 APC 큐)
	// - Alertable Wait X -> CP 결과 처리를 GetQueuedCompletionStatus에서 함
	// 멀티쓰레드와 궁합이 굉장히 좋음

	// CreateIoCompletionPort
	// GetQueueCompletionStatus

	vector<Session*> sessionManager;

	// CP 생성
	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// WorkerThreads
	for (int32 i = 0; i < 5; i++)
		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
	
	// Main Thread = Accept 담당
	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;

		Session* session = new Session();
		session->socket = clientSocket;
		sessionManager.push_back(session);

		cout << "Client Connected !" << endl;

		// 소켓을 CP에 등록
		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, 
								/*key*/(ULONG_PTR)session, /*최대코어개수만큼할당*/0);
		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		OverlappedEx* overlappedEx = new OverlappedEx();
		overlappedEx->type = IO_TYPE::READ;

		DWORD recvLen = 0;
		DWORD flags = 0;
		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);

		// 유저가 게임 접속 종료 -> 문제발생
		Session* s = sessionManager.back();
		sessionManager.pop_back();
		delete s;
		//::closesocket(session.socket);
		//::WSACloseEvent(wsaEvent);
	}

	GThreadManager->Join();

	// 소켓 리소스 반환
	::closesocket(listenSocket);

	// winsock 종료
	::WSACleanup();
}
```

`Completion Port` 모델은 `APC Queue` 대신 `Completion Port`를 사용하는 것이 핵심    
비동기 입출력 결과 및 쓰레드에 대한 정보를 담고있는 구조이며, `CreateIoCompletionPort()` 함수로 생성    
* 첫 번째 인자로 `overlapped` 입출력을 지원하는 객체로서의 핸들, 또는 `INVALID_HANDLE_VALUE`로 지정함    
* 두 번째 인자로 이미 존재하는 `completion port`를, 없을 경우 `NULL`로 지정    
* 세 번째 인자로 `I/O completion packet`에 들어갈 부가적인 정보인 `CompletionKey`를 입력
* 네 번째 인자로 동시에 실행할 수 잇는 작업자 쓰레드의 개수를 설정, 0으로 설정시 CPU 개수와 같을 수로 설정됨

보통 `Completion Port`에 접근하는 쓰레드인 작업자(Worker) 쓰레드를 CPU 개수 * n개만큼 생성하여 사용함    
작업자 쓰레드에서 `Completion Port`에 저장된 결과를 감시하고 처리하는 `GetQueuedCompletionStatus()` 함수를 호출하여 사용    
* 첫 번째 인자로 `Completion Port`를 지정    
* 두 번째 인자에 비동기 입출력 작업으로 전송된 바이트 수를 저장    
* 세 번째 인자에 `COmpletionKey`를 저장    
* 네 번째 인자에 비동기 입출력 함수 호출시 사용한 `OVERLAPPED` 구조체를 받아옴    
* 네 번째 인자로 `CreateIoCompletionPort()`시 사용한 `CompletionKey`를 받아옴    
* 다섯 번째 인자로 작업자 쓰레드가 대기할 시간을 지정, `INFINITE`로 설정시 입출력 완료 패킷이 생성되어 운영체제가 깨울 때까지 무한정 대기    

[`Overlapped` / `Completion Port` 모델에 대한 자세한 내용이 적힌 글](https://dbehdrhs.tistory.com/85)    
[`CreateIoCompletionPort()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/fileio/createiocompletionport)    
[`GetQueuedCompletionStatus()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-getqueuedcompletionstatus)    
