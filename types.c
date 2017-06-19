#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "types.h"
#include "frame.h"

u1 u1Read(FILE* fd) {
    u1 byte;
    fread(&byte, sizeof(u1), 1, fd);
    return byte;
}

u2 u2Read(FILE* fd) {
    u2 toReturn = 0;
    u1 byte1, byte2;
    fread(&byte1, sizeof(u1), 1, fd);
    fread(&byte2, sizeof(u1), 1, fd);
    toReturn = byte1 << 8;
    toReturn |= byte2;
    return toReturn;
}

u4 u4Read(FILE* fd) {
    u4 toReturn = u2Read(fd) << 16;
    toReturn |= u2Read(fd);
    return toReturn;
}

u1 u1ReadFrame(frame* f) { return *(++f->pc) }

i1 signed1ReadFrame(frame* f) { return *(++f->pc) }

u2 u2ReadFrame(frame* f) {
    u2 aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}

u4 u4ReadFrame(frame* f) {
    u4 aux;
    aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}