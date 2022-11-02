---
title:  "C++ lecture section12 [4/4]"
excerpt: "Modern C++"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.20 19:00:00
---

# 람다(lambda)

함수 객체를 빠르게 만드는 문법    
람다 자체가 C++11의 새로운 기능은 아님    

```cpp
enum class ItmeType
{
	None,
	Armor,
	Weapon,
	Jewelry,
	Consumable
};

enum class Rarity
{
	Common,
	Rare,
	Unique
};

class Item
{
	public:
		Item() {}
		Item(int itemid, Rarity rarity, ItemType type)
			: _itemid(itemid), _rarity(rarity), _type(type) {}
	public:
		int _itemid = 0;
		Rarity _rarity = Rarity::Common;
		ItemType _type = ItemType::None;
};

int main()
{
	vector<Item> v;
	v.push_back(Item(1, Rarity::Common, ItemType::Weapon));
	v.push_back(Item(2, Rarity::Common, ItemType::Armor));
	v.push_back(Item(3, Rarity::Rare, ItemType::Jewelry));
	v.push_back(Item(4, Rarity::Unique, ItemType::Weapon));

	{
		struct IsUniqueItem
		{
			FindItemByItemid(int itemid) : _itemid(itemid)
			{

			}

			bool operator()(Item& item)
			{
				return item._rarity == Rarity::Unique;
			}

			int _itemid;
		};
		auto findit = std::find_if(v.begin(), v.end(), IsUniqueItem());

		int itemid = 4;
		auto findit = std::find_if(v.begin(), v.end(), FindItemByItemid(itemid);
		
		if (findit != v.end())
			cout << "아이템 ID : " << findit->_itmeid << endl;
	}

	{
		// 람다 표현식(lambda expression)
		// 클로저 (closure) : 람다에 의해 만들어진 실행시점 객체
		auto isUniqueLambda = [](Item& item)
		{
			return item._rarity == Rarity::Unique;
		};
		auto findit = std::find_if(v.begin(), v.end(), 
			[](Item& item) { return item._rarity == Rarity::Unique; });
		
		auto findByItemidLambda = [=](Item& item) { return Item.itemid == _itemid; };
		auto findit = std::find_it(v.begin(), v.end(), findByItemidLambda);

		if (findit != v.end())
			cout << "아이템 ID : " << findit->_itmeid << endl;
	}

	{
		class Knight
		{
			public:
				auto ResetHpJob()
				{
					auto f = [this]()
					{
						this->_hp = 200;
					};
					return f;
				}
			public:
				int _hp = 100;
		}

		Knight* k = new Knight();
		auto job = k->ResetHpJob();
		delete k;
		job(); // 메모리 오염 발생
	}
}
```

\[캡처](인자값) { 구현부 }의 형태로 사용    
`[]` : 캡처(capture), 함수 객체 내부에 변수를 저장하는 개념과 유사    
* 기본 캡처 모드는 값(복사) 방식(`=`)과 참조 방식(`&`) 중에 선택할 수 있음    
* 변수마다 캡처 모드를 지정해서 사용하는 것도 가능함    
* 캡처 모드를 일괄적으로 설정하는 것을 지양해야함    

***

# 스마트 포인터(smart pointer)

```cpp
class Knight
{
	public:
		Knight() { cout << "Knight 생성" << endl; }
		~Knight() { cout << "Knight 소멸" << endl; }

		void Attack()
		{
			if (_target)
			{
				_target->_hp -= _damage;
				cout << "Hp : " << _target->_hp << endl;
			}
		}

	public:
		int _hp = 100;
		int _damage = 10;
		Knight* _target = nullptr;
};

int main()
{
	Knight* k1 = new Knight();
	Knight* k2 = new Knight();

	k1->_target = k2;
	delete k2;
	k1->Attack(); // 크래시는 발생하지 않으나 이미 해제된 메모리를 참조하여 엉뚱한 결과가 발생함

	return 0;
}
```

```cpp
class RefCountBlock
{
	public:
		int _refCount = 1;
};

template<typename T>
class SharedPtr
{
	public:
		SharedPtr() {}
		SharedPtr(T* ptr) : _ptr(ptr)
		{
			if (_ptr != nullptr)
			{
				_block = new RefCountBlock();
				cout << "RefCount : " << _block->_refCount << endl;
			}
		}

		SharedPtr(const SharedPtr* sptr) : _ptr(sptr._ptr), _block(sptr._block)
		{
			if (_ptr != nullptr)
			{
				_block->refCount++;
				cout << "RefCount : " << _block->_refCount << endl;
			}
		}

		void operator=(const SharedPtr& sptr)
		{
			_ptr = sptr._ptr;
			_block = sptr._block;
			if (_ptr != nullptr)
			{
				_block->refCount++;
				cout << "RefCount : " << _block->_refCount << endl;
			}
		}

		~SharedPtr()
		{
			if (_ptr != nullptr)
			{
				_block->_refCount--;
				cout << "RefCount : " << _block->_refCount << endl;
				if (_block->_refCount == 0)
				{
					delete _ptr;
					delete _block;
					cout << "Delete data" << endl;
				}
			}
		}
	public:
		T* _ptr = nullptr;
		RefCountBlock* _block = nullptr;
}

int main()
{
	SharedPtr<Knight> k2;

	{
		SharedPtr<Knight> k1(new Knight());
		k2 = k1;
	}
}
```

```cpp
class Knight
{
	public:
		Knight() { cout << "Knight 생성" << endl; }
		~Knight() { cout << "Knight 소멸" << endl; }

		void Attack()
		{
			if (_target)
			{
				_target->_hp -= _damage;
				cout << "Hp : " << _target->_hp << endl;
			}
		}

	public:
		int _hp = 100;
		int _damage = 10;
		shared_ptr<Knight>() _target = nullptr;
};

int main()
{
	shared_ptr<Knight> k1 = make_shared<Kngiht>();
	{
		shared_ptr<Knight> k2 = make_shared<Kngiht>();
		k1->_target = k2;
	}

	k1->Attack();

	return 0;
}
```

스마트 포인터 : 포인터를 알맞은 정책에 따라 관리하는 객체 (포인터를 래핑해서 사용)
* `shared_ptr`, `weak_ptr`, `unique_ptr` 존재
* Modern C++에서는 기존 포인터는 거의 사용하지 않는다고 보면 됨

```cpp
int main()
{
	shared_ptr<Knight> k1 = make_shared<Kngiht>();
	{
		shared_ptr<Knight> k2 = make_shared<Kngiht>();
		k1->_target = k2;
		k2->_target = k1;
	}
	k1->Attack();

	return 0;
}
```

순환구조일 경우 `shared_ptr`에서는 문제가 발생할 수 있음    
이 경우 `weakcount`를 가지고있는 `weak_ptr`을 활용할 수 있음    
단, `weak_ptr`은 직접적으로 메모리 해제에 관여하지 않으므로 명시적으로 메모리 체크 후 `shared_ptr`로 변환하는 과정이 요구됨    

`unique_ptr`은 일반적인 복사를 막아 다른 곳에 넘겨줄 수 없음    
