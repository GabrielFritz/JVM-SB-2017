#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

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


#define CPUBLIC      0x0001  
#define CFINAL       0x0010  
#define CSUPER       0x0020  
#define CINTERFACE   0x0200  
#define CABSTRACT    0x0400
#define CSYNTHETIC   0x1000  
#define CANNOTATION  0x2000 
#define CENUM        0x4000  

#define FPRIVATE     0x0002  
#define FPROTECTED   0x0004  
#define FSTATIC      0x0008  
#define FVOLATILE    0x0040  
#define FTRANSIENT   0x0080  

#define MSYNCHRONIZED    0x0020  
#define MBRIDGE          0x0040  
#define MVARARGS         0x0080  
#define MNATIVE          0x0100  
#define MSTRICT          0x0800  

typedef uint8_t     u1;
typedef uint16_t    u2;
typedef uint32_t    u4;
typedef int8_t      i1;

u1 u1Read(FILE* fd);
u2 u2Read(FILE* fd);
u4 u4Read(FILE* fd);

union {
    u4 U4;
    float Float;
} u4tofloat;

union {
    long long Long;
    double Double;
} longtodouble;
#endif