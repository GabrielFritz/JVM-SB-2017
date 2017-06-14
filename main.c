#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classloader.h"

char mainfile[1024] = "";
char outputfile[1024] = "";
int print_flag = 0;

void set_flags(int argc,char **argv)
{
    switch(argc)
    {
        case 3:
            strcpy(outputfile,argv[2]);
            print_flag=1;
            //Nota: O comando "break;" foi omitido de propósito aqui.
        case 2:
            strcpy(mainfile,argv[1]);
        break;
        case 1:
            printf("# Digite o nome do arquivo contendo o método Main: ");
            scanf("%s",mainfile);
            fflush(stdin);
            printf("# Digite o nome do arquivo de saída: ");
            scanf("%s",outputfile);
            fflush(stdin);
            print_flag= 1;
        break;
        default:
            printf("Programa executado incorretamente.\n");
            printf("Modo de uso:\n\t");
            printf("./jvm <nome_main_class nome_output>\n");
            exit(0);
    }
}
//Essa função apresenta um "_" precedendo seu nome, pois trata-se, apenas, de um teste. Será excluída da vesão final.
void _testa_flags() {
    printf("Nome do arquivo Main: %s\n",mainfile);
    printf("Nome do arquivo de saída: %s\n",outputfile);
    printf("Print Flag: %d\n",print_flag);
}

int main(int argc,char **argv)
{
    method_info* Main;
    set_flags(argc,argv);
    _testa_flags();


    return 0;
}
