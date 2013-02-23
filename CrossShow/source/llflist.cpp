#include "../include/llflist.h"
#include<list>

template<class T>
bool linkdlist<T>::isempty()
{return (first==NULL);}

//ȱʡ�Ĺ��캯��,ʱ�临�Ӷ�Ϊ O(1)
template<class T>
linkdlist<T>::linkdlist()
{
	first=NUll;
	last=NULL;
	count=0;
}

//ȱʡ�Ĺ��캯��,ʱ�临�Ӷ�Ϊ O(n)
template<class T>
void linkdlist<T>::destroylist()
{
	nodetype<T> *temp;
	while(first!=NULL)
	{    //ɾ����������������׽����β������ɾ��
		temp=first;            //���׽�������ʱ���temp    
		first=first->link;    //�׽��ָ����һ�����,�׽�����������
		delete temp;        //ɾ���ղ�ָ���׽���temp
		temp=NULL;            //��tempָ��Ϊ�գ��Է�ֹ�ڴ���ʳ���
	}
	//��β�����NULL���������Ϊ0
	first=NULL;
	last=NULL;
	count=0;
}

//��ʼ������,ʱ�临�Ӷ�Ϊ O(n)
template<class T>
void linkdlist<T>::initlist()
{destroylist();}

//����:�õ�����ĳ���,ʱ�临�Ӷ�Ϊ O(1)
template<class T>
int linkdlist<T>::length()
{return count;}

//����:���ص�һ���������,ʱ�临�Ӷ�Ϊ O(1)
template<class T>
T linkdlist<T>::front()
{
	assert(last!=NULL);
	return first->info;
}

//����:���ص�һ���������,ʱ�临�Ӷ�Ϊ O(1)
template<class T>
T linkdlist<T>::back()
{
	assert(last!=NULL);
	return last->info;
}
//����:�����������Ǵ�����������,ʱ�临�Ӷ�Ϊ O(n)
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

//����:�����������ͷ��,ʱ�临�Ӷ�Ϊ O(1)
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
//����:�����������β��,ʱ�临�Ӷ�Ϊ O(1)
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

//����:��������ɾ��һ����� O(n)
template<class T>
void linkdlist<T>::deletenode(const T& deleteitem)
{
	nodetype<T> *current,*trailcurrent;
	bool found;
	if (first!=NULL)            //case1:��һ��������
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
//����:�����������ݵ���һ������ O(n)
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
		//copy ���н��
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

//����:��������
//ǰ��:NULL;����copylist();
template<class T>
linkdlist<T>::~linkdlist()
{
	destroylist();

}

//����:���ƹ��캯��
//ǰ��:NULL;����destroylist();
template<class T>
linkdlist<T>::linkdlist(const linkdlist<T>& otherlist)
{
	first=NULL;
	copylist(otherlist);
}

//����:���ظ�ֵ����=
//ǰ��:NULL;����destroylist();
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