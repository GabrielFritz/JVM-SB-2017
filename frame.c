#include "frame.h"

int isempty_opstack(operand_stack* s) {
    return s == NULL;
}

void operand_stack_init(operand_stack** s)
{
    *s = NULL;
}

void push_operand(operand_stack** s,operand op)
{
    operand_stack* aux;
    aux = (operand_stack*) malloc(sizeof(operand_stack));
    aux->op = op;
    aux->next = *s;
    *s = aux; 
}

operand pop_operand(operand_stack** s)
{
    operand_stack* aux;
    operand op;
    if(!isempty_opstack(*s)) {
        op = (*s)->op;
        aux=*s;
        *s = (*s)->next;
        free(aux);
    } else {
        printf("Operand Stack vazia.\n");
        exit(1);
    }
    return op;
}

int isempty_stackframe(stack_frame* s)
{
    return s == NULL
}

void stack_frame_init(stack_frame* s)
{
    *s = NULL;
}

void push_frame(stack_frame **s)
{
    stack_frame* aux;
    aux = (stack_frame*) malloc(sizeof(stack_frame));
    aux->next = *s;
    *s=aux;
}

void pop_frame(stack_frame **s)
{
    stack_frame* aux;
    if(!isempty_stackframe(*s)) {
        aux = *s;
        *s = (*s)->next;
        free(aux);
    } else {
        printf("Stack Frame vazia.\n");
        exit(0);
    }
}