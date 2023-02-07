---
title:  "Server lecture section4 [1/6]"
excerpt: "네트워크 라이브러리 제작"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.12.5 18:00:00
---

# Socket Utils

```cpp
// SocketUtils.h

#include "NetAddress.h"

/*------------------
	SocketUtils
-------------------*/

class SocketUtils
{
public:
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
	static SOCKET CreateSocket();

	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}
```

`LPFN_`는 각각 뒤에 붙는 함수들의 함수포인터    
`ConnectEx()`는 클라이언트에서 서버로 접속할 때 사용하는 `connect()` 함수의 비동기 버전으로 사용    
* 첫 번째 인자로 바인드된 소켓 지정
* 두 번째 인자로 `sockaddr` 구조체 지정
* 세 번째 인자로 `sockaddr` 구조체의 길이 입력
* 네 번째 인자에는 연결 후 곧바로 데이터를 전송할 경우 사용할 버퍼를 지정
* 다섯 번째 인자에는 네 번째 인자가 가리키는 버퍼의 크기를 입력
* 여섯 번째 인자로 네 번째 인자 설정시 실제로 전송된 바이트 수를 받아옴
* 일곱 번째 인자로 `LPWSAOVERLAPPED` 구조체를 지정

`DisconnectEx()`와 `AcceptEx()` 역시 비동기 입출력 방식으로 작동    

[`LPFN_CONNECTEX`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_connectex)    
[`DisconnectEx()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms737757(v=vs.85))    
[`AccetEx()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-acceptex)    


```cpp
// SocketUtils.cpp

#include "SocketUtils.h"

/*------------------
	SocketUtils
-------------------*/

LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);

	// 런타임에 주소 얻어오는 API
	SOCKET dummySocket = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	Close(dummySocket);
}

void SocketUtils::Clear()
{
	::WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

// ListenSocket의 특성을 ClientSocket에 그대로 적용
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}
```

윈도우에서는 소켓 생성시 `socket()` 외에 `WSASocket()` 함수도 사용 가능    
* 첫 번째, 두 번째, 세 번째 인자는 `socket()` 함수와 동일
* 네 번째 인자에 생성할 소켓의 특성을 정의하는 `LPWSAPROTOCOL_INFO` 구조체를 지정    
* 다섯 번째 인자에 소켓 그룹 ID 지정, 0 입력시 그룹 작업이 수행되지 않음    
* 여섯 번째 인자에 추가 소켓 특성을 지정하는 플래그를 지정
	* `WSA_FLAG_OVERLAPPED` : overlapped I/O를 지원하는 소켓 생성
	* 기본적으로 winsock에서는 `socket()` 함수로 소켓 생성시 `WSA_FLAG_EVERLAPPED` 옵션이 적용되어있음    
[`WSASocket()` 에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/Winsock2/nf-winsock2-wsasocketa)    

<br/>

`WSAIoctl()` 함수는 소켓의 모드를 제어하는데 사용
* 첫 번째 인자로 제어할 소켓 짖어
* 두 번째 인자로 함수가 수행하는 연산의 제어코드 입력
	* 확장 함수를 호출하기 위해서는 `SIO_GET_EXTENSION_FUNCTION_POINTER`로 함수 포인터를 얻어옴
* `BindWindowsFunction()` 함수 내에서는 런타임시 Winsock 확장 함수(`ConnectEx()`, `DisconnectEx()`, `AcceptEx()`)를 불러오기 위해 사용    
	* 각각 함수에 대한 전역 고유 식별자(GUID)가 존재(`WSAID_CONENCTEX` 등)    
	* 최근 윈도우에서는 확장 함수를 바로 사용할 수도 있다고 함(확인필요)    

[`WSAIoctl()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaioctl)    

<br/>

```cpp
// NetAddress.h

/*----------------
	NetAddress
----------------*/

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	SOCKADDR_IN&	GetSockAddr() { return _sockAddr; }
	wstring			GetIpAddress();
	uint16			GetPort() { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		_sockAddr = {};
};
```

`NetAddress`는 클라이언트들의 주소를 관리하기 위해 사용하는 클래스    

```cpp
// NetAddress.cpp

#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : _sockAddr(sockAddr)
{
}

NetAddress::NetAddress(wstring ip, uint16 port)
{
	::memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr = Ip2Address(ip.c_str());
	_sockAddr.sin_port = ::htons(port);
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &_sockAddr.sin_addr, buffer, len32(buffer));
	return wstring(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}
```

`InetPtonW()` 함수로 `IPv4` 또는 `IPv6` 인터넷 네트워크 주소를 숫자 이진 형식으로 변환    
* 첫 번째 인자로 `IPv4`인지(`AF_NET`) `IPv6`인지(`AF_INET6`) 지정
* 두 번째 인자로 변환할 IP주소를 입력
* 세 번째 인자에 변환한 숫자 이진 표현을 저장

`InetNtopW()` 함수는 네트워크 주소를 텍스트 형식으로 변환    
* 사용할 버퍼를 `WCHAR`형으로 지정했을 경우 버퍼의 크기를 `sizeof(buffer)`가 아닌 `sizeof(buffer) / sizeof(WCHAR)`로 설정해야함    
* 따라서 이를 편하게 하기 위한 매크로를 `Types.h`에 설정

[`InetPtonW()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inetptonw)    
[`InetNtopW()`에 대한 자세한 정보](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inetntopw)    


```cpp
// Types.h

#define size16(val) static_cast<int16>(sizeof(val))
#define size32(val) static_cast<int32>(sizeof(val))
#define len16(arr)	static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)	static_cast<int32>(sizeof(arr)/sizeof(arr[0]))

// CoreGlobal.h

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadLockProfiler();
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GDeadLockProfiler;
		SocketUtils::Clear();
	}
} GCoreGlobal;

// GameServer.cpp

#include "SocketUtils.h"

int main()
{
	SOCKET socket = SocketUtils::CreateSocket();

	SocketUtils::BindAnyAddress(socket, 7777);

	SocketUtils::Listen(socket);

	SOCKET clientSocket = ::accept(socket, nullptr, nullptr);

	cout << "Client Connected!" << endl;

	while (true)
	{

	}

	GThreadManager->Join();
}
```
