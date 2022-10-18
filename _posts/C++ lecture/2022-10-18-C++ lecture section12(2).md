---
title:  "C++ lecture section12 [2/4]"
excerpt: "Modern C++"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.10.18 18:00:00
---

# nullptr

기존에는 0과 `NULL`을 사용    

```cpp
void Test(int a)
{
	...
}

void Test(void* ptr)
{
	...
}

int main()
{
	Test(0);
	Test(NULL);
	Test(nullptr);
}
```

함수가 오버로딩 되어있을 경우 0이나 `NULL`을 사용하면 포인터를 활용할 때 문제가 생길 수 있음    
즉, 위와 같은 경우 `Test(0)`과 `Test(NULL)`은 모두 `int`형을 매개변수로 하는 버전으로 작동함    
반면 `nullptr`은 명확히 포인터 타입을 가리키기 때문에 `Test(nullptr)`은 `void*` 형을 매개변수로 하는 버전으로 작동함    
또한 가독성 측면에서도 `nullptr`의 사용은 필수적임    

```cpp
class NullPtr
{
	public:
		//어떤 타입의 포인터와도 치환 가능
		template<typename T>
		operator T* () const
		{
			return 0;
		}
		// 어떤 타입의 멤버 포인터와도 치환 가능
		template<typename C, typename T>
		operator T C::* () const
		{
			return 0;
		}
	private:
		// 주소값 &을 막음
		vodi operator&() const;
};
```

***

# using

```cpp
typedef vector<int>::iterator Vecit;

typedef int id;
using id2 = int;

typedef void(*MyFunc)();
using MyFunc2 = void(*)();

template<typename T>
typedef std::vector<T> List; // 불가능함

template<typename T>
using List2 = std::list<T> // 가능함

```

`using`이 `typedef`보다 직관성이 좋음    
`using`은 `typedef`와는 달리 템플릿을 이용할 수 있음    
기본적으로 `typedef`보다 `using`이 모든 면에서 뛰어남    

***

# enum class

```cpp
enum PlayerType
{
	PT_Knight,
	PT_Archer,
	PT_Mage
};

double value = PT_Knight;
```

`enum`의 경우 `unscoped enum`으로 `enum`에 설정되어있는 이름은 전역 사용 범위를 가지고있음    
암묵적인 변환이 가능함    

```cpp
enum class ObjectType
{
	Player,
	Monster,
	Projectile
};

double value = ObjectType::Plyaer; // 불가능
```

`enum class`의 경우 `scoped enum`으로 설정되어있는 이름이 영역 안에서만 유효함    
암묵적인 변환이 불가능함    

***

# delete (삭제된 함수)

```cpp
class Knight
{
	public:
	
	private:
		void operator=(const Knight* k); // 사용은 가능하나 문제가있음
		void operator=(const Knight& k) = delete;
	private:
		int _hp = 100;
}
```

기존에는 사용하지 않는 함수를 `private`에 정의되지 않은 함수로 만들어줌    
그러나 `private`에 정의하더라도 꺼내서 쓸 수 있는 경우가 발생할 수 있음     
`private`에 선언한 함수의 구현부를 만들지 않을 시에는 함수의 사용을 방지할 수 있으나, 문제를 조기에 발견할 수 없음    

함수 뒤에 `= delete`를 붙여줄 경우 더이상 사용하지 않을 함수라는 것을 확실하게 나타낼 수 있음    

***

# override, final

```cpp
class Player
{
	public:
		virtual void Attack()
		{
			...
		}
};

class Knight : public Player
{
	public:
		// 재정의(override)
		virtual void Attack() override
		{
			...
		}
		// 오버로딩(overloading) : 함수 이름 재사용
		void Attack(int a)
		{
			...
		}
};

int main()
{
	Knight* knight = new Knight();
	knight->Attack();

	Player* player = new Knight();
	player->Attack();

	return 0;
}
```

`virtual`로 지정한 함수의 최초 사용이 어디였는지를 알 수 없음    
따라서 재정의한 함수라는 것을 나타태기 위해 함수 뒤에 `override` 키워드를 붙여 사용할 수 있음    

마지막으로 사용될 함수의 경우 `final`을 붙여 더이상 재정의되지 않도록 만들 수 있음    