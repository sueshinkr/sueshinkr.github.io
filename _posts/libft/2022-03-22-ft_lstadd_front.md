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
2. [왜 이중포인터?](#매개변수로-이중-포인터를-받는-이유)

***

## 노드 삽입 / 추가 / 삭제
[자세한 설명과 영상 링크](https://opentutorials.org/module/1335/8821)    
head 노드(데이터가 저장되어있지 않은 단순한 첫번째 노드)를 `*lst[0]`이라고 보면 링크의 내용을 그대로 적용시킬 수 있다.    
결국 연결 리스트는 선형적으로 이루어져있으니 앞 뒤 연결을 어떻게 잘 하느냐가 중요한 듯 하다.    

***

## 매개변수로 이중 포인터를 받는 이유
```c
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}

void	ft_lstadd_front(t_list *lst, t_list *new)
{
	new->next = lst;
	lst = new;
}
```
위 두 경우에는 기능상의 차이가 있을까?    
물론 차이가 있기에 던진 질문이다.    
* 매개변수를 이중 포인터로 사용할 경우, 마지막에 맨 처음 노드를 new로 지정해주는 `*lst = new`부분이 정상적으로 작동한다. 이중포인터(\*\*lst) 변수의 주소값 내부에 저장된 값(*lst라는 포인터 변수의 값)을 바꾼 것이기 때문이다.    
* 매개변수를 그냥 포인터로 사용할 경우, `lst = new`는 매개변수로 받아온 포인터 변수의 값을 바꾸는 것이 되어버린다. 이때 포인터 변수의 값은 해당 함수 안에서의 지역변수 취급이 되어 함수 안에서만 바뀔 뿐이다.    
관련 내용을 한번 다룬적이 있다. 다만 본 내용을 동료평가때 새롭게 깨닫게 된 터라 순서는 좀 뒤죽박죽이다.    
[링크](https://sueshinkr.github.io/libft/ft_lstsize/#%ED%95%A8%EC%88%98%EC%97%90-%EC%A0%84%EB%8B%AC%ED%95%98%EC%97%AC-%EA%B0%92-%EB%B0%94%EA%BE%B8%EA%B8%B0) 

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


