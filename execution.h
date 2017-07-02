#ifndef EXEC_H
#define EXEC_H

//#include "classfile.h"
#include "frame.h"
#include "heap.h"

#define MNAME "<clinit>"
#define MDESCR "()V"

typedef struct {
    frame* frame;
    class_heap* start;
} execution;

int count_args(char* d);
void execute_method(execution* e);
void init_methodexecution(execution* e,char* class,char* method, char* descriptor, int args);
ClassFile* check_class(execution* e, char* name);

#endif
