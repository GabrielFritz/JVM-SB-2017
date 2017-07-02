#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include "util.h"
#include "classfile.h"

u1 search_tag(cp_info* cp, u2 i) { return cp[i].tag;}

int search_int(cp_info* cp, u2 i) { return (int)cp[i].info.Integer_info.bytes;}

float search_float(cp_info* cp, u2 i) {
    u4tofloat.U4 = cp[i].info.Float_info.bytes;
    return u4tofloat.Float;
}

char* search_utf8(cp_info* cp, u2 i) {
  return (char*)cp[i-1].info.Utf8_info.bytes;
}

int findtype(char* type) {
    if (!strcmp(type, "ConstantValue")) {
        return CONSTANTVALUE;
    } else if (!strcmp(type, "Code")) {
        return CODE;
    } else if (!strcmp(type, "Exceptions")) {
        return EXCEPTIONS;
    } else if (!strcmp(type, "InnerClasses")) {
        return INNERCLASSES;
    } else {
        return OTHER;
    }
}

void free_cte_pool(ClassFile *cf) {
    cp_info *cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
       if (cp->tag == UTF8)
         free(cp->info.Utf8_info.bytes);
    }
    free(cf->constant_pool);
}


void free_methods(ClassFile *cf) {
  /*method_info* method_aux;
  char *type;

  for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
    attribute_info* att_aux;
    for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux) {
      type = (char*)calloc(cf->constant_pool[att_aux->attribute_name_index - 1].info.Utf8_info.length,sizeof(char));
      strcpy(type, (char*)cf->constant_pool[att_aux->attribute_name_index - 1].info.Utf8_info.bytes);
      int i = findtype(type);
      if (i == CODE){
        if (att_aux->type.Code.code_length > 0) {
          free(att_aux->type.Code.code);
        }
        if (att_aux->type.Code.exception_table_length > 0) {
          free(att_aux->type.Code.exception_table);
        }
      }
      free(type);
      //free_attr(cf, method_aux);
    }
    free(method_aux->attributes);
  }*/
  free(cf->methods);
}
void free_clFile(ClassFile* cf) {
  if (!cf)
    return;
  free_cte_pool(cf);
  if (cf->fields)
    free(cf->fields);
  if (cf->interfaces)
    free(cf->interfaces);
  free_methods(cf);
  /*if (cf->methods)
    free(cf->methods);
  if (cf->attributes)
    free(cf->attributes);*/
  free(cf);
}

void shutdown(FILE *fd, FILE *fout, ClassFile* cf) {
    free_clFile(cf);
    fclose(fout);
    fclose(fd);
    printf("Finalizado!\n");
}


FILE* open_file(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo, "rb");
    if (!fp) {
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}

FILE* io_handler(int argc, char *argv[], char *nomearquivo, FILE **fout)
{
  FILE *fd = NULL;
  char saidaarquivo[1024] = "output_class.txt";
  if (argc == 1) {
    do {
      printf("Digite o nome do arquivo: ");
      scanf("%s", nomearquivo);
      fflush(stdin);
      fd = open_file(nomearquivo);
    } while (!fd);
  } else if (argc == 3) {
    strcpy(nomearquivo, argv[1]);
    fd = open_file(nomearquivo);
    if (!fd) {
      printf("Arquivo de entrada não encontrado!\n");
      exit(0);
    }
    strcpy(saidaarquivo, argv[2]);

  } else {
    printf("Uso do programa: ./leitorexibidor [nome-do-class nome-da-saída]\n");
    exit(0);
  }
  if (!(*fout = fopen(saidaarquivo, "w+"))) {
    fprintf(stderr, "Erro %d na abertura do arquivo de saida\n", errno);
    exit(0);
  }
  return fd;
}