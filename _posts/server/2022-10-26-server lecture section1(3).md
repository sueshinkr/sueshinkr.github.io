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
