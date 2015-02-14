#include "def.h"

#define STACK_MAX 10

struct Stack {
    PAGENO     data[STACK_MAX];
    int        size;
};
typedef struct Stack Stack;


void stack_init(Stack *S) {
    S = (Stack *)calloc(10, sizeof(Stack));
    S->size = 0;
}

void stack_push(Stack *S, PAGENO p) {
    if (S->size < STACK_MAX)
        S->data[S->size++] = p;
    else {
        S = (Stack *)realloc(S, 2 * S->size);
        S->data[S->size++] = p;
    }
}

PAGENO stack_top(Stack *S) {
    if (S->size == 0) {
        printf("\t*** Error *** stack empty\n");
        return -1;
    } 
    return S->data[S->size-1];
}

PAGENO stack_pop(Stack *S) {
    if (S->size == 0)
        printf("\t*** Error *** stack empty\n");
    else {
    	PAGENO PageNo = S->data[S->size-1];
    	S->data[S->size-1] = -1;
        S->size--;
        return PageNo;
    }
}