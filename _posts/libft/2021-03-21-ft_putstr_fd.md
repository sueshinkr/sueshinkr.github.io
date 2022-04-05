---
title:  "[libft] ft_putstr_fd"
excerpt: "putstr_fd 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.21 18:30:00
---

# putstr_fd?

```c
//원 함수 없음
    void ft_putstr_fd(char *s, int fd);
```

## 구현해야할 함수 기능    
:  Outputs the string ’s’ to the given file descriptor.    

## 해석 및 부연설명    
:  주어진 file descriptor 문자열 's'를 출력한다.    

## ex)    
```c
ft_putstr_fd("abcde", 1);

```
코드 실행 결과
```c
abcde
```
지정한 file descriptor(1)에 정상적으로 문자열 "abcde"가 출력되었다.    

# 의문점 및 생각해볼점    
`putchar_fd` 함수에서 다 짚고 넘어간 것 같다.    

***

# ft_putstr_fd 구현

```c
void	ft_putstr_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

```
str에 저장되어있는 문자열을 전부 다 출력해야하므로, `write` 함수에서 str에 저장되어있는 데이터로부터 `ft_strlen(str)`만큼의 바이트를 입력받도록 했다.    

