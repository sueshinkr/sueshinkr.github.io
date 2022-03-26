---
title:  "[libft] ft_strnstr"
excerpt: "strnstr 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.17 13:30:48
---

# strnstr?

```c
#include <string.h>

    char *strnstr(const char *big, const char *little, size_t len);
```

## Linux manpage description    
:  The strnstr() function locates the first occurrence of the null-terminated string little in the string big, where not more than len characters are searched.    
Characters that appear after a ‘\0’ character are not searched..    

## 해석 및 부연설명    
:  strchr에서 한발 더 나아가 문자열을 검색하는 함수이다.    
little에 해당하는 문자열이 big에 있다면 그 시작주소를 반환하고, 없다면 NULL을 반환한다. 만약 little이 빈 문자열이라면 그냥 big을 반환한다.    
str계열 함수답게 NULL에서는 작동을 멈추게 된다.    

## ex)    
```c
char	str[] = "abcdefghijk";
char	find[] = "efg";
printf("%s\n", strnstr(str, find, 30));
```
코드 실행 결과
```c
efghijk
```
정상적으로 str 문자열 내부의 "efg"를 찾아 주소가 반환되었다.     

# 의문점 및 생각해볼점    
1. [""과 NULL의 차이](#""-vs-null)

***

## "" vs NULL
둘은 엄연히 다른 개념. **!little**으로 조건문을 걸었을 때 **little = ""**이라면 아무런 반응을 하지 않는다.    
그래서 찾아본 결과, NULL은 진짜로 아무것도 없는거고 ""은 빈 문자열이 존재하는 차이라고 한다. 아무튼 차이가 있다는 점을 잘 기억해아 할듯 하다.    

***

# ft_strnstr 구현

```c
char	*ft_strnstr(const char *big, const char *little, size_t size)
{
	size_t	count;
	size_t	little_len;

	count = 0;
	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *)big);
	while (*big && size-- > 0)
	{
		if (*big == little[count])
			count++;
		else
		{
			big -= count;
			count = 0;
		}
		if (count == little_len)
			return ((char *)big - count + 1);
		big++;
	}
	return (NULL);
}

```
little = ""일 경우를 찾아내기 위해 !little_len 조건을 추가로 걸어주었다.    
strnst 함수도 const char *로 받아서 char *로 반환하는 것에 주의해야한다.    

