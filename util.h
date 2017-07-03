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

u1 search_tag(cp_info* cp, u2 i);
int search_int(cp_info* cp, u2 i);
float search_float(cp_info* cp, u2 i);
char* search_utf8(cp_info* cp, u2 i);
void shutdown(FILE *fd, FILE *fout, ClassFile* cf);
int findtype(char* type);
FILE* open_file(char *nomearquivo);
FILE* io_handler(char *argv[], char *nomearquivo, FILE **fout);
void free_cte_pool(ClassFile *cf);
void free_methods(ClassFile *cf);
void free_clFile(ClassFile* cf);
#endif