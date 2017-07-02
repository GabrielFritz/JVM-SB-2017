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
    if(argc == 2) {
        execution* e = (execution*)malloc(sizeof(execution));
        framestack_init(&(e->frame));
        classheap_init(&(e->start));
        init_methodexecution(e,argv[1],"main","([Ljava/lang/String;)V",0);
        execute_method(e);
    }
    else {
    	printf("Modo de executar: ./jvm <nome-do-.class> \n");
    }
    return 0;
}
