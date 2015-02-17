#include <stdio.h>
#include <string.h>
#include "def.h"

extern List *List_create(void);
extern PAGENO backtrace(PAGENO PageNo, char *key, struct List *list);

extern void List_print(List *list);

int get_predecessors(char *key, int k, char *result[]) {
    if (k<1) {
		printf("param k start from 1\n");
	    return -1;
    }
	int i;

	if (result == NULL) {
		result = (char **)malloc(sizeof(char) * 10 * k);

		for (i=0; i<k; i++)
    		result[i] = (char *)calloc(10, sizeof(char));
	}

	struct List *list = List_create();
	assert(list && "*** NULL LIST ***");
	
	PAGENO PageNo = backtrace(ROOT, key, list);
	assert(PageNo > 0 && "PageNO = 0?!");

	free(result);

    return 0;
}
