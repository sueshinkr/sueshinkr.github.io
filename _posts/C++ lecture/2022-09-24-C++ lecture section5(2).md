---
title:  "C++ lecture section5 [2/4]"
excerpt: "포인터"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.24 13:00:00
---

# 참조 기초

`int& reference = number;`의 형식으로 사용    
* 변수 `number`에 `reference`라는 또다른 이름을 부여
* 로우레벨(어셈블리) 관점에서 보면 실제 작동 방식은 포인터와 같음    
* 원본 데이터를 사용하면서도 사용성을 향상시키기 위해 참조 전달을 사용

```cpp
void PrintInfoByRef(Statinfo& info)
{
	cout << "-----------------" << endl;
	cout << "HP : " << info.hp << endl;
	cout << "ATT : " << info.hp << endl;
	cout << "DEF : " << info.hp << endl;
	cout << "-----------------" << endl;
}

int main()
{
	StatInfo info;
	PrintInfoByRef(info);
	return 0;
}
```

***

# 포인터 vs 참조

 성능적인 면에서는 포인터와 참조에 차이가 없음    
 편의성 면에서는 참조가 더 유용하지만, 편의성이 좋은 것이 무조건 장점은 아님
 * 포인터의 경우 주소를 전달함으로써 원본을 사용한다는 것을 명확하게 알 수 있음
 * 반면 참조는 인식하지 못하고 지나칠 가능성이 존재함
	* 이럴 때 원본을 실수로 변경하는 것을 막기 위해 `const` 키워드를 사용할 수 있음

포인터에도 `const` 키워드를 사용할 수 있음
* `*` 이전에 `const` 사용
	* `StatInfo* const info` : 포인터 변수 `info`에 설정된 주소값을 변경하지 못함
* `*` 이후에 `const` 사용
	* `const StatInfo* info` : 포인터 변수 `info`에 설정된 주소에 저장되어있는 값을 변경하지 못함

참조의 경우 대상이 필요하므로 반드시 초기화를 해주어야함    
반면 포인터의 경우 초기화 없이 선언만 하는 것도 가능함    
* 포인터 변수의 값을 0으로 초기화하고 싶은 경우 `NULL` 또는 `nullptr`을 사용    
* 참조는 0으로 초기화할 수 없음

결론적으로 포인터와 참조에 큰 차이는 없으니 둘 중 어느것을 선택하는지는 스타일에 따라 다름    
* 없는 경우를 고려해야한다면 포인터(`NULL` 체크 필수), 읽는 용도로만 사용된다면 `const` 참조를 사용하는 것이 유용할 수 있음    
* 그 외의 일반적인 경우는 참조가 사용하기 편리함
* ```cpp
	#define OUT

	void ChangeInfo(OUT StatInfo& info)
	{
		info.hp = 1000;
		...
	}

	int main()
	{
		...
		ChangeInfo(OUT info);
		return 0;
	}
	```
	위 코드와 같이 내용물의 변경이 이루어지는 경우 아무런 역할도 하지 않는 `OUT`을 정의하여 해당 함수가 값을 변경할 수 있다는 것을 명시하는 방법도 있음    
* 포인터를 사용하던 함수를 이어서 만든다면 계속 포인터를 사용하여 섞어쓰는 것을 지양함    

포인터를 참조로 넘겨주려면 `PrintInfoByRef(*pointer);`의 형식으로 사용    
참조를 포인터로 넘겨주려면 `PrintInfoByPtr(&reference);`의 형식으로 사용    

***

# 배열 기초 


`TYPE 이름[개수];`의 형태로 선언    
* 배열의 크기는 반드시 상수여야함    
* 배열의 이름은 배열의 시작 위치를 가리키는 `TYPE*` 포인터를 나타냄    

```cpp
struct StatInfo
{
	int hp;
	int att;
	int def;
};

int main()
{
	const int monsterCount = 10;
	StatInfo monsters[monsterCount];

	StatInfo* monster_0 = monsters;
	Statinfo* monster_1 = monsters + 1;
	Statinfo& monster_2 = *(monsters + 2);
	StatInfo& monster_3 = monsters[3];
}
```

포인터의 산술 연산을 활용하여 배열의 원소에 접근함    
포인터와 참조는 자유자재로 변환 가능하므로 배열 원소의 접근에도 이를 활용할 수 있음    

`*(monsters + i)`로 접근하는 것은 가독성이 떨어지므로 `monsters[i]`의 형태로 인덱스를 활용할 수 있음    

배열 초기화 문법
* `int numbers[5] = {};` : 모든 원소를 0으로 초기화
* `int numbers[10] = { 1,2,3,4,5 };` : 값을 지정한 원소를 제외한 나머지 원소들을 0으로 초기화
* `int numbers[] = { 1,2,3, ... };`, `char helloStr[] = "Hello"` : 데이터 개수만큼의 크기를 가진 배열을 초기화

***

# 포인터 vs 배열

```cpp
int main()
{
	const char* test1 = "Hello World";
	char test2[] = "Hello World";
}
```

포인터는 해당 데이터가 들어있는 주소값을 나타냄    
배열은 해당 데이터 자체가 되며, 배열의 이름이 데이터들 중 첫번째 값인 시작 주소를 나타냄    

```cpp

void Test(char a[])
{
	a[0] = 'x';
}

int main()
{
	char test[] = "Hello World";
	Test(test);
	cout << test << endl;
}
```

배열을 함수의 인자로 넘겨줄시 배열 원소를 복사하여 넘겨주는 것이 아닌, 배열의 시작 주소를 전달하여 동작함    
즉, 매개변수로 배열을 전달하면 컴파일러가 이를 포인터형으로 치환하여 사용함    