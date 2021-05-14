#ifndef DLIST_H_
#define DLIST_H_
#include <iostream>

template <typename T>
class Double_list
{
public:
	class iterator; 
	friend class iterator; //класс итератора должен иметь доступ к полям класса Double_list

private:
	class Double_node; 
	friend class Double_node;

	class Double_node //
	{
	public:
		friend class Double_list<T>;
		friend class iterator;

		//Создать вузол с какимто значением типа T
		Double_node(T node_val): val(node_val) {}
		Double_node() {}
		~Double_node() {}

		void print_val() {std::cout << val << " "; }

		Double_node *next; //указывает на следующий узел списка
		Double_node *prev; //указывает на предыдущий узел. 
		T val; //данные.

	};

public:
	class iterator
	{
		friend class Double_list<T>; 

	public:

		// нулевой конструкрор
		iterator() :the_node(0) {}
		//здесь мы создаем итератор с указателя на узел Double_node
		iterator(Double_node *dn): the_node(dn) {}
		//конструктор копии
		iterator(const iterator &it): the_node(it.the_node) {}

		iterator& operator=(const iterator &it)
		{
			the_node = it.the_node;
			return *this;
		}

		// в этом классе оператор == означает, что два итератора указывают на
		// один и тот же узел
		bool operator == (const iterator &it) const
		{
			return (the_node == it.the_node);
		}


		bool operator!=(const iterator &it) const
		{
			return !(it == *this);
		}

		//переводит итератор на следующий узел списка. 
		iterator& operator++()
		{
			if (the_node == 0)
				throw "incremented an empty iterator";
			if (the_node->next == 0)
				throw "tried to increment too far past the end";
			
			the_node = the_node->next;
			return *this;
		}

		//переводит итератор на предідущий узел списка. 
		iterator & operator--()
		{
			if (the_node == 0)
				throw "decremented an empty iterator";
			if (the_node->prev == 0)
				throw "tried to decrement past the beginning";

			the_node = the_node->prev;
			return *this;
		}

		//Возвращает значение данных.
		T& operator*() const
		{
			if (the_node == 0)
				throw "tried to dereference an empty iterator";
			return the_node->val;
		}

	private:
		Double_node *the_node;


	};

private:
	Double_node *head;  //указывает на начало списка. 
	Double_node *tail;  //указывает на елемент, который идет за последним

	iterator head_iterator; //итератор, который всегда указывает на начало списка
	iterator tail_iterator; //итератор, который всегда указывает на элемент, который находится за последним.

public:
	Double_list()
	{
		head = tail = new Double_node;
		tail->next = nullptr;
		tail->prev = nullptr;

		//инициализирует итераторы
		head_iterator = iterator(head);
		tail_iterator = iterator(tail);
	}

	//Создать список, который содержит один элемент. 
	Double_list(T node_val)
	{
		head = tail = new Double_node;
		tail->next  = nullptr;
		tail->prev = 0;

		head_iterator = iterator(head);
		tail_iterator = iterator(tail);
		add_front(node_val);
	}

	~Double_list()
	{
		Double_node *node_to_delete = head;
		for (Double_node *sn = head; sn != tail;)
		{
			sn = sn->next;
			delete node_to_delete;
			node_to_delete = sn;
		}

		delete node_to_delete;
	}

	bool is_empty() {return head == tail;}


	iterator front() {return head_iterator;}
	iterator rear() {return tail_iterator;}

	//вставляем узел в начало списка
	void add_front(T node_val)
	{
		Double_node *node_to_add = new Double_node (node_val);
		node_to_add->next = head;
		node_to_add->prev = nullptr;
		head->prev = node_to_add;
		head = node_to_add;
		//так как head изменился, нужно изменить head_iterator
		head_iterator= iterator(head);
	}

	//вставляем узел в конец списка
	void add_rear(T node_val)
	{
		if (is_empty())
			add_front(node_val);
		else
		{
			Double_node *node_to_add = new Double_node(node_val);
			node_to_add->next = tail;
			node_to_add->prev = tail->prev;
			tail->prev->next = node_to_add;
			tail->prev =  node_to_add;
			//изменяем tail_iterator
			tail_iterator = iterator(tail);
		}
	}


	//вставляет в спсиок node_val после итератора key_i
	bool insert_after(T node_val, const iterator &key_i)
	{
		for (Double_node *dn = head; dn != tail; dn = dn->next)
		{
			//Найден ли узел для заданного итератора
			if (dn == key_i.the_node)
			{
				Double_node *node_to_add = new Double_node(node_val);
				node_to_add->prev = dn;
				node_to_add->next = dn->next;
				dn->next->prev = node_to_add;
				dn->next = node_to_add;
				return true;
			}
		}
		return false;
	}

	//Удалить первый элемент списка. 
	T remove_front()
	{
		if (is_empty())
			throw "tried to remove from an empty list";
		Double_node *node_to_remove = head;
		T return_val = node_to_remove->val;
		head = node_to_remove->next;
		head->prev = 0;
		head_iterator = iterator(head);

		delete node_to_remove;
		return return_val;

	}

	T remove_rear()
	{
		if (is_empty())
			throw "tried to remove from an empty list";

		Double_node *node_to_remove = tail->prev;

		if(node_to_remove->prev == 0)
		{
			return remove_front();
		}
		else
		{
			T return_val = node_to_remove->val;
			node_to_remove->prev->next = tail;
			tail->prev = node_to_remove->prev;
			delete node_to_remove;
			return return_val;
		}
	}


	bool remove_it(iterator &key_i)
	{
		for (Double_node *dn =  head; dn != tail; dn = dn-next)
		{
			//Найден ли узел для заданного итератора
			if (dn == key+i.the_node)
			{
				dn->prev->next = dn->next;
				dn->next->prev = dn->prev;
				delete dn;
				key_i.the_node =0;
				return true;
			}
		}

		return false;
	}

	//Возвращает первый итератор, указывающий на node_val
	iterator find(T node_val) const
	{
		for (double_node *dn = head; dn != tail; dn = dn->next)
		{
			if (dn->val == node_val)
				return iterator(dn);
		}

		//Если node_val нет в списке возвращает tail_iterator
		return tail_iterator;
	}

	//возвращает итератор, который указывает на n-ый элемент списка
	iterator get_nth(const int element_num) const
	{
		if (element_num < 1)
			return tail_iterator;

		int count = 1;
		for(Double_node *dn = head; dn != tail; dn = dn->next)
		{
			if (count++ == element_num)
				return iterator(dn);
		}

		return tail_iterator;
	}

	//Количество узлов в списке. 
	int size() const
	{
		int count = 0;
		for (Double_node *dn = head; dn != tail; dn = dn->next) 
			++count;
		return count;
	}


	
	void print() const
	{
		for (Double_node *dn = head; dn!= tail; dn = dn->next)
		{
			dn->print_val();
		}

		std::cout << std::endl;
	}

};

#endif	


Использование списка

#include "dlist.h"

int main()
{
	Double_list<int> the_list;

	Double_list<int>::iterator list_iter;

	for (int i=0; i<5; ++i)
	{
		the_list.add_front(i);
	}

	the_list.print();

	the_list.remove_front();


	for (list_iter = the_list.front(); list_iter != the_list.rear(); ++ list_iter)
	{
		std::cout << *list_iter << " ";
	}
	std::cout << std:: endl;

	//выводим в обратном порядке
	for (list_iter = the_list.rear(); list_iter != the_list.front();)
	{
		--list_iter;
		std::cout << *list_iter << " ";
	}

	std::cout << std::endl;

	system("PAUSE");
	return 0;
