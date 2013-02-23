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
//ADT������ģ���ܶ���
template<class T>
class linkdlist
{
	friend ostream& operator<<(ostream&,const linkdlist<T>&);
	//��������������н��
public:

	const linkdlist<T>& operator=(const linkdlist<T>&);
	//����=
	void initlist();
	//����:��ʼ������
	//ǰ��:NULL
	//����:    first=NUll;last=NULL;count=0;
	bool isempty();
	//����:����Ƿ�Ϊ��
	//ǰ��:NULL
	//����:    �����return TRUE,����return FALSE;
	int     length();
	//����:�õ�����ĳ���
	//ǰ��:����������
	//����:    ���������count
	void destroylist();
	//����:ɾ�����������еĽ��
	//ǰ��:����������
	//����:first=NUll;last=NULL;count=0;
	T front();
	//����:���ص�һ���������
	//ǰ��:���������ڲ��Ҳ����ǿձ�
	//����:������Ϊ���������ֹ�����򷵻ص�һ���������

	T back();
	//����:���ص�һ���������
	//ǰ��:���������ڲ��Ҳ����ǿձ�
	//����:������Ϊ���������ֹ�����򷵻ص�һ���������
	bool search(const T& searchitem);
	//����:�����������Ǵ�����������
	//ǰ��:���������ڲ��Ҳ����ǿձ�
	//����:���ھͷ���TRUE�������ھͷ���FALSE

	void insertfirst(const T& newitem);
	//����:�����������ͷ��
	//ǰ��:����������
	//����:����β�������ָ������δβ��count��1

	void insertlast(const T& newitem);
	//����:�����������β��
	//ǰ��:����������
	//����:����β�������ָ������δβ��count��1
	void deletenode(const T& deleteitem);
	//����:��������ɾ��һ�����
	//ǰ��:����������
	//����:����search()����Ƿ����?,������ɾ����
	//����β�������ָ������δβ��count��1
	linkdlist();
	//����:����ģ���ȱʡ���캯��
	//ǰ��:��ʼ��һ����������initlist();
	//����:first=NUll;last=NULL;count=0;

	linkdlist(const linkdlist<T>&otherlist);
	//����:���ƹ��캯��

	~linkdlist();
	//����:��������
	//ǰ��:NULL;����destroylist();
	//����:���ڴ��е��������еĽ��ɾ��
	void print();
	//����:��ӡ���н������
	//ǰ��:NULL
	//����:���������еĽ���ӡ����        

protected:
	int count;                //������
	nodetype<T> *first;        //�����׽��
	nodetype<T> *last;        //����β���

private:
	void copylist(const linkdlist<T>&otherlist);
	//����:�����������ݵ���һ������
	//ǰ��:���������ڲ��Ҳ����ǿձ�
	//����:��this����ȥ������һ���������
};

//��Ԫ����ʵ��
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