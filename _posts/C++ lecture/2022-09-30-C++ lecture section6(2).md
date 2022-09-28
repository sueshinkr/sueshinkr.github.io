---
title:  "C++ lecture section6 [2/3]"
excerpt: "객체지향 여행"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.30 11:00:00
---

# 은닉성

은닉성(Data Hiding) : 클래스에서는 조작해서는 안되는 데이터를 은닉할 수 있음    

```cpp
class Car
{
	public:
		void MoveHandle() {}
		void PushPedal() {}
		void OpenDoor() {}
		void TurnKey()
		{
			...
			RunEngine();
		}
	protected:
		void RunEngine() {}
	private:
		void DisassembleCar() {}
		void ConnectCircuit() {}
	public:
		// 핸들
		// 페달
		...
};

class SuperCar : public Car // 여기서의 public은 상속 접근 지정자로 쓰임
{
	public:
		void PushRemoteCOntroller()
		{
			RunEngine();
		}
}
```

멤버 접근 지정자(제한자)
* `public` : 누구든지 접근 가능
* `protected` : 파생 클래스에서는 자유롭게 접근 가능
* `private` : 해당 클래스 내부에서만 접근 가능

상속 접근 지정자를 통해 현재의 부모를 다음 세대에 어떻게 전달할 것인지 선택할 수 있음    
* `public` : 부모의 멤버 지정을 그대로 사용
* `protected` : 부모의 `public`을 `protected`로 전달
* `private` : 모든 멤버를 `private`로 전달
* 대부분 `public`으로 사용됨    
* 생략할시에는 `private`로 적용됨

```cpp
class Berserker
{
	public:
		int GetHP() { return _hp; }
		void SetHP(int hp)
		{
			_hp = hp;
			if (_hp < 50)
				SetBerserkerMode();
		}
	private:
		int _hp = 100;
		void SetBerserkerMode()
		{
			cout << "매우 강해짐!" << endl;
		}
}

int main()
{
	Berserker b;
	b.SetHP(20);
}
```

캡슐화(Encapsulation) : 연관된 데이터와 함수를 논리적으로 묶어놓은 것    
데이터의 조작 경로를 제어하는데 매우 유용하게 사용됨    

***

# 다형성 #1

다형성(Polymorphism) : 겉은 똑같아도 기능이 다르게 동작하는 것
* 오버로딩(Overloading) : 함수 중복 정의 = 함수 이름의 재사용
* 오버라이딩(Overriding) : 부모 클래스의 함수를 자식 클래스에서 재정의

```cpp
class Player
{
	public:
		void Move() { cout << "Move Player" << endl; }
		void Move(int a) { cout << "Move Player(int)" << endl; }
	public:
		int _hp;
};
```
서로 다른 시그니처를 가지고있는 `Move()`는 오버로딩으로 활용되어 에러를 발생시키지 않음    

```cpp
class Knight : public Plyaer
{
	public:
		void Move() { cout << "Move Knight" << endl; }
	public:
		int _stamina;
};

class Mage : public Player
{
	public:
		int _mp;
};

void MovePlayer(Player* player)
{
	player->Move();
}

void MoveKnight(Kngiht* knight)
{
	knight->Move();
}

int main()
{
	player p;
	MovePlayer(&p);
	MovePlayer(&p);  // 에러 - 플레이어는 기사가 아닐 수 있음

	Knight k;
	MoveKnight(&k);
	MovePlayer(&k);  // 정상작동 - 기사는 반드시 플레이어임

	return 0;
}
```

`MovePlayer()` 함수는 모든 자식 클래스들이 사용할 수 있음    
따라서 `MoveKnight()` 함수와 같이 각각의 자식 클래스별로 함수를 정의할 필요가 없기 때문에 훨씬 효율적인 구조를 작성할 수 있음    
단, 이 경우 컴파일 시점에 어떤 함수를 실행할지가 결정되는 정적 바인딩이 되어있기 때문에 `Move()`는 오버라이딩된 함수 대신 부모 클래스의 함수를 호출함    

```cpp
class Player
{
	public:
		...
		virtual void VMove() { cout << "Move Player" << endl; }
	...
}

class Knight : public Plyaer
{
	public:
		virtual void VMove() { cout << "Move Knight" << endl; }
	...
};

void MovePlayer(Player* player)
{
	player->Move();
}

int main()
{
	Knight k;
	MovePlayer(&k); 
}
```

오버라이딩된 함수를 사용하기 위해서는 `virtual` 키워드를 통해 가상 함수를 만들어 동적 바인딩을 적용해야함    
동적 바인딩은 실행 시점에 어떤 함수를 사용할지가 결정됨    
부모 클래스에서 가상 함수로 선언했을 경우, 자식 클래스에서 재정의를 하여도 항상 가상 함수로 취급됨    

***

# 다형성 #2

가상 함수를 사용하면 생성자의 선처리 영역에서 가상 함수 테이블이 생성됨    
가상 함수 테이블에는 가상으로 선언된 함수들의 주소가 저장됨    
가상 함수를 사용시에는 해당 객체가 가지고있는 가상 함수 테이블을 참조하여 어느 함수를 선택할 것인지가 결정됨    

순수 가상 함수 : 구현 없이 인터페이스만 전달하는 용도로 사용하고 싶을 경우에 사용
* `virtual void VAttack() = 0;`의 형식으로 사용
* 이 경우 부모에서는 함수가 정의되지 않으며, 반드시 상속받는 자식 클래스에서 해당 함수를 정의하여 사용하여야 함
* 순수 가상 함수가 선언된 순간 해당 함수를 포함한 클래스는 추상 클래스로 간주됨
* 추상 클래스의 객체는 선언될 수 없음

***

# 초기화 리스트

초기화를 하지 않으면 각 데이터에 쓰레기 값이 들어가서 문제가 발생할 수 있음    

```cpp
class Player
{
	public:
		Player() {}
		Player(int id) {}
}

class Inventory
{
	public:
		Inventory() { cout << "Inventory()" << endl; }
		Inventory(int size) { cout << "Inventory(int)" << endl; _size = size; }
		~Inventory() { cout << "~Inventory()" << endl; }
	public:
		int _size = 10;
}

class Knight : public Player
{
	public:
		Knight() : Player(1), _hp(100), _inventory(20) // 초기화 리스트 
		{
			_hp = 100 // 생성자 내에서 초기화
		}
	public:
		int _hp = 100; // C++11 문법
		Inventory _inventory;
}
```

C+11 문법을 이용한 초기화 : 클래스 내에서 멤버 변수를 선언함과 동시에 초기화    
초기화 리스트를 이용한 초기화 : 상속 관계에서 원하는 부모 생성자 호출에 사용    
생성자 내에서 초기화 : 생성자 내에서 평범하게 초기화    

`Knight`와 `Player`같이 Is-A 관계인 경우 상속관계 
`Knight`와 `Inventory`같이 has_A 관계인 경우 포함관계    

포함관계의 경우에도 포함되는 클래스의 생성자가 선처리 영역에서 암시적으로 생성됨    
즉, 생성자 내에서 포함되는 클래스의 생성자를 호출할 경우 앞서 기본 생성자로 생성된 클래스 객체가 소멸됨    
반면 초기화 리스트를 사용하여 포함되는 클래스를 초기화할 경우에는 중복 문제가 발생하지 않음    

참조나 `const`타입과 같이 정의와 동시에 초기화가 필요한 경우에도 초기화 리스트를 사용해야함    
