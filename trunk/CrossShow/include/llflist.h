#ifdef LLF_LIST_
#define LLF_LIST_

#include<iostream>
using namespace std;
template<class T>
struct nodetype
{
	T info;
	nodetype<T> *link;
};
//ADT链表类模板总定义
template<class T>
class linkdlist
{
	friend ostream& operator<<(ostream&,const linkdlist<T>&);
	//遍历输出链表所有结点
public:

	const linkdlist<T>& operator=(const linkdlist<T>&);
	//重载=
	void initlist();
	//功能:初始化函数
	//前置:NULL
	//后置:    first=NUll;last=NULL;count=0;
	bool isempty();
	//功能:检查是否为空
	//前置:NULL
	//后置:    如空则return TRUE,否则return FALSE;
	int     length();
	//功能:得到链表的长度
	//前置:链表必须存在
	//后置:    返回链表的count
	void destroylist();
	//功能:删除链表中所有的结点
	//前置:链表必须存在
	//后置:first=NUll;last=NULL;count=0;
	T front();
	//功能:返回第一个结点数据
	//前置:链表必须存在并且不能是空表
	//后置:如链表为空则程序中止，否则返回第一个结点数据

	T back();
	//功能:返回第一个结点数据
	//前置:链表必须存在并且不能是空表
	//后置:如链表为空则程序中止，否则返回第一个结点数据
	bool search(const T& searchitem);
	//功能:检查给定数据是存在于链表中
	//前置:链表必须存在并且不能是空表
	//后置:存在就返回TRUE，不存在就返回FALSE

	void insertfirst(const T& newitem);
	//功能:插入新项到链表头部
	//前置:链表必须存在
	//后置:链表尾结点重新指向链表未尾，count加1

	void insertlast(const T& newitem);
	//功能:插入新项到链表尾部
	//前置:链表必须存在
	//后置:链表尾结点重新指向链表未尾，count加1
	void deletenode(const T& deleteitem);
	//功能:从链表中删除一个结点
	//前置:链表必须存在
	//后置:先用search()检查是否存在?,存在则删除，
	//链表尾结点重新指向链表未尾，count减1
	linkdlist();
	//功能:链表模板的缺省构造函数
	//前置:初始化一个链表，调用initlist();
	//后置:first=NUll;last=NULL;count=0;

	linkdlist(const linkdlist<T>&otherlist);
	//功能:复制构造函数

	~linkdlist();
	//功能:析构函数
	//前置:NULL;调用destroylist();
	//后置:把内存中的链表所有的结点删除
	void print();
	//功能:打印所有结点数据
	//前置:NULL
	//后置:把链表所有的结点打印出来        

protected:
	int count;                //链表长度
	nodetype<T> *first;        //链表首结点
	nodetype<T> *last;        //链表尾结点

private:
	void copylist(const linkdlist<T>&otherlist);
	//功能:复制链表数据到另一个链表
	//前置:链表必须存在并且不能是空表
	//后置:用this链表去建立另一个链表对象
};

//友元函数实现
template<class T>
ostream& operator<<(ostream& osobject,const linkdlist<T>& list)
{
	nodetype<T> *current;
	current=list.first;
	while(current!=NULL)
	{
		osobject<<current->info<<" ";
		current=current->link;
	}
	return osobject;
}

#endif