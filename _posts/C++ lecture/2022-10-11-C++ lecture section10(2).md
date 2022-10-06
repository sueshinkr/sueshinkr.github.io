---
title:  "C++ lecture section11 [2/2]"
excerpt: "콜백 함수"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.11 18:00:00
---

# 템플릿 기초 #1

```cpp
template <typename T>
void Print(T a)
{
	cout << a << endl;
}

int main()
{
	print(50);
	print(50.0);
	print("Hello World");
	print<int>(50.0f);

	return 0;
}
```

템플릿 : 함수나 클래스를 찍어내는 틀
* 함수 템플릿과 클래스 템플릿이 존재    
* `template <typename T>` 또는 `template <class T1, class T2>`의 형식으로 사용    

```cpp
class Knight
{
	public:
		...
	public:
		int _hp = 100;
};

template <typename T>
void Print(T a)
{
	cout << a << endl;
}

template<<>
void Print(Knight a)
{
	cout << "Knight!!!" << endl;
	cout << a._hp << endl;
}

ostream& operator<<(ostream& os, const Knights& k)
{
	os << k._hol
	return os;
}

int main()
{
	Knight k1;
	Print(k1);
}
```

`Knight` 클래스형으로 `Print` 함수를 사용하고 싶은 경우, `Print` 함수에서 사용하는 `<<`연산자에 대해 `Knight`형의 오버로딩이 필요함    
또는 템플릿 특수화를 통해 `Print` 템플릿 함수를 `Knight`에 대해서만 재정의할 수 있음    

***

# 템플릿 기초 #2

```cpp
template<typename T, iny SIZE>
class RandomBox
{
	public:
		T GetRandomData()
		{
			int idx = rand() % 10;
			return _data[idx];
		}
	public:
		T_data[SIZE];
}

template<int SIZE>
class RandomBox<double, SIZE>
{
	public:
		T GetRandomData()
		{
			cout << "RandomBox Double" << endl;
			int idx = rand() % 10;
			return _data[idx];
		}
	public:
		T_data[SIZE];

}

int main()
{
	srand(static_cast<unsigned int>time(nullptr));

	RandomBox<int, 10> rb1;

	for (int i = 0; i < 10; i++)
		rb1._data[i] = i;
	int value1 = rb1.GetRandomData();
	cout << value1 << endl;

	RandoBox<double, 20> rb2;
	for (int i = 0; i < 20; i++)
		rb2._data[i] = i + 0.5f;
	int value2 = rb2.GetRandomData();
	cout << value2 << endl;
}
```

클래스 템플릿도 함수 템플릿과 거의 비슷하게 사용    
서로 다른 인수를 사용하면 서로 다른 템플릿 클래스가 생성되며, 각각 별개의 클래스로 취급됨    
템플릿 특수화도 사용 가능    

***

# 콜백 함수

콜백(Callback) : 어떤 상황 발생시 특정 기능을 호출    

```cpp
class Item
{
	public:
	public:
		int _itemid = 0;
		int _rarity - 0
		int _owenerid = 0;
};

class FindByOwnerid
{
	public:
		bool operator()(const Item*)
			return (item->_ownerid == _ownerid);
	public:
		int _owenerid;
};

class FindByRarity
{
	public:
		bool operator()(const Item*)
			return (item->_rarity == _rarity);
	public:
		int _rarity;
};

template <typename T>
Item* Finditem(Item items[], int itemCount, T selector)
{
	for (int i = 0; i < itemCount; i++)
	{
		Item* item = &items[i];
		if (selector(item))
			return item;
	}
	return nullptr;
}

int main()
{
	Item items[10];

	FindByOwnerid functor1;
	functor1._ownerid = 100;

	FindByRariry functor2;
	functor2._rarity = 1;

	Item* item1 = Finditem(items, 10, functor1);
	Item* item2 = Finditem(items, 10, functor2);

	return 0;
}
```