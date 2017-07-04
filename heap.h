#ifndef HEAP_H
#define HEAP_H
#include "classfile.h"
#include "util.h"
#include "types.h"

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
    char* name;
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

/*Contem a lista de classes que ja foram carregadas.*/
typedef struct _class_heap {
    ClassFile cf;
    int num_static;
    field* static_fields;
    obj_heap* objects;
    struct _class_heap* next;
} class_heap;

void classheap_init(class_heap** ch);
void free_classheap(class_heap* ch);
int is_static(u2 access_flag);
void push_class(class_heap** ch,ClassFile cf);
ClassFile* search_classheap(class_heap* ch, char* name);
field* search_field(char* name, char* descr, object* o);
field* search_staticfield(class_heap* start, char* class, char* field);
#endif
