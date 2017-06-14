#ifndef OPCODE_H
#define OPCODE_H

typedef struct {
    char sign[50];
    unsigned short count;
    unsigned int* index;
} opcode;

opcode* opcodes;

void load_opcodes();

#endif