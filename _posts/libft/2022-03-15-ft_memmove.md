---
title:  "[libft] ft_memmove"
excerpt: "memmove 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.15 23:00:48
---

# memmove?

```c
#include <string.h>

    void *memmove(void *dst, const void *src, size_t n);
```

## Linux manpage description    
:   The memmove() function copies n bytes from memory area src to memory area dst.    
The memory areas may overlap: copying takes place as though the bytes in src are first copied into a temporary array that does not overlap src or dst, and the bytes are then copied from the temporary array to dst.    

## 해석 및 부연설명    
:  `memcpy`와 같은 기능을 수행하지만, 메모리가 overlap 되어있을 경우를 생각해 src의 데이터를 src나 dst와 overlap되지 않는 곳에 카피해놓은 후에 dst로 복사하는 과정을 거친다.  

## ex)    
```c
char	dst[] = "abcdefghijklmnop";
char	src[] = "12345";
memmove(dst, src, sizeof(src) - 1);
printf("%s\n", dst);
```
코드 실행 결과
```c
12345fghijklmnop
```
`memcopy`와 마찬가지로 복사할 src의 맨 마지막에 있을 `NULL`을 고려하여 size를 설정해야한다.

# 의문점 및 생각해볼점    
1. [`memcpy`와의 차이점은?](#memcpy와의-차이점)    

***

## `memcpy`와의 차이점

[memcpy에서 썼던 링크 재탕](https://blog.naver.com/PostView.nhn?isHttpsRedirect=true&blogId=sharonichoya&logNo=220510332768)    
`memcpy`에서 문제가 되었던 두 메모리가 overlap 되어있을 경우의 문제점이 말끔히 해결된다.    
그럼 `memcpy`를 쓸 이유가 없는 것 아닌가? 싶었지만 당연하게도 `memcpy`가 `memmove`보다 빠르게 동작한다고 한다.    

***

# ft_memmove 구현

```c
void	*ft_memmove(void *dst, const void *src, size_t size)
{
	void	*result;

	result = dst;
	if (dst == src)
		return (result);
	if (dst < src)
	{
		while (size-- > 0)
			*(unsigned char *)dst++ = *(unsigned char *)src++;
	}
	else
	{
		while (size-- > 0)
			*(unsigned char *)(dst + size) = *(unsigned char *)(src + size);
	}	
	return (result);
}
```
mac기준 매뉴얼에는 버퍼를 사용한다는 말 자체가 없이 'the copy is always done in a non-destructive manner'라고만 적혀있다. 그래서 경우를 나누어 처리하였는데    
1. src값이 dst보다 클 때    
앞에서부터 차례대로 복사하면 아무런 문제가 생기지 않는다.    
2. dst값이 src보다 클 때    
앞에서부터 복사하면 메모리가 덮어씌워지는 문제가 생긴다. 간단하게 뒤에서부터 넣어주면 데이터가 정상적으로 복사된다.    

dst와 src가 둘다 `NULL`일 경우는 `ft_memcpy`함수와 같은 방식으로, 원 `memmove`와 같은 값이 반환되도록 구현했다.

