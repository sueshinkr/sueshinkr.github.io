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


***


## short / int / long / long long
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


## unsigned형
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


## 정수형 선택
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


## 정수형 상수
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


## C++가 상수의 데이터형을 결정하는 방법
`cout << "Year = " << 1492 << "\n";`    
위와 같이 변수에 상수를 저장하는 경우에 C++에서는 일반적으로 정수형 상수를 모두 `int`형으로 저장함    
단, 접미어를 붙었을 때는 해당 접미어에 맞는 데이터형으로 저장됨    
접미어가 없는 10진 정수는 `int`, `long`, `long long`형 중에서 해당 정수가 범위 내에 속하는 가장 작은 크기의 데이터형으로 저장됨    
접미어가 없는 16진 / 8진 정수는 `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`형 중에서 해당 정수가 범위 내에 속하는 가장 작은 크기의 데이터형으로 저장됨    


***


## char형
```C++
// chartype.cpp

#include <iostream>

int main()
{
	using namespace std;
	char ch;
	
	cout << "원하는 문자 하나를 입력하시오 : " << endl;
	cin >> ch;
	cout << "감사합니다. ";
	cout << "당신이 입력한 문자는 " << ch << "입니다." << endl;
	return 0;
}
```
각 문자는 특정 정수와 일대일로 대응하며, 문자 세트는 `ASCII`를 일반적으로 사용함    
그러나 `ASCII`는 제한이 크기 때문에 C++에서는 보다 범용적인 `Unicode`를 사용할 수 있는 확장 문자형을 지원함    
위 예제에서 입력된 문자는 해당 문자에 대응하는 정수로 변수 `ch`에 저장되며, 이를 출력할 때는 해당 정수에 대응하는 문자로 변환되어 출력됨    

```C++
// morechar.cpp

#include <iostream>

int main()
{
	using namespace std;
	char ch = 'M';
	int i = ch;
	cout << ch << "의 ASCII 코드는 " << i << "입니다." << endl;
	
	cout << "이 문자 코드에 1을 더해보겠습니다." << endl;
	ch = ch + 1;
	i = ch;
	cout << ch << "의 ASCII 코드는 " << i << "입니다." << endl;

	cout << "cout.put(ch)를 사용하여 char형 변수 ch를 화면에 출력 : ";
	cout.put(ch);

	cout.put('!');

	cout << endl << "종료" << endl;
	return 0;
}
```
`char`형 변수도 정수형 변수이므로 정수 연산을 적용할 수 있음    
`cout.put()` : `ostream` 클래스에 포함되어있는 `put()`이라는 멤버 함수를 `cout`객체를 통해 사용
* 멤버 함수 : 클래스에 속하며 해당 클래스의 특정 객체를 통해서만 사용할 수 있음    
	마침표로 객체 이름과 함수 이름을 연결하여 사용하며, 이 때의 마침표는 멤버 연산자(membership operator)로 작동함    
* C++ Release 2.0 이전에 `cout`가 문자 상수를 `int`형으로 저장했기 때문에 사용된 함수

`'A', ' '`의 형식과 같이 작은따옴표로 문자를 둘러싸는 것으로 문자 상수를 표기    
특정 코드 체계에 종속적이지 않기 때문에 수치 코드를 사용하는 것보다 좋은 방식임    
키보드로 직접 입력할 수 없거나, C++에서 특별한 의미가 부여된 문자(문자열 분리로 사용되는 큰따옴표 등)들은 이스케이프 시퀀스(escape sequence)라는 특수한 표기법을 사용함    
* `'\n', '\t', '\\'` 등
* 이스케이프 시퀀스 역시 하나의 문자로 취급되기 때문에 문자 상수로 사용할 때에는 반드시 작은따옴표로 둘러쌓여있어야 함    
* `'\x8', '\010'`의 형식으로 사용되는 수치 이스케이프 시퀀스도 있으나 가능하면 `'\b'`와 같은 기호 이스케이프 시퀀스를 사용하는것이 권장됨    

```C++
// bondini.cpp

#include <iostream>

int main()
{
	using namespace std;
	cout << "\a암호명 \"화려한 외출\" 작전이 방금 개시되었습니다!\n";
	cout << "8자리 비밀번호를 입력하십시오 : ________\b\b\b\b\b\b\b\b";
	long code;
	cin >> code;
	cout << "\a입력하신 비밀번호는 " << code << "입니다.\n";
	cout << "\a비밀번호가 맞습니다! Z3 계획을 진행하십시오!\n";
	return 0;
}
```
8개의 밑줄 문자가 출력된 후 프로그램은 `\b` 이스케이프 시퀀스에 해당하는 백스페이스 문자를 사용하여 커서를 첫번째 밑줄 문자 위치로 이동시킴    

***

### universal character names
C++ 표준은 기본적인 소스 문자 세트를 지원하는데 더해 확장 소스 문자 세트와 확장 실행 문자 세트를 제공하는 것을 허용함    
유니버설 캐릭터 이름(universal character names) : 특정 키보드와는 무관한 국제 문자들을 표현하는 메커니즘    
* `\u 또는 \U` + `16진수로 나타낸 ISO 10646(국제 표준 문자 세트) 코드`
* C++에서는 유니버설 코드(universal code)가 아닌 유니버설 코드 네임(universal code name)이라는 용어를 사용하는데, 이는 문자에 해당하는 코드가 다양한 시스템에서 그 시스템에 맞는 다양한 인코딩을 가질 수 있기 때문임    
	즉, 사용자의 소스 코드는 모든 시스템에 대해 동일한 유니버설 코드 네임을 사용할 수 있으나 컴파일러는 해당 코드를 특정 시스템에서 그 시스템에 적절한 내부 코드로 표현할 수 있음

***

### signed char / unsigned char
기본 `char`형은 `signed`형인지 `unsigned`형인지 정해져있지 않으나, 사용자가 명시적으로 제한할 수는 있음    
표준 `ASCII` 문자를 사용할 때는 둘 다 상관없으므로 그냥 `char`로 사용하면 됨    

***

### 확장 char형 : wchar_t
1바이트로 표현할 수 없는 문자 세트의 경우(한국어 등)
* 컴파일러 개발업체가 `char`형을 처음부터 2바이트 이상으로 만듬
* 또는 기본 문자 세트와 확장 문자 세트를 동시에 지원    
	기본 문자 세트는 `char`형으로, 확장 문자 세트는 `wchar_t(wide character type)`형으로 나타냄
	* `wchar_t`형은 기초 데이터형(underlying type)이라는 정수형과 동일한 크기 및 부호 속성을 가지며, 기초 데이터형은 시스템에 따라 `unsigned short`형 / `int`형 등으로 달라질 수 있음
	* `L'P', L"tall"`과 같이 확장 문자 상수나 확장 문자 문자열은 앞에 `L`을 붙여서 나타냄
	* `cin` / `cout`는 `wchar_t`형을 처리할 수 없기 때문에 `wcin` / `wcout`객체가 제공됨

***

### C++11형 : char16_t / char32_t
`wchar_t`의 부호와 길이는 가변이기 때문에 문제가 발생할 수 있어 C++11에서는 `char16_t(unsigned 16비트)`형과 `char32_t(unsigned 32비트)`형을 사용하기도 함    
* `char16_t`는 접두사로 u를 사용하며 /u00F6 형태의 유니버설 캐릭터 이름과 매칭됨
* `char32_t`는 접두사로 U를 사용하며 /U0000222B 형태의 유니버설 캐릭터 이름과 매칭됨


***


## bool형
참 또는 거짓 중 한가지 값만 가질 수 있는 변수이며, 미리 정의된 `true`와 `false`로 참과 거짓을 나타냄    
어떠한 수치 값이나 포인터 값도 하나의 `bool`값으로 묵시적 변환이 가능함
* 0이 아닌 값들은 `true`로 변환
* 0 값은 `false`로 변환


***
***


# 3.2 const 제한자
