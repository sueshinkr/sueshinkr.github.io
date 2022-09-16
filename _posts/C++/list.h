// list.h

#ifndef LIST_H_
#define LIST_H_

typedef int Item;

class List
{
	private:
		static const int MAX = 10;
		Item items[MAX];
		int top;
	public:
		List();
		bool isempty() const;
		bool isfull() const;
		void add(const Item &);
		void show() const;
		void visit(void (*pf)(Item &));
};

#endif