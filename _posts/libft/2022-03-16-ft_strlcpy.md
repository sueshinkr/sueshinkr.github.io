---
title:  "[libft] ft_strlcpy"
excerpt: "strlcpy 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.16 08:00:48
---

# strlcpy?

```c
#include <string.h>

    size_t strlcpy(char *dst, const char *src, size_t size);
```

## Linux manpage description    
: The strlcpy() function copies up to size - 1 characters from the NUL-terminated string src to dst, NUL-terminating the result.    
The strlcpy() and strlcat() functions return the total length of the string they tried to create. For strlcpy() that means the length of src.    

## 해석 및 부연설명    
:  `memcpy` 함수에서 봤던 `strncpy` 함수의 특징처럼 `strlcpy` 함수도 문자열 뒤에 `NUL`이 있는지를 검사하여 찾는순간 복사를 종료한다. 다만 `strncpy`과 다른점은 
1. **strncpy**는 dst를 반환하지만, **strlcpy**는 src의 길이를 반환한다.
2. src의 길이가 size보다 작을 경우 **strncpy**는 남은부분을 전부 \0으로 채우지만, **strlcpy**는 하나만 채우고 끝낸다.
3. src의 길이가 size보다 클 경우 **strncpy**는 size만큼만 복사하고 종료되어 0이 붙지 않지만, **strlcpy**는 size가 존재할 경우(1 이상)엔 무조건 뒤에 \0을 붙여준다. 따라서 `strlcpy`가 조금 더 안전한 함수이다.    

## ex)    
```c
char	dst[] = "abcdefghijklmnop";
char	src[] = "12345";
strlcpy(dst, src, sizeof(src));
printf("%s\n", dst);
```
코드 실행 결과
```c
12345
```
size만큼 복사가 끝나거나 src가 끝나면 그 뒤에 `NUL`을 붙여주니 size가 잘못되어도 비교적 안전하다.    

# 의문점 및 생각해볼점    
1. [strncpy와의 차이점은?](#strncpy와의-차이점)    
2. [restrict?](#restrict란)    

***

## strncpy와의 차이점
[위에서 이미 얘기했다](#해석-및-부연설명)

***

## restrict란?
BSD Library Functions Manual 기준으로는 `strlcpy`가 **size_t strlcpy(char * restrict dst, const char * restrict	src, size_t dstsize)**으로 정의되어있다.    
여기서 사용하는 `restrict` 포인터는 각 포인터가 서로 다른 메모리 공간을 점유하니 걱정말고 최적화하라는 의미다.    
이 키워드가 쓰여졌을 때 같은 메모리를 점유중인 데이터들을 써도 에러가 발생하지는 않지만, 최적화할 때 문제가 생겨 무언가의 오류가 발생할 여지가 있다고 한다. 다만 libft과제에서는 사용이 금지되어있다.    

***

# ft_strlcpy 구현

```c
size_t	ft_strlcpy(char dst, char *src, size_t size)
{
	int	src_len;

	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	while (size-- > 1 && *src)
		*dst++ = *src++;
	*dst = 0;
	return (src_len);
}

```

