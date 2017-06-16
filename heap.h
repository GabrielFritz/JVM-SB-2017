#ifndef HEAP_H
#define HEAP_H
#include "classfile.h"

typedef union {
    int Int;
    short Short;
    float Float;
    long long Long;
    double Double;
    char Char;
    void* Ref;
} field_type;

typedef struct {
    char* nome;
    char* descriptor;
    field_type value;
} field;

typedef struct {
    int num_fields;
    field* fields;
} object;

typedef struct _obj_heap {
    object obj;
    struct _obj_heap* next;
} obj_heap;

typedef struct _class_heap {
    ClassFile cf;
    int num_static;
    field* static_fields;
    obj_heap* objects;
    struct _class_heap* next;
} class_heap;

#endif
