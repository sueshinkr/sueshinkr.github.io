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

이벤트 사용시
* 소켓 하나당 이벤트 하나 연동
* 다수의 이벤트 체크시의 한계 존재

콜백 사용시
* `WaitForSingleObjectEx`, `WaitForMultipleObjectsEx`, `SleepEx` 등을 통해 APC queue로 진입하는 순간 예약된 모든 콜백함수들을 실행
* 콜백 함수는 `LPWSAOVERLAPPED`로 선언한 `overlapped`의 주소를 매개변수로 받는데, 이를 `session`의 주소로 캐스팅하여 사용할 수 있음(단, `overlapped`가 `session` 구조체의 처음에 위치하도록 구조가 짜여있어야 함)    

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

	// ICOP (Completion Port) 모델
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
