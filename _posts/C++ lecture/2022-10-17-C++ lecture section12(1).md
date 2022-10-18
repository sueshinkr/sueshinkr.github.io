---
title:  "C++ lecture section12 [1/4]"
excerpt: "Modern C++"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.17 13:00:00
---

# algorithm

자료구조 : 데이터를 저장하는 구조    
알고리즘 : 데이터를 어떻게 사용할 것인가?    

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// find()
	{
		int number = 50;
		vector<int>::iterator itFind = find(v.begin(), v.end(), number);
		if (itFind == v.end())
			cout << "not found" << endl;
		else
			cout << "found" << endl;
	}

	// find_it()
	{
		struct CanDivideBy11
		{
			bool operator()(int n)
			{
				return (n % 11) == 0;
			}
		};
		vector<int>::iterator itFind = find_if(v.begin(), v.end(), CanDivideBy11());
		if (itFind == v.end())
			cout << "not found" << endl;
		else
			cout << "found" << endl;
	}

	// count_if()
	{
		struct isOdd
		{
			bool operator()(int n)
			{
				return (n % 2) != 0;
			}
		};
		int n = count_if(v.begin(), v.end(), isOdd());
		cout << n << endl;
	
	// all_of() / any_of() / none_of()

		bool b1 = all_of(v.begin(), v.end(), isOdd());
		bool b2 = any_of(v.begin(), v.end(), isOdd());
		bool b3 = none_of(v.begin(), v.end(), isOdd());
		cout << b1 << "\t" << b2 << "\t" << b3 << endl;
	}

	// for_each()
	{
		struct MultiplyBy3
		{
			void operator()(int& n)
			{
				n = n * 3;
			}
		};
		for_each(v.begin(), v.end(), MultiplyBy3());
	}

	// remove() / remove_if()
	{
		struct isOdd
		{
			bool operator()(int n)
			{
				return (n % 2) != 0;
			}
		};
		vector<int>::iterator it = remove_if(v.begin(), v.end(), isOdd());
		// 1 4 3 5 8 2 => 4 8 2 5 8 2

		v.erase(it, v.end());
		//v.erase(remove_if(v.begin(), v.end(), isOdd());
	}

	return 0;
}
```

***

# auto

Modern C++ (C++11)부터 사용된 문법    
형식 연역(type deduction)이라고도 불림    
추론 규칙은 생각보다 복잡할 수 있음    

```cpp
int main()
{
	int& reference = a;
	const int cst = b;

	auto test1 = reference;
	auto test2 = cst;

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
	{
		int& data = v[i];
		//auto& data = v[i];
		data = 100;
	}

	return 0;
}
```

기본 `auto`는 `const`와 `&`를 무시함    

가독성이 저하되기 때문에 `auto`만 사용하는 것은 권장하지 않음    
단, 타이핑이 길어지는 경우에는 사용함    

***

# 중괄호 초기화 {}

```cpp
int main()
{
	int a = 0;
	int b(0);
	int c{0};

	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	vector<int> v2(10, 1);
	vector<int> v3{1, 2, 3, 4};

	return 0;
}
```

중괄호 초기화는 `vector`등의 컨테이너와 잘 어울림    
축소 변환을 방지할 수 있음(축소 변환시 에러 발생)    
함수 선언과 변수 초기화를 명확히 구분할 수 있음    

여러개를 초기화할시 `initializer_list<>` 생성자를 사용함    
이는 다른 생성자보다 우선권이 높기 때문에 문제가 생길 수 있음    

