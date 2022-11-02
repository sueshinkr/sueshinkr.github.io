---
title:  "Server lecture section1 [1/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.24 15:00:00
---

# 서버 개론

서버 : 다른 컴퓨터에서 연결이 가능하도록 대기 상태로 상시 실행중인 프로그램    
* 영업중인 식당과 유사
	* 손님이 올 수 있도록 식당을 열고 대기중
	* 손님이 오면 메뉴 / 정책에 따라 서비스 제공

Web Server(HTTP Server)
* 테이크아웃 전문, 질의 / 응답 형태
* 정보를 드물게 요청 / 갱신, 손님에게 먼저 접근하지 않음
* 손님이 떠난 후에는 해당 손님의 상태를 당분간 잊고 지냄(Stateless)

Game Server(TCP Server, Binary Server, Stateful Server)
* 일반 식당, 실시간 Interaction이 존재
* 요청 / 갱신 횟수가 많으며 직원이 손님에게 항상 접근 가능해야함
* 손님이 식당에 머무는 동안 손님의 상태에 따라 서비스를 제공(Stateful)

멀티쓰레드 / 네트워크 / DB / 게임 로직 / 서버 구조에 대해 공부

***

# 멀티쓰레드 개론

프로세스(프로그램) / 쓰레드 / CPU 코어

단일 CPU 코어는 한번에 한 쓰레드만 작동시킬 수 있음    
CPU 코어와 쓰레드가 일대일로 대응되는 것이 이상적인 상황    

쓰레드들은 Heap 영역과 데이터 영역을 공유    
따라서 멀티쓰레드의 경우 공유된 자원들을 어떻게 사용하는지에 대한 문제(컨텍스트 스위칭 등)가 발생    

***

# 쓰레드 생성

```cpp
#include <thread>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread2(int32 num)
{
	cout << num << endl;
}

int main()
{
	//std::thread t(HelloThread);
	std::thread t;
	t = std::thread(HelloThread);

	int32 count = t.hardware_concurrency();
	auto id = t.get_id();

	//t.detach();
	if (t.joinable())
		t.join();

	vector<std::thread> v;

	for (int32 i = 0; i < 10; i++)
	{
		v.push_back(std::thread(HelloThread2, i));
	}
	for (int32 i = 0; i < 10; i++)
	{
		if (v[i].joinable())
			v[i].join();
	}

	cout << "Hello Main" << endl;
}
```

각 운영체제별로 쓰레드를 생성하는 방법이 달랐으나 C++11부터 `thread` 헤더를 통해 일관된 방법을 사용 가능    

`std::thread threadName`의 형식으로 쓰레드 객체 생성    
혹은 `std::thread threadName(Function)`의 형식으로 쓰레드 객체 생성과 동시에 다른 함수로 진입 가능    

쓰레드 객체에서 사용 가능한 여러 메소드들이 존재
* `t.hardware_concurrency()` : CPU 코어 개수 확인 (100% 정확하지는 않음)
* `t.get_id()` : 쓰레드에 부여된 id 확인, 쓰레드 객체와 실제 쓰레드가 연결되지 않았을 경우 0을 리턴
* `t.detach()` : 쓰레드 객체에서 실제 쓰레드를 분리
* `t.joinable()` : 쓰레드에 부여된 id가 0인지를 체크하여 연결이 되어있는지 확인
* `t.join()` : 쓰레드가 종료되기까지 대기

쓰레드 진입 함수에 매개변수가 존재할경우 `std::thread threadName(Function, arg1, arg2, ...)` 형식으로 쓰레드 객체 생성 및 초기화 가능     

***

# Atomic

간단한 증감 연산의 경우에도 어셈블리어로 보면 세줄으로 이루어져있음    
그렇기 때문에 여러개의 쓰레드가 공유된 데이터를 가지고 작업할경우 작업 순서가 얽혀(data race가 발생) 원하는 결과를 얻지 못할 가능성이 높음    
즉, 작업의 순서가 보장되지 않는 동기화 문제가 발생    

```cpp
#include <thread>
#include <atomic>

atomic<int32> sum = {0};

void Add()
{
	for (int32 i = 0; i < 1000000; i++)
	{
		sum.fetch_add(1);
		//sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 1000000; i++)
	{
		sum.fetch_sub(1);
		//sum--;
	}
}

int main()
{
	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;
}
```

동기화 문제를 방지하는 방법중 하나로 `atomic` 키워드를 사용할 수 있음    
즉, All or Nothing이 되도록 사용    
변수를 `atomic`으로 선언시 Cpu가 해당 변수를 조작하는 연산이 끝날 때까지 다음 연산을 대기하도록 함    
단, 연산 속도가 저하되므로 필요한 곳에만 사용    

***

# Lock 기초

`vector`, `list` 등의 자료구조들은 대부분 싱글 쓰레드에서만 정상작동함    
이런 경우 동기화 문제를 해결하기 위해 Lock 개념을 사용할 수 있음
멀티쓰레드 환경에서는 대부분의 경우 Lock을 사용하여 동기화 문제를 해결함    

```cpp
#include <thread>
#include <mutex>

vector<int32> v;
mutex m;

template<typename T>
class LockGuard
{
	public:
		LockGuard(T& m)
		{
			_mutex = &m;
			_mutex->lock();
		}

		~LockGuard()
		{
			_mutex->unlock();
		}
	private:
		T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; i++)
	{
		m.lock();
		v.push_back(i);
		m.unlock();

		LockGuard<std::mutex> lockGuard(m);
		v.push_back(i);

		std::lock_guard<std::mutex> lockGuard2(m);
	
		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		uniqueLock.lock();
	}
}

int main()
{
	v.reserve(20000);
	std::thread t1(Push);
	std::thread t2(Push);
	t1.join();
	t2.join();
	
	cout << v.size() << endl;
}
```

`mutex` : Mutual Exclusive(상호배타), <mutex> 헤더를 사용    
데이터 경합이 일어나는 동작 전에 `lock()` 함수로 Lock을 걸어주어 다른 쓰레드가 동시에 데이터에 접근하는 것을 방지, 동작 후에 `unlock()` 함수를 사용하여 다음 동작이 발생하도록 허용
* 단, `mutex` 사용시 사실상 싱글쓰레드처럼 동작하기 때문에 속도가 저하됨    
* `mutex`는 재귀적 호출이 불가능
* Lock을 해제하지 않았을 경우 교착상태가 되어 해당 데이터에 영원히 접근이 불가능해지는 문제가 발생

RAII (Resource Acquisition is initialization) : `mutex`를 멤버로 가지는 클래스를 만들어 생성자에서 Lock, 소멸자에서 Unlock    
클래스의 객체를 스택변수로 사용하기 때문에 자동으로 소멸자가 호출되어 Unlock됨    

`std::lock_guard`가 표준으로 존재    
`std::unique_lock`의 경우 세부적인 옵션을 설정 가능    
* `defer_lock` : 객체 선언시 자동으로 `lock`되지 않고, `lock()`을 명시적으로 호출해주어야함    