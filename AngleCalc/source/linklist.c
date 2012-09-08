#include <stdlib.h>

#include "linklist.h"


SList* slist_append(SList *list, void *data)
{
	SList *new_list = NULL;
	SList *last = NULL;
	
	new_list = (SList*)malloc(sizeof(SList));
	new_list->data = data;
	new_list->next = NULL;
	
	if(list == NULL){
		return new_list;
	}else{
		last = slist_last(list);
		last->next = new_list;
		return list;
	}
}

SList* slist_append_int(SList *list, int value)
{
	SList *new_list = NULL;
	SList *last = NULL;

	new_list = (SList*)malloc(sizeof(SList));
	new_list->value = value;
	new_list->next = NULL;

	if(list == NULL){
		return new_list;
	}else{
		last = slist_last(list);
		last->next = new_list;
		return list;
	}
}

SList* slist_last(SList *list)
{
	if(list){
		while(list->next)
			list = list->next;
	}
	
	return list;
}

SList* slist_prepend(SList *list,void *data)
{
	SList *new_list = NULL;
	
	new_list = (SList *)malloc(sizeof(SList));
	new_list->data = data;
	new_list->next = list;
	
	return new_list;
}

SList* slist_delete_last(SList *list)
{
	/*********this function just for data=malloc or new ...,not for内建立数据类型指针，如int ****************/
	SList *tmp, *prev = NULL;
	
	tmp = list;
	while(tmp){
		if(tmp->next==NULL)
		{
			if(prev){
				prev->next = NULL;
			}
			else{
				list = tmp->next;
			}
			free(tmp->data);
			free(tmp);
			break;
		}
		prev = tmp;
		tmp = prev->next;
	}
	
	return list;
}
SList* slist_delete_last_simple(SList *list)
{
	/*********this function just for simple 内建立数据类型指针，如int ****************/
	SList *tmp, *prev = NULL;
	
	tmp = list;
	while(tmp){
		if(tmp->next==NULL)
		{
			if(prev){
				prev->next = NULL;
			}
			else{
				list = tmp->next;
			}
			free(tmp);
			break;
		}
		prev = tmp;
		tmp = prev->next;
	}
	
	return list;
}
SList* slist_delete(SList *list, void *data)
{
	SList *tmp, *prev = NULL;
	
	tmp = list;
	while(tmp){
		if(tmp->data == data){
			if(prev){
				prev->next = tmp->next;
			}
			else{
				list = tmp->next;
			}
			
			free(tmp);
			break;
		}
		prev = tmp;
		tmp = prev->next;
	}
	
	return list;
}

void slist_print(SList *list,SListDataPrintFunc fun)
{
	while(list){
		SList *next = list->next;
		fun(list->data);
		list = next;
	}
}


SList* slist_insert(SList *list, void *data,int position)
{
	SList *prev_list;
	SList *tmp_list;
	SList *new_list;
	
	if(position < 0){
		return slist_append(list,data);
	}else if(position == 0){
		return slist_prepend(list,data);
	}
	
	new_list = (SList*)malloc(sizeof(SList));
	new_list->data = data;
	
	if(NULL == list){
		new_list->next = NULL;
		return new_list;
	}
	
	prev_list = NULL;
	tmp_list = list;
	
	while ((position-- > 0) && tmp_list)
    {
      prev_list = tmp_list;
      tmp_list = tmp_list->next;
    }
	
	if (prev_list)
    {
      new_list->next = prev_list->next;
      prev_list->next = new_list;
    }
	else
    {
      new_list->next = list;
      list = new_list;
    }

	return list;
}

SList* slist_get_element(SList *list,int n)
{
	while(n-- > 0 && list){
		list = list->next;
	}
	
	return list;
}

SList* slist_find(SList *slist,SListDataCompareFunc func, void *ctx)
{
	while(slist){
		if(func(ctx,slist) == SLIST_RET_OK){
			return slist;
		}
		
		slist = slist->next;
	}
	
	return NULL;
}

void slist_foreach(SList *list,SListDataVisitFunc func, void *ctx)
{
	while (list)
    {
      SList *next = list->next;
      (*func)(ctx,list->data);
      list = next;
    }
}

void slist_destory(SList* list)
{
	while(list){
		SList *next = list->next;
		free(list);
		list = next;
	}
}

unsigned slist_length(SList *list)
{
	unsigned length=0;

	while(list){
		++length;
		list = list->next;
	}
	
	return length;
}
