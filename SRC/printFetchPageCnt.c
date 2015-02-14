/************************************************************************
 *                                                                      *
 *     This function prints and resets the numer of page fetches        *
 *     in current program. i.e. the numer of FetchPage funcitons        *
 *     called.											                *
 *                                                                      *
 ************************************************************************/

#include "def.h"
extern int fetchPage_cnt;

void printFetchPageCnt() {
	if (fetchPage_cnt==0) {
		printf("NO fetch_page executed: 0\n");
		return;
	}
	printf("FetchPage function has been called %d times", fetchPage_cnt);
	fetchPage_cnt = 0;
	return;
}
