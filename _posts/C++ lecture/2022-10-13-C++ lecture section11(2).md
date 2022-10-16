---
title:  "C++ lecture section11 [2/3]"
excerpt: "STL"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.13 14:00:00
---

# list #1

```cpp
#include <list>

int main()
{
	list<int> li;

	for (int i = 0; i < 100; i++)
		li.push_back(i);

	int size = li.size();
	int first = li.front();
	int last = li.back();

	list<int>::iterator itBegin = li.begin();
	list<int>::iterator itEnd = li.end();

	for (list<int>::iterator it = li.begin(); it != li.end(); ++it)
	{
		cout << (*it) << endl;
	}

	li.insert(itBegin, 100);
	li.erase(li.begin());
	li.pop_front();
	li.remove(10);

	return 0;
}
```

`list`는 연결 리스트 구조    
연결 리스트의 원소는 노드(node)라고 부름    
각각의 노드가 불연속한 메모리 위치에 존재하며, 각 노드에 다음 노드의 위치를 가리키는 포인터가 존재함    

```cpp
class Node
{
	public:
		Node* _next;
		int data;
};
```

연결 리스트에는 단일 / 이중 / 원형 연결 리스트가 있음    
이중 연결 리스트는 `Node* _prev` 데이터가 추가된 형태    
원형 연결 리스트는 `head` 노드와 `tail` 노드가 연결되어있는 형태    

***

# list #2

데이터들이 연속되어있을 필요가 없으므로, 노드를 연결하는 포인터들의 주소를 변경하는 것으로 간단하게 중간 삽입/삭제가 가능함    
마찬가지로 처음/끝 부분의 삽입/삭제 역시 어렵지 않음    
단, 임의 접근시 데이터들이 연속되어있지 않기 때문에 해당 데이터를 한번에 찾아갈 수 없으며 따라서 노드를 타고 이동해야한다는 비효율이 발생함    

벡터와는 달리 원소 삭제시 노드들을 새롭게 이어주기만 하면 되므로 `remove()` 함수를 지원함    
STL에서의 `list`에는 마지막 노드 다음에 더미 노드가 존재하여 `end()`를 가리킴    
이터레이터의 `+` 오퍼레이터가 정의되어있지 않음    

중간 삽입/삭제의 처리 자체는 빠르지만, 해당 위치의 노드로 찾아가는 임의 접근은 느림    
따라서 이미 특정 위치의 이터레이터가 존재하는 경우에만 중간 삽입/삭제가 빠르다고 할 수 있음    

***

# list #3
 
```cpp
#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<typename T>
class Node
{
	public:
		Node() : _next(nullptr), _prev(nullptr), _data(T()) {}
		Node(const T& value) : _next(nullptr), _prev(nullptr), _data(value) {}
	public:
		Node* _next;
		Node* _prev;
		T _data;
};

template<typename T>
class Iterator
{
	public:
		Iterator() : _node(nullptr) {}
		Iterator(Node<T>* node) : _node(node) {}

		Iterator& operator++()
		{
			_node = _node->_next;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator<T> temp = *this;
			_node = _node->_next;
			return temp;
		}

		Iterator& operator--()
		{
			_node = _node->_prev;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator<T> temp = *this;
			_node = _node->_prev;
			return temp;
		}

		T& operator*()
		{
			return _node->_data;
		}
		bool operator==(const Iterator& right)
		{
			return _node == right._node;
		}
		bool operator!=(const Iterator& right)
		{
			return _node != right._node;
		}

	public:
		Node<T>* _node;
};

template<typename T>
class List
{
	public:
		List() : _size(0) 
		{
			_header = new Node<T>();
			_header->_next = _header;
			_header->_prev = _header;
		}
		~List()
		{
			while (_size > 0)
				pop_back();

			delete _header;
		}

		void push_back(const T& value)
		{
			AddNode(_header, value);
		}
		Node<T>* AddNode(Node<T>* before, const T& value)
		{
			Node<T>* node = new Node<T>(value);
			Node<T>* prevNode = before->_prev;

			prevNode->_next = node;
			node->_prev = prevNode;
			node->_next = before;
			before->_prev = node;
			
			_size++;

			return node;
		}

		void pop_back()
		{
			RemoveNode(_header->_prev);
		}
		Node<T>* RemoveNode(Node<T>* node)
		{
			Node<T>* prevNode = node->_prev;
			Node<T>* nextNode = node->_next;

			prevNode->_next = nextNode;
			nextNode->_prev = prevNode;

			delete node;
			_size--;

			return nextNode;
		}

		int size() { return _size; }

	public:
		typedef Iterator<T> iterator;
		iterator begin() { return iterator(_header->_next); }
		iterator end() { return iterator(_header); }
		iterator insert(iterator it, const T& value)
		{
			return iterator(AddNode(it._node, value));
		}
		iterator erase(iterator it)
		{
			return iterator(RemoveNode(it._node));
		}

	public:
		Node<T>* _header;
		int _size;
};

int main()
{
	List<int> li;
	List<int>::iterator eraseit;

	for (int i = 0; i < 10; i++)
	{
		if (i == 5)
			eraseit = li.insert(li.end(), i);
		else
			li.push_back(i);
	}

	li.pop_back();
	li.erase(eraseit);

	for (List<int>::iterator it = li.begin(); it != li.end(); ++it)
		cout << (*it) << endl;
}
```

***

# deque

`deque` : double-ended queue    
* `vector`, `list`와 마찬가지로 시퀀스 컨테이너(Sequence Contatiner)에 속함
	* 시퀀스 컨테이너 : 데이터가 삽입 순서대로 나열되는 형태
* 블록별로 메모리가 저장되는 구조를 가지고있으며, 블록 내의 원소들은 메모리가 연속되어있으나 블록끼리는 메모리가 불연속적임    

```cpp
int main()
{
	deque<int> dq;

	dq.push_back(1);
	dq.push_front(2);
	cout << dq[0] << endl;
	return 0;
}
```

처음/끝 부분의 삽입/삭제가 벡터보다 빠르게 동작함    
중간 삽입/삭제는 벡터와 같이 비효율적으로 동작    
블록별로 카운트를 할 수 있기 때문에 임의접근이 가능함    

