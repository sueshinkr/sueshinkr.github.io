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

***

# vector #3

***

# vector #4