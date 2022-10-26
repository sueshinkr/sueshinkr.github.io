---
title:  "Server lecture section1 [3/8]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.24 18:00:00
---

# Event

```cpp
#include <thread>
#include <mutex>

mutex m;
queue<int32> q;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}
```

데이터에 접근하는 일이 빈번하게 일어나지 않는 경우 `Consumer()` 함수에서처럼 항상 체크하는 것은 효율이 좋지 않음    

```cpp
#include <thread>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		::SetEvent(handle);
		this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);
		// Auto일경우 곧바로 Non-Signal로 바뀜
		// Manual일경우 ::ResetEvent(handle); 필요

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}
int main()
{
	// 커널 오브젝트
	// Usage Count
	// Signal / Non-Signal
	// Auto / Manual
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*ManualReset*/, FALSE/*initialState*/, NULL);
	
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}
```

이벤트를 활용하여 무의미한 대기를 제거    
쓰레드는 시그널이 있을 때까지 `block`되고, 커널이 이벤트의 시그널을 확인한 후 쓰레드에 알려주면 다시 활성화됨    
`CreateEvent`는 winAPI에서 제공하는 함수이며, linux에서는 `semaphore`를 사용해야 하는 듯 함    
위처럼 커널 오브젝트를 활용하는경우 다른 프로그램과의 동기화에도 유리함    
