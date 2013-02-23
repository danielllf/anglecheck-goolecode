#include "../include/llflist.h"
#include<list>

template<class T>
bool linkdlist<T>::isempty()
{return (first==NULL);}

//缺省的构造函数,时间复杂度为 O(1)
template<class T>
linkdlist<T>::linkdlist()
{
	first=NUll;
	last=NULL;
	count=0;
}

//缺省的构造函数,时间复杂度为 O(n)
template<class T>
void linkdlist<T>::destroylist()
{
	nodetype<T> *temp;
	while(first!=NULL)
	{    //删除方法：从链表的首结点往尾部方向删除
		temp=first;            //把首结点存入临时结点temp    
		first=first->link;    //首结点指到下一个结点,首结点就脱离链表
		delete temp;        //删除刚才指向首结点的temp
		temp=NULL;            //置temp指针为空，以防止内存访问出错
	}
	//首尾结点置NULL，结点数量为0
	first=NULL;
	last=NULL;
	count=0;
}

//初始化链表,时间复杂度为 O(n)
template<class T>
void linkdlist<T>::initlist()
{destroylist();}

//功能:得到链表的长度,时间复杂度为 O(1)
template<class T>
int linkdlist<T>::length()
{return count;}

//功能:返回第一个结点数据,时间复杂度为 O(1)
template<class T>
T linkdlist<T>::front()
{
	assert(last!=NULL);
	return first->info;
}

//功能:返回第一个结点数据,时间复杂度为 O(1)
template<class T>
T linkdlist<T>::back()
{
	assert(last!=NULL);
	return last->info;
}
//功能:检查给定数据是存在于链表中,时间复杂度为 O(n)
template<class T>
bool linkdlist<T>::search(const T& searchitem)
{
	nodetype<T> *current;
	bool found;
	found=false;
	current=first;
	while(current!=NULL && !found)
	{
		if (current->info==searchitem)
			found=true;
		else
			current=current->link;
	}
	return found;
}

//功能:插入新项到链表头部,时间复杂度为 O(1)
template<class T>
void linkdlist<T>::insertfirst(const T& newitem)
{
	nodetype<T> *newnode;
	newnode=new nodetype<T>;
	assert(newnode!=NULL);
	newnode->info=newitem;
	newnode->link=first;
	count++;
	if (last==NULL)
		last=NULL;
}
//功能:插入新项到链表尾部,时间复杂度为 O(1)
template<class T>
void linkdlist<T>::insertlast(const T& newitem)
{
	nodetype<T> *newnode;
	newnode=new nodetype<T>;
	assert(newnode!=NULL);
	newnode->info=newitem;
	newnode->link=first;

	if (last==NULL)
	{
		first=newnode;
		last=newnode;
		count++;
	}    
	else
	{
		last=newnode;
		last->link=newnode;
		count++;
	}
}

//功能:从链表中删除一个结点 O(n)
template<class T>
void linkdlist<T>::deletenode(const T& deleteitem)
{
	nodetype<T> *current,*trailcurrent;
	bool found;
	if (first!=NULL)            //case1:是一个空链表
		cout<<"cannot delete a empty list/n";
	else
	{
		if(first->info==deleteitem)//case2:
		{
			current=first;
			first=first->link;
			//delete current;
			count--;
			if (first==NULL)
			{
				last=NULL;
				delete current;
			}
			else
			{
				found=false;
				trailcurrent=first;
				current=first->link;
				while(current!=NULL && !found)
				{
					if(current->info!=deleteitem)
					{
						trailcurrent=current;
						current=current->link;
					}
					else
						found=true;    
				}//end while
				if (found)
				{
					trailcurrent->link=current->link;
					count--;
					if (last==current)
						last=trailcurrent;
					delete current;
				}
				else
					cout<<"item is not be in list./n";

			}//end else
		}//end else
	}
}
//功能:复制链表数据到另一个链表 O(n)
template<class T>
void linkdlist<T>::copylist(const linkdlist<T>&otherlist)
{
	nodetype<T> *current,*newnode;
	if(first!=NULL)
		destroylist();
	if(otherlist.first==NULL)
	{
		first=NULL;
		last=NULL;
		count=0;
	}
	else
	{
		current    =otherlist.first;
		count    =otherlist.count;
		//copy first node
		first=new nodetype<T>;
		assert(first!=NULL);
		first->info=current->info;
		first->link=NULL;
		last=first;
		current=current->link;
		//copy 所有结点
		while(current!=NULL)
		{
			newnode =new nodetype<T>;
			assert(newnode!=NULL);
			newnode->info=current->info;
			newnode->link=NULL;
			last->link=newnode;
			last=newnode;
			current=current->link;
		}//end while    
	}//end else
}//end copylist

//功能:析构函数
//前置:NULL;调用copylist();
template<class T>
linkdlist<T>::~linkdlist()
{
	destroylist();

}

//功能:复制构造函数
//前置:NULL;调用destroylist();
template<class T>
linkdlist<T>::linkdlist(const linkdlist<T>& otherlist)
{
	first=NULL;
	copylist(otherlist);
}

//功能:重载赋值运算=
//前置:NULL;调用destroylist();
template<class T>
const linkdlist<T>& linkdlist<T>::operator=(const linkdlist<T>& otherlist)
{
	if(this!=&otherlist)
		copylist(otherlist);
	return *this;
}

template<class T>
void linkdlist<T>::print()
{
	nodetype<T> *current;
	current=first;
	while(current!=NULL)
	{
		cout<<"current->info="<<current->info<<"/n";
		current=current->link;
	}    
	cout<<"/n length="<<count;
}
//end all code