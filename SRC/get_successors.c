#include "def.h"

extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int CompareKeys(char *Key, char *Word);

int get_successors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("param k start from 1\n");
	    return -1;
    }

	int i, j, len=0;
	PAGENO PageNo = treesearch_page(ROOT, key);
	struct PageHdr *PagePtr = FetchPage(PageNo);
	struct KeyRecord *KeyRcPtr = PagePtr->KeyListPtr;
	printf("*** pageNo found:%lu\n", PageNo);
	if (result == NULL) {
		result = (char **)malloc(sizeof(char) * 10 * k);

		for (i=0; i<k; i++)
    		result[i] = (char *)malloc(sizeof(char) * 10);
	}

	if (PageNo == 0) {
		printf("\n*** ERROR *** not fount key\n");
		return -1;
	}
	printf("\n*** Checkpoint 1\n");
	while (CompareKeys(KeyRcPtr->StoredKey,key)!=0) {
		printf("** test: %s\n", KeyRcPtr->StoredKey);
		KeyRcPtr = KeyRcPtr->Next;
	}

	KeyRcPtr = KeyRcPtr->Next; 
	printf("\n*** Checkpoint 2\n");
	for (i=0; i<k && PagePtr!=NULL; i++) {
		if (KeyRcPtr == NULL) {
			PagePtr = FetchPage(PagePtr->PgNumOfNxtLfPg);
			if (PagePtr == NULL)
				break;
			KeyRcPtr = PagePtr->KeyListPtr;
		}
		result[i] = (char *)KeyRcPtr->StoredKey;
		KeyRcPtr = KeyRcPtr->Next;
		len++;
	}
	printf("\n*** Checkpoint 3\n");
	/* sort the result array */
	for (i=0; i<len; i++) {
		for (j = i+1; j<len; j++) {
			if (CompareKeys(result[i], result[j])==2) {
				char *tmp = result[i];
				result[i] = result[j];
				result[j] = tmp;
			}
		}
	}

	printf("\n*** successors of %s (%d) ***\n\t", key, k);

	for (i=0; len>0; len--, i++)
		printf("%s ",result[i]);

	printf("\n");

	free(result);

    return 0;
}
