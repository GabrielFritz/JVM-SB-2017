#ifndef FRAME_H
#define FRAME_H

#include "classloader.h"

typedef struct {
    int type;
    union {
        int int_type;
        long long long_type;
        float float_type;
        double double_type;
        void* reference_type;
        char byte_type;
        u1 char_type;
        short short_type;
    } op_type;
} operand;

typedef struct {
    int array_size;
    int type;
    operand* arr;
} Array;

typedef struct {
    cp_info* class_belong;
    u1 *curr;
    u1* pc;
} execution_env;

typedef struct _op_stack{
    operand op;
    struct _op_stack* next;
} operand_stack;

typedef struct {
    u4* local_var;
    execution_env exec_env;
    operand_stack* top;
} Frame;

typedef struct {
    Frame frame;
    struct stack_frame* next;
} stack_frame;

typedef struct {
    char* nome;
    char* descricao;
    union {
        int int_type;
        short short_type;
        float float_type;
        long long long_type;
        double double_type;
        char char_type;
        void* reference_type;
    } type;
} field;

typedef struct {
    int field_count;
    field* fields;
} object;

typedef struct _object_list {
    object obj;
    struct _object_list* next;
} object_list;

typedef struct _class_list{
    ClassFile* classFile;
    int fields_count;
    field* static_fields;
    object_list* objects;
    struct _class_list *next;
} class_list;

#endif