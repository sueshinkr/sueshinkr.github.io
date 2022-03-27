---
title:  "[libft] ft_lstsize"
excerpt: "lstsize 함수 구현"

categories:
  - libft
tags:
  - [42seoul, libft]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.03.22 18:00:48
---

# lstsize?

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
//원 함수 없음
    int ft_lstsize(t_list *lst);
```

## 구현해야할 함수 기능    
:  Counts the number of elements in a list.    

## 해석 및 부연설명    
:  리스트에 포함되어있는 원소의 갯수를 샌다.    

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

printf("%d\n", ft_lstsize(list1));
```
코드 실행 결과
```c
3
```
리스트의 갯수가 정상적으로 출력되었다.    

# 의문점 및 생각해볼점    
1. [포인터 매개변수](#함수에-전달하여-값-바꾸기)    

***

## 함수에 전달하여 값 바꾸기
포인터 매개변수를 전달한다고 해서 무조건 값이 바뀌는것은 아니다.    
이런 생각을 한 이유는, 아래 적어놓은 ft_lstsize 함수의 **lst = lst->next** 부분에서 내가 뭔가 착각을 하고 있었기 때문이다.    
lst는 포인터 변수이고, 그 lst의 값을 바꾸었으니 호출이 끝난 후에도 lst의 위치는 리스트의 맨 끝으로 이동해있는게 아닐까 하는 착각을 하고있었다.    
결론만 얘기하면 당연히 아니다. 평범한 swap 함수를 생각해보면, int형 포인터 둘을 매개변수로 받아서 그 포인터를 통해 **해당 주소에 저장된 값**을 바꾸는 것이지 **주소값** 자체를 바꾸는 것이 아니다.    
함수에서 받아온 포인터 변수의 주소값은 복사본일 뿐, 해당 값은 아무리 바뀌어도 원본에 영향을 미치지 못한다.    
그러나 해당 주소 내부에 저장된 값은 어디서 건드리든 딱 지정된 메모리의 값을 바꾸는 것이기 때문에 그대로 적용된다.    
다시 lstsize 함수로 돌아와서, 여기에서는 매개변수로 받아온 lst라는 주소값만 건드리고있기 때문에 원본이랑은 아무 상관이 없는 것이 된다.    
생각해보면 간단한건데 헷갈렸다. 포인터 개념이 아직 부족한걸지도 모르겠다.    

***

# ft_lstsize 구현

```c
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

```
lst의 값이 NULL이라면 해당 원소가 아예 존재하지 않는다는 뜻이다.    
따라서 lst가 NULL이 나올때까지 lst = lst->next를 통해 다음 원소로 진행해가며 그 횟수를 세면 된다.    
