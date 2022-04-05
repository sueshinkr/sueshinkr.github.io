---
title:  "[libft] ft_striteri"
excerpt: "striteri 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.20 22:00:00
---

# striteri?

```c
//원 함수 없음
    void ft_striteri(char *s, void (*f)(unsigned int, char*));
```

## 구현해야할 함수 기능    
:  Applies the function f to each character of the string passed as argument, and passing its index as first argument. Each character is passed by address to f to be modified if necessary.    

## 해석 및 부연설명    
:  문자열 s의 각 문자에 함수 f를 적용시킨다. `strmapi` 함수와는 달리 새 문자열을 생성하지 않고, 대신 함수 포인터의 두번째 매개변수로 각 문자의 주소값을 사용하여 기존 문자열에 저장되어있는 값들을 변화시킨다.    

## ex)    
```c
void	func(unsigned int i, char *c)
{
	*c += i;
}

int	main()
{
	char	str[] = "aaaaaa";

	ft_striteri(str, &func);
	printf("%s\n", str);
}

```
코드 실행 결과
```c
abcdef
```
문자열 str의 각 문자에 함수 `func`가 정상적으로 적용되었다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_striteri 구현

```c
void	ft_striteri(char *str, void (*func)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (*str)
		func(i++, str++);
}

```
구현에 특별히 고려해야 할 점은 없었다.    
덧) 함수 이름의 iteri는 iterate + i인 것 같다. 문자열 str의 인덱스 i에 해당하는 문자들에 반복적으로 함수 `func`를 적용하는거라고 이해하면 될 듯 하다.    

