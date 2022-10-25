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
date: 2022.10.24 15:00:00
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

