#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <inttypes.h>
#include "types.h"
#include "classfile.h"

void shutdown(FILE *fd, FILE *fout, ClassFile* cf);
int findtype(char* type);
FILE* open_file(char *nomearquivo);
FILE * io_handler(int argc, char *argv[], char *nomearquivo, FILE **fout);
void free_cte_pool(ClassFile *cf);
void free_methods(ClassFile *cf);
void free_clFile(ClassFile* cf);
#endif