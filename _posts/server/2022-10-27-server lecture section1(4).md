---
title:  "Server lecture section1 [4/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.27 18:00:00
---

# 캐시

데이터 전송 비용 >> CPU 연산 비용    
따라서 임시 메모리인 캐시를 활용함    

레지스터 / L1 캐시 / L2 캐시 / ...    
어떠한 데이터를 RAM에서 꺼내와 임시적으로 캐시에 저장    
동일하거나 인접한 데이터를 우선 캐시 메모리에서 체크한 후 사용    
캐시는 Temporal Locality(시간적 지역성)과 Spatial Locality(공간적 지역성) 특성을 가지고있음     

```cpp
#include <windows.h>

int32 buffer[10000][10000];

int main()
{
	memset(buffer, 0, sizeof(buffer));

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;
		
		for (int32 i = 0; i < 10000; i++)
			for (int32 j = 0; j < 10000; j++)
				sum += buffer[i][j];

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10000; i++)
			for (int32 j = 0; j < 10000; j++)
				sum += buffer[j][i];

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}
}
```

전자의 경우 캐시의 Spatial Locality 특성에 의해 저장된 데이터들을 곧바로 사용    
따라서 접근하는 데이터들이 흩어져있어 캐시가 아닌 RAM에 접근해야하는 후자보다 속도가 빠르게 동작함    

***

# CPU 파이프라인

```cpp
#include <thread>

// 가시성, 코드 재배치
int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready;

void Thread_1()
{
	while (!ready)
		;
	y = 1; // Store y
	r1 = x; // Load x
}

void Thread_2()
{
	while (!ready)
		;
	x = 1; // Store x
	r2 = y; // Load y
}

int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;
		count++;

		x = y = r1 = r2 = 0;
		thread t1(Thread_1);
		thread t2(Thread_2);

		ready = true;
		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}
	cout << count << "번만에 빠져나옴" << endl;
}
```

cpu의 각 코어마다 캐시가 별도로 존재하기 때문에 가시성 문제 발생    
컴파일러 또는 CPU에 의한 코드 재배치 문제 발생 가능    
* CPU는 Fetch / Decode / Execute / Write-back의 파이프라인 구조를 가지고있음
* 한번에 처리할 수 있는 instruction의 수를 늘려 효율을 증가
* 단, 명령어들의 순서를 바꿀 경우 더 빠르게 동작할 수 있는 경우가 존재하고 이 때 코드 재배치가 이루어짐    

C++11에서는 환경에 관계없이 표준으로 이러한 문제를 제어할 수 있음    

***

# 메모리 모델

C++11에서 메모리 모델이 추가    

여러 쓰레드에서 동일한 메모리에 동시 접근하여 데이터를 쓰는 경우 경합 조건(Race Condition)이기 때문에 Undefined Behavior가 발생
* `Lock(mutex)`를 이용한 상호 배타적 접근(Mutual Exclusive)
* `Atomic`(원자적) 연산을 이용

```cpp
int main()
{
	atomic<bool> flag = false;
	cout << flag.is_lock_free() << endl; // false일경우 lock이 필요한 non-atomic 연산

	// flag = ture;
	flag.store(true, memory_order::memory_order_seq_cst);

	// bool val = flag;
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// 이전 flag 값을 prev에 넣고, flag 값을 수정
	{
		// bool prev = flag;
		// flag = true;
		bool prev = flag.exchange(true);
	}

	// CAS (Compare-And-Swap) 조건부 수정
	{
		/*
		if (flag == expected)
		{
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}
		*/

		while (true)
		{
			bool expected = false;
			bool desired = true;
			flag.compare_exchange_weak(expected, desired);
			// 다른 쓰레드의 interruption을 받아서 중간에 실패하는 Spurious Failure 발생 가능
			// 따라서 while 루프와 같이 사용
			// strong과 큰 성능차이는 없음
		}
	}
}
```

`Atomic` 연산(더이상 쪼개질 수 없는 연산)의 경우 모든 쓰레드가 동일 객체에 대해 동일한 수정 순서를 관찰함    
* `atomic` 클래스를 이용하지 않더라도 `Atomic` 연산이 될 수 있음
* `atomic` 클래스의 `is_lock_free()` 메소드를 통해 원래 환경에서 원자적 연산을 지원하는지 여부를 확인할 수 있음

```cpp
#include <thread>
#include <atomic>

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;

	ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer()
{
	while (ready.load(memory_order::memory_order_seq_cst) == false)
		;
	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();
}
```

Memory Model(정책)
* Sequentially Consistent (seq_cst)
	* 가장 엄격 = 컴파일러 최적화 여지가 적음 = 직관적
	* 가시성 / 코드 재배치 문제 바로 해결
* Acquire-Release (acquire, release)
	* `seq_cst`와 `relaxed`의 중간
	* `release` 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지
	* `acquire` 명령으로 같은 변수를 읽는 쓰레드가 있다면 `release` 이전의 명령들이 `acquire`하는 순간에 관찰 가능 = 가시성 보장
* Relaxed (relaxed)
	* 자유로움 = 컴파일러 최적화 여지 많음 = 직관적이지 않음
	* 가시성 문제가 해결되지 않음 / 코드 재배치도 멋대로 가능
	* 가장 기본 조건인 동일 객체의 동일 순서만 보장
	* 거의 사용할 일이 없음

인텔, AMD의 경우 애당초 순차적 일관성을 보장하여 `seq_cst`를 써도 별다른 부하가 없음    
단, ARM의 경우 꽤 차이가 있음    

절취선을 그어주는 C++ 표준인 `std::atomic_thread_fence()`와 같은 함수도 존재함    
메모리 가시성을 강제하고, 메모리 재배치를 금지하는 효과    

[`memory_order`에 대한 전체적인 내용](https://modoocode.com/271)
[`memory_order`에 대한 자세한 정보](https://en.cppreference.com/w/cpp/atomic/memory_order)


