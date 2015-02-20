#include <stdio.h>
#include <string.h>
#include "def.h"

extern int CompareKeys(char *Key, char *Word);
extern struct PageHdr *FetchPage(PAGENO Page);
extern void backtrace(struct PageHdr *PagePtr, struct KeyRecord *KeyRcPtr, char *result[], int *cur, int *k, char *key);

int get_predecessors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("k should be positive not -1");
	    return -1;
    }
	int i, cur=0;

	if (result == NULL) {
		result = (char **)malloc(sizeof(char) * 10 * k);

		for (i=0; i<k; i++)
    		result[i] = (char *)calloc(10, sizeof(char));
	}

	/** deal with keys in same page **/
	struct PageHdr *PagePtr = FetchPage(ROOT);

	#ifdef DEBUG
	printf("content of this page...\n");
	while(KeyRcPtr!=NULL) {
		printf("%s ", KeyRcPtr->StoredKey);
		KeyRcPtr = KeyRcPtr->Next;
	}
	printf("\n\t-----------------------\n\n");
	#endif

	backtrace(PagePtr, PagePtr->KeyListPtr, result, &cur, &k, key);

	/** print out results **/
	printf("found %d predecessors:\n", cur);

	for (i=cur-1; i>=0; i--) {
		printf("%s\n", result[i]);
	}

	free(result);

    return 0;
}
