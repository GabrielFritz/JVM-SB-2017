#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include "util.h"
#include "classfile.h"

u1 search_tag(cp_info* cp, u2 i) {return cp[i-1].tag;}

int search_int(cp_info* cp, u2 i) { return (int)cp[i-1].info.Integer_info.bytes;}

float search_float(cp_info* cp, u2 i) {
    u4tofloat.U4 = cp[i-1].info.Float_info.bytes;
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
       if (cp->tag == UTF8){
         free(cp->info.Utf8_info.bytes);
         cp->info.Utf8_info.bytes = NULL;
       }
    }
    free(cf->constant_pool);
    cf->constant_pool = NULL;
}

void free_attribute(attribute_info* att, ClassFile* cf){
  char* type;
  type = (char*)calloc(cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1,sizeof(char));
  strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
  int i = findtype(type);
  switch (i) {
  case CODE:
    if (att->type.Code.code_length != 0) {
      free(att->type.Code.code);
      att->type.Code.code = NULL;
    }
    if (att->type.Code.exception_table_length != 0) {
      free(att->type.Code.exception_table);
      att->type.Code.exception_table = NULL;
    }
    if (att->type.Code.attributes_count != 0) {
      attribute_info* att_aux;
      for (att_aux = att->type.Code.attributes; att_aux < att->type.Code.attributes + att->type.Code.attributes_count; ++att_aux)
        free_attribute(att_aux, cf);
      free(att->type.Code.attributes);
      att->type.Code.attributes = NULL;
    }
    break;
  case EXCEPTIONS:
    if (att->type.Exceptions.number_of_exceptions != 0) {
      free(att->type.Exceptions.exception_index_table);
      att->type.Exceptions.exception_index_table = NULL;
    }
    break;
  case INNERCLASSES:
    if (att->type.InnerClasses.number_of_classes != 0) {
      free(att->type.InnerClasses.classes);
      att->type.InnerClasses.classes = NULL;
    }
    break;
  case OTHER:
    if (att->attribute_length != 0) {
      free(att->type.Other.bytes);
      att->type.Other.bytes = NULL;
    }
    break;
  }
  free(type);
  type = NULL;
}

void free_methods(ClassFile *cf) {
  method_info* method_aux;
  
  for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
    attribute_info* att_aux;
    for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux)
      free_attribute(att_aux, cf);
    free(method_aux->attributes);
    method_aux->attributes = NULL;
  }
  free(cf->methods);
  cf->methods = NULL;
}

void free_attributes(ClassFile *cf) {
  attribute_info* att_aux;
  for (att_aux = cf->attributes; att_aux < cf->attributes + cf->attributes_count; ++att_aux)
    free_attribute(att_aux, cf);
  free(cf->attributes);
  cf->attributes = NULL;
}

void free_clFile(ClassFile* cf) {
  if (!cf)
    return;
  if (cf->fields){
    free(cf->fields);
    cf->fields = NULL;
  }
  if (cf->interfaces){
    free(cf->interfaces);
    cf->interfaces = NULL;
  }
  if (cf->methods)
    free_methods(cf);
  if (cf->attributes)
    free_attributes(cf);
  free_cte_pool(cf);
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

//Executcao: ./jvm <nome-da-classe> <arquivo-txt-saida>
//Retorna o *FILE para o arquivo .class
FILE* io_handler(char *entradaarquivo, char *saidaarquivo, FILE **fout)
{
  FILE *fd = NULL;
  strcat(entradaarquivo,".class");
  fd = open_file(entradaarquivo);
  if (!fd) {
    printf("Arquivo de entrada não encontrado!\n");
    exit(0);
  }

  if (!(*fout = fopen(saidaarquivo, "w+"))) {
    printf("Erro na abertura do arquivo de saida\n");
    exit(0);
  }
  return fd;
}
