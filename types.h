#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <inttypes.h>
#include "frame.h"

#define BOOLEANTYPE   4
#define CHARTYPE      5
#define FLOATTYPE         6
#define DOUBLETYPE    7
#define BYTETYPE        8
#define SHORTTYPE         9
#define INTEGERTYPE          10
#define LONGTYPE       11

#define CONTINUED 0
#define CLASS 7
#define FIELDREF 9
#define METHOD 10
#define INTERFACE 11
#define NAMEANDTYPE 12
#define UTF8 1
#define STRING 8
#define INTEGER 3
#define FLOAT 4
#define LONG 5
#define DOUBLE 6
#define CONSTANTVALUE 5
#define CODE 1
#define EXCEPTIONS 2
#define INNERCLASSES 3
#define OTHER 4

typedef uint8_t     u1;
typedef uint16_t    u2;
typedef uint32_t    u4;
typedef int8_t      i1;

u1 u1Read(FILE* fd);
u2 u2Read(FILE* fd);
u4 u4Read(FILE* fd);
u1 u1ReadFrame(frame* f);
i1 signed1ReadFrame(frame* f);


union {
    u4 U4;
    float Float;
} u4tofloat;

union {
    long long Long;
    double Double;
} longtodouble;
#endif