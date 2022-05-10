---
title:  "[libft] ft_strlcat"
excerpt: "strlcat 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.16 10:00:48
---

# strlcat?

```c
#include <string.h>

    size_t strlcat(char *dst, const char *src, size_t size);
```

## Linux manpage description    
:  The strlcat() function appends the NUL-terminated string src to the end of dst. It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.    
The strlcpy() and strlcat() functions return the total length of the string they tried to create. For strlcat() that means the initial length of dst plus the length of src.    

## 해석 및 부연설명    
:  `strlcat`은 dst뒤에 src를 붙여 총 길이(`NUL` 포함)가 size가 되도록 만든다.    
정확히는 size - dst의 길이 - 1만큼 src를 복사해 dst 뒤에 붙이고, 마지막에 \0을 넣어준다. 리턴값은 원래 dst의 길이 + src의 길이이다. 

## ex)    
```c
char	dst[] = "12345";
char	src[] = "abcdefghijklmnop";
printf("%d\n", strlcat(dst, src, 10);
printf("%s\n", dst);
```
코드 실행 결과
```c
21
12345abcd
```
정상적으로 dst 뒤에 size - dst의 길이 - 1인 4개 문자가 src에서 복사되어 dst 뒤에 붙여넣어졌다.    

# 의문점 및 생각해볼점    
1. [size에 따른 `strlcat`의 작동방식](#size별로-나눈-case)

***

## size별로 나눈 case
경우에 따라 어떻게 작동하는지가 조금씩 다른데    
1. size가 dst의 길이보다 작다면 따로 문자열을 조작하는 과정 없이 size + src의 길이를 리턴하고 끝낸다.    
2. size가 dst의 길이보다 클 경우 마지막에 \0을 넣어줄 한자리를 빼고 총 길이가 size - 1이 되도록 dst뒤에 src를 붙여나간다.    
물론 마지막엔 \0을 넣어준 후 원래 dst의 길이 + src의 길이를 리턴한다.    
3. size가 dst의 길이 + src의 길이보다 클 경우엔 src가 끝날때까지만 붙여넣고 마지막에 \0을 넣어준다.    
리턴값은 2번 경우와 같이 원래 dst의 길이 + src의 길이를 리턴한다.    

***

# ft_strlcat 구현

```c
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_i;
	size_t	src_len;
	size_t	dst_len;

	src_i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len > size)
		return (size + src_len);
	while (*src && dst_len + src_i + 1 < size)
	{
		*(dst + dst_len + src_i) = *src++;
		src_i++;
	}
	*(dst + dst_len + src_i) = 0;
	return (dst_len + src_len);
}
```
size변수가 `size_t`, `unsigned`형으로 들어오기때문에 언더플로우가 발생하지 않도록 dst_len + src_i < size -1 대신 위 코드처럼 조건을 설정했다.


