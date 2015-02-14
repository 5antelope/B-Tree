#include <stdio.h>
#include <string.h>
#include "def.h"

extern struct PageHdr *FetchPage(PAGENO Page);
extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern void insertKeyInResult(char **result[], char *key, int k);
extern int CompareKeys(char *Key, char *Word);

#define DEBUG

int get_predecessors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("param k start from 1\n");
	    return -1;
    }

    int i, j, len=0;

    if (result == NULL) {
	    result = (char **)malloc(sizeof(char) * 10 * k);
	    for (i=0; i<k; i++)
			result[i] = (char *)calloc(10, sizeof(char));
	}
 	
 	PAGENO PageNo = ROOT;
	struct PageHdr *PagePtr = FetchPage(PageNo);
	struct KeyRecord *KeyPtr;

	while (PagePtr->PgTypeID == 'N') {
		KeyPtr = PagePtr->KeyListPtr;
		PageNo = KeyPtr->PgNum;
		PagePtr = FetchPage(PageNo);
	}

	KeyPtr = PagePtr->KeyListPtr;
	while (CompareKeys(KeyPtr->StoredKey, key)!=0) {
	#ifdef DEBUG
		printf(">>>>>>>>>>> check point 1 <<<<<<<<<\n");
	#endif
		insertKeyInResult(&result, KeyPtr->StoredKey, k);
	#ifdef DEBUG
		printf(">>>>>>>>>>> check point 2 <<<<<<<<<\n");
	#endif
		KeyPtr = KeyPtr->Next;
	#ifdef DEBUG
		if (KeyPtr==NULL) {printf("FUCK!!!!\n"); break;}
		printf(">>>>>>>>>>> check point 3 <<<<<<<<<\n");
	#endif
		len = len<k?len+1:k;
	}
#ifdef DEBUG
	printf(">>>>>>>>>>> check point 4 <<<<<<<<<\n");
#endif	
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

	printf("\n*** predecessors of %s (%d) ***\n\t", key, k);

	for (i=0; i<len; i++)
		printf("%s ", result[i]);

	printf("\n");

    free(result);

    return 0;
}
