#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "classfile.h"
#include "heap.h"
#include "frame.h"

int operandheap_isempty(operand_heap* top) { return top==NULL;  }

/*!
 * Adiciona um operador na pilha de operandos
 * @param[in] top     Topo da pilha de operandos
 * @param[in] op      Operador
 * @param[in] op_type Tipo do operador
 *
 * A funcao aloca a memoria necessaria para a operacao, e atribui os valores de
 * op para uma variavel auxiliar do tipo operand_heap. Esta variavel e' colocada
 * na pilha.
 * */
void push_op(operand_heap **top,operand_type op,int op_type)
{
    operand_heap* aux = (operand_heap*)calloc(1,sizeof(operand_heap)); //cppcheck acusa memory leak na variavel
    aux->op = op;
    aux->type = op_type;
    aux->below = *top;
}

/*!
 * Inicia a pilha de operandos.
 * @param[in] top Topo da pilha de operandos.
 * */
void init_opheap(operand_heap** top) {*top = NULL;}

/*!
 * Retorna o operador do topo da pilha de operandos. (Destrutivo)
 * @param[in]   top Topo da pilha de operandos
 * @paran[out]  op  Operando no topo da pilha
 *
 * Se a pilha de operandos nao estiver vazia, a variavel op recebe o operando
 * que esta no topo e o topo passa a apontar para o proximo elemento da pilha.
 * */
operand_type pop_op(operand_heap** top)
{
    operand_type op;
    if(!operandheap_isempty(*top)) {
        operand_heap* aux = *top;
        op = (*top)->op;
        *top = (*top)->below;
        free(aux);
    }
    else {
        printf("ERRO. OperandHeap vazia.\n");
        exit(1);
    }
    return op;
}

int framestack_isempty(frame* f){ return f==NULL; }

void framestack_init(frame** s) { *s = NULL; }

/*!
 * Colocar um frame vazio na pilha de frames
 * @param[in] s Frame a ser inserido na pilha
 *
 * Um espaco de memoria e' reservado. O novo frame aponta para o primeiro da
 * lista e a cabeca da lista passa a apontar para o novo.
 * */
void push_frame(frame** s)
{
    frame* aux= (frame*)malloc(sizeof(frame));
    aux->below = *s;
    *s = aux;
}

void pop_frame(frame** s)
{
    if(!framestack_isempty(*s)) {
        frame* aux = *s;
        *s = (*s)->below;
        free(aux);
    }
    else {
        printf("ERRO: StackFrame Vazia.\n");
        exit(1);
    }
}

/*!
 * Carrega o frame com os dados do metodo.
 * @param[in] start         Topo do heap de classes ja' carregadas
 * @param[in] cf            Dados do arquivo ponto Class
 * @param[in] frame         Frame a ser carregado
 * @param[in] method_name   Nome do metodo a ser carregado
 * @param[in] descriptor    Descritor do metodo a ser carregado
 *
 * Pesquisa-se pelo metodo na classe em que foi chamado e, caso nao encontre, em
 * suas superclasses. Ao encontrar, popula o frame com os dados.
 * */
void frame_init(class_heap* start, ClassFile cf,frame* frame, char* method_name, char* descriptor)
{
    ClassFile* cf_aux = &cf;
    method_info* method = NULL;
    attribute_info code;
    u2 super_index = cf_aux->super_class;
    char super_name[128];

    while(cf_aux!=NULL
    && (method=search_method(cf_aux,method_name,descriptor))==NULL
    && super_index != 0)
    {
        strcpy(super_name, search_utf8(cf_aux->constant_pool,
                                        cf_aux->constant_pool[super_index].info.Class_info.name_index));

        cf_aux = search_classheap(start,super_name); // Se nao foi encontrado, ele pesquisa na superclasse
        if(cf_aux != NULL)
            super_index = cf_aux->super_class;
        printf("Super name atual eh: %s\n", &super_name);
        printf("Super index atual eh: %u\n", &super_index);
        printf("method eh %x\n", method);
    }
    if(!method) {
        printf("ERRO. Method Inexistente: %s %s\n",method_name,descriptor);
        exit (1);
    }

    attribute_info* att_aux;
    for(att_aux = method->attributes; att_aux<method->attributes+method->attributes_count;++att_aux)
    {
        if(!strcmp(search_utf8(cf_aux->constant_pool, //Atributo Code nao encontrado
            att_aux->attribute_name_index),"Code"))
        {
            code = *att_aux; //avancar na lista de atributos
            break;
        }
    }
    

    frame->constant_pool = cf_aux->constant_pool;
    init_opheap(&(frame->top));
    frame->code = code.type.Code.code;
    frame->local_arr = (operand_type*)malloc(sizeof(operand_type)*code.type.Code.max_locals);
    frame->pc = frame->code;
}

/*!
 * Retorna o endereco da instrucao a ser executada
 * @param[in]   f   Frame do metodo a ser executado
 * @param[out]  u1  Valor do endereco de memoria da instrucao
 *
 * Retorna o valor apontado pelo frame.
 * */
u1 u1ReadFrame(frame* f) { return *(++f->pc); }

i1 signed1ReadFrame(frame* f) { return *(++f->pc); }

u2 u2ReadFrame(frame* f) {
    u2 aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}

u4 u4ReadFrame(frame* f) {
    u4 aux;
    aux = *(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    aux<<=8;
    aux|=*(f->pc++);
    return aux;
}
