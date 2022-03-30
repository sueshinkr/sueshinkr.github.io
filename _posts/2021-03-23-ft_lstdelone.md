---
title:  "[libft] ft_lstdelone"
excerpt: "lstdelone 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.23 22:30:48
---

# lstdelone?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    void ft_lstdelone(t_list *lst, void (*del)(void *));
```

## 구현해야할 함수 기능    
:  Takes as a parameter an element and frees the memory of the element’s content using the function ’del’ given as a parameter and free the element. The memory of ’next’ must not be freed.    

## 해석 및 부연설명    
:  두 번째 매개변수로 받은 `del` 함수를 이용해 첫번째 매개변수로 받은 원소 lst의 요소 content의 메모리를 `free`하고, 원소도 `free`한다. 요소 next의 메모리는 `free`되지 않아야 한다.    

## ex)    
```c
static void	del(void *content)
{
	free(content);
}

int	main(void)
{
	t_list	*list;

	list = ft_lstnew(ft_strdup("abcde"));
	ft_lstdelone(list, &del);
}
```
코드 실행 결과
```c
// valgrind로 검사한 결과
==18741== HEAP SUMMARY:
==18741==     in use at exit: 0 bytes in 0 blocks
==18741==   total heap usage: 2 allocs, 2 frees, 22 bytes allocated
==18741== 
==18741== All heap blocks were freed -- no leaks are possible
```
할당되었던 메모리들이 정상적으로 `free`되었다.    

# 의문점 및 생각해볼점    
1. [free(): invalid pointer](#잘못된-free)

***

## 잘못된 `free`
위 ex)에서 **list = ft_lstnew("abcde")** 라고 한 상태로 `ft_lstdelone`을 실행했을 때, `free(): invalid pointer`라는 오류 메세지가 떴다.    
`free`하라고 지정된 메모리가 정상적이지 않거나 변경되었을 때 뜨는 에러라고 한다.    
이렇게 이유를 찾아놓고도 대체 뭐가 문제인지 한참을 고민하다가, list의 content가 동적할당되지 않았다는 사실을 뒤늦게 깨달았다. 메모리 관련 함수들은 정말로 주의를 기울일 필요가 있을 것 같다...    

***

# ft_lstdelone 구현

```c
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
}

```

lst->content와 lst를 차례대로 `free`해주면 끝이다.    
