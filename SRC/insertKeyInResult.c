#include "def.h"

#define DEBUG

void insertKeyInResult(char **result[], char *key, int k) {
	int i, len = 0;

	for (i=0; i<k && (*result)[len][0]!=0; i++)
		len++;

	#ifdef DEBUG
		printf("\tTEST LEN: %d\n", len);
		for (i=0; i<len; i++)
			printf("*** %s\n", (*result)[i]);
	#endif

	if (len==0) {
		(*result)[len] = key;
	}
	else if (len<k) {
		(*result)[len] = key;
	}
	else if (len==k) {
		for (i=0; i<k-1; i++)
			(*result)[i] = (char *)(*result)[i+1];
		(*result)[k-1] = (char *)key;
	}
	else {
		printf("*** ERROR *** index outof bound\n");
	}
}