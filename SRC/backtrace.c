#include "def.h"

extern PAGENO List_pop(List *list);
extern void List_push(List *list, PAGENO value);
extern struct PageHdr *FetchPage(PAGENO Page);

void backtrace_helper(struct PageHdr *PagePtr, char *result[], int *cur, int *k);

void backtrace(struct List *list, char *result[], int *cur, int *k) {
	if (k==0 || list->count==0)
		return;
	PAGENO PageNo = List_pop(list);
	struct PageHdr *PagePtr = FetchPage(PageNo);
	if (IsLeaf(PagePtr))
        backtrace_helper(PagePtr, result, cur, k);
    else {
    	struct KeyRecord *KeyRcPtr = PagePtr->KeyListPtr;
    	while (KeyRcPtr!=NULL) {
    		List_push(list, KeyRcPtr->PgNum);
    		KeyRcPtr = KeyRcPtr->Next;
    	}
    }
    backtrace(list, result, cur, k);
}

void backtrace_helper(struct PageHdr *PagePtr, char *result[], int *cur, int *k) {
	struct KeyRecord *KeyRcPtr = PagePtr->KeyListPtr;

	assert(KeyRcPtr!=NULL);
	NUMKEYS NumKeys = PagePtr->NumKeys;

	while (NumKeys>0 && (*k)>0) {
		struct KeyRecord *RcPtr = KeyRcPtr;
		int t = NumKeys;
		while (t>1) {
			RcPtr = RcPtr->Next;
			t--;
		}
		result[*cur] = (char *)RcPtr->StoredKey;
		(*cur)++;
		(*k)--;
		NumKeys--;
	}
}
