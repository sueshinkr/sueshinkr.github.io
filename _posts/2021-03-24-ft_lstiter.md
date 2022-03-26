---
title:  "[libft] ft_lstiter"
excerpt: "lstiter 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.24 00:30:00
---

# lstiter?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    void ft_lstiter(t_list *lst, void (*f)(void *));
```

## 구현해야할 함수 기능    
:  Iterates the list ’lst’ and applies the function ’f’ to the content of each element.    

## 내멋대로 해석    
:  리스트의 모든 원소들의 content에 함수 f를 적용한다.    

## ex)    
```c
static void	func(void *content)
{
	char	*temp;

	temp = (char *)content;
	while (*temp)
		*temp++ += 1;
}

int	main(void)
{
	t_list	*list1;
	t_list	*list2;
	t_list	*list3;

	list1 = ft_lstnew(ft_strdup("abcde"));
	list2 = ft_lstnew(ft_strdup("12345"));
	list3 = ft_lstnew(ft_strdup("!@#$"));
	list1->next = list2;
	list2->next = list3;

	ft_lstiter(list1, func);
	printf("%s\n", (char *)list1->content);
	printf("%s\n", (char *)list2->content);
	printf("%s\n", (char *)list3->content);
}
```
코드 실행 결과
```c
bcdef
23456
"A$%
```
리스트의 모든 원소들에 함수 func가 정상적으로 적용되었다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_lstiter 구현

```c
void	ft_lstiter(t_list *lst, void (*func)(void *))
{
	while (lst)
	{
		func(lst->content);
		lst = lst->next;
	}
}

```

반복문을 통해 리스트를 끝까지 돌면서 func를 적용시켜주면 된다.    
