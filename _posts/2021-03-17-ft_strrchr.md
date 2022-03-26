---
title:  "[libft] ft_strrchr"
excerpt: "strrchr 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.17 02:23:48
---

# strchr?

```c
#include <string.h>

    char *strrchr(const char *s, int c);
```

## Linux manpage description    
:  The strrchr() function returns a pointer to the last occurrence of the character c in the string s.    

## 해석 및 부연설명    
:  문자열 s에서 문자 c를 탐색하여 가장 마지막으로 발견된 곳의 위치를 포인터로 반환하는 함수다. 찾지 못한 경우 NULL을 반환한다.    

## ex)    
```c
char	str[] = "abcdefgehijk";
char	c = 'e';
printf("%s\n", strrchr(str, c));
```
코드 실행 결과
```c
ehijk
```
그냥 strchr의 reverse버전 함수다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_strrchr 구현

```c
char	*ft_strrchr(const char *str, int c)
{
	int	len;

	len = ft_strlen(str);
	while (len >= 0)
	{
		if (*(str + len) == (char)c)
			return ((char *)(str + len));
		len--;
	}
	return (NULL);
}

```


