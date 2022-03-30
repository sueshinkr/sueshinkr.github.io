---
title:  "[libft] ft_strtrim"
excerpt: "strtrim 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.18 15:30:48
---

# strtrim?

```c
//원 함수 없음
    char *ft_strtrim(char const *s1, char const *set);
```

## 구현해야할 함수 기능    
:  Allocates (with malloc(3)) and returns a copy of ’s1’ with the characters specified in ’set’ removed from the beginning and the end of the string.    

## 해석 및 부연설명    
:  문자열 s1에서 문자열 set에 들어있는 문자들을 앞뒤로 잘라낸 문자열을 새롭게 할당하여 반환한다. 할당 실패시에서는 `NULL`을 반환한다.    

## ex)    
```c
char	str[] = "abcaafgdefgff";
char	set[] = "abfg";
printf("%s\n", ft_strtrim(str, set));
```
코드 실행 결과
```c
caafgde
```
"abfg" 에 포함되어있는 ab와 fgff가 잘린 나머지가 정상적으로 할당되었다.    

# 의문점 및 생각해볼점    
1. [정확한 작동방식?](#ftstrtrim의-문자열-처리)
2. [문자가 전부 다 잘렸을 경우](#빈-문자열-처리)

***

## `ft_strtrim`의 문자열 처리
우선 set 문자열에 포함되어있는 개별 문자들이 있는지 str의 처음과 끝부터 검사를 시작한다.    
만약 있을경우 그 부분을 넘기고, set 문자열에 포함되어있지 않은 문자가 나올때까지 과정을 반복한다.    

***

## 빈 문자열 처리
앞선 `ft_substr` 함수와 마찬가지로 빈 문자열을 생성해 할당하도록(`ft_strdup("")`) 처리하였다.    
`NULL`로 처리할까도 생각해보았으나, `ft_strtrim` 함수에서는 할당을 실패의 경우에 `NULL`을 반환하도록 지시하였으니 그것과는 좀 다른 경우인 것 같다는 생각이다.    

***

# ft_strtrim 구현

```c
static int	set_start(char const *str, char const *set)
{
	int	i;
	int	start_i;

	i = 0;
	start_i = 0;
	while (set[i])
	{
		if (str[start_i] == set[i])
		{
			start_i++;
			i = 0;
		}
		else
			i++;
	}
	return (start_i);
}

static int	set_end(char const *str, char const *set)
{
	int	i;
	int	end_i;

	i = 0;
	end_i = ft_strlen(str) - 1;
	while (set[i])
	{
		if (str[end_i] == set[i] && end_i != 0)
		{
			end_i--;
			i = 0;
		}
		else
			i++;
	}
	return (end_i);
}

char	*ft_strtrim(char const *str, char const *set)
{
	char	*trimstr;
	char	*result;
	int		start_i;
	int		end_i;

	start_i = set_start(str, set);
	end_i = set_end(str, set);
	if (start_i > end_i)
		return (ft_strdup(""));
	trimstr = (char *)malloc((end_i - start_i + 2) * sizeof(char));
	if (!trimstr)
		return (NULL);
	result = trimstr;
	while (end_i - start_i + 1 > 0)
		*trimstr++ = *(str + start_i++);
	*trimstr = 0;
	return (result);
}

```
`static` 함수로 정의한 `set_start`와 `set_end`에서 각각 set에 포함되어있는 문자열을 제외하고 남은 문자열의 인덱스, 즉 새로 만들 문자열의 시작과 끝을 가리키는 값을 반환하도록 구성하였다.    
그 후 구해놓은 인덱스를 기준으로 원래의 문자열에서 잘라내어 붙여주면 끝이다.     
strat_i가 end_i보다 클 때는 문자가 모두 제거되는 경우이므로 빈 문자열을 반환하였다.    
