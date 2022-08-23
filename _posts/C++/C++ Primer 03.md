---
title:  "C++ Primer 03"
excerpt: "데이터 처리"

categories:
  - C++
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.08.23 20:00:00
---

# 학습목표
* C++의 변수 이름을 정하는 규칙
* C++에 내장된 정수형 : `unsigned long`, `long`, `unsigned int`, `int`, `unsigned short`, `short`, `char`, `unsigned char`, `signed char`, `bool`
* C++11에서 추가된 변수형 : `unsigned long long`, `long long`
* 여러 정수형의 시스템 한계값을 나타내는 `climits` 파일
* 여러 정수형의 수치 상수들
* `const` 제한자를 사용하여 기호 상수 만들기
* C++에 내장된 부동 소수점형 : `float`, `double`, `long double`
* 여러 부동 소수점형의 시스템 한계값을 나타내는 `cfloat` 파일
* 여러 부동 소수점형의 수치 상수들
* C++의 산술 연산자
* 자동 데이터형 변환
* 강제 데이터형 변환

***
# 3.1 간단한 변수

## 변수 이름
의미를 쉽게 알 수 있는 변수 이름을 사용할 것을 권장    
C++에서 변수명을 설정할 때는 지켜야하는 규칙이 있음    
* 변수명에는 영문자, 숫자, 밑줄(_)만 사용 가능
* 숫자를 변수 이름의 첫 문자로 상요할 수 없음
* 변수 이름에서 대문자와 소문자는 별개로 취급
* C++의 키워드는 변수명으로 사용할 수 없음
* 두개의 밑줄 문자로 시작하거나 밑줄문자 + 대문자로 시작하는 이름은 컴파일러와 리소스가 사용하기로 예약되어있음
* 하나의 밑줄 문자로 시작하는 이름은 컴파일러와 리소스가 전역 식별자(global identifier)로 사용하기로 예약되어있음
* 변수명의 길이에는 제한이 없으며 변수명에 쓰인 모든 문자가 유효함    
	ANSI C(C99)에서는 63개 문자만 유효한것으로 처리함    

두 개 이상의 단어를 결합할때는 단어들 사이를 밑줄로 구분하거나 단어들의 첫 문자를 대문자로 쓰는 것이 관례


***


## 정수형
소수부가 없는 수, 메모리 용량의 한계가 있으므로 정수들의 부분 집합만을 나타낼 수 있음    
C++에서는 데이터형에 따라 서로 다른 크기의 메모리를 사용함    
기본 정수형은 크기 순서대로 `char` - `short` - `int` - `long` - `long long`이 있음    
기본형들에 대해 `signed`형(양수 + 음수값 모두 포함)과 `unsigned`형(양수값만 포함)이 각각 따로 존재    

### short / int / long / long long
C++은 해당 데이터형들의 최소 크기만을 지정하여 융통성있는 표준을 제공함
* `short`형은 최소한 16비트 폭을 가짐
* `int`형은 최소한 `short` 크기 이상, 일반적으로는 32비트를 사용
* `long`형은 최소한 32비트 폭을 가지며 `int` 크기 이상
* `long long`형은 최소한 64비트 폭을 가지며 `long` 크기 이상

`sizeof`연산자로 변수나 데이터형의 크기를 바이트 단위로 확인할 수 있음     
또는 `climits` 헤더 파일에서 정의된 값을 통해서도 확인할 수 있음    

```C++
// limits.cpp

#include <iostream>
#include <climits>

int main()
{
	using namespace std;
	int n_int = INT_MAX;
	short n_short = SHRT_MAX;
	long n_long = LONG_MAX;
	long long n_llong = LLONG_MAX;

	cout << "int는 " << sizeof (int) << " 바이트이다." << endl;
	cout << "short는 " << sizeof n_short << " 바이트이다." << endl;
	cout << "long은 " << sizeof n_long << " 바이트이다." << endl << endl;
	cout << "long long은 " << sizeof n_llong << " 바이트이다." << endl;
	cout << endl;

	cout << "최댓값 : " << endl;
	cout << "int : " << n_int << endl;
	cout << "short : " << n_short << endl;
	cout << "long : " << n_long << endl << endl;
	cout << "long long : " << n_llong << endl << endl;
	cout << "int의 최솟값 = " << INT_MIN << endl;
	cout << "바이트 당 비트수 = " << CHAR_BIT << endl; 
	return 0;
}
```
`sizeof`를 데이터형 이름에 사용할시 괄호가 필요, 변수명에 사용할시에는 괄호가 없어도 상관없음    
`INT_MAX`, `CHAR_BIT`(CHAR형의 비트 수 = 바이트 당 비트 수) 등은 `climits` 파일에 정의되어있는 기호 상수임
* `#define SHORT_MAX 32767`의 형식으로 정의되어있으며, `#define`은 `#include`와 같은 전처리 지시자임
* `#define` 지시자가 `INT_MAX`가 나올때마다 32767로 대체하라고 전처리기에게 지시
* 결과적으로 소스 코드 내의 모든 `INT_MAX`가 대체되어 컴파일 과정으로 넘겨짐
* 단, 이는 C로부터의 유물이며 C++에서는 `const` 키워드를 사용하는 더 좋은 방법이 있음

정의된 변수를 초기화하지 않으면 변수의 값은 미확정(undefined) 상태가 되어 그 메모리 위치에 우연히 남아있던 것이 해당 변수의 값으로 행세하게 됨    
따라서 변수를 선언할 때부터 값을 대입하여 초기화까지 하는 것을 권장함    

전통적인 C 스타일의 초기화 문법 : `int owls = 101;`    
C++의 새로운 초기화 문법 : `int owls(101);`    
C++11에서의 초기화 문법 : `int owls = { 101 };` / `int owls{ 101 };`    
	중괄호 안이 공백일시 0으로 초기화됨    

***

### unsigned형
음수 값을 저장할 수 없는 대신 변수에 저장할 수 있는 최대값이 늘어남    
```C++
// exceed.cpp

#include <iostream>
#define ZERO 0
#include <climits>

int main()
{
	using namespace std;
	short Dohee = SHRT_MAX;
	unsigned short Insuk = Dohee;

	cout << "도희의 계좌에는 " << Dohee << "원이 들어있고, ";
	cout << "인숙이의 계좌에도 " << Insuk << "원이 들어있다." << endl;
	cout << "각각의 계좌에 1원씩 입금한다." << endl << "이제 ";
	Dohee = Dohee + 1;
	Insuk = Insuk + 1;
	cout << "도희의 잔고는 " << Dohee << "원이 되었고, ";
	cout << "인숙이의 잔고는 " << Insuk << "원이 되었다." << endl;
	cout << "이럴수가! 도희가 나 몰래 대출을 했나?" << endl;
	Dohee = ZERO;
	Insuk = ZERO;
	cout << "도희의 계좌에는 " << Dohee << "원이 들어있고, ";
	cout << "인숙이의 계좌에도 " << Insuk << "원이 들어있다." << endl;
	cout << "각각의 계좌에 1원씩 인출한다." << endl << "이제 ";
	Dohee = Dohee - 1;
	Insuk = Insuk - 1;
	cout << "도희의 잔고는 " << Dohee << "원이 되었고, ";
	cout << "인숙이의 잔고는 " << Insuk << "원이 되었다." << endl;
	cout << "이럴수가! 인숙이가 복권에 당첨되었나?" << endl;
	return 0;
}
```
데이터형의 표현 한계값을 벗어날 경우(오버플로우 또는 언더플로우) `unsigned` 정수형의 경우 항상 반대편 끝으로 넘어감    
C++에서는 `signed` 정수형의 경우도 대부분 동일하게 동작하지만, 보장할 수는 없음    

***

### 정수형 선택
일반적으로는 컴퓨터가 가장 효율적으로 처리하는 정수 형식인 `int`형을 사용하는 것을 권장    
음수가 될 수 없는 수를 나타낼 때는 `unsigned` 정수형을 사용해 보다 큰 값을 나타냄    
수를 나타내기 위해 16비트보다 더 많이 필요한 수는 `long`형을 사용함    
* `int`형이 32비트인 경우라도 `long`형을 사용하는데, 이는 16비트 `int`형을 사용하는 시스템으로 이식할 경우에 오동작을 막기 위함임
* 이십억으로도 모자랄 경우에는 `long long`형을 사용

`short`형이 `int`형보다 크기가 작을 경우에는 메모리 절약을 위해 `short`형을 사용함
* 메모리 절약이 중요하다면 두 데이터형의 크기가 같을 경우에도 `short`형을 사용해야함
* 16비트 `int`형을 사용하는 시스템에서 32비트 `int`형을 사용하는 시스템으로 프로그램을 이식할경우, `int`형의 경우 두 배의 메모리를 요구하게 되지만 `short`형은 그렇지 않아 비트를 절약할 수 있음

1바이트만 필요할 경우에는 `char`형을 사용할 수 있음    

***

### 정수형 상수
```C++
// hexoct1.cpp

#include <iostream>

int main()
{
	using namespace std;
	int chest = 42;
	int waist = 0x42;
	int inseam = 042;

	cout << "손님 몸매는 한마디로 끝내줍니다!\n";
	cout << "가슴둘레 " << chest << "\n";
	cout << "허리둘레 " << waist << "\n";
	cout << "인심길이 " << inseam << "\n";
	return 0;
}

```
정수형 상수 : 프로그램에 직접 써 넣는 정수
* 처음 숫자가 1~9 : 10진수
* 처음 숫자가 0, 두번째 숫자가 1~7 : 7진수
* 처음 두개의 문자가 0x 혹은 0X : 16진수

`cout`는 기본적으로 정수를 10진수로 출력   

```C++
// hexoct2.cpp

#include <iostream>

int main()
{
	using namespace std;
	int chest = 42;
	int waist = 42;
	int inseam = 42;

	cout << "손님 몸매는 한마디로 끝내줍니다!\n";
	cout << "가슴둘레 " << chest << " (10진수)\n";
	cout << hex;
	cout << "허리둘레 " << waist << " (16진수)\n";
	cout << oct;
	cout << "인심길이 " << inseam << " (8진수)\n";
	return 0;
}
```
`cout << hex` 구문은 `cout`이 정수를 나타내는 방식을 변경함
* `cout << dec` : 십진수로 변경
* `cout << hex` : 16진수로 변경
* `cout << oct` : 8진수로 변경

***

### C++가 상수의 데이터형을 결정하는 방법

