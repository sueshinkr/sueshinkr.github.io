---
title:  "C++ lecture section1 [2/3]"
excerpt: "어셈블리 언어 입문"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.14 19:00:00
---

# 문자와 엔디안

문자는 아스키코드를 활용해서 읽음    
동일한 데이터라고 해도 어떻게 분석하고 받아들이냐에 따라 다른 의미가 될 수 있음    

리틀 엔디안 : 대부분 사용, 메모리 주소가 증가함에 따라 변수가 거꾸로 저장됨    
* 캐스팅(데이터의 크기가 큰 자료형에서 작은 자료형으로 바꾸는 것)에 유리

빅 엔디안 : 변수가 메모리 주소 증가에 맞추어 순서대로 저장됨    
* 숫자 비교에 유리

***

# 사칙연산

더하기 : `add a, b`
* a는 레지스터 또는 메모리
* b는 레지스터 또는 메모리 또는 상수
* 단, 메모리를 사용할 때는 `[]`로 주소가 아닌 값을 받아와아하며 a와 b 모두 메모리일 수는 없음
* 메모리에 상수를 더할 때는 더할 상수의 크기를 지정해줘야함

빼기 : `sub a, b`, add와 동일함    

곱하기 : `mul reg`
* 곱해지는 레지스터의 크기에 따라 작동이 달라짐
* `mul bl` = al * bl, 연산 결과는 ax에 저장됨
* `mul bx` = ax * bx, 연산 결과는 상위 16비트가 dx, 하위 16비트가 ax에 저장됨

나누기 : `div reg`
* 나눠지는 레지스터의 크기에 따라 작동이 달라짐
* `div bl` = ax / bl, 연선 결과는 몫이 al에, 나머지가 ah에 저장됨

***

# 시프트 연산과 논리 연산

산술 시프트(shift)에서는 부호를 나타내는 최상위비트는 그대로 유지됨    
`shl 레지스터, 값` 또는 `shr 레지스터, 값`의 형태로 사용    
시프트 연산으로 곱셈 및 나눗셈을 간편하게 할 수 있음    
게임서버에서 ObjectID를 만들 때도 유용하게 사용    

논리 연산에는 `not`, `and`, `or`, `xor`이 있음    
* `not A` : 하나에 조건에 대해 0이면 1, 1이면 0
* `A and B` : 둘다 1이면 1, 나머지는 0
* `A or B` : 둘중 하나라도 1이면 1, 아니면 0
* `A xor B` : 둘다 1이거나 둘다 0이면 0, 아니면 1
* `and al, bl`, `not al` 등의 형태로 사용

비트플래그(bitflag)에 사용    
`xor`을 두번 사용시 원래 값으로 돌아오는 특성이 있으므로 암호학에서 유용하게 사용함    
`xor`을 자기 자신으로 사용시 모든 값이 0으로 대입되기 때문에 오류 체크 등에 사용함    

***

# 분기문

특정 조건에 따라서 코드 흐름을 제어

`cmp dst, src`의 형태로 사용, `dst`가 기준    
비교를 한 결과물은 Flag Register에 저장    
`cmp` 결과에 따라 Flag Register의 각 비트가 경우에 맞게 변환되고, 아래의 `jmp` 등의 연산자들이 그 비트를 참조하여 조건을 판별함

`jmp/je/jne/jg/jge... [label]`의 형태로 이동    
* `jmp` : 무조건 점프
* `je` : JumpEqulas, 같으면 점프
* `jne` : JumpNotEquals, 다르면 점프
* `jg` : JumpGreater, 크면 점프
* `jge` : JumpGreaterEquals, 크거나 같으면 점프
* ...

```assembly
mov ax, 100

mov bl, 2
div bl

cmp ah, 1
je LABEL_ODD
mov rcx, 1
jmp LABEL_EVEN

LABEL_ODD:
	mov rcx, 0
LABEL_EVEN:
	PRINT_HEX 1, rcx
	NEWLINE
```
