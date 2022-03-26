---
title:  "[libft] ft_lstadd_front"
excerpt: "lstadd_front 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.22 16:30:48
---

# lstadd_front?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    void ft_lstadd_front(t_list **lst, t_list *new);
```

## 구현해야할 함수 기능    
:  Adds the element ’new’ at the beginning of the list.    

## 해석 및 부연설명    
:  원소 'new'를 리스트의 맨 앞에 추가한다. 이 때 첫번째 매개변수는 리스트 맨 앞에 위치한 원소의 주소를 나타내는 포인터이고, 두번째 매개변수는 리스트에 추가할 원소이다.    

## ex)    
```c
t_list	*start;
t_list	*list1;
t_list	*list2;
t_list	*list3;

list1 = ft_lstnew("abcde");
list2 = ft_lstnew("12345");
list3 = ft_lstnew("!@#$");

ft_lstadd_front(&start, list3);
printf("%s\n", (char *)start->content);
ft_lstadd_front(&start, list2);
printf("%s\n", (char *)start->content);
ft_lstadd_front(&start, list1);
printf("%s\n", (char *)start->content);
```
코드 실행 결과
```c
!@#$
12345
abcde
```
리스트의 시작 노드인 start에 정상적으로 노드들이 추가되었다.    

# 의문점 및 생각해볼점    
1. [연결 리스트의 구조](#노드-삽입--추가--삭제)    

***

## 노드 삽입 / 추가 / 삭제
[자세한 설명과 영상 링크](https://opentutorials.org/module/1335/8821)    
libft에서는 head 노드(데이터가 저장되어있지 않은 단순한 첫번째 노드)를 따로 사용하지 않아 위 링크랑은 조금 차이가 있는 것 같긴 하지만, 기본적인 구조는 동일하다.    
결국 연결 리스트는 선형적으로 이루어져있으니 앞 뒤 연결을 어떻게 잘 하느냐가 중요한 듯 하다.    

***

# ft_lstadd_front 구현

```c
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}

```
이중 포인터 **lst는 리스트의 첫번째 노드가 저장되어있는 포인터의 주소가 저장되어있다. 따라서 *lst는 첫번째 노드의 주소를 가리키며, new->next를 *lst로 지정해줌으로써 새로 추가할 new가 *lst 앞에 위치하도록 한다.    
다만 이렇게 끝낼경우 리스트의 시작지점은 여전히 *lst로 남아있기 때문에, *lst를 new로 지정하여 new 노드가 리스트의 시작지점을 가리키도록 한다.    

덧) head 노드가 따로 존재할 때는 코드를 다른 방식으로 짜야한다.    

