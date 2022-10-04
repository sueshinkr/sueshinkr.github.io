---
title:  "C++ lecture section5 [3/4]"
excerpt: "포인터"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.27 13:00:00
---

# 로또 번호 생성기

```cpp
// Lotto.cpp

#include <iostream>
using namespace std;

void Swap(int& a, int& b);
void Sort(int* arr, int len);
void ChooseLotto(int* arr);

int main()
{
	srand(time(0));
	int numbers[6];

	ChooseLotto(numbers);
	return 0;
}

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

void Sort(int* arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (arr[i] > arr[j])
				Swap(arr[i], arr[j]);
		}
	}
}

void ChooseLotto(int* arr)
{
	int temp;

	for (int i = 0; i < 6; i++)
	{
		temp = rand() % 45 + 1;
		for (int j = 0; j < i; j++)
		{
			if (temp == arr[j])
			{
				j = -1;
				temp = rand() % 45 + 1;
			}
		}
		arr[i] = temp;
	}
	Sort(arr, 6);
	
	cout << "로또 번호 생성 완료\n";
	for (int i = 0; i < 6; i++)
		cout << arr[i] << " ";
	cout << endl;
}
```

***

# 다중 포인터

```cpp
void SetMessage(const char** a)
{
	*a = "Bye";
}

int main()
{
	const char* msg = "Hello";
	SetMessage(&msg);
	std::cout << msg << endl;
	return 0; 
}
```

`*` 연산자를 하나씩 제거하면서 타고 간다고 생각    
`void SetMessage(const char*& a)`와 같이 포인터와 참조를 섞어쓰는 것도 가능함    

***

# 다차원 배열

```cpp
int main()
{
	int arr2d[2][5] = { {1, 2, 3, 4, 5}, { 11, 12, 13, 14, 15} };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << arr2d[i][j] << " ";
		cout << endl;
	}
}
```

`int arr[2][5]`와 `int arr[10]`은 메모리상으로는 차이가 없음    
배열 자료형의 크기만큼 메모리 주소를 이동해가면서 배열의 각 원소에 접근    
2D배열은 대표적으로 2D 로그라이크 맵에 사용됨    

***

# 포인터 마무리

포인터는 데이터의 메모리 주소를 담고있는 통로 역할로 크기가 작음    
반면 배열은 실제로 데이터들의 집합이기 때문에 크기가 비대해질 수 있음    
`TYPE형 1차원 배열`과 `TYPE*형 포인터`는 완전히 호환 가능함    

```cpp
int main()
{
	int arr2[2][2] = {{1, 2}, {3, 4}};
	int** pp = (int**)arr2 // 불가능
	int(*p2)[2] = arr2; // 가능
}
```

다차원 배열과 다차원 포인터는 아예 별개의 문법으로 사용되며, 서로 호환되지 않음    
`int(*)[2]`의 경우 2개의 원소를 가지는 `int`형 배열의 포인터라는 의미로 사용됨    

C++의 특성상 메모리를 다루는 작업에 주의해야함    
