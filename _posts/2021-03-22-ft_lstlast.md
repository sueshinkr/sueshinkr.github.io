---
title:  "[libft] ft_lstlast"
excerpt: "lstlast 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.22 19:00:48
---

# lstlast?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    t_list *ft_lstlast(t_list *lst);
```

## 구현해야할 함수 기능    
:  Returns the last element of the list.    

## 해석 및 부연설명    
:  리스트의 맨 마지막 원소를 반환한다.    

## ex)    
```c
t_list	*list1;
t_list	*list2;
t_list	*list3;

list1 = ft_lstnew("abcde");
list2 = ft_lstnew("12345");
list3 = ft_lstnew("!@#$");
list1->next = list2;
list2->next = list3;

printf("%s\n", (char *)ft_lstlast(list1)->content);
```
코드 실행 결과
```c
!@#$
```
정상적으로 마지막 원소인 list3가 반환되었다.    

# 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_lstlast 구현

```c
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

```
lst의 값이 NULL이라면 애초에 리스트가 존재하지 않는다는 뜻이니 NULL을 반환한다.    
그렇지 않다면 반복문을 돌려 lst->next가 NULL일 때의 lst가 마지막 원소이니 해당 원소를 반환한다.    
