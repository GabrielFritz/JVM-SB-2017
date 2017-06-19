#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "classfile.h"
#include "heap.h"
#include "frame.h"

int operandheap_isempty(operand_heap* top) { return top==NULL;  }

void push_op(operand_heap **top,operand_type op,int op_type)
{
    operand_heap* aux = (operand_heap*)malloc(sizeof(operand_heap));
    aux->op = op;
    aux->type = op_type;
    aux->below = *top;
}

void init_opheap(operand_heap** top) {*top = NULL;}

operand_type pop_op(operand_heap** top)
{
    operand_type op;
    if(!operandheap_isempty(*top)) {
        operand_heap* aux = *top;
        op = (*top)->op;
        *top = (*top)->below;
        free(aux);
    }
    else {
        printf("ERRO. OperandHeap vazia.\n");
        exit(1);
    }
    return op;
}

int framestack_isempty(frame* f){ return f==NULL; }

void framestack_init(frame** s) { *s = NULL; }

void push_frame(frame** s)
{
    frame* aux= (frame*)malloc(sizeof(frame));
    aux->below = *s;
    *s = aux;
}

void pop_frame(frame** s)
{
    if(!framestack_isempty(*s)) {
        frame* aux = *s;
        *s = (*s)->below;
        free(aux);
    }
    else {
        printf("ERRO: StackFrame Vazia.\n");
        exit(1);
    }
}

void frame_init(class_heap* start, ClassFile cf,frame* frame, char* method_name, char* descriptor)
{
    ClassFile* cf_aux = &cf;
    method_info* method = NULL;
    attribute_info code;
    u2 super_index = cf_aux->super_class;
    char super_name[128];

    while(cf_aux!=NULL
    && (method=search_method(cf_aux,method_name,descriptor))==NULL
    && super_index != 0)
    {
        strcpy(super_name,
        search_utf8(cf_aux->constant_pool,
        cf_aux->constant_pool[super_index].info.Class_info.name_index));

        cf_aux = search_classheap(start,super_name);
        if(cf_aux != NULL) super_index = cf_aux->super_class;
    }
    if(!method) {
        printf("ERRO. Method Inexistente: %s %s\n",method_name,descriptor);
        attribute_info* att_aux;
        for(att_aux = method->attributes; att_aux<method->attributes+method->attributes_count;++att_aux)
        {
            if(!strcmp(search_utf8(cf_aux->constant_pool,
                att_aux->attribute_name_index),"Code"))
            {
                code = *att_aux;
                break;
            }
        }
    }

    frame->constant_pool = cf_aux->constant_pool;
    init_opheap(&(frame->top));
    frame->code = code.type.Code.code;
    frame->local_arr = (operand_type*)malloc(sizeof(operand_type)*code.type.Code.max_locals);
    frame->pc = frame->code;
}

u1 u1ReadFrame(frame* f) { return *(++f->pc); }

i1 signed1ReadFrame(frame* f) { return *(++f->pc); }

u2 u2ReadFrame(frame* f) {
    u2 aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}

u4 u4ReadFrame(frame* f) {
    u4 aux;
    aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}