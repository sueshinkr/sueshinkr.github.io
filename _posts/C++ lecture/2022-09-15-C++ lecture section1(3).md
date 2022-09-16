---
title:  "C++ lecture section1 [3/3]"
excerpt: "어셈블리 언어 입문"

categories:
  - Cpp_lecture
tags:
  - [C++]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.09.15 19:00:00
---

# 반복문

특정 조건을 만족할때까지 반복해서 실행

```assembly
mov ecx, 10
LABEL_LOOF:
	PRINT_STRING msg
	NEWLINE
	dec ecx
	cmp ecx, 0
	jne LABEL_LOOF
```

```assembly
mov eax, 100
xor ebx, ebx ; = mov ebx, 0
xor ecx, ecx

LABEL_SUM:
	inc ecx, 1
	add ebx, ecx
	cmp ecx, eax
	jne LABEL_SUM

PRINT_DEC 4, ebx
NEWLINE
```

`cmp`와 `jmp`를 사용하여 반복문 생성 가능    

```assembly
mov ecx, 100
xor ebx, ebx
LABEL_LOOP_SUM:
	add ebx, ecx
	loop LABEL_LOOP_SUM

PRINT_DEC 4, ebx
NEWLINE
```

`loop [라벨]`을 사용하여 반복문 생성 가능    
해당 구문을 만나면 `ecx` 레지스터가 하나 줄어드는 것을 이용해 반복    

***

# 배열과 주소

배열 : 동일한 타입의 데이터 묶음
* 배열을 구성하는 각 값을 배열 요소(element)라고 함
* 배열의 위치를 가리키는 숫자를(index)라고 함
* `a db 0x01, 0x02, 0x03, 0x04`, `b times 5 dw 1`, `num resb 10`등의 형태로 사용

```assembly
xor ecx, ecx

LABEL_PRINT_LOOP:
	PRINT_HEX 1, [a+ecx]
	NEWLINE
	inc ecx, 1
	cmp ecx, 5
	jne LABEL_PRINT_LOOP

a db 0x01, 0x02, 0x03, 0x04
```

```assembly
xor ecx, ecx

LABEL_PRINT_LOOP:
	PRINT_HEX 2, [b+ecx*2]
	NEWLINE
	inc ecx, 1
	cmp ecx, 5
	jne LABEL_PRINT_LOOP

b times 5 dw 1
```

시작 주소를 기준으로 인덱스 * 크기만큼을 이동하는 방식이 많이 사용됨    

***

# 함수 기초

어셈블리에서는 함수보다 프로시저(procedure)라는 용어를 더 많이 사용함    

```assembly
CMAIN:
	call PRINT_MSG
	ret

PRINT_MSG:
	PRINT_STRING msg
	NEWLINE
	ret
```

함수로 하나의 기능 단위를 관리할 수 있음    

```assembly
MAX:
	cmp eax, ebx
	jg L1
	mov ecx, ebx
	jmp L2
L1:
	mov ecx, eax
L2:
	ret
```
인자의 개수가 늘어난다면 위와 같은 방법으로는 구현이 힘들어짐    
레지스터에 이미 중요한 값이 저장되어있었다면 문제가 생길 수 있기 때문에 데이터 영역의 변수를 활용할 수도 있으나, 여전히 영구적인 메모리를 사용하기에 낭비가 발생함    
즉, 함수에는 유효 범위 / 정리 / 유동적인 유효 범위 확장의 개념이 필요하기 때문에 이를 만족시킬 수 있는 스택(stack)이라는 메모리 영역을 사용
* 함수가 사용하는 일종의 메모장
* 매개변수 전달, 돌아갈 주소 관리에 사용됨

***

# 스택 메모리

CODE / DATA / BSS / HEAP / STACK

스택은 높은 주소에서 시작됨    
함수 호출시 매개변수 / 반환 주소값 / 지역 변수가 스택에 쌓임    

포인터 레지스터
* ip(Instruction Pointer) : 다음 수행 명령어의 위치
* sp(Stack Pointer) : 현재 스택의 top 위치
* bp(Base Pointer) : 스택 상대주소 계산용

`push`로 데이터를 스택에 저장하고, `pop`으로 꺼내옴    
단, `pop`의 경우 실제로 저장되어있는 데이터를 제거하지는 않고 top을 가리키는 포인터인 `sp`의 위치만 조정함    

```assembly
mov rbp, rsp

push 1
push 2
call MAX
PRINT_DEC 8, rax
NEWLINE
add rsp, 16

MAX:
	push rbp
	mov rbp, rsp

	mov rax, [rbp+16]
	mov rbx, [rbp+24]
	cmp rax, rbx
	jg L1
	mov ra, rbx
L1:
	pop rbp
	ret
```

`rbp`에는 `sp`의 주소를 고정시켜 상대주소를 계산하는데 용이하게 사용    
`bp`를 관리하는 일련의 과정을 스택 프레임이라고 함    
스택 프레임에는 넘겨준 인자 뿐만 아니라 리턴주소, 이전 함수가 사용하던 스택 프레임 공간을 알기 위한 `bp`값 등이 있음    