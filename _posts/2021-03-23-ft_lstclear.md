---
title:  "[libft] ft_lstclear"
excerpt: "lstclear 함수 구현"

categories:
  - 42seoul
tags:
  - libft
date: 2022.03.23 23:30:48
---

# lstclear?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    void ft_lstclear(t_list **lst, void (*del)(void *));
```

##### 구현해야할 함수 기능    
:  Deletes and frees the given element and every successor of that element, using the function ’del’ and free(3). Finally, the pointer to the list must be set to NULL.    

##### 내멋대로 해석    
:  두 번째 매개변수로 받은 del 함수를 이용해 리스트의 모든 원소들을 free하고 삭제한다.     

##### ex)    
```c
static void	ft_del(void *content)
{
	free(content);
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

	ft_lstclear(&list1, &ft_del);
}
```
코드 실행 결과
```c
// valgrind로 검사한 결과
==30970== HEAP SUMMARY:
==30970==     in use at exit: 0 bytes in 0 blocks
==30970==   total heap usage: 6 allocs, 6 frees, 65 bytes allocated
==30970== 
==30970== All heap blocks were freed -- no leaks are possible
```
할당되었던 메모리들이 정상적으로 free되었다.    

##### 의문점 및 생각해볼점    
딱히 없다.    

***

# ft_lstclear 구현

```c
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cur;

	while (*lst)
	{
		cur = *lst;
		*lst = (*lst)->next;
		ft_lstdelone(cur, del);
	}
	*lst = NULL;
}

```

리스트를 돌면서 lst->content와 lst를 차례대로 free해주면 끝이다. 이 때 free하기 전에 다음 원소로 넘어가는 작업도 필요하므로 임시로 쓸 cur 리스트를 만들어 활용했다.    
