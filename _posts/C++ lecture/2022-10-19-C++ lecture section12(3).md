---
title:  "C++ lecture section12 [3/4]"
excerpt: "Modern C++"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.19 22:00:00
---

# 오른값 참조(rvalue reference)

lvalue(왼값) : 단일식을 넘어서 계속 지속되는 개체    
rvalue(오른값) : lavlue가 아닌 나머지 (임시 값, 열거형, 람다, i++ 등)    

```cpp
class Pet
{

};

class Knight
{
	public:
		Knight()
		{
			cout << "Knight()" << endl;
		}
		Knight(const Kngiht& knight)
		{
			cout << "const Knight&" << endl;
		}
		~Knight()
		{
			if (_pet)
				delete _pet;
		}

		// 이동 생성자
		Knight(Knight&& knight)
		{

		}

		void operator=(const Knight& knight)
		{
			cout << "operator=(const Knight&)" << endl;
			_hp = knight._hp;
			if (knight._pet)
				_pet = new Pet(*knight._pet);
		}

		// 이동 대입 연산자
		void operator=(Knight&& knight) noexcept
		{
			cout << "operator(Knight&&)" << endl;
			_hp = knight._hp;
			_pet = knight._pet;
			knight._pet = nullptr;
		}
	public:
		int _hp = 100;
		Pet* _pet = nullptr;
};

void TestKnight_Copy(Knight knight) {}
void TestKnight_LValueRef(Knight& knight) {} 
void TestKnight_ConstLValueRef(const Knight& knight) {}
void TestKnight_RValueRef(Knight&& knight) {} // 이동 대상

int main()
{
	Knight k1;
	TestKnight_Copy(k1);
	TestKnight_LValueRef(k1);
	TestKnight_LValueRef(Knight()); // Knight()로 생성된 객체는 Lvalue가 아니므로 불가능
	TestKnight_ConstLValueRef(Knight()); // const가 붙은 rvalue는 가능

	TestKnight_RValueRef(k1); // lvalue는 불가능
	TestKnight_RvalueRef(Knight()); // rvalue는 가능
	TestKnight_RvalueRef(static_cast<Knight&&>(k1)); 

	Knight k2;
	k2._pet = new Pet();
	k2._hp = 1000;

	Knight k3;
	k3 = static_cast<Kngiht&&>(k2);
	k3 = std::move(k2); // 오른값 참조로 캐스팅, 이름 후보중 하나가 rvalue_cast

	std::unique_ptr<Knight> uptr = std::make_unique<Knight>();
	std::unique_ptr<Knight> uptr2 = std::move(uptr);

	return 0;
}
```

rvalue로 참조할시 원본 데이터가 유지되지 않아도 됨    
rvalue로 참조하는 것은 원본은 날려도 된다는 힌트를 주는 쪽에 가까움    
따라서 이동시 얕은 복사도 사용 가능    

***

# 전달 참조(forwarding reference)

원래는 보편 참조(universal reference) 라는 이름을 가지고있었음    

```cpp
class Knight
{
	public:
		Knight() { cout << "기본 생성자" << endl; }
		Knight(const Knight&) { cout << "복사 생성자" << endl; }
		Knight(Knight&&) noexcept { cout << "이동 생성자" << endl; }
}

void TestRValueRef(Knight&& k)
{
}

void Test_Copy(Knight k)
{
}

template<typename T>
void Test_ForwardingRef(T&& param)
{
	// 왼값 참조일 경우 복사
	// 오른값 참조일 경우 이동
	Test_Copy(std::forward<T>(param));
}

int main()
{
	Knight k1;
	Test_RvalueRef(std::move(k1)); // rvalue_cast

	test_ForwardingRef(k1); // lvalue
	test_ForwardingRef(std::move(k1)); // rvalue

	auto&& k2 = k1; // lvalue
	auto&& k3 = std::move(k1); // rvalue

	Knight k;
	Knight& k4 = k; // lvalue
	Knight&& k5 = std::move(k); // rvalue
	Test_RvalueRef(k5); // 오른값 참조를 참조 - 불가능
	Test_RvalueRef(std::move(k5)); // 오른값 참조 - 가능
}
```

전달 참조는 형식 연역(type deduction)이 일어날때만 발생    
`lvalue`를 넣어주면 `lvalue` 참조, `rvalue`를 넣어주면 `rvalue` 참조함    
함수의 케이스를 줄일 수 있어 편리함    
단, 이 때 `lvalue`와 `rvalue` 둘 중 어느 것을 받더라도 같은 동작을 해야함    

오른값 : 왼값이 아님 = 단일식에서 벗어나면 사용할 수 없음    
오른값 참조 : 오른값만 참조할 수 있는 참조 타입    
