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
date: 2022.10.26 18:00:00
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
* `SetEvent()` : 시그널 변경
* `WaitForSingleObject()` : 시그널이 있을때까지 대기

위처럼 커널 오브젝트를 활용하는경우 다른 프로그램과의 동기화에도 유리    

단, 이벤트와 LOCK이 한묶음으로 실행되는 것이 아닌 것에 유의해야함    

***

# Condition Variable

```cpp
#include <thread>
#include <mutex>

mutex m;
queue<int32> q;

// CV는 유저레벨 오브젝트
condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 해제하고
		// 4) 함수를 통해 다른 쓰레드에게 통지
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		// wait중인 쓰레드 하나를 깨움
		cv.notify_one();
		//this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; });
		// 1) Lock을 잡고
		// 2) 조건 확인
		// 3-1) 만족할경우 빠져나와서 코드를 진행
		// 3-2) 만족하지 않을 경우 Lock을 해제한 후 대기상태로 전환

		// notify_one을 했으면 조건식을 만족한 것이 아닌가?
		// Spurious Wakeup (가짜 기상)
		// notify_one을 할 때 Lock이 잡혀있는 상태가 아니기 때문
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
`Condition Variable`(조건변수)은 `mutex`과 `condition_variable` 헤더에 포함되어있는 C++ 표준    
`Condition Variable`은 `mutex`를 직접 Lock, Unlock하기 때문에 이를 가능하게 해주는 `unique_lock`을 사용함    

`Condition Variable`은 이벤트와 비슷한 개념이라고 볼 수 있으나, 조건을 한번 더 확인함    
즉 이벤트에서는 시그널을 확인하여 대기상태 해제 후 Lock을 걸어줌과 동시에 코드를 진행하였으나, `Condition Variable`에서는 함수를 통해 대기상태 해제 후 Lock을 걸어준 뒤 조건을 확인하여 만족할 경우에만 코드를 진행함    

`notify_one`을 함수를 사용하여 대기상태가 해제되었을 때는 Lock이 걸려있지 않은 상태이기 때문에 `Spurious Wakeup` 상황이 발생할 수 있음    
이를 해결하기 위해 잠금을 유지한 채로 `notify` 등의 함수를 사용해 통지할 수 있으나, 이럴 경우 통지된 쓰레드는 이전의 Lock이 해제될 때까지 대기해야 하기 때문에 성능이 저하됨    
따라서 `wait()`함수 내에서 루프를 돌며 조건문을 확인하는 절차가 필요    

***

# future

일회성으로 한번만 발생하는 이벤트에 유용    

```cpp
#include <thread>
#include <mutex>
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기(synchronous) 실행
	int64 sum = Calculate();
	cout << sum << endl;

	// std::future
	{
		// 1) deferred : lazy evaluation - 지연해서 실행
		// 2) async : 별도의 쓰레드를 만들어 실행
		// 3) defferd | async : 둘중 알아서 선택
		std::future<int64> future = std::async(std::launch::async, Calculate);
		 
		std::future_status status = future.wait_for(1ms);
		if (status == future_status::ready)
		{

		}

		int64 sum = future.get();

		class Knight
		{
		public:
			int64 GetHP() { return 100; }
		};
		
		Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight);
	}

	// std::promise
	{
		// 미래에 결과물을 반환해줄 것을 약속
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		thread t(TaskWorker, std::move(task));
		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}
}
```

`async` : 원하는 함수를 비동기적으로 실행    
`promise` : 결과물을 `promise`를 통해 `future`로 받아줌    
`packaged_task` : 원하는 함수의 실행 결과를 `packaged_task`를 통해 `future`로 받아줌    

***

