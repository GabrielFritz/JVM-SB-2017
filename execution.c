#include "execution.h"
//#include "classfile.h"
//#include "frame.h"
//#include "types.h"
//#include "heap.h"
#include "instruction_set.h"

/*!
 * Realiza a contagem dos argumentos de um metodo
 * @param[in]   d   String contendo a definicao do metodo
 * @param[out]  n   Quantidade de argumentos do metodo
 *
 * Faz o parse da string d ate encontrar os metodos e incrementa n.
 * */
int count_args(char* d) {
    int i=0;
    int n=0;
    while(d[i]!=')') {
        if(d[i]!='(' && d[i]!='[') {
            ++n;
            if(d[i] == 'L') {
                while(d[i]!=';') ++i;
            }
        }
        ++i;
    }
    return n;
}

/*!
 * Executa as operacoes do metodo de uma classe.
 * @param[in] e Interpretador
 *
 * Enquanto nao e' encontrado um nop, a funcao le uma operacao e chama a funcao
 * para executa-la de acordo com o ponteiro de funcoes.
 * */
void execute_method(execution* e) {
    int flag =0;
    while(!flag) {
        u1 i = u1ReadFrame(e->frame);

        if (i > 0xc4) { //instrucao maxima eh wide, com valor 196
            printf("Valor lido nao corresponde ao code de uma instrucao.\n");
            exit(1);
        }
        else {
            printf("Iniciando execucao da instrucao de codigo %d\n", i);
            flag = instr_array[i](e);   //termina a execucao quando encontra um nop
        }
    }
}

/*!
 * Prepara o frame para a execucao de um metodo.
 * @param[in] e           Interpretador
 * @param[in] class       Nome da classe
 * @param[in] method      Nome do Metodo
 * @param[in] descriptor  Descritor do Metodo
 * @param[in] args        Argumentos de Entrada do metodo
 *
 * Primeiro, e' checada se a classe ja' esta' carregada. Em seguida, um novo
 * frame e' inserido na pilha de frames e iniciado com os dados do metodo a ser
 * executado.
 * Para cada argumento, e' criada uma entrada na pilha de operandos e na pilha
 * de tipos. Por fim, os operadores sao colocados no array local da pilha de
 * frames do interpretador.
 * */
void init_methodexecution(execution* e,char* class,char* method, char* descriptor, int args){
    ClassFile* cf = check_class(e,class);
    push_frame(&(e->frame)); //novo frame da nova classe
    frame_init(e->start,*cf,e->frame,method,descriptor); //
    int sizeindex =0;
    
    if(e->frame->below) {
        operand_heap* opaux;
        init_opheap(&opaux);
        for(int i=0;i<args;++i) { //pilha auxiliar para contagem de sizeindex
            int type = e->frame->below->top->type;
            operand_type new = pop_op(&(e->frame->below->top));
            push_op(&opaux,new,type);
            if(type==EXCEPTIONS) ++sizeindex;   //type == 2, aumentar o tamanho necessario
            ++sizeindex;
        }
        for(int i=0;i<args;++i) { //devolver operandos para pilha original
            int type = opaux->type;
            operand_type new = pop_op(&opaux);
            push_op(&(e->frame->below->top),new,type);
        }
        for(int i=sizeindex-1; i>-1; --i) {
            if(e->frame->below->top->type == EXCEPTIONS) --i; //type == 2, aumentar indice
            e->frame->local_arr[i] = pop_op(&(e->frame->below->top));
        }
    }
}

/*!
 * Chama a execucao de uma classe que ainda nao foi carregada
 * @param[out]  cf  Classe carregada
 * @param[in] e Interpretador que executara a classe
 * @param[in] name  Nome da classe a ser executada
 *
 * Primeiramente, e' verificada se a classe pertence ao heap de classes ja'
 * carregadas. Caso nao pertenca, ela e' carregada, colocada no heap e e'
 * iniciada a execucao de seu construtor.
 * */
ClassFile* check_class(execution* e, char* name) {
    ClassFile* cf = search_classheap(e->start,name);
    if(!cf) { //classe ainda nao pertence ao heap de classes
        cf = load_ClassFile(name);
        push_class(&(e->start),*cf);

        if(search_method(cf,MNAME,MDESCR)) {
            init_methodexecution(e,name,MNAME,MDESCR,0);
            execute_method(e);
        }
    }
    return cf;
}
