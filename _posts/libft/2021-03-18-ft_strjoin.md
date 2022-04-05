---
title:  "[libft] ft_strjoin"
excerpt: "strjoin 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.18 15:00:48
---

# strjoin?

```c
//원 함수 없음
    char *ft_strjoin(char const *s1, char const *s2);
```

## 구현해야할 함수 기능    
:  Allocates (with malloc(3)) and returns a new string, which is the result of the concatenation of ’s1’ and ’s2’.    

## 해석 및 부연설명    
:  문자열 s1과 s2를 합한 문자열을 새롭게 할당하여 반환한다. 할당 실패시에서는 `NULL`을 반환한다.        

## ex)    
```c
char	str1[] = "abcdefg";
char	str2[] = "hijklmnop";
printf("%s\n", ft_strjoin(str1, str2));
```
코드 실행 결과
```c
abcdefghijklmnop
```
str1과 str2가 이어진 새로운 문자열이 정상적으로 만들어졌다.    

# 의문점 및 생각해볼점    
1. [`strcat`과의 차이점](#strcat과는-무엇이-다른가)    

***

## `strcat`과는 무엇이 다른가
`strcat`도 문자열 dst 뒤에 src를 붙이는 것이므로 비슷하다고 볼 수 있지만, 결정적으로 `strcat`은 원본 dst에 src를 더하는 과정이지만 `strjoin`은 아예 새로운 문자열을 만들어낸다는 차이점이 있다.    


***

# ft_strjoin 구현

```c
char	*ft_strjoin(char const *str1, char const *str2)
{
	char	*result;
	char	*joinstr;
	int		joinstr_len;

	joinstr_len = ft_strlen(str1) + ft_strlen(str2);
	joinstr = (char *)malloc((joinstr_len + 1) * sizeof(char));
	result = joinstr;
	if (!joinstr)
		return (NULL);
	while (joinstr_len-- > 0)
	{
		if (*str1)
			*joinstr = *str1++;
		else if (*str2)
			*joinstr = *str2++;
		joinstr++;
	}
	*joinstr = 0;
	return (result);
}

```
str1과 str2의 길이 합 + 맨 뒤 `NUL`을 넣어줄 크기까지 할당을 한 후, 순서대로 str1과 str2의 문자들을 받아와 채운다.    

