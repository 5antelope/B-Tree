#include "def.h"

extern void FreePage(struct PageHdr *PagePtr);
extern void List_push(List *list, PAGENO value);
extern struct PageHdr *FetchPage(PAGENO Page);

PAGENO helper(struct PageHdr *PagePtr, struct KeyRecord *KeyListTraverser, char *Key,
	NUMKEYS NumKeys, struct List *list);

PAGENO backtrace(PAGENO PageNo, char *key, struct List *list) {
	PAGENO result;
	struct PageHdr *PagePtr = FetchPage(PageNo);
	if (IsLeaf(PagePtr))
		result = PageNo;
	else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys > 0)) {
		PAGENO ChildPage = helper(PagePtr, PagePtr->KeyListPtr,
			key, PagePtr->NumKeys, list);
		List_push(list, ChildPage);
		result = backtrace(ChildPage, key, list);
	} else {
		assert(0 && "*** WTF! ***");
	}
	List_push(list, result);
	FreePage(PagePtr);
	return result;
}

PAGENO helper(struct PageHdr *PagePtr, struct KeyRecord *KeyListTraverser, char *Key,
	NUMKEYS NumKeys, struct List *list) {
	int Result;
	char *Word;
	int CompareKeys(char *Key, char *Word);

	Word = KeyListTraverser->StoredKey;
    (*(Word + KeyListTraverser->KeyLen)) = '\0';
    Result = CompareKeys(Key, Word);

    NumKeys = NumKeys - 1;

	if (NumKeys > 0) {
        if (Result == 2) { /* New key > stored key:  keep searching */
			List_push(list, KeyListTraverser->PgNum);
            KeyListTraverser = KeyListTraverser->Next;
            return (
                helper(PagePtr, KeyListTraverser, Key, NumKeys, list));
        } else                                /* New key <= stored key */
            return (KeyListTraverser->PgNum); /* return left child */
    } else /* This is the last key in this page */
    {
        if ((Result == 1) || (Result == 0))    /* New key <= stored key */
            return (KeyListTraverser->PgNum);  /* return left child */
        else                                   /* New key > stored key */
            return (PagePtr->PtrToFinalRtgPg); /* return rightmost child */
    }    
}
