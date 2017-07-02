#ifndef FRAME_H
#define FRAME_H

#include "types.h"
#include "classfile.h"
#include "heap.h"
#include "util.h"

/*!
 * Armazena dados de um operando, dependendo de seu tipo.
 * */
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

/*!
 * Pilha de operandos LIFO de um frame. Armazena o valor de constantes e de
 * variaveis locais ou fields.
 * */
typedef struct _operand_heap {
    operand_type op;
    int type;
    struct _operand_heap* below;
} operand_heap;

/*!
 * Estrutura utilizada para armazenar dados e resultados parciais
 * */
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

u1 u1ReadFrame(frame* f);
i1 signed1ReadFrame(frame* f);
u2 u2ReadFrame(frame* f);
u4 u4ReadFrame(frame* f);
#endif
