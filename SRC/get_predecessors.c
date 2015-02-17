#include <stdio.h>
#include <string.h>
#include "def.h"

extern List *List_create(void);
extern int CompareKeys(char *Key, char *Word);
extern struct PageHdr *FetchPage(PAGENO Page);
extern void treesearch_page_record(PAGENO PageNo, char *key, struct List *list);
extern void backtrace(struct List *list, char *result[], int *cur, int *k);
extern void List_print(List *list);
extern PAGENO List_pop(List *list);
extern void List_destroy(List *list);

int get_predecessors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("param k start from 1\n");
	    return -1;
    }
	int i, cur, len=k;

	if (result == NULL) {
		result = (char **)malloc(sizeof(char) * 10 * k);

		for (i=0; i<k; i++)
    		result[i] = (char *)calloc(10, sizeof(char));
	}

	/** deal with keys in same page **/
	struct List *list = List_create();
	treesearch_page_record(ROOT, key, list);
	PAGENO PageNo = List_pop(list);
	struct PageHdr *PagePtr = FetchPage(PageNo);
	struct KeyRecord *KeyRcPtr = PagePtr->KeyListPtr;

#ifdef DEBUG
	printf("content of this page...\n");
	while(KeyRcPtr!=NULL) {
		printf("%s ", KeyRcPtr->StoredKey);
		KeyRcPtr = KeyRcPtr->Next;
	}
	printf("\n\t-----------------------\n\n");
#endif

	KeyRcPtr = PagePtr->KeyListPtr;
	char **tmp = (char **)malloc(sizeof(char) * 20 * 10);
	for (i=0; i<10; i++)
		tmp[i] = (char *)calloc(20, sizeof(char));
	
	for (i=0; CompareKeys(KeyRcPtr->StoredKey,key)!=0; i++) {
		tmp[i] = (char *)KeyRcPtr->StoredKey;
		KeyRcPtr = KeyRcPtr->Next;
	}
	i--;
	for (cur=0; i>=0 && k>0; i--, cur++) {
		assert(tmp[i]!=0);
		result[cur] = tmp[i];
		k--;
	}
	
	free(tmp);

	/** deal with left **/
	if (k>=1)
		backtrace(list, result, &cur, &k);

	/** print out results **/
	for (i=len-k; i>0; i--) 
		printf("%s ", result[i-1]);

	free(result);
	List_destroy(list);

    return 0;
}
