---
title:  "C++ lecture section11 [1/3]"
excerpt: "STL"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.12 18:00:00
---

# vector #1

STL(Standard Template Library) : 프로그래밍할 때 필요한 자료구조/알고리즘들을 템플릿으로 제공하는 라이브러리
* 컨테이너(Container) : 데이터를 저장하는 객체 (자료구조, Data Structure)
* 벡터(vector)는 컨테이너의 하위 개념

```cpp
#include <vector>
using namespace std;

vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
v.push_back(4);
v.push_back(5);

const int size = v.size();
for (int i = 0; i < size; i++)
{
	cout << v[i] << endl;
}
```

vector : 동적 배열    
동적 배열은 어떻게 배열을 유동적으로 사용하는가?    
* 메모리 할당시 여유분을 두고 할당함
* 여유분까지 꽉 찼을시 메모리를 증설함

여유분은 얼마나? 증설은 얼마나? 기존 데이터 처리는?

```cpp
vector<int> v;
v.size(); // 실제 사용 데이터 개수
v.capacity(); // 어유분을 포함한 용량 개수

for (int i = 0; i < 1000; i++)
{
	v.push_back(100);
	cout << v.size() << " " << v.capacity() << endl;
}
```

`v.push_back()` : 마지막 배열이 가리키는 메모리에 값 저장    
`v.pop_back()` : 마지막 배열이 가리키는 메모리의 값을 제거   
`v.front`, `v.back()` : 처음 / 마지막 데이터 확인    

`capacity`는 컴파일러에 따라 1.5 ~ 2배씩 늘어남    
메모리 증설시 여유분을 늘린 메모리 공간을 확보 후 기존의 메모리를 옮김    

`v.reserve()` 함수를 사용하여 `capacity`를 지정할 수 있음    
* `v.clear()`등으로 메모리에 저장된 값을 삭제해도 `capacity`는 줄어들지 않음
* 임시 벡터 생성 후 스왑하는 것으로 `capacity`로 할당된 메모리까지 확실하게 날릴 수는 있음

`v.resize()` 함수를 사용하여 `size`를 지정할 수 있음
* 이 때 `capacity`도 자동으로 늘어남    
* `v.push_back()` 함수 사용시 `size`로 지정한 메모리 이후에 값이 저장되기 때문에 `v.resize()`로 지정한 메모리에는 각각 값을 대입해주어야 함    
* `vector<int> v(1000)`과 같이 생성과 동시에 사이즈를 변경할 수 있음    

벡터에 다른 벡터를 대입시 벡터가 가지고있던 모든 특성과 데이터가 복사됨    

***

# vector #2

반복자(Iterator) : 포인터와 유사한 개념, 컨테이너의 원소(데이터)를 가리키고 다음/이전 원소로 이동 가능    
* 값 참조, 증감 연산자들이 연산자 오버로딩을 통해 지원됨
* `v.begin()`은 벡터의 시작 원소를, `v.end()`는 벡터의 마지막 원소 바로 다음 주소를 가리키는 이터레이터를 반환함
* 복사 과정을 거치지 않는 전위증감이 후위증감보다 성능이 좋음
* `iterator`는 `vector` 뿐 아니라 다른 컨테이너에도 공통적으로 있는 개념이기 때문에 일반화하여 사용할 수 있음

```cpp
int main()
{
	vector<int> v(10);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
		v[i] = i;

	vector<int>::iterator it;
	int* ptr;

	it = v.begin();
	ptr = &v[0];

	cout << (*it) << endl;
	cout << (*pt) << endl;

	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		cout << (*it) << endl;

	return 0;
}
```

`vector<int>::const_iterator cit = v.cbegin()`의 형태로 `const`처럼 사용 가능    
`for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)`의 형태로 역순으로 사용 가능    

***

# vector #3

`vector`같은 배열 구조의 경우 처음/중간 위치의 삽입/삭제가 상당히 비효율적임    
끝 위치의 삽입/삭제는 아무런 문제 없이 효율적으로 작동함    
모든 데이터들이 연속되어있기 때문에 임의 접근(random access)이 가능    

`v.insert(v.begin() + 2, 5);`의 형태로 중간에 삽입할 수 있음    
`v.erase(v.begin() + 2);`의 형태로 중간 데이터를 삭제할 수 있음    

```cpp
for (vector<int>::iterator it = v.begin(); it != v.end(); )
{
	int data = *it;
	if (data = 3)
		it = v.erase();
	else
		++it;
}
```
위와 같이 이터레이터를 사용하는 도중에 `erase()`, `clear()` 함수 등으로 조작할 경우 주의가 필요함    

***

# vector #4

```cpp
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Iterator
{
	public:
		Iterator() : _ptr(nullptr) {}
		Iterator(T* ptr) : _ptr(ptr) {}
		Iterator& operator++()
		{
			_ptr++;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator temp = *this;
			_ptr++;
			return temp;
		}

		Iterator& operator--()
		{
			_ptr--;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator temp = *this;
			_ptr--;
			return temp;
		}

		Iterator operator+(const int count)
		{
			Iterator temp = *this;
			temp._ptr += count;
			return temp;
		}
		Iterator operator-(const int count)
		{
			Iterator temp = *this;
			temp._ptr -= count;
			return temp;
		}

		bool operator==(const Iterator& right) { return _ptr == right._ptr; }
		bool operator!=(const Iterator& right) { return !(*this == right); }
		T& operator*() { return *_ptr; }

	public:
		T* _ptr;
};


template<typename T>
class Vector
{
	public:
		Vector() : _data(nullptr), _size(0), _capacity(0) {}
		~Vector()
		{
			if (_data)
				delete[] _data;
		}

		void push_back(const T& val)
		{
			if (_size == _capacity)
			{
				int newcapacity = static_cast<int>(_capacity * 1.5);
				if (newcapacity == _capacity)
					newcapacity++;
				
				reserve(newcapacity);
			}
			_data[_size] = val;
			_size++;
		}

		void reserve(int capacity)
		{
			_capacity = capacity;
			T* newData = new T[_capacity];
			for (int i = 0; i < _size; i++)
				newData[i] = _data[i];
			if (_data)
				delete[] _data;

			_data = newData;
		}
		T& operator[](const int pos) { return _data[pos]; }
		int size() { return _size; }
		int capacity() { return _capacity; }
	public:
		typedef Iterator<T> iterator;
		iterator begin() { return iterator(&_data[0]); }
		iterator end() { return begin() + _size; }
	private:
		T* _data;
		int _size;
		int _capacity;

};

int main()
{
	Vector<int> v;

	//v.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		v.push_back(i);
		cout << v.size() << " " << v.capacity() << endl;
	}

	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;

	cout << "-------------------" << endl;

	for (Vector<int>::iterator it = v.begin(); it != v.end(); ++it)
		cout << (*it) << endl;
}
```