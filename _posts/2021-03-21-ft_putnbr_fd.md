---
title:  "[libft] ft_putnbr_fd"
excerpt: "putnbr_fd 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.21 20:50:00
---

# putnbr_fd?

```c
//원 함수 없음
    void ft_putnbr_fd(int n, int fd);
```

## 구현해야할 함수 기능    
:  Outputs the integer ’n’ to the given file descriptor.    

## 내멋대로 해석    
:  주어진 file descriptor 정수 'n'을 출력한다    

## ex)    
```c
ft_putnbr_fd(2147483647, 1);

```
코드 실행 결과
```c
2147483647
```
지정한 file descriptor(1)에 정상적으로 정수 2147483647이 출력되었다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_putnbr_fd 구현

```c
void	ft_putnbr_fd(int n, int fd)
{	
	/*
	char	*temp;

	temp = ft_itoa(n);
	write(fd, temp, ft_strlen(temp));
	*/

	char	temp;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
	}
	while (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		n %= 10;
	}
	if (n < 10)
	{
		temp = n | '0';
		write(fd, &temp, 1);
	}
}

```
처음엔 아래처럼 짰다가, 생각해보니 이미 만들어둔 ft_itoa 함수가 있어 그걸 활용하는 쪽으로 방향을 틀어 주석의 코드처럼 구성하였다.    
그런데 이럴 경우 ft_putnbr_fd 함수에서는 free함수를 쓸 수 없기에 메모리 누수가 생길 가능성이 있었다. 결국 다시 원래 코드를 쓰기로 결정했다.    
