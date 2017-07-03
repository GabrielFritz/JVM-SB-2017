#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "types.h"
//#include "util.h"
#include "classfile.h"
//#include "frame.h"
//#include "heap.h"
//#include "instruction_set.h"
#include "execution.h"

void shutdown(FILE *fd, FILE *fout, ClassFile* cf, execution* e){
  if(e)
    free_execution(e);
  if(cf)
    free_clFile(cf);
  if(fd)
    fclose(fd);
  if(fout)
    fclose(fout);
}

int main(int argc,char* argv[]){
  FILE* fout = NULL;
  FILE* fd = NULL;
  char arquivoentrada[1024];
  ClassFile* cf = NULL;
  execution* e = NULL;

  if(argc == 2) { //./jvm <nome-da-classe>
    e = (execution*)malloc(sizeof(execution));
    framestack_init(&(e->frame));
    classheap_init(&(e->start));
    init_methodexecution(e,argv[1],"main","([Ljava/lang/String;)V",0);
    execute_method(e);
  }
  else if (argc == 3) { //./jvm <nome-da-classe> <arquivo-txt-saida>
    fd = io_handler(argv, arquivoentrada, &fout); //ponteiro arquivo .class
    cf = readClass(fd);
    print_class(cf, arquivoentrada, fout);
    //free_classFile();
    /*ClassFile* cf = readClass(FILE* fd);
      print_class(ClassFile* cf, char* strcat(nomearquivo,".class"), FILE* fout);*/
  }
  else { //colocar um modo de entrada do arquivo durante a execucao
    printf("Modo de executar JVM: ./jvm <Nome-da-Classe>\n");
    printf("Modo de executar Leitor/Exibidor: ./jvm <Nome-da-Classe> <arquivo-saida.txt>\n");
  }
  shutdown(fd, fout, cf, e);
  return 0;
}
