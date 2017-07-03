#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "types.h"
//#include "util.h"
//#include "classfile.h"
//#include "frame.h"
//#include "heap.h"
//#include "instruction_set.h"
#include "execution.h"

int main(int argc,char* argv[]){
    if(argc == 2) { //./jvm <nome-da-classe>
        execution* e = (execution*)malloc(sizeof(execution));
        framestack_init(&(e->frame));
        classheap_init(&(e->start));
        init_methodexecution(e,argv[1],"main","([Ljava/lang/String;)V",0);
        execute_method(e);
    }
    else if (argc == 3) { //./jvm <nome-da-classe> <arquivo-txt-saida>
		char arquivoentrada[1024];
		FILE* fout = NULL;
		FILE* fd = io_handler(argv, arquivoentrada, &fout); //ponteiro arquivo .class
    	
    	ClassFile* cf = readClass(fd);
    	print_class(cf, arquivoentrada, fout);
    	shutdown(fd, fout, cf);
		/*ClassFile* cf = readClass(FILE* fd);
    	print_class(ClassFile* cf, char* strcat(nomearquivo,".class"), FILE* fout);*/
    }
    else {
    	printf("Modo de executar JVM: ./jvm <Nome-da-Classe>\n");
    	printf("Modo de executar Leitor/Exibidor: ./jvm <Nome-da-Classe> <arquivo-saida.txt>\n");
    }
    return 0;
}
