#ifndef FRAME_H
#define FRAME_H

#include "constantpool.h"

typedef union _operand_type {
    int Int;
    long long Long;
    float Float;
    double Double;
    void* Ref;
} operand_type;

typedef struct _vector {
    int size,type;
    operand_type* array;
} vector;

typedef struct _operand_heap {
    operand_type op;
    int type;
    struct _operand_type;
    struct _operand_heap* below;
} operand_heap;

typedef struct _frame {
    cp_info* constant_pool;
    operand_type* local_arr;
    operand_heap* top;
    u1* code;
    struct _frame *below;
    u1* pc;
} frame;

int operandheap_isempty(operand_heap* top);
void push_op(operand_heap **top,operand_type op,int op_type);
void init_opheap(operand_heap** top);
operand_type pop_op(operand_heap** top);

int framestack_isempty(frame* f);
void framestack_init(frame** s);
void push_frame(frame** s);
void pop_frame(frame** s);
void frame_init(class_heap* start, ClassFile cf,frame* frame, char* method_name, char* descriptor);

#endif
