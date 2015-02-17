/*
    searches the subtree starting at the node stored at the page
    with number
        PageNo
    for the
        key.
    It returns the offset from the postings file, or ERROR, if not found
 */

#include "def.h"

extern POSTINGSPTR searchLeaf(struct PageHdr *PagePtr, char *key);
extern int FreePage(struct PageHdr *PagePtr);
extern PAGENO FindPageNumOfChild_record(struct PageHdr *PagePtr,
                                 struct KeyRecord *KeyListTraverser, char *Key,
                                 NUMKEYS NumKeys, struct List *list);
extern PAGENO FindPageNumOfChild(struct PageHdr *PagePtr,
                                 struct KeyRecord *KeyListTraverser, char *Key,
                                 NUMKEYS NumKeys);
extern struct PageHdr *FetchPage(PAGENO Page);

extern void List_push(List *list, PAGENO value);
extern void List_print(List *list);

void treesearch_page_record(PAGENO PageNo, char *key, struct List *list) {
    struct PageHdr *PagePtr = FetchPage(PageNo);
    if (IsLeaf(PagePtr)) {
        return;
    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys == 0)) {
        treesearch_page_record(FIRSTLEAFPG, key, list);
    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys > 0)) {
        PAGENO ChildPage = FindPageNumOfChild_record(PagePtr, PagePtr->KeyListPtr, key,
                                              PagePtr->NumKeys, list);
        List_push(list, ChildPage);
        treesearch_page_record(ChildPage, key, list);
    } else {
        assert(0 && "this should never happen");
    }
    FreePage(PagePtr);
}

PAGENO treesearch_page(PAGENO PageNo, char *key) {
    PAGENO result;
    struct PageHdr *PagePtr = FetchPage(PageNo);
    if (IsLeaf(PagePtr)) { /* found leaf */
        result = PageNo;
    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys == 0)) {
        /* keys, if any, will be stored in Page# 2
           THESE PIECE OF CODE SHOULD GO soon! **/
        result = treesearch_page(FIRSTLEAFPG, key);
    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys > 0)) {
        PAGENO ChildPage = FindPageNumOfChild(PagePtr, PagePtr->KeyListPtr, key,
                                              PagePtr->NumKeys);
        result = treesearch_page(ChildPage, key);
    } else {
        assert(0 && "this should never happen");
    }
    FreePage(PagePtr);
    return result;
}

/**
 * find the posting pointer to which the key should reside, given the
 * starting page number to look at.
 *
 * to search the whole tree, pass in ROOT as the page number.
 */
POSTINGSPTR treesearch(PAGENO PageNo, char *key) {
    /* recursive call to find page number */
    const PAGENO page = treesearch_page(PageNo, key);
    /* from page number we traverse the leaf page */
    struct PageHdr *PagePtr = FetchPage(page);
    POSTINGSPTR result = searchLeaf(PagePtr, key);
    FreePage(PagePtr);
    return result;
}
