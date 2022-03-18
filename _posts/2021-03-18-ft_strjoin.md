---
title:  "[libft] ft_strjoin"
excerpt: "strjoin 함수 구현"

categories:
  - 42seoul
tags:
  - libft
date: 2022.03.18 15:00:48
---

# strjoin?

```c
//원 함수 없음
    char *ft_strjoin(char const *s1, char const *s2);
```

##### 구현해야할 함수 기능    
:  Allocates (with malloc(3)) and returns a new string, which is the result of the concatenation of ’s1’ and ’s2’.    

##### 내멋대로 해석    
:  문자열 s1과 s2를 합한 문자열을 새롭게 할당하여 반환한다. 할당 실패시에서는 NULL을 반환한다.        

##### ex)    
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

##### 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_strjoin 구현

```c
char	*ft_strjoin(char const *str1, char const *str2)
{
	char	*joinstr;
	int		joinstr_len;
	int		i;

	i = 0;
	joinstr_len = ft_strlen(str1) + ft_strlen(str2);
	joinstr = (char *)malloc((joinstr_len + 1) * sizeof(char));
	if (!joinstr)
		return (NULL);
	while (i < joinstr_len)
	{
		if (*str1)
			joinstr[i] = *str1++;
		else if (*str2)
			joinstr[i] = *str2++;
		i++;
	}
	joinstr[i] = 0;
	return (joinstr);
}

```


