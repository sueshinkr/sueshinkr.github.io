---
title:  "[libft] ft_striteri"
excerpt: "striteri 함수 구현"

categories:
  - 42seoul
tags:
  - libft
date: 2022.03.20 22:00:00
---

# striteri?

```c
//원 함수 없음
    void ft_striteri(char *s, void (*f)(unsigned int, char*));
```

##### 구현해야할 함수 기능    
:  Applies the function f to each character of the string passed as argument, and passing its index as first argument. Each character is passed by address to f to be modified if necessary.    

##### 내멋대로 해석    
:  문자열 s의 각 문자에 함수 f를 적용시킨다. strmapi와는 달리 새 문자열을 생성하지 않고, 대신 함수 포인터의 두번째 매개변수로 각 문자의 주소값을 사용하여 기존 문자열을 변화시킨다.    

##### ex)    
```c
void	func(unsigned int i, char *c)
{
	c += i;
}

int	main()
{
	char	str[] = "aaaaaa";

	ft_striteri(str, *func);
	printf("%s\n", str);
}

```
코드 실행 결과
```c
abcdef
```
문자열 str의 각 문자에 함수 func가 정상적으로 적용되었다.    

##### 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_strmapi 구현

```c
void	ft_striteri(char *str, void (*func)(unsigned int, char*))
{
	int	i;

	i = 0;
	while (str[i])
	{
		func(i, &str[i]);
		i++;
	}
}


```
구현에 특별히 고려해야 할 점은 없었다.    
