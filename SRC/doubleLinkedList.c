#include "def.h"

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

PAGENO List_remove(List *list, ListNode *node);

List *List_create() {
    List *list =  calloc(1, sizeof(List));
    assert(list && "*** NULL LIST ***");
    return list;
}

void List_destroy(List *list) {
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev)
            free(cur->prev);
    }
    free(list->last);
    free(list);
}

void List_push(List *list, PAGENO value) {
    ListNode *node = calloc(1, sizeof(ListNode));

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } 
    else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

    return;
}

PAGENO List_pop(List *list) {
    ListNode *node = list->last;
    if (node==NULL) {
        printf("\t*** Error: stack empty ***\n");
        return -1;
    }
    return List_remove(list, node);
}

PAGENO List_remove(List *list, ListNode *node) {
    PAGENO result;

    assert(list->first && list->last && "LIST IS EMPTY");
    assert(node && "NODE CANNOT BE NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        assert(list->first != NULL && "INVALID LIST! FIRST IS NULL");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        assert(list->last != NULL && "INVALID LIST! NEXT IS NULL");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

    return result;
}

void List_print(List *list) {
    printf("\n\t*** list result:\n\n");
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            printf("%lu ", cur->value);
            cur = cur->prev;
        }
    }
}
