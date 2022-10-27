---
title:  "Server lecture section1 [2/8]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.25 15:00:00
---

# DeadLock

DeadLock 문제를 100% 완벽하게 해결할 수는 없음    

```cpp
class User
{

};

class UserManager
{
	public:
		static UserManager* Instance()
		{
			static UserManager instance;
			return &instance;
		}

		User* GetUser(int32 id)
		{
			lock_guard<mutex> guard(_mutex);
			return nullptr;
		}

		void ProcessSave();

	private:
		mutex _mutex;
};
```

```cpp
class Account
{

};

class AccountManager
{
	public:
		static AccountManager* Instance()
		{
			static AccountManager instance;
			return &instance;
		}

		Account* GetAccount(int32 id)
		{
			lock_guard<mutex> guard(_mutex);
			return nullptr;
		}

		void ProcessLogin();

	private:
		mutex _mutex;
};
```

```cpp
void AccountManager::ProcessLogin()
{
	// accountLock
	lock_guard<mutex> guard(_mutex);

	// userLock
	User* user = UserManager::Instance()->GetUser(100);
}

void UserManager::ProcessSave()
{
	// userLock
	lock_guard<mutex> guard(_mutex);

	// accountLock
	Account* account = AccountManager::Instance()->GetAccount(100);
}

void Func1()
{
	for (int32 i = 0; i < 100; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;
}
```

userLock과 accountLock이 서로 교차되어 실행되기 때문에 교착상태에 빠져 DeadLock이 발생함    
Lock의 순서를 바꾸어주는 것으로 간단하게 해결할 수는 있으나, 찾기 어려울 수 있음    
`mutex`에 계층을 부여하여 순서를 확인하는 방법을 사용하기도 하지만 언제나 확실한 방법은 아님    

```cpp
mutex m1;
mutex m2;
std::lock(m1, m2);

lock_guard<mutex> g1(m1, std::adopt_lock);
lock_guard<mutex> g2(m2, std::adopt_lock);
```

`std::lock(m1, m2)`와 같은 형식으로 사용할 경우 자체적으로 순서를 판별하여 Lock을 걸어줌    
`lock_guard`에 `adopt_lock` 옵션을 사용하면 해당 `mutex`가 이미 Lock된 상태임을 지정하여 소멸시 해제만 요구함    

***

# Lock 구현 이론

존버메타(스핀락) / 랜덤메타 / 갑질메타    

컨텍스트 스위칭 : 유저모드와 커널모드 사이를 이동하면서 부하가 발생    

***

# SpinLock

SpinLock : 경합상황에 무한정으로 대기    
컨텍스트 스위칭이 발생하지 않기 때문에 CPU 점유율이 상당히 높아짐    

`volatile` : 컴파일러의 최적화를 금지    
`atomic`에는 `volatile` 기능이 포함되어있음    

```cpp
#include <thread>
#include <mutex>

class SpinLock
{
	public:
		void lock()
		{
			bool expected = false;
			bool desired = true;

			// CAS(Compare-And-Swap) 의사코드
			/*
			if (_locked == expected)
			{
				expected = _locked;
				_locked = desired;
				return true;
			}
			else
			{
				expected = _locked;
				return false;
			}
			*/

			while (_locked.compare_exchange_strong(expected, desired) == false)
			{
				expected = false;
			}
		}

		void unlock()
		{
			_locked.store(false);
		}
	private:
		atomic<bool> _locked = {false};
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
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

`atiomic`의 `compare_exchange_strong` 메소드로 락을 확인하고 획득하는 부분을 atomic하게 처리할 수 있음    
즉, `while(_locked){}`와 `_locked_ture;` 두가지 행동으로 이루어진 코드를 `_locked.compare_exchange_strong(expected, desired)` 구문을 통해 한번에 처리하여 Lock에 동시 접근하는 것을 방지함    

***

# Sleep

시스템 콜(system call)은 유저모드가 아닌 커널모드에서 동작함    
실행중인 쓰레드는 할당시간 경과(time slice 소진), 인터럽트 발생시 `ready` 상태로 돌아감    
쓰레드에서 입출력 등의 시스템 콜(system call) 발생시 `waiting` 상태가 됨    
`sleep` 역시 시스템 콜의 일부로 특정 시간동안 쓰레드를 `block`시켜 스케쥴링되지 않도록 만든 후 `ready` 상태로 만듬    
단, 이 때 `ready` 상태가 되는 것이므로 특정 시간이 지났다고 해서 쓰레드가 곧바로 재실행되는 것이 아님에 유의    

```cpp
while (_locked.compare_exchange_strong(expected, desired) == false)
{
	expected = false;

	//this_thread::sleep_for(std::chrono::milliseconds(100));
	this_thread::sleep_for(100ms);
	this_thread::yield(); // == sleep_for(0ms);
}
```

위와 같이 스핀락 도중 sleep을 걸어주는 것도 가능    
`yield()`의 경우 현재 timeslice를 포기하고 쓰레드를 곧바로 `ready` 큐에 다시 집어넣음    
따라서 `sleep_for(0)`과 `yield()`는 동일한 효과를 발생시킴    
`ms`와 같은 접미사의 경우 c++14부터 도입된 기능임    
