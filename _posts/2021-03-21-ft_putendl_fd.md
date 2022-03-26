---
title:  "[libft] ft_putendl_fd"
excerpt: "putendl_fd 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.21 19:30:00
---

# putendl_fd?

```c
//원 함수 없음
    void ft_putendl_fd(char *s, int fd);
```

## 구현해야할 함수 기능    
:  Outputs the string ’s’ to the given file descriptor, followed by a newline.    

## 내멋대로 해석    
:  주어진 file descriptor 문자열 's'를 출력한 뒤 개행을 출력한다.    

## ex)    
```c
ft_putendl_fd("abcde", 1);
ft_putendl_fd("12345", 1);

```
코드 실행 결과
```c
abcde
12345
```
지정한 file descriptor(1)에 정상적으로 문자열 "abcde\n"과 "12345\n"이 출력되었다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_putendl_fd 구현

```c
void	ft_putendl_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
}  

```
ft_putstr_fd 함수에서 개행부분만 추가하면 끝이다.    
