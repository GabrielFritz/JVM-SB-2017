#include "execution.h"
#include "classfile.h"
//#include "frame.h"
//#include "types.h"
//#include "heap.h"
#include "instruction_set.h"

char* instrucoes_nomes[] = { //10 instrucoes por linha
    "nop", "aconst_null", "iconst_m1", "iconst_0", "iconst_1", "iconst_2", "iconst_3", "iconst_4", "iconst_5", "lconst_0", //0 ao 9
    "lconst_1", "fconst_0", "fconst_1", "fconst_2", "dconst_0", "dconst_1", "bipush", "sipush", "ldc", "ldc_w",
    "ldc2_w", "iload", "lload", "fload","dload", "aload", "iload_0", "iload_1", "iload_2", "iload_3",
    "lload_0", "lload_1", "lload_2", "lload_3", "fload_0", "fload_1", "fload_2", "fload_3", "dload_0", "dload_1",
    "dload_2", "dload_3", "aload_0", "aload_1", "aload_2", "aload_3", "iaload", "laload", "faload", "daload",
    "aaload", "baload", "caload", "saload", "istore", "lstore", "fstore", "dstore", "astore", "istore_0",
    "istore_1", "istore_2", "istore_3", "lstore_0", "lstore_1", "lstore_2", "lstore_3", "fstore_0", "fstore_1", "fstore_2",
    "fstore_3", "dstore_0", "dstore_1", "dstore_2", "dstore_3", "astore_0", "astore_1", "astore_2", "astore_3", "iastore",
    "lastore", "fastore", "dastore", "aastore", "bastore", "castore", "sastore", "pop", "pop2", "dup",
    "dup_x1", "dup_x2", "dup2", "dup2_x1", "dup2_x2", "swap", "iadd", "ladd", "fadd", "dadd",
    "isub", "lsub", "fsub", "dsub", "imul", "lmul", "fmul", "dmul", "idiv", "ldiv", //100 ao 109
    "fdiv", "ddiv", "irem", "lrem", "frem", "drem", "ineg", "lneg", "fneg", "dneg",
    "ishl", "lshl", "ishr", "lshr", "iushr", "lushr", "iand", "land", "ior", "lor",
    "ixor", "lxor", "iinc", "i2l", "i2f", "i2d", "l2i", "l2f", "l2d", "f2i",
    "f2l", "f2d", "d2i", "d2l", "d2f", "i2b", "i2c", "i2s", "lcmp", "fcmpl",
    "fcmpg", "dcmpl", "dcmpg", "ifeq", "ifne", "iflt", "ifge","ifgt", "ifle", "if_icmpeq", //150 ao 159
    "if_icmpne", "if_icmplt", "if_icmpge", "if_icmpgt", "if_icmple", "if_acmpeq", "if_acmpne", "goto", "jsr", "ret",
    "tableswitch", "lookupswitch", "ireturn", "lreturn", "freturn", "dreturn", "areturn", "return", "getstatic", "putstatic",
    "getfield", "putfield", "invokevirtual", "invokespecial", "invokestatic", "invokeinterface", "invokedynamic", "new", "newarray", "anewarray",
    "arraylength", "athrow", "checkcast", "instanceof", "monitorenter", "monitorexit", "wide", "multianewarray", "ifnull", "ifnonnull",
    "goto_w", "jsr_w", "breakpoint", NULL, NULL, NULL, NULL, NULL, NULL, NULL, //200 ao 209
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, "impdep1", "impdep2" //250 ao 255
};

void free_execution(execution *e){
  if(e->frame)
    free_frame(e->frame);
  if(e->start)
    free_classheap(e->start);
  free(e);
  e = NULL;
}

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
        u1 i = u1ReadFrame(e->frame); //retirar do frame o codigo da instrucao a ser executada

        if (i > 0xc9) { //instrucao maxima eh jsr_w, com valor 201
            printf("Valor lido nao corresponde ao code de uma instrucao valida\n");
            exit(1);
        }
        else {
            printf("Executar instrucao %s de codigo hexa %02x\n", instrucoes_nomes[i],i);
            flag = instr_array[i](e);   //termina a execucao quando encontra um return ou nop
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
    frame_init(e->start,*cf,e->frame,method,descriptor); //inicializa o frame do metodo a ser executado (null)
    
    if(e->frame->below) {
        int sizeindex =0;
        operand_heap* opaux;
        init_opheap(&opaux);
        //A seguir: manipular os argumentos passados por esse chamador
        for(int i=0;i<args;++i) { //pilha auxiliar para contagem de sizeindex
            int type = e->frame->below->top->type;
            operand_type new = pop_op(&(e->frame->below->top)); //retira o parametro da pilha do metodo chamador, o below
            push_op(&opaux,new,type); // insere na pilha do metodo chamado
            if(type==EXCEPTIONS) ++sizeindex; //type == 2, aumentar o tamanho necessario
            ++sizeindex;
        }
        for(int i=0;i<args;++i) { //devolver operandos para pilha original, a pilha do método chamador
            int type = opaux->type;
            operand_type new = pop_op(&opaux);
            push_op(&(e->frame->below->top),new,type);
        }
        for(int i=sizeindex-1; i>-1; --i) {
            if(e->frame->below->top->type == EXCEPTIONS) --i; //type == 2, aumentar indice
            e->frame->local_arr[i] = pop_op(&(e->frame->below->top)); //coloca parametros no array local da funcao chamada
        }
    }
        free(cf);
        cf = NULL;
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
    if(!cf ) { //classe ainda nao pertence ao heap de classes
        cf = load_ClassFile(name); //ERRO: AO CARREGAR UMA SEGUNDA CLASSE, O TOPO DA LISTA *(e->start) MUDA BIZARRAMENTE
        push_class(&(e->start),*cf);

        if(search_method(cf,MNAME,MDESCR)) {
            //init_methodexecution(e,name,MNAME,MDESCR,0);

            init_methodexecution(e,
                search_utf8(cf->constant_pool,cf->constant_pool[(cf->this_class)-1].info.Class_info.name_index) //nome da classe incluindo pacote
                ,MNAME,MDESCR,0);
            execute_method(e);
        }
    }
    return cf;
}
