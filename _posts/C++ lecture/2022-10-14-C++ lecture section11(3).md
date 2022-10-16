---
title:  "C++ lecture section11 [3/3]"
excerpt: "STL"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.14 13:00:00
---

# map #1

벡터, 리스트같은 선형 자료구조는 원하는 조건에 해당하는 데이터를 빠르게 찾을 수 없다는 단점이 있음    
따라서 연관 컨테이너 형태의 자료구조를 사용    

`map` : 균형 이진 트리(AVL) 구조, 노드 기반

```cpp
class Player
{
	public:
		Player() : _playerid(0) {}
		Player(int playerid) : _playerid(playerid) {}
	public:
		int _playerid;
}

class Node
{
	public:
		Node* _left;
		Node* _right;
		int _key;
		Player* _value;
};

int main()
{
	map<int, int> m; // (Key, Value)

	for (int i = 0; i < 100000; i++)
		m.insert(pair<int, int>(i, i * 100));
 
	for (int i = 0; i < 50000; i++)
	{
		int randomValue = rand() % 50000;
		m.erase(randomValue);
	}
	
	map<int, int>::iterator findit = m.find(10000);
	if (findit != m.end())
		cout << "찾음" << endl;
	else
		cout << "못찾음" << endl;
}
```

***

# map #2

`erase()`의 경우 삭제한 횟수를 리턴값으로 반환    
같은 키의 값을 중복 삭제시에도 카운트값만 달라질 뿐 문제는 발생하지 않음    

`insert()`의 경우 중복된 키값을 생성할시 요청이 무시됨    
`pair<map<datatype, datatype>::iterator, bool>`을 반환
* `iterator`는 삽입한 키에 해당하는 이터레이터
* `bool`은 삽입의 성공 혹은 실패 여부

```cpp
for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
{
	pair<const int, int>* p = (*it);
	int key = p.first; // == it->first
	int value = p.second; // == it->second
}

map<int, int>::iterator findit = m.find(10000);
if (findit !- m.end())
	findit->second = 20000;
else
	m.insert(make_pair(10000, 200));

m[5] = 500;
for (int i = 0; i < 10; i++)
	cout << m[i] << endl;
```

`[]` 연산자 사용시 대입을 하지 않더라도 (Key, Value) 형태의 데이터가 추가됨    

***

# set, multimap, multiset


`multimap` : `map`에서 중복키를 허용    

```cpp
int main()
{
	multimap<int, int> mm;

	mm.insert(make_pair(1, 100));
	mm.insert(make_pair(1, 200));
	mm.erase(1);

	pair<mulitimap<int, int>::iterator, multimap<int, int> itPair;
	itPair = mm.equal_range(1);
	for(mulitimap<int, int>::iterator it = itPair.first, it != itPair.second, ++it)
		cout << it->fisrt << " " << it->second << endl;

	return 0;
}
```

`multimap`에서 `erase()`할시 해당 키에 속하는 모든 값을 삭제함    
`equal_range()` 함수로 특정 키에 해당하는 첫번째 값의 이터레이터와 마지막 값 다음 이터레이터를 구할 수 있음    
따로 구할 경우에는 `lower_bound()`, `upper_bound()` 함수 사용    

```cpp
int main()
{
	multiset<int> ms;

	ms.insert(100);
	ms.insert(100);

	multiset<int>::iterator findit = mm.find(100);
	pair<mulitiset<int, int>::iterator, multimap<int, int> itPair; ms.equal_range(100);
	for (multiset<int>::iterator it = itPair.first, it != itPair.second, ++it)
		cout << *it << endl;
}
```

`set` : `map`과 거의 동일하나 `Key`값이 `Value`가 됨    
`multiset` : `set`에서 중복키를 허용    

***

# 연습문제

```cpp
#include <iostream>
#include <vector>
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

	cout << "-----------------------------\n";

	{
		int number = 50;
		bool found = false;
		vector<int>::iterator it;
		for (it = v.begin(); it != v.end(); ++it)
		{
			if ((*it) == number)
			{
				found = true;
				cout << "found" << endl;
				break;
			}
		}
		cout << (*it) << endl;
	}

	cout << "-----------------------------\n";

	{
		bool found = false;
		vector<int>::iterator it;
		for (it = v.begin(); it != v.end(); ++it)
		{
			if ((*it) % 11 == 0)
			{
				found = true;
				cout << "found" << endl;
				break;
			}
			cout << (*it) << endl;
		}
	}

	cout << "-----------------------------\n";

	{
		int count = 0;
		vector<int>::iterator it;
		for (it = v.begin(); it != v.end(); ++it)
		{
			if ((*it) % 2 == 1)
				count++;
		}
		cout << count << endl;
	}

	cout << "-----------------------------\n";

	{
		vector<int>::iterator it;
		for (it = v.begin(); it != v.end(); ++it)
			cout << (*it) << endl;
		cout << "---------------after change-------------\n";
		for (it = v.begin(); it != v.end(); ++it)
		{
			(*it) *= 3;
			cout << (*it) << endl;
		}
	}
}
```