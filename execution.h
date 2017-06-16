#ifndef EXEC_H
#define EXEC_H

#include "classfile.h"
#include "frame.h"
#include "heap.h"

typedef struct {
    frame* frame;
    class_heap* start;
} execution;

#endif