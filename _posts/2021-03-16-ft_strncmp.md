---
title:  "[libft] ft_strncmp"
excerpt: "strncmp 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.16 22:00:48
---

# strncmp?

```c
#include <string.h>

    int strncmp(const char *s1, const char *s2, size_t n);
```

## Linux manpage description    
:  The strcmp() function compares the two strings s1 and s2. The locale is not taken into account (for a locale-aware comparison, see strcoll(3)).    
It returns an integer less than, equal to, or greater than zero if s1 is found, respectively, to be less than, to match, or be greater than s2.    
The strncmp() function is similar, except it compares only the first (at most) n bytes of s1 and s2.    

## 해석 및 부연설명    
:  strcmp는 문자열 s1과 s2를 비교하는 함수다. 한글자씩 비교하면서 s1이 작으면 음수, 같으면 0, 크면 양수를 리턴한다.    
strncmp는 strcmp에서 n글자만 비교하는 기능이 추가된 함수다.    
참고로 첫째줄 괄호에 나와있는 locale은 사용자 인터페이스에서 사용되는 언어, 지역 설정, 출력 형식 등을 정의하는 문자열이라고 하는데, strcmp에서는 고려하지 않는다고 한다.    

## ex)    
```c
char	s1[] = "abcdefg";
char	s2[] = "abcdddd";
printf("%d\n", strncmp(s1, s2, 5));
```
코드 실행 결과
```c
1
```
다섯번째 문자인 e와 d중 s1의 e가 더 크므로 1이 반환되었다.    

# 의문점 및 생각해볼점    
1. [비교방식](#비교하는-값의-형식)    
2. [정확한 리턴값은?](#strncmp의-리턴값)    

***

## 비교하는 값의 형식
BSD Library Functions Manual에 따르면 **The	comparison is done using unsigned characters,	so that	\200 is greater than \0** 이라고 적혀져있다.    
따라서 입력받은 str1과 str2를 unsigned형으로 형변환하여 처리해야 한다.    

***

## strncmp의 리턴값
직접 여러 케이스들을 넣고 실험해본 결과 ***s1 - *s2**의 값이 반환되는 것을 확인하였다.    
예를들어 s1="abcdefgh"이고 s2="abcdefg"라면 끝까지 비교했을 때 h와 \0이 비교되어 h의 ascii값인 104가 반환된다. 반대라면 -104가 반환된다.     

***

# ft_strncmp 구현

```c
int	ft_strncmp(const char *str1, const char *str2, size_t size)
{
	unsigned char	*str1_unchar;
	unsigned char	*str2_unchar;

	str1_unchar = (unsigned char *)str1;
	str2_unchar = (unsigned char *)str2;
	while ((*str1_unchar || *str2_unchar) && size-- > 0)
	{
		if (*str1_unchar != *str2_unchar)
			return (*str1_unchar - *str2_unchar);
		str1_unchar++;
		str2_unchar++;
	}
	return (0);
}

```
size만큼 비교하다가 만약 두 문자열 모두 \0으로 끝나버리면 함수도 거기서 작동이 끝나야한다.    
size가 0일때는 0을 반환한다.    


