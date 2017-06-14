#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


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

u1 u1Read(FILE* fd);
u2 u2Read(FILE* fd);
u4 u4Read(FILE* fd);

union {
    u4 U4;
    float Float;
} u4tofloat;

union {
    long Long;
    double Double;
} longtodouble;

typedef struct {
    char sign[50];
    unsigned short count;
    unsigned int* index;
} opcode;

opcode* opcodes;
void load_opcodes();

#endif