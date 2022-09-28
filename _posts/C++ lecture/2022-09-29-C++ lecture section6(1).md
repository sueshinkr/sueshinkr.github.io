---
title:  "C++ lecture section6 [1/3]"
excerpt: "객체지향 여행"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.29 09:00:00
---

# 객체지향의 시작

절차(procedural)지향 프로그래밍 : 함수가 메인    
객체(object)지향 프로그래밍 : 객체가 메인, 객체에는 데이터와 데이터를 조작하는 동작이 모두 포함됨    

```cpp
class Knight
{
	public:  // 멤버 함수
		void Move(int y, int x);
		void Attack();
		void Die() { _hp = 0; cout << "Die" << endl; }
	public:  // 멤버 변수
		int _hp;
		int _attack;
		int _posY;
		int _posX;
};

void Knight::Move(int y, int x)
{
	_posY = y;
	_posX = x;
	cout << "Move" << endl;
}

void Knight::Attack()
{
	cout << "Attack : " << _attack << endl;
}

int main()
{
	Knight k1;
	k1._hp = 100;
	k1._attack = 10;
	k1._posY = 0;
	k1._posX = 0;

	Knight k2;
	k2._hp = 80;
	k2._attack = 5;
	k2._posY = 1;
	k2._posX = 1;

	k1.Move(2, 2);
	k1.Attack();
	k1.Die();

	return 0;
}
```

`class`는 일종의 설계도 역할을 함    
객체의 크기는 멤버 변수 데이터 크기의 합이 됨    

***

# 생성자와 소멸자 #1

생성자(constructor) : 클래스의 시작을 알리는 함수, 여러개 존재 가능
소멸자(destructor) : 클래스의 끝을 알리는 함수, 하나만 존재

```cpp
class Knight
{
	public:
		Knight() { cout << "기본 생성자 호출" << endl; };
		Knight(const Knight& knight)
		{
			_hp = knight._hp;
			_attack = knight._attack;
			_posX = knight._posX;
			_posY = knight._posY;
		}
		Knight(int hp)
		{
			_hp = hp;
			_attack = 10;
			_posX = 0;
			_posY = 0;
		}
		~Knight() { cout << "소멸자 호출" << endl; };
		...
}
```

`Knight()` : 기본 생성자, 인자 없음    
* 생성자를 명시하지 않은 경우, 기본 생성자가 컴파일러에 의해 자동으로 만들어져 암시적(implicit) 생성자로 사용됨
* 생성자를 명시적(explicit)으로 선언한 경우 기본 생성자는 생성되지 않음
* 기본 생성자 외에도 함수의 오버로딩을 사용하여 필요에 따라 여러 종류의 생성자를 설정할 수 있음    

`Knight(const Knight& knight)` : 복사 생성자, 동일한 타입의 다른 객체를 이용하여 초기화할 때 사용    
* 복사 생성자를 명시하지 않은 경우 암시적 복사 생성자가 사용됨
* 암시적 복사 생성자는 객체의 모든 멤버를 동일하게 복사함

`~Knight()` : 소멸자

***

# 생성자와 소멸자 #2

`Knight k2(k1);` `Kngiht k3 = k1;`은 복사 생성자가 사용됨    
`Knight k4; k4 = k1;`은 `k4`를 생성자를 사용하여 생성한 후 `k1`을 대입함    

`Knight(int hp)`와 같이 인자를 1개만 받는 생성자를 타입 변환 생성자라고 부르기도 함    
* 타입 변환 생성자가 존재하는 경우 암시적 형변환이 문제가 될 수 있음
* 따라서 명시적인 용도로만 사용하도록 생성자 앞에 `explicit` 키워드를 붙여 활용할 수 있음    

***

# 상속성

클래스는 상속성을 가지기 때문에 유용하게 재사용할 수 있음    

```cpp
class Player
{
	public:
		Player()
		{
			_hp = 0;
			_attack = 0;
			_defence = 0;
			cout << "Player 기본 생성자 호출" << endl; 
		}
		~Player() { cout << "Player 소멸자 호출" << endl; }
		void Move() { cout << "Player Move" << endl;}
		void Attack() { cout << "Player Attack" << endl; }
		void Die() { cout << "Player Die" << endl;}
	public:
		int _hp;
		int _attack;
		int _defence;
};

class Knight : public Player
{
	public:
		Knight()
		// 선처리 영역, 여기서 부모의 생성자를 호출
		{
			_stamina = 100;
			 cout << "Knight 기본 생성자 호출" << endl; 
		}
		Knight(int stamina) : Player(100)
		{
			_stamina = stamina;
			cout << "Knight(int stamina) 생성자 호출" << endl;
		}
		~Knight() { cout << "Knight 소멸자 호출" << endl; }
		int _stamina;
}

class Mage : public Player
{
	public:
		int _mp;
}

int main()
{
	Knight k;
	k._hp = 10;
	k.Attack();
}
```

부모 클래스에 정의되어있는 함수를 재정의할 수 있음    
자식 클래스에도 생성자가 있어야하며, 부모의 생성자 먼저 호출 후 자식의 생성자가 호출됨    
* 정확히는 자식 클래스 생성자의 선처리 영역에서 부모의 생성자가 먼저 호출된 후 자식 클래스 생성자의 구문들을 처리함
* 소멸자는 생성자의 역순으로 호출됨    

`Kngiht(int stamina) : Player(100)`과 같이 사용하여 부모의 생성자를 선택할 수 있음    