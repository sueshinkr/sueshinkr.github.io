// list.cpp

#include <iostream>
#include "list.h"

List::List()
{
	top = 0;
}

bool List::isempty() const
{
	return top == 0;
}

bool List::isfull() const
{
	return top == MAX;
}

void List::add(const Item & item)
{
	if(top < MAX)
		items[top++] = item;
}

void List::visit(void (*pf)(Item &))
{
	for(int i = 0; i < top; i++)
		pf(items[i]);
}

void List::show() const
{
	if(isempty())
		std::cout << "리스트가 비어있습니다.\n";
	else
	{
		for(int i = 0; i < top; i++)
			std::cout << "Item #" << i + 1 << ": " << items[i] << "\n";
	}
}
