---
title:  "C++ lecture section7 [2/2]"
excerpt: "동적 할당"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.05 16:00:00
---

# 타입 변환 #3

```cpp
class Item
{
	public:
		Item() { cout << "item()" << endl; }
		Item(const Item& item) { cout << "item(const item&)" << endl; }
		~Item() { cout << "~item()" << endl; }
	public:
		int _itemType = 0;
		int _itemDbid = 0;
		char _dummy[4096] = {};
}

void Testitem(Item item)
{
}

void TestItemPtr(Item* item)
{
}

int main()
{
	{
		Item item1;
		Item* item2 = new Item();

		TestItem(item1);
		TestItem(*item2);
		TestItemPtr(&item1);
		TestItemPtr(item2);

		delete item2;
	}

	{
		Item item3[100] = {};
		Item* item4[100] = {};

		for (int i = 0; i < 100; i++)
			item4[i] = new Item();
		for (int i = 0; i < 100; i++)
			delete item4[i];
	}
}
```
`item1`은 선언시 생성자가 호출되고 블록을 벗어남과 동시에 파괴자가 호출됨    
`item2`는 할당시 생성자가 호출되고, `delete`를 해주어야만 파괴자가 호출됨    
`delete`로 할당을 해제해주지 않으면 메모리 누수가 발생하여 점차 가용 메모리가 줄어드는 문제가 발생함    

값에 의한 전달로 함수 호출시 객체가 복사되며 복사 생성자를 호출함    
포인터 매개변수로 함수 호출시에는 복사하는 것이 아니므로 별다른 일이 생기지 않음    

`item3` 배열에는 실제 `Item` 객체가 100개 존재    
`item4` 배열에는 `Item` 객체의 주소를 나타내는 포인터가 100개 존재    

***

# 타입 변환 #4

```cpp
class Knight
{
	public:
		int _hp = 0;
}

class Item
{
	public:
		Item() { cout << "item()" << endl; }
		Item(int itemType) : _itemType(itemType) {}
		Item(const Item& item) { cout << "item(const item&)" << endl; }
		~Item() { cout << "~item()" << endl; }
	public:
		int _itemType = 0;
		int _itemDbid = 0;
		char _dummy[4096] = {};
}

enum ItemType
{
	IT_WEAPON = 1,
	IT_ARMOR = 2
};

class Weapon : public Item
{
	public:
		Weapon() : Item(IT_WEAPON) { cout << "Weapon()" << endl;}
		~Weapon() { cout << "~Weapon()" << endl; }
	public:
		int _damage = 0;
};

class Armor : public Item
{
	public:
		Armor() : Item(IT_ARMOR) { cout << "Armor()" << endl; }
		~Armor() { cout << "~Armor()" << endl; }
};

int main()
{
	{
		Knight* knight = new Knight();
		Item* item = (Item*)knight;
		item->_itemType = 2;
		item->_itemDbid = 1; 
		delete knight; // 에러 발생
	}

	{
		Item* item = new Item;
		Weapon* weapon = (Weapon*)item;
		weapon->_damage = 10; // 에러 발생
		delete item;
	}

	{
		Weapon* weapon = new Weapon();
		Item* item = weapon;
		delete weapon;
	}
}
```

`Knight`형을 `Item`형으로 변환하는 것과 같이 잘못된 포인터형 변환시 메모리 오염에 주의해야함    

부모 클래스를 자식 클래스 형으로 변환하려면 명시적 변환이 필요    
단, 변환시 문제가 발생할 소지가 매우 높음    

자식 클래스를 부모 클래스 형으로 변환시에는 암시적 변환이 가능    

***

# 타입 변환 #5

```cpp
int main
{
	Item* inventory[20] = {};
	srand((unsigned int)time(nullptr));

	for (int i = 0; i < 20; i++)
	{
		int randValue = rand() % 2;
		switch (randValue)
		{
			case 0:
				inventory[i] = new Weapon();
				break;
			case 1:
				inventory[i] = new Armor();
				break;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		Item* item = inventory[i];
		if (item == nullptr)
			continue;
		if (item->_itemType == IT_WEAPON)
		{
			Weapon* weapon = (Weapon*)item;
			cout << "Weapon Damage : " << weapon->_damage << endl;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		Item* item = inventory[i];
		if (item == nullptr)
			continue;

		delete item; // 소멸자를 가상 함수로 선언시 가능한 해제

		// 소멸자가 가상 함수로 선언되지 않았을 시 올바른 해제
		if (item->_itemType == IT_WEAPON)
		{
			Weapon* weapon = (Weapon*)item;
			delete weapon;
		}
		else
		{
			Armor* armor = (Armor*)item;
			delete armor;
		}
	}
	return 0;
}
```

`Item` 클래스의 소멸자가 가상 함수로 선언되지 않았을 시 `item` 객체를 곧바로 `delete`할 경우 원본 객체가 `Weapon`형이어도 `Item`형의 소멸자만 호출됨    
반면 `Item` 클래스의 소멸자를 가상 함수로 선언한 경우에는 `delete`가 대상 객체의 타입에 따른 소멸자를 호출함    
따라서 부모-자식 관계에서 부모 클래스의 소멸자는 `virtual` 키워드를 붙여 가상 함수로 선언해야함    

***

# 얕은 복사 vs 깊은 복사 #1

```cpp
class Pet
{
	public:
		Pet() { cout << "Pet()" << endl; }
		~Pet() { cout << "~Pet()" << endl; }
		Pet(const Pet& pet) { cout << "Pet(const)" << endl; }
}

class Knight
{
	public:
	public:
		int _hp = 100;
		Pet* pet;
};

int main()
{
	Pet* pet = new Pet();

	Knight knight; // 기본 생성자 사용
	knight._hp = 200;
	knight._pet = pet;

	Knight knight2 = knight; // 복사 생성자 사용
	//Knight knight2(knight);

	Knight knight3; // 기본 생성자 사용
	knight3 = knight; // 복사 대입 연산자 사용

	return 0;
}
```

복사 생성자와 복사 대입 연산자는 따로 선언하지 않았을시 컴파일러가 암시적으로 디폴트 버전을 생성하여 사용함    
단, 이 때는 얕은 복사(Shallow Copy)가 수행되므로 멤버 데이터가 비트열 단위로 똑같이 복사됨    
즉 포인터 멤버를 가지고 있는 경우에는 얕은 복사 수행시 해당 주소값이 그대로 복사되어 복사된 개체와 원본 개체가 동일한 대상을 가리키는 문제가 발생함    

```cpp
class Knight
{
	public:
		Knight() { _pet = new Pet(); }
		Knight(const Knight& knight)
		{
			_hp = knight._hp;
			_pet = new Pet(*knight._pet); 
		}
		Knight& operator=(const Knight& knight)
		{
			_hp = knight._hp;
			_pet = new Pet(*knight._pet); 
			return *this;
		}
		~Knight() { delete _pet; }
}
```


따라서 멤버 데이터가 참조(주소) 값이라면 원본 객체가 참조하는 대상까지 새로 만들어서 복사하는 깊은 복사(Deep Copy)를 수행해야함    
이를 위해 복사 생성자와 복사 대입 연산자를 명시적으로 정의해주어야함    

***

# 얕은 복사 vs 깊은 복사 #2

암시적 복사 생성자
* 부모 클래스의 복사 생성자 호출
* 멤버 클래스의 복사 생성자 호출
* 멤버가 기본 타입일 경우 얇은 복사가 수행되며 메모리를 복사

명시적 복사 생성자
* 부모 클래스의 기본 생성자 호출
* 멤버 클래스의 기본 생성자 호출
* 다른 생성자를 호출하고싶다면 멤버 초기자 리스트를 사용해야함

암시적 복사 대입 연산자
* 부모 클래스의 복사 대입 연산자 호출
* 멤버 클래스의 복사 대입 연산자 호출
* 멤버가 기본 타입일 경우 얇은 복사가 수행되며 메모리를 복사

명시적 복사 대입 연산자
* 컴파일러가 자동적으로 해주는 것이 없음
* 따라서 사용자가 직접 부모 클래스 및 멤버 클래스에 대한 복사 대입 연산을 처리해주어야함

***

# 캐스팅 4총사

```cpp
class Player
{
	public:
		virtual ~Player() {}
};

class Knight : public Player
{
};

class Dog
{
};

int main()
{
	int hp = 100;
	int maxHp = 200;
	float ratio = static_cast<float>(hp) / maxHp;

	Player* p = new Knight();
	Knight* k1 = static_cast<Knight*>(p);
	Knight* k2 = dynamic_cast<Knight*>(p);
	return 0;
}
```

`static_cast` : 타입 원칙에 비춰볼 때 상식적인 캐스팅만 허용    
* `int` <-> `float`와 같은 경우
* 부모 클래스에서 자식 클래스로의 다운캐스팅, 단 안전성은 보장 못함

`dynamic_cast` : 상속 관계에서의 안전한 형변환    
* `RTTI`(RunTime Type Information)으로 다형성을 활용
* 반드시 가상 함수가 포함되어 있어야하며, 객체의 메모리에 기입되어있는 가상 함수 테이블을 체크하여 캐스팅
* 잘못된 타입으로 캐스팅했을시 `nullptr`을 반환
* 따라서 올바른 타입으로 캐스팅했는지 여부를 확인하는데 유용함

```cpp
void PrintName(char* str)
{
	cout << str << endl;
}

int main()
{
	PrintName("ABCDE"); // 에러
	PrintName(const_cast<char*>("ABCDE"));
	return 0;
}
```

`const_cast` : `const`를 붙이거나 제거하는데 사용    

```cpp
int main()
{
	Player* p = new Knight();
	Knight* k = dynamic_cast<Knight*>(p);

	__int64 address = reinterpret_cast<__int64>(k);
	Dog* dog = reinterpret_cast<Dog*>(k);

	void* r = malloc(1000);
	Dog* dog2 = reinterpret_cast<Dog*>(r);
}
```

`reinterpret_cast` : 가장 위험하고 강력한 형태의 캐스팅    
* 포인터랑 전혀 관계없는 다른 타입으로의 변환 등에 사용
* `void*` 형으로 반환되는 `malloc`을 활용할 때도 사용됨
