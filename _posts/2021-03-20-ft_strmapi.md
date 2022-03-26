---
title:  "[libft] ft_strmapi"
excerpt: "strmapi 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.20 13:30:48
---

# strmapi?

```c
//원 함수 없음
    char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
```

## 구현해야할 함수 기능    
:  Applies the function ’f’ to each character of the string ’s’ , and passing its index as first argument to create a new string (with malloc(3)) resulting from successive applications of ’f’.    

## 해석 및 부연설명    
:  문자열 s의 각 문자에 함수 f가 적용된(해당 문자의 인덱스를 함수 f의 첫번째 인자로 사용) 새로운 문자열을 생성하여 반환한다. 할당 실패시엔 NULL을 반환한다.    

## ex)    
```c
char	func(unsigned int i, char c)
{
	c += i;
	return (c);
}

int	main()
{
	char	str[] = "aaaaaa";

	printf("%s\n", ft_strmapi(str, &func));
}

```
코드 실행 결과
```c
abcdef
```
문자열 str의 각 문자에 함수 func가 적용된 문자열이 정상적으로 만들어졌다.    

# 의문점 및 생각해볼점    
1. [함수 포인터?](#함수-포인터)    

***

## 함수 포인터
[기본 개념 링크](https://norux.me/8)    
함수 포인터는 말 그대로 함수를 가리키는 포인터로써, 함수가 저장되어있는 메모리의 주소를 나타낸다.    
함수 포인터에 함수 할당시엔 해당 함수의 매개변수와 반환형 타입을 일치시켜 주어야한다.    
주로 콜백(callback) 매커니즘을 구현하거나 함수를 배열로 관리하고자 할 때 사용한다고 한다.
* 콜백 : 사용할 함수가 명시적으로 호출되지 않고, 함수 포인터에 의해 호출되는 방식.    
= 암시적 호출, 즉 주어진 상황을 통해 판단하여 함수를 호출하는 방식.    
이를 사용하면 매번 기능이 달라질 때마다 수정할 필요 없이 필요한 함수를 만들어 매개변수를 전달하면 되어 편하다고 한다.    
대충 어떤 느낌인지는 알겠는데, 실제로 써먹으면서 활용해봐야 뭔가 좀 알 수 있을 것 같다.    


***

# ft_strmapi 구현

```c
char	*ft_strmapi(char const *str, char (*func)(unsigned int, char))
{
	int		i;
	char	*func_str;

	i = 0;
	func_str = (char *)malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!func_str)
		return (NULL);
	while (*str)
		*(func_str + i) = func(i++, *str++);
	*(func_str + i) = 0;
	return (func_str);
}

```
구현에 특별히 고려해야 할 점은 없었다.    

