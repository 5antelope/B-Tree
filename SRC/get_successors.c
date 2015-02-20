#include "def.h"

extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int CompareKeys(char *Key, char *Word);

int get_successors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("k should be positive not -1");
	    return -1;
    }

	int i, j, len=0;
	PAGENO PageNo = treesearch_page(ROOT, key);

	assert(PageNo > 0);

	struct PageHdr *PagePtr = FetchPage(PageNo);
	struct KeyRecord *KeyRcPtr = PagePtr->KeyListPtr;

	if (result == NULL) {
		result = (char **)malloc(sizeof(char) * 10 * k);

		for (i=0; i<k; i++)
    		result[i] = (char *)malloc(sizeof(char) * 10);
	}

	while (strcmp(key, KeyRcPtr->StoredKey)>0) {
		KeyRcPtr = KeyRcPtr->Next;
	}
	if (strcmp(key, KeyRcPtr->StoredKey)==0)
		KeyRcPtr = KeyRcPtr->Next; 
	
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

	printf("found %d successors:\n", len);

	for (i=0; len>0; len--, i++) {
		printf("%s\n",result[i]);
	}

	free(result);

    return 0;
}
