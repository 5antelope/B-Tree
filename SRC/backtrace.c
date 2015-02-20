#include "def.h"

extern struct PageHdr *FetchPage(PAGENO Page);
extern void FreePage(struct PageHdr *PagePtr);
extern int CompareKeys(char *Key, char *Word);

// #define DEBUG

void backtrace(struct PageHdr *PagePtr, struct KeyRecord *KeyRcPtr, char *result[], int *cur, int *k, char *key) {

	if (*k==0)
		return;

	if (IsLeaf(PagePtr)) {
		if (KeyRcPtr==NULL) {
			return;
		}
		if (strcmp(key, KeyRcPtr->StoredKey)==0) {
			#ifdef DEBUG
			printf("\t*** FOUND!\n");
			#endif
			return;
		}
		else if (strcmp(key, KeyRcPtr->StoredKey)>0 ) {
			#ifdef DEBUG
			printf("\t*** CHECKX %s \n",KeyRcPtr->StoredKey);
			#endif
			backtrace(PagePtr, KeyRcPtr->Next, result, cur, k, key);
			if (*k==0)
				return;
			result[*cur] = (char *)KeyRcPtr->StoredKey;
			(*cur)++;
			(*k)--;
		}
		else if (strcmp(key, KeyRcPtr->StoredKey)<0) {
			#ifdef DEBUG
			printf("\t*** EXCEED!\n");
			#endif
			return;
		}
	}
	else {
		struct PageHdr *_PagePtr = NULL;
		if (strcmp(key, KeyRcPtr->StoredKey)<0) {	// exceed
			_PagePtr = FetchPage(KeyRcPtr->PgNum);
			assert(_PagePtr!=NULL);
					backtrace(_PagePtr, _PagePtr->KeyListPtr, result, cur, k, key);
		}
		else if (strcmp(key, KeyRcPtr->StoredKey)>=0 && KeyRcPtr->Next!=NULL) {
			backtrace(PagePtr, KeyRcPtr->Next, result, cur, k, key);
			if (*k==0)	return;
			_PagePtr = FetchPage(KeyRcPtr->PgNum);
			 assert(_PagePtr!=NULL);
			 		backtrace(_PagePtr, _PagePtr->KeyListPtr, result, cur, k, key);
		}
		else if (strcmp(key, KeyRcPtr->StoredKey)>=0 && KeyRcPtr->Next==NULL) {

			_PagePtr = FetchPage(PagePtr->PtrToFinalRtgPg);

			assert(_PagePtr!=NULL);
					
			backtrace(_PagePtr, _PagePtr->KeyListPtr, result, cur, k, key);
			_PagePtr = FetchPage(KeyRcPtr->PgNum);
			backtrace(_PagePtr, _PagePtr->KeyListPtr, result, cur, k, key);						
		}
		if (*k==0)	return;
	}

	return;
}
