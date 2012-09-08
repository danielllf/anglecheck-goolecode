#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	SLIST_RET_OK = 0,
	SLIST_RET_OOM,
	SLIST_RET_STOP,
	SLIST_RET_PARAMS,
	SLIST_RET_FAIL
}SListRet;

typedef struct _SList SList;

struct _SList{
	int value;
	void *data;
	SList *next;
};

typedef void (*SListDataPrintFunc)(void *data);
typedef SListRet (*SListDataCompareFunc)(void *ctx,void *data);
typedef void (*SListDataVisitFunc)(void *ctx, void *data);

SList* slist_append(SList *list, void *data);
SList* slist_append_int(SList *list, int value);
SList* slist_prepend(SList *list,void *data);
SList* slist_delete(SList *list, void *data);
SList* slist_insert(SList *list, void *data,int position);
SList* slist_get_element(SList *list,int n);
SList* slist_delete_last(SList *list);
SList* slist_delete_last_simple(SList *list);
unsigned slist_length(SList *list);

void slist_print(SList *slist,SListDataPrintFunc fun);
SList* slist_find(SList *slist,SListDataCompareFunc func, void *ctx);
void slist_foreach(SList *slist,SListDataVisitFunc func, void *ctx);

SList *slist_last(SList *list);
void slist_destory(SList* thiz);

#ifdef __cplusplus
}
#endif

#endif
