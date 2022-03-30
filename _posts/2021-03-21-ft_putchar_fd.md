---
title:  "[libft] ft_putchar_fd"
excerpt: "putchar_fd 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.21 18:00:00
---

# putchar_fd?

```c
//원 함수 없음
    void ft_putchar_fd(char c, int fd);
```

## 구현해야할 함수 기능    
:  Outputs the character ’c’ to the given file descriptor.    

## 해석 및 부연설명    
:  주어진 file descriptor에 문자 'c'를 출력한다.    

## ex)    
```c
ft_putchar_fd('a', 1);

```
코드 실행 결과
```c
a
```
지정한 file descriptor(1)에 정상적으로 문자 'a'가 출력되었다.    

# 의문점 및 생각해볼점    
1. [file descriptor?](#file-descriptor란)
2. [표준 입출력?](#표준-입출력-스트림)

***

## file descriptor란
[기본 개념 링크](https://code4human.tistory.com/123)    
파일 오픈 또는 소켓(물리적으로 연결된 네트워크상에서의 데이터 송수신에 사용할 수 있는 소프트웨어적인 장치)을 생성하여 시스템에 할당 시 파일 디스크립터(FD) 번호가 부여되고, 이 번호를 대표로 사용하여 해당 파일/소켓에 접근할 수 있다.    
FD 0, 1, 2는 고정되어있는데 이는 `unistd.h` 헤더파일에 명시되어있다. 따라서 FD는 3부터 차례로 부여된다.    
* FD = 0 : Standard input, 표준 입력    
* FD = 1 : Standard output, 표준 출력    
* FD = 2 : Standard error, 표준 에러출력    

***

## 표준 입출력 스트림
[개념이 잘 정리되어있는 링크](https://shoark7.github.io/programming/knowledge/what-is-standard-stream)    
* 스트림(stream) :  프로그램을 드나드는 데이터를 바이트의 흐름으로(byte stream) 표현한 단어.    
프로그램과 각 장치 사이에 놓여지는 다리 역할로 스트림을 통하면 여러 장치별로 설정을 따로 해줄 필요가 사라진다.    
* 표준 입출력 : 프로그램이 기본적으로 사용할 입출력 대상.    
표준 입력은 키보드, 표준 출력은 콘솔(모니터)로 설정되어있다. 표준 출력에는 정상적인 출력을 반환하는 표준 출력과 프로그램의 비정상 종료시에 반환되는 표준 에러가 있다.    

***

# ft_putchar_fd 구현

```c
void	ft_putchar_fd(char chr, int fd)
{
	write(fd, &chr, 1);
}

```
write함수 자체의 기본 구조가 
1. 첫번째 매개변수로 들어오는 파일 디스크립터에 
2. 두번째 매개변수로 들어오는 데이터를 
3. 세번째 매개변수로 들어오는 바이트 수만큼 입력하는 형태로 이루어져있다.    

따라서 그냥 `write` 함수를 사용하면 간단하게 구현 가능하다.    

