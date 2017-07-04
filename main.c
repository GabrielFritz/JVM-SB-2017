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

	execution* e = NULL;
	ClassFile* cf = NULL;
	FILE* fd = NULL;
	FILE* fout = NULL;

    if(argc == 2) { //./jvm <nome-da-classe>
        e = (execution*)malloc(sizeof(execution));
        framestack_init(&(e->frame)); //inicializa a pilha de stacks (null)
        classheap_init(&(e->start)); //inicializa a pilha de classes (null)
        init_methodexecution(e,argv[1],"main","([Ljava/lang/String;)V",0);
        execute_method(e);
    }
    else if (argc == 3) { //./jvm <nome-da-classe> <arquivo-txt-saida>
		char arquivoentrada[1024];
		fd = io_handler(argv[1], argv[2], &fout); //ponteiro arquivo .class
    	
    	cf = readClass(fd);
    	print_class(cf, arquivoentrada, fout);
		/*ClassFile* cf = readClass(FILE* fd);
    	print_class(ClassFile* cf, char* strcat(nomearquivo,".class"), FILE* fout);*/
    }
    else {
    	int opcaoinserida = 0;
    	char arquivoentrada[1024];

	    do {
	      printf("Insira a execucao desejada:\n");
	      printf("\t1. Iniciar JVM:\n");
	      printf("\t2. Iniciar Leitor/Exibidor:\n");
	      scanf("%d", &opcaoinserida);
	      fflush(stdin);
	    } while (opcaoinserida != 1 && opcaoinserida != 2);
	    
	    if (opcaoinserida == 1) {
			printf("Digite o nome da classe a ser executada (sem a extensao .class): ");
			scanf("%s", arquivoentrada);
			fflush(stdin);
			printf("\n");

			e = (execution*)malloc(sizeof(execution));
	        framestack_init(&(e->frame)); //inicializa a pilha de stacks (null)
	        classheap_init(&(e->start)); //inicializa a pilha de classes (null)
	        init_methodexecution(e,arquivoentrada,"main","([Ljava/lang/String;)V",0);
	        execute_method(e);
	    }

	    if (opcaoinserida == 2) {
	    	printf("Digite o nome da classe a ser lida (sem a extensao .class): ");
			scanf("%s", arquivoentrada);
			fflush(stdin);
			printf("\n");

			char saidaarquivo[1024];
			printf("Digite o nome do arquivo texto de saida (incluindo a extensao .txt): ");
			scanf("%s", saidaarquivo);
			fflush(stdin);
			printf("\n");

			fd = io_handler(arquivoentrada, saidaarquivo, &fout); //ponteiro arquivo .class
    	
    		ClassFile* cf = readClass(fd);
    		print_class(cf, arquivoentrada, fout);
    		printf ("Leitura finalizada.\n");

	    }
    }
    shutdown(fd, fout, cf, e);
    return 0;
}
