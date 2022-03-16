---
title:  "[libft] ft_strlcat"
excerpt: "strlcat"

categories:
  - 42seoul
tags:
  - libft
last_modified_at: 2021-03-16T08:06:00-05:00
---

# strlcat?

```c
#include <string.h>

     size_t strlcat(char *dst, const char *src, size_t size);
```

##### Linux manpage description    
:  The strlcat() function appends the NUL-terminated string src to the end of dst. It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.    
The strlcpy() and strlcat() functions return the total length of the string they tried to create. For strlcat() that means the initial length of dst plus the length of src.    

##### 내멋대로 해석    
:  strlcat은 dst뒤에 src를 붙여 총 길이가 size가 되도록 만든다. 경우에 따라 어떻게 작동하는지가 조금씩 다른데    
1. size가 애초부터 dst보다 작다면 그냥 size + src의 길이를 리턴하고 끝낸다.    
2. size가 dst보다 클 경우 마지막에 0을 넣어줄 한자리를 빼고 총 길이가 size - 1이 되도록 dst뒤에 src를 붙여나간다. 물론 마지막엔 0을 넣어준 후 dst길이 + src길이를 리턴한다.

##### ex)    
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
````

##### 의문점 및 생각해볼점    
딱히 없나...?

***


***

```c
size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t size)
{
	size_t	dst_i;
	size_t	src_i;
	size_t	src_len;

	dst_i = 0;
	src_i = 0;
	src_len = ft_strlen(src);
	while (*dst)
	{
		dst++;
		dst_i++;
	}
	if (dst_i > size)
		return (size + src_len);
	while (*src && dst_i + src_i + 1 < size)
	{
		*dst++ = *src++;
		src_i++;
	}
	*dst = 0;
	return (dst_i + src_len);
}
```

