---
title:  "[libft] ft_lstadd_back"
excerpt: "lstadd_back 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.22 19:30:48
---

# lstadd_back?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    void ft_lstadd_back(t_list **lst, t_list *new);
```

## 구현해야할 함수 기능    
:  Adds the element ’new’ at the end of the list.    

## 해석 및 부연설명    
:  리스트의 맨 뒤에 원소 'new'를 추가한다.    

## ex)    
```c
t_list	*start;
t_list	*list1;
t_list	*list2;
t_list	*list3;

list1 = ft_lstnew("abcde");
list2 = ft_lstnew("12345");
list3 = ft_lstnew("!@#$");
start = NULL;

ft_lstadd_back(&start, list1);
printf("%s\n", (char *)ft_lstlast(start)->content);
ft_lstadd_back(&start, list2);
printf("%s\n", (char *)ft_lstlast(start)->content);
ft_lstadd_back(&start, list3);
printf("%s\n", (char *)ft_lstlast(start)->content);
```
코드 실행 결과
```c
abcde
12345
!@#$
```
리스트의 마지막 노드에 정상적으로 노드들이 추가되었다.    

# 의문점 및 생각해볼점    
1. [잘못된 메모리 접근](#segmentation-fault)

***

## segmentation fault
위 ex에서 **start = NULL** 부분을 빠트렸을경우 바로 segmentation fault에 빠져버렸다.    
처음 start가 ft_lstadd_back 함수로 전달되면 우선 if문에서 NULL인지 아닌지를 검사하는데, 이 때 start는 NULL이 아니므로 else문이 실행되어 ft_lstlast 함수로 전달된다.    
바로 여기서 문제가 생긴다. ft_lstlast함수에서는 while문에서 lst->next != NULL 조건을 체크하지만, 매개변수로 가져온 start->next는 초기화되지 않은 값으로 단순히 아무 주소나 가리키고있을 뿐이다.    
따라서 lst = lst->next가 한번 실행된 후의 lst->next는 존재할 수 없다. 여기서 segmentation fault가 발생한 것이었다.    
그렇기에 start를 NULL로 지정하여 맨 처음에는 원소 new를 그대로 start에 넣어주는 것으로 해결할 수 있었다.     

***

# ft_lstadd_back 구현

```c
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
}

```
*lst의 값이 NULL이라면 애초에 리스트가 존재하지 않는다는 뜻이니 바로 새로운 원소를 넣어준다.    
그렇지 않다면 lstlast 함수를 이용해 리스트 맨 뒤로 가서 그곳에 새로운 원소를 넣어준다.    
