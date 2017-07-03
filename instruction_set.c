#include <math.h>
#ifndef INSTR_H
#define INSTR_H
#include "instruction_set.h"
#endif
#include "execution.h"
#include "classfile.h"
#include "types.h"
#include "frame.h"
#include "heap.h"
#include "util.h"

int nop(execution* e) {
    return 1;
}

int aconst_null(execution* e) {
    operand_type op;
    op.Ref = NULL;
    push_op(&(e->frame->top),op,1);
    return 0;
}

int iconst_m1(execution *e){
    operand_type op;
    op.Int = -1;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_0(execution *e){
    operand_type op;
    op.Int = 0;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_1(execution *e){
    operand_type op;
    op.Int = 1;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_2(execution *e){
    operand_type op;
    op.Int = 2;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_3(execution *e){
    operand_type op;
    op.Int = 3;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_4(execution *e){
    operand_type op;
    op.Int = 4;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iconst_5(execution *e){
    operand_type op;
    op.Int = 5;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int lconst_0(execution *e){
	operand_type op;
    op.Long = 0;
	push_op(&(e->frame->top),op,2);
	return 0;
} 
int lconst_1(execution *e){
	operand_type op;
    op.Long = 1;
	push_op(&(e->frame->top),op,2);
	return 0;
} 
int fconst_0(execution *e){
	operand_type op;
    op.Float = 0.0;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int fconst_1(execution *e){
	operand_type op;
    op.Float = 1.0;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int fconst_2(execution *e){
	operand_type op;
    op.Float = 2.0;
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int dconst_0(execution *e){
    operand_type op;
    op.Double = 0.0;
	push_op(&(e->frame->top),op,2);
	return 0;
} 
int dconst_1(execution *e){
    operand_type op;
    op.Double = 1.0;
	push_op(&(e->frame->top),op,2);
	return 0;
} 
int bipush(execution *e){
    operand_type op;
    op.Int = u1ReadFrame(e->frame);

    if (op.Int & 0x80) { //operando negativo
        op.Int |= 0xFFFFFF00;
    }
    //printf("\t\tPreparar bipush do valor %d que em hexa eh %x\n", op.Int, op.Int);

	push_op(&(e->frame->top),op,1);
	return 0;
} 
int sipush(execution *e){
    operand_type op;
    op.Int = u2ReadFrame(e->frame);

if (op.Int & 0x8000) { //operando negativo
        op.Int |= 0xFFFF0000;
    }

	push_op(&(e->frame->top),op,1);
	return 0;
}
int ldc(execution *e){
    operand_type op;
    u1 i = u1ReadFrame(e->frame);
    switch (search_tag(e->frame->constant_pool,i)) {
        case INTEGER:
            op.Int = search_int(e->frame->constant_pool,i);
        break;
        case FLOAT:
            op.Float = search_float(e->frame->constant_pool,i);
        break;
        case STRING:
            op.Ref = (char*) search_utf8(e->frame->constant_pool,
                                        e->frame->constant_pool[i-1].info.String_info.string_index);
        break;
        default:
            printf("Invalid index for LDC.\n");
            exit(1);

    }
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int ldc_w(execution *e){
    operand_type op;
    u2 i = u2ReadFrame(e->frame);
    switch (search_tag(e->frame->constant_pool,i)) {
        case INTEGER:
            op.Int = search_int(e->frame->constant_pool,i);
        break;
        case FLOAT:
            op.Float = search_float(e->frame->constant_pool,i);
        break;
        case STRING:
            op.Ref = (char*) search_utf8(e->frame->constant_pool,
                                        e->frame->constant_pool[i-1].info.String_info.string_index);
        break;
        default:
            printf("Invalid index for LDC.\n");
            exit(1);

    }
    push_op(&(e->frame->top),op,1);
    return 0;
}

double search_double(cp_info* cp, u2 i) {
    longtodouble.Long = ((long long) cp[i-1].info.Double_info.high_bytes << 32) | (cp[i-1].info.Double_info.low_bytes);
    return longtodouble.Double;
}

long long search_long(cp_info* cp, u2 i) {
    long long Long = ((long long) cp[i-1].info.Long_info.high_bytes << 32) | (cp[i-1].info.Long_info.low_bytes);
    return Long;
}

int ldc2_w(execution *e){
    operand_type op;
    u2 i = u2ReadFrame(e->frame);
    switch (search_tag(e->frame->constant_pool,i)) {
        case DOUBLE:
            op.Double = search_double(e->frame->constant_pool,i);
        break;
        case LONG:
            op.Long = search_long(e->frame->constant_pool,i);
        break;
        default:
            printf("Invalid index for LDC.\n");
            exit(1);
    }
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int iload(execution *e){
    u1 i = u1ReadFrame(e->frame);
    operand_type op = e->frame->local_arr[i];
	push_op(&(e->frame->top),op,1);
	return 0;
}
int iload_0(execution *e){
    operand_type op = e->frame->local_arr[0];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int iload_1(execution *e){
    operand_type op = e->frame->local_arr[1];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int iload_2(execution *e){
    operand_type op = e->frame->local_arr[2];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int iload_3(execution *e){
    operand_type op = e->frame->local_arr[3];
    push_op(&(e->frame->top),op,1);
    return 0;
}
int fload(execution *e){
    u1 i = u1ReadFrame(e->frame);
    operand_type op = e->frame->local_arr[i];
    push_op(&(e->frame->top),op,1);
    return 0;
}
int fload_0(execution *e){
    operand_type op = e->frame->local_arr[0];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int fload_1(execution *e){
    operand_type op = e->frame->local_arr[1];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int fload_2(execution *e){
    operand_type op = e->frame->local_arr[2];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int fload_3(execution *e){
    operand_type op = e->frame->local_arr[3];
    push_op(&(e->frame->top),op,1);
    return 0;
} 
int lload(execution *e){
    u1 i = u1ReadFrame(e->frame);
    operand_type op = e->frame->local_arr[i];
	push_op(&(e->frame->top),op,2);
	return 0;
}
int lload_0(execution *e){
    operand_type op = e->frame->local_arr[0];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int lload_1(execution *e){
    operand_type op = e->frame->local_arr[1];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int lload_2(execution *e){
    operand_type op = e->frame->local_arr[2];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int lload_3(execution *e){
    operand_type op = e->frame->local_arr[3];
    push_op(&(e->frame->top),op,2);
    return 0;
}
int dload(execution *e){
    u1 i = u1ReadFrame(e->frame);
    operand_type op = e->frame->local_arr[i];
	push_op(&(e->frame->top),op,2);
	return 0;
}
int dload_0(execution *e){
    operand_type op = e->frame->local_arr[0];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int dload_1(execution *e){
    operand_type op = e->frame->local_arr[1];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int dload_2(execution *e){
    operand_type op = e->frame->local_arr[2];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int dload_3(execution *e){
    operand_type op = e->frame->local_arr[3];
    push_op(&(e->frame->top),op,2);
    return 0;
} 
int aload(execution *e){
    u1 i = u1ReadFrame(e->frame);
    operand_type op = e->frame->local_arr[i];
	push_op(&(e->frame->top),op,1);
	return 0;
}
int aload_0(execution *e){
    operand_type op = e->frame->local_arr[0];
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int aload_1(execution *e){
    operand_type op = e->frame->local_arr[1];
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int aload_2(execution *e){
    operand_type op = e->frame->local_arr[2];
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int aload_3(execution *e){
    operand_type op = e->frame->local_arr[3];
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int iaload(execution *e){
    operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector* v = (vector*) arr_ref.Ref;
	push_op(&(e->frame->top),v->array[i.Int],1);
	return 0;
} 
int laload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector* v = (vector*)arr_ref.Ref;
    push_op(&(e->frame->top),v->array[i.Int],2);
	return 0;
} 
int faload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector* v = (vector*)arr_ref.Ref;
    push_op(&(e->frame->top),v->array[i.Int],1);
	return 0;
} 
int daload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector* v = (vector*)arr_ref.Ref;
    push_op(&(e->frame->top),v->array[(i.Int)],2);
	return 0;
} 
int aaload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector* v = (vector*)arr_ref.Ref;
    push_op(&(e->frame->top),v->array[i.Int],1);

    /*if (*arr_ref == NULL) {
        printf ("ERROR: Invalid reference from aaload.");
        exit(1);
    }*/
    return 0;
} 
int store(execution *e){
    int index = u1ReadFrame(e->frame);
    e->frame->local_arr[index] = pop_op(&(e->frame->top));
	return 0;
} 
int store_0(execution *e){
    e->frame->local_arr[0] = pop_op(&(e->frame->top));
	return 0;
} 
int store_1(execution *e){
    e->frame->local_arr[1] = pop_op(&(e->frame->top));
	return 0;
} 
int store_2(execution *e){
    e->frame->local_arr[2] = pop_op(&(e->frame->top));
	return 0;
} 
int store_3(execution *e){
    e->frame->local_arr[3] = pop_op(&(e->frame->top));
	return 0;
} 
int iastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->array[i.Int].Int = value.Int;
    return 0;
} 
int lastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->array[i.Int].Long = value.Long;
	return 0;
} 
int fastore(execution *e){
	operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->array[i.Int].Float = value.Float;
    return 0;
} 
int dastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top)); //ESTA DESEMPILHANDO UMA REFERENCIA INVALIDA
    vector* v = ref.Ref;
    v->array[i.Int].Double = value.Double;
	return 0;
} 
int aastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->array[i.Int].Ref = value.Ref;
	return 0;
} 
int pop(execution *e){
	if(e->frame->top->type==1) pop_op(&(e->frame->top));
    else {
        printf("Type 2 POP Error.\n"); //instrucao pop nao serve para Long e Double
        exit(1);
    }
    return 0;
} 
int pop2(execution *e){ //realiza 2 pops se for Long ou double; realiza 1 pop otherwise
	if(e->frame->top->type==1 && e->frame->top->below->type==1) { //a comparacao nao eh com == 2?
        pop_op(&(e->frame->top));
        pop_op(&(e->frame->top));
    }
    else {
        pop_op(&(e->frame->top));
    }
    return 0;
} 
int dup(execution *e){ //duplica o topo a pilha
    int type = e->frame->top->type;
    operand_type op = pop_op(&(e->frame->top));
    push_op(&(e->frame->top),op,type);
    push_op(&(e->frame->top),op,type);
	return 0;
} 
int dup_x1(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    int type2 = e->frame->top->type;
    operand_type op2 = pop_op(&(e->frame->top));
    push_op(&(e->frame->top),op1,type1);
    push_op(&(e->frame->top),op2,type2);
    push_op(&(e->frame->top),op1,type1);
	return 0;
} 
int dup_x2(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    int type2 = e->frame->top->type;
    operand_type op2 = pop_op(&(e->frame->top));
    int type3 = e->frame->top->type;
    operand_type op3 = pop_op(&(e->frame->top));
    push_op(&(e->frame->top),op1,type1);
    push_op(&(e->frame->top),op3,type3);
    push_op(&(e->frame->top),op2,type2);
    push_op(&(e->frame->top),op1,type1);
	return 0;
} 
int dup2(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    if(type1==1) { //a comparacao nao eh com == 2? - Se tratam dos casos LONG e DOUBLE
        
        int type2 = e->frame->top->type; //tambem espera um valor '2'
        operand_type op2 = pop_op(&(e->frame->top)); //retira segunda parte do operando
        
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
    } else {
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op1,type1);
    }
	return 0;
} 
int dup2_x1(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    int type2 = 0;
    operand_type op2;
    if(type1==1) { //Form 1 - todos tipo 1
        type2 = e->frame->top->type;
        op2 = pop_op(&(e->frame->top));
    }
    int type3 = e->frame->top->type; // NUNCA USADO
    operand_type op3 = pop_op(&(e->frame->top)); 
    if(type1==1) { //Form 1 - todos tipo 1
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type2);
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
    } else { //Form 2 - Value 1  eh do tipo 2
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type1);
        push_op(&(e->frame->top),op1,type1);
    }
	return 0;
} 
int dup2_x2(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    int type2 = 0;
    operand_type op2;
    if(type1==1) {
        type2 = e->frame->top->type;
        op2 = pop_op(&(e->frame->top));
    }
    int type3 = e->frame->top->type;
    operand_type op3 = pop_op(&(e->frame->top)); 
    int type4 = 0;
    operand_type op4;
    if(type3 == 1) {
        type4 = e->frame->top->type;
        op4 = pop_op(&(e->frame->top));  
    }
    if(type1==1 && type3==1) {
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op4,type4);
        push_op(&(e->frame->top),op3,type3);
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
    } else if(type3==1) {
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op4,type4);
        push_op(&(e->frame->top),op3,type3);
        push_op(&(e->frame->top),op1,type1);
    } else if(type1==1) {
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type3);
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
    } else {
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type3);
        push_op(&(e->frame->top),op1,type1);
    }
	return 0;
} 
int swap(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    int type2 = e->frame->top->type;
    operand_type op2 = pop_op(&(e->frame->top));
    if( type1!=1||type2!=1) {
        printf("ERRO. SWAP em operando type 2\n");
        exit(1);
    }
    push_op(&(e->frame->top),op1,type1);
    push_op(&(e->frame->top),op2,type2);
	return 0;
}  
int iadd(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int += op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int ladd(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long += op2.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int fadd(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float += op2.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int dadd(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Double += op2.Double;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int isub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op2.Int -= op1.Int; //o que foi empilhado primeiro sera "popeado" por ultimo
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int lsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op2.Long -= op1.Long;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int fsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op2.Float -= op1.Float;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int dsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op2.Double -= op1.Double;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int imul(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int *= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lmul(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long *= op2.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int fmul(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float *= op2.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int dmul(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Double *= op2.Double;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int idiv(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    operand_type result = op2;
    if (op1.Int == 0) {
    	printf("ERRO. Divisao por zero.\n");
        exit(1);
    }
    else {
    	result.Int = op2.Int / op1.Int;
    	push_op(&(e->frame->top),result,1);
    }
	return 0;
}

int ldiv_(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    operand_type result = op2;

    if (op1.Long == 0) {
    	printf("ERRO. Divisao por zero.\n");
        exit(1);
    }
    else {
    	result.Long = op2.Long / op1.Long;
    	push_op(&(e->frame->top),result,1);
    }
	return 0;
}  
int fdiv(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    
    operand_type result = op2; //consulta em http://en.cppreference.com/w/c/numeric/math/isnan

    if (isnan(op1.Float) || isnan(op2.Float))
    	result.Float = NAN; //operando Not-A-Number: NaN
    
    else if (op2.Float == 0) {
    	if (op1.Float == 0)
            result.Float = NAN; //divisao 0/0: NaN
    	else
    		result.Float = 0; //divisao 0/numero
    }
    else if (!isfinite(op2.Float)) {
    	if (!isfinite(op1.Float))
            result.Float = NAN; //infinito/infinito: NaN

    	else
    		result.Float = op2.Float/op1.Float; //infinito/outra coisa - verificar sinais dos operandos
    }
    else
    	result.Float = op2.Float / op1.Float; //divisao finito/finito

    push_op(&(e->frame->top),result,1);
	return 0;
}  
int ddiv(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    operand_type result = op1; //consulta em http://en.cppreference.com/w/c/numeric/math/isnan

    if (isnan(op1.Double) || isnan(op2.Double))
        result.Double = NAN; //operando Not-A-Number: NaN
    
    else if (op2.Double == 0) {
    	if (op1.Double == 0)
            result.Double = NAN; //divisao 0/0: NaN
    	else
    		result.Double = 0; //divisao 0/numero
    }
    else if (!isfinite(op2.Double)) {
    	if (!isfinite(op1.Double))
            result.Double = NAN; //infinito/infinito: NaN
    	else
    		result.Double = op2.Double/op1.Double; //infinito/outra coisa - verificar sinais dos operandos
    }
    else
    	result.Double = op2.Double / op1.Double; //divisao finito/finito

    push_op(&(e->frame->top),result,1);
	return 0;
}  
int irem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    
    if (op2.Int == 0) {
    	printf("ERRO. Divisao por zero.\n");
        exit(1);
    }
    else {
    	op2.Int %= op1.Int;
    	push_op(&(e->frame->top),op2,1);
    }
	return 0;
}  
int lrem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    
    if (op2.Long == 0) {
    	printf("ERRO. Divisao por zero.\n");
        exit(1);
    }
    else {
    	op2.Long %= op1.Long;
    	push_op(&(e->frame->top),op2,1);
    }
	return 0;
}  
int frem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    op1.Float = fmod(op2.Float,op1.Float);
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int drem_(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //denominador
    operand_type op2 = pop_op(&(e->frame->top)); //numerador
    op1.Double = fmod(op2.Double,op1.Double);
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int ineg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    op1.Int = 0 - op1.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    op1.Long = 0 - op1.Long;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int fneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    op1.Float = 0 - op1.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int dneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    op1.Double = 0 - op1.Double;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int ishl(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int &= 0x1F;
    op1.Int = op2.Int << op1.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lshl(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long &= 0x3F;
    op1.Long = op2.Long << op1.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int ishr(execution *e){
    operand_type op1 = pop_op(&(e->frame->top)); //shift amount
    operand_type op2 = pop_op(&(e->frame->top)); //valor shiftado
    op1.Int &= 0x1F;
    if(op2.Int<0) {
        int n=0;
        for(int i=0;i<op1.Int;++i){
            n >>= 1;
            n |= 0x80000000;
        }
        op1.Int = op2.Int>>op1.Int;
        op1.Int |= n;
    } else {
        op1.Int = op2.Int>>op1.Int;
    }
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lshr(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long &= 0x3F;
    if(op2.Long<0) {
        int n=0;
        for(int i=0;i<op1.Long;++i){
            n >>= 1;
            n |= 0x8000000000000000;
        }
        op1.Long = op2.Long>>op1.Long;
        op1.Long |= n;
    } else {
        op1.Long = op2.Long>>op1.Long;
    }
    push_op(&(e->frame->top),op1,2);
	return 0;
}
int iushr(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int &= 0x1F;
    op1.Int = op2.Int << op1.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
} 
int lushr(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long &= 0x3F;
    op1.Long = op2.Long << op1.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
} 
int iand(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int &= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int land(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long &= op2.Long;
    push_op(&(e->frame->top),op1,2);
    return 0;
}
int ior(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int |= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lor(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long |= op2.Long;
    push_op(&(e->frame->top),op1,2);
    return 0;
}  
int ixor(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int ^= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lxor(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long ^= op2.Long;
    push_op(&(e->frame->top),op1,2);
    return 0;
}
int iinc(execution *e){
    u1 i = u1ReadFrame(e->frame);
    int inc = u1ReadFrame(e->frame); //retirar da pilha o valor incrementado, que pode ser 1 ou -1
    if (inc & 0x80) { //eh negativo
        inc -= 256; //extensão do zero
    }

    e->frame->local_arr[i].Int += inc;
	return 0;
} 
int i2l(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Long = op1.Int;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int i2f(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Float = (float) op1.Int;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int i2d(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Double = (double) op1.Int;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int l2i(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (int)op1.Long;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int l2f(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Float = (float) op1.Long;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int l2d(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Double = (double) op1.Long;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int f2i(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (int)op1.Float;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int f2l(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Long = (long long) op1.Float;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int f2d(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Double = (double) op1.Float;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int d2i(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (int)op1.Double;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int d2l(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Long = (long long)op1.Double;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int d2f(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Float = (float)op1.Double;
    push_op(&(e->frame->top),op2,1);
	return 0;
}
int i2c(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (char)op1.Int;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int i2s(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (short)op1.Int;
    push_op(&(e->frame->top),op2,1);
	return 0;
}  
int lcmp(execution *e){
    operand_type op2 = pop_op(&(e->frame->top));
    operand_type op1 = pop_op(&(e->frame->top));
    if(op1.Long>op2.Long){
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Long<op2.Long) {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    } else {
        operand_type aux;
        aux.Int = 0;
        push_op(&(e->frame->top),aux,1);
    }
	return 0;
}  
int fcmpl(execution *e){
    operand_type op2 = pop_op(&(e->frame->top));
    operand_type op1 = pop_op(&(e->frame->top));
    if(op1.Float>op2.Float){
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Float<op2.Float) {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Float==op2.Float){
        operand_type aux;
        aux.Int = 0;
        push_op(&(e->frame->top),aux,1);
    } else {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    }
    return 0;
}  
int fcmpg(execution *e){
    operand_type op2 = pop_op(&(e->frame->top));
    operand_type op1 = pop_op(&(e->frame->top));
    if(op1.Float>op2.Float){
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Float<op2.Float) {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Float==op2.Float){
        operand_type aux;
        aux.Int = 0;
        push_op(&(e->frame->top),aux,1);
    } else {
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    }
    return 0;
}  
int dcmpl(execution *e){
    operand_type op2 = pop_op(&(e->frame->top));
    operand_type op1 = pop_op(&(e->frame->top));
    if(op1.Double>op2.Double){
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Double<op2.Double) {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Double==op2.Double){
        operand_type aux;
        aux.Int = 0;
        push_op(&(e->frame->top),aux,1);
    } else {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    }
    return 0;
}  
int dcmpg(execution *e){
    operand_type op2 = pop_op(&(e->frame->top));
    operand_type op1 = pop_op(&(e->frame->top));
    if(op1.Double>op2.Double){
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Double<op2.Double) {
        operand_type aux;
        aux.Int = -1;
        push_op(&(e->frame->top),aux,1);
    } else if(op1.Double==op2.Double){
        operand_type aux;
        aux.Int = 0;
        push_op(&(e->frame->top),aux,1);
    } else {
        operand_type aux;
        aux.Int = 1;
        push_op(&(e->frame->top),aux,1);
    }
    return 0;
}  
int ifeq(execution *e) {
 	operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int == 0) { 
    	e->frame->pc += off;
    }
	return 0;
}
int ifne(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int != 0) { 
        e->frame->pc += off;
    }
    return 0;
}  
int iflt(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int < 0) { 
        e->frame->pc += off;
    }
    return 0;
}  
int ifge(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int >= 0) { 
        e->frame->pc += off;
    }
    return 0;
}  
int ifgt(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int > 0) { 
        e->frame->pc += off;
    }
    return 0;
}  
int ifle(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if (value.Int <= 0) { 
        e->frame->pc += off;
    }
    return 0;
}  
int if_icmpeq(execution *e){ 
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int == value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_icmpne(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int != value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_icmplt(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int < value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_icmpge(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int >= value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_icmpgt(execution *e){
	operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int > value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_icmple(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Int <= value2.Int){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_acmpeq(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Ref == value2.Ref){
        e->frame->pc +=off;
    }
    return 0;
}  
int if_acmpne(execution *e){
    operand_type value2 = pop_op(&(e->frame->top));
    operand_type value1 =pop_op(&(e->frame->top));
    short off = (short) u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(value1.Ref != value2.Ref){
        e->frame->pc +=off;
    }
    return 0;
}  
int goto_(execution *e){
	short off = (short)u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    //operand_type op; //variavel nao utilizada
    e->frame->pc += off;
	return 0;
} 
int jsr(execution *e){
    u2 off = u2ReadFrame(e->frame);
    off-=3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    operand_type op;
    op.Ref = e->frame->pc;
    push_op(&(e->frame->top),op,1);
    e->frame->pc += off;
    return 0;
}
int ret(execution *e){
	u1 i = u1ReadFrame(e->frame);
    e->frame->pc = e->frame->local_arr[i].Ref;
    return 0;
} 
int tableswitch(execution *e){
	u1* return_pc = e->frame->pc-1;
    int delta = (e->frame->pc - e->frame->code);
    int pad_size = (4-delta%4);
    for(int i=1;i<pad_size;++i) u1ReadFrame(e->frame);
    int default_offset = (int) u4ReadFrame(e->frame); //COMPORTAMENTO ESTRANHO - REVER INSTRUCAO
    int lowcase_off = (int) u4ReadFrame(e->frame); //COMPORTAMENTO ESTRANHO - REVER INSTRUCAO
    int highcase_off = (int) u4ReadFrame(e->frame); //COMPORTAMENTO ESTRANHO - REVER INSTRUCAO
    int size_switch = (highcase_off-lowcase_off+1);
    if(size_switch<0){
        printf("ERRO. Tableswitch LOWOFF greater than HIGHOFF.\n");
        exit(1);
    }
    int* off = (int*)calloc(size_switch,sizeof(int));
    for(int i=0;i<size_switch;++i) off[i]=u4ReadFrame(e->frame);
    e->frame->pc = return_pc;
    operand_type i = pop_op(&(e->frame->top));
    if(i.Int < lowcase_off || i.Int>highcase_off)
        e->frame->pc = e->frame->pc+default_offset;
    else
        e->frame->pc = e->frame->pc +(off[i.Int-lowcase_off]);
    free(off);
    return 0;
}  
int lookupswitch(execution *e){
	u1* return_pc = e->frame->pc-1;
    operand_type a = pop_op(&(e->frame->top));
    int delta = (e->frame->pc - e->frame->code);
    int pad_size = (4-delta%4);
    for(int i=1;i<pad_size;++i) u1ReadFrame(e->frame);
    int default_offset = (int) u4ReadFrame(e->frame);
    int pairs = u4ReadFrame(e->frame);
    int off[pairs][2];
    for(int i=0;i<pairs;++i) {
        off[i][0] = u4ReadFrame(e->frame);
        off[i][1] = u4ReadFrame(e->frame);
    }
    e->frame->pc = return_pc;
    for(int i=0;i<pairs;++i)
        if(a.Int == off[i][0]){
            e->frame->pc +=off[i][1];
            return 0;
        }
    e->frame->pc +=default_offset;
    return 0;
}
int ireturn(execution *e){
    operand_type op = pop_op(&(e->frame->top)); //retira o operando-topo do frame atual
    pop_frame(&(e->frame)); //descarta o frame atual
    push_op(&(e->frame->top),op,1); //coloca o operando no topo do novo frame atual
	return 1;
}  
int lreturn(execution *e){
    operand_type op = pop_op(&(e->frame->top));
    pop_frame(&(e->frame));
    push_op(&(e->frame->top),op,2);
    return 1;
}  
int freturn(execution *e){
    operand_type op = pop_op(&(e->frame->top));
    pop_frame(&(e->frame));
    push_op(&(e->frame->top),op,1);
    return 1;
}  
int dreturn(execution *e){
    operand_type op = pop_op(&(e->frame->top));
    pop_frame(&(e->frame));
    push_op(&(e->frame->top),op,2);
    return 1;
}  
int areturn(execution *e){
    operand_type op = pop_op(&(e->frame->top));
    pop_frame(&(e->frame));
    push_op(&(e->frame->top),op,1);
    return 1;
}  
int return_(execution *e){ //finaliza sem retornar nenhum valor
	pop_frame(&(e->frame));
    return 1;
}  
int getstatic(execution *e){
    u2 fieldi = u2ReadFrame(e->frame);
    u2 classi = e->frame->constant_pool[fieldi-1].info.Fieldref_info.class_index; //VETOR COMECA EM 0
    u2 classnamei =  e->frame->constant_pool[classi-1].info.Class_info.name_index;
    u2 nameandtypei = e->frame->constant_pool[fieldi-1].info.Fieldref_info.name_and_type_index;
    u2 fieldnamei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* classname = search_utf8(e->frame->constant_pool,classnamei);
    char* fieldname = search_utf8(e->frame->constant_pool,fieldnamei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);

    if(!strcmp(classname,"java/lang/System") //
        && !strcmp(fieldname,"out")
        && !strcmp(descriptor,"Ljava/io/PrintStream;")) //Instrucao print
    {
        operand_type op;
        op.Long = 0;
        push_op(&(e->frame->top),op,1); //empilha operando inutil, depois sera retirado
    } else {
        //ClassFile* cf = check_class(e,classname); //checa a classe e carrega se ainda nao tiver sido
        check_class(e,classname); //checa a classe e carrega se ainda nao tiver sido
        field* f  = search_staticfield(e->start,classname,fieldname); //encontra o field nas classes
        if(!f){
            printf("ERRO. Field not found on getstatic.\n");
            exit(1);
        } else {
            operand_type op;
            if(descriptor[0]=='B'||descriptor[0]=='C') { //byte ou char
                op.Int = f->value.Char;
                push_op(&(e->frame->top),op,1);
            } else if(descriptor[0]=='S') { //Short
                op.Int = f->value.Short;
                push_op(&(e->frame->top),op,1);
            } else if(descriptor[0] == 'I' || descriptor[0]=='Z') { //Int
                op.Int = f->value.Int;
                push_op(&(e->frame->top),op,1);
            } else if(descriptor[0] == 'F') { //Float
                op.Float = f->value.Float;
                push_op(&(e->frame->top),op,1);
            } else if(descriptor[0]=='L' ||descriptor[0]=='['){ //Referencia
                op.Ref = f->value.Ref;
                push_op(&(e->frame->top),op,1);
            } else if(descriptor[0] =='D') { //Double
                op.Double = f->value.Double;
                push_op(&(e->frame->top),op,2);
            } else if(descriptor[0] =='J') { //Long
                op.Long = f->value.Long;
                push_op(&(e->frame->top),op,2);
            }
        }
    }
    return 0;
}

void put_static(class_heap* start, char* class, char* field, operand_type op) {
    class_heap* aux = start;
    while(aux) {
        if(!strcmp(class,search_utf8(aux->cf.constant_pool,aux->cf.constant_pool[(aux->cf.this_class)-1].info.Class_info.name_index)))
        {
            for(int s=0;s<aux->num_static;++s) {
                if(!strcmp(field,aux->static_fields[s].name)) {
                    aux->static_fields[s].value.Long = op.Long;
                    return;
                }
            }
            return;
        }
        aux=aux->next;
    }
}

int putstatic(execution *e){
    u2 fieldi = u2ReadFrame(e->frame);
    u2 classi = e->frame->constant_pool[fieldi-1].info.Fieldref_info.class_index;

    u2 classnamei =  e->frame->constant_pool[classi-1].info.Class_info.name_index;
    u2 nameandtypei = e->frame->constant_pool[fieldi-1].info.Fieldref_info.name_and_type_index;
    u2 fieldnamei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    //u2 descri = e->frame->constant_pool[nameandtypei].info.NameAndType_info.descriptor_index; //variavel utilizada apenas para definir variavel descriptor
    char* classname = search_utf8(e->frame->constant_pool,classnamei);
    char* fieldname = search_utf8(e->frame->constant_pool,fieldnamei);
    //char* descriptor = search_utf8(e->frame->constant_pool,descri); //variavel nao utilizada

    //ClassFile* cf = check_class(e,classname);
    check_class(e,classname);
    field* f  = search_staticfield(e->start,classname,fieldname);
    if(f) {
        operand_type op = pop_op(&(e->frame->top));
        put_static(e->start,classname,fieldname,op);
    } else {
        printf("ERRO: field not found on putstatic.\n");
        exit(1);
    }

    return 0;
} 
int getfield(execution *e){
    u2 i = u2ReadFrame(e->frame);
    u2 nameandtypei = e->frame->constant_pool[i-1].info.Fieldref_info.name_and_type_index;
	u2 namei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* name = search_utf8(e->frame->constant_pool,namei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);
    operand_type ref = pop_op(&(e->frame->top));
    object* o = (object*) ref.Ref;
    field* f = search_field(name,descriptor,o);
    operand_type op;
    if(descriptor[0]=='B'||descriptor[0]=='C') {
        op.Int = f->value.Char;
        push_op(&(e->frame->top),op,1);
    } else if(descriptor[0]=='S') {
        op.Int = f->value.Short;
        push_op(&(e->frame->top),op,1);
    } else if(descriptor[0] == 'I' || descriptor[0]=='Z') {
        op.Int = f->value.Int;
        push_op(&(e->frame->top),op,1);
    } else if(descriptor[0] == 'F') {
        op.Float = f->value.Float;
        push_op(&(e->frame->top),op,1);
    } else if(descriptor[0]=='L' ||descriptor[0]=='['){
        op.Ref = f->value.Ref;
        push_op(&(e->frame->top),op,1);
    } else if(descriptor[0] =='D') {
        op.Double = f->value.Double;
        push_op(&(e->frame->top),op,2);
    } else if(descriptor[0] =='J') {
        op.Long = f->value.Long;
        push_op(&(e->frame->top),op,2);
    }
    return 0;
} 
int putfield(execution *e){
    u2 i = u2ReadFrame(e->frame);
    u2 nameandtypei = e->frame->constant_pool[i-1].info.Fieldref_info.name_and_type_index;
    u2 namei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* name = search_utf8(e->frame->constant_pool,namei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);
    operand_type aux = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    object* o = (object*) ref.Ref;
    field* f = search_field(name,descriptor,o);
    if(!f) {
        printf("ERRO in putfield. field is NULL");
        exit(1);
    } else {
        f->value.Long = aux.Long;
    }
	return 0;
} 
int invokevirtual(execution *e){
    u2 methodi = u2ReadFrame(e->frame);
    u2 classi = e->frame->constant_pool[methodi-1].info.Method_info.class_index;
    u2 nameandtypei = e->frame->constant_pool[methodi-1].info.Method_info.name_and_type_index;
    u2 namei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* namec = search_utf8(e->frame->constant_pool,e->frame->constant_pool[classi-1].info.Class_info.name_index);
    char* namem = search_utf8(e->frame->constant_pool,namei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);
    if(strcmp(namec,"java/io/PrintStream") == 0
        && (strcmp(namem,"println") == 0 || strcmp(namem,"print") == 0)) //eh PrintStrim e (println ou print)
    {
        operand_type op;
        switch (descriptor[1]) { //define o tipo da variavel a ser mostrada
            case '[':
                op = pop_op(&(e->frame->top));
                printf("%p",op.Ref);
            break;
            case 'Z':
                op = pop_op(&(e->frame->top));
                printf("%s",op.Int?"true":"false");
            break;
            case 'J':
                op = pop_op(&(e->frame->top));
                printf("%lld",op.Long);
            break;
            case 'C':
                op = pop_op(&(e->frame->top));
                printf("%c",(char)op.Int);
            break;
            case 'B':
                op = pop_op(&(e->frame->top));
                printf("%d",op.Int);
            break;
            case 'I':
                op = pop_op(&(e->frame->top));
                printf("%d",op.Int);
            break;
            case 'S':
                op = pop_op(&(e->frame->top));
                printf("%hi",(short)op.Int);
            break;
            case 'D':
                op = pop_op(&(e->frame->top));
                printf("%lf",op.Double);
            break;
            case 'F':
                op = pop_op(&(e->frame->top));
                printf("%f",op.Float);
            break;
            case 'L':
                op = pop_op(&(e->frame->top));
                if(strstr(descriptor,"java/lang/String"))
                    printf("%s",(char*)op.Ref);
                else
                    printf("%p",op.Ref);
            break;
        }
        if(!strcmp(namem,"println")) printf("\n"); //inserir \n final para a instrucao println
        pop_op(&(e->frame->top));
    } else {
        int n = count_args(descriptor);
        //ClassFile* cf = check_class(e,namec);
        check_class(e,namec);
        init_methodexecution(e,namec,namem,descriptor,n);
        execute_method(e);
        
    }
	return 0;
} 
int invokespecial(execution *e){
    u2 methodi = u2ReadFrame(e->frame);
    u2 classi = e->frame->constant_pool[methodi-1].info.Method_info.class_index;
    u2 nameandtypei = e->frame->constant_pool[methodi-1].info.Method_info.name_and_type_index;
    u2 namei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* namec = search_utf8(e->frame->constant_pool,e->frame->constant_pool[classi-1].info.Class_info.name_index);
    char* namem = search_utf8(e->frame->constant_pool,namei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);
    int n = count_args(descriptor);
    //ClassFile* cf = check_class(e,namec); //variavel nao utilizada
    check_class(e,namec);
    init_methodexecution(e,namec,namem,descriptor,n);
    execute_method(e);
	return 0;
}
int invokestatic(execution *e){
    u2 methodi = u2ReadFrame(e->frame); //o frame possui apenas o indice do constant pool
    u2 classi = e->frame->constant_pool[methodi-1].info.Method_info.class_index;
    u2 nameandtypei = e->frame->constant_pool[methodi-1].info.Method_info.name_and_type_index;
    u2 namei = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.name_index;
    u2 descri = e->frame->constant_pool[nameandtypei-1].info.NameAndType_info.descriptor_index;
    char* namec = search_utf8(e->frame->constant_pool,e->frame->constant_pool[classi-1].info.Class_info.name_index);
    char* namem = search_utf8(e->frame->constant_pool,namei);
    char* descriptor = search_utf8(e->frame->constant_pool,descri);
    int n = count_args(descriptor);
    if(!(!strcmp(namec,"java/lang/Object") && !strcmp(namem,"registerNatives") && !strcmp(descriptor,"(V)")))
    {
        //ClassFile* cf = check_class(e,namec); //variavel nao utilizada
        check_class(e,namec);
        init_methodexecution(e,namec,namem,descriptor,n);
        execute_method(e);
    }
	return 0;
}

//Numero de fields estaticos da classe, necessario para execucao do New
int num_fields(execution* e, ClassFile* cf) {
    int n =0;

    if (cf->fields_count == 0) //Virtual, nao possui fields
        return 0;

    for(field_info* aux=cf->fields; aux<cf->fields+cf->fields_count;++aux) {
        if(is_static(aux->access_flags)) ++n;
    }
    ClassFile* aux = cf;
    while(aux && aux->super_class) { //obtencao de fields das superclasses
        u2 supernamei = aux->constant_pool[(aux->super_class)-1].info.Class_info.name_index;
        char* supername = NULL;
        supername = search_utf8(aux->constant_pool, supernamei); //nome da superclasse
        aux = check_class(e,supername);
        if(aux) {
            for(field_info* f = aux->fields;f<aux->fields+aux->fields_count;++f) {
                if(is_static(f->access_flags)) ++n;
            }
        }
    }
    return n;
}

void field_init(execution* e,ClassFile* cf, field* f) {
    int i=0;
    for(field_info* aux=cf->fields; aux<cf->fields+cf->fields_count;++aux) {
        char* descriptor = search_utf8(cf->constant_pool,aux->descriptor_index);
        char* name = search_utf8(cf->constant_pool,aux->name_index);
        if(is_static(aux->access_flags)) {
            f[i].descriptor = descriptor;
            f[i].name = name;
            f[i++].value.Long = 0;
        }
    }
    ClassFile* aux = cf;
    while(aux->super_class) {
        u2 supernamei = aux->constant_pool[(aux->super_class)-1].info.Class_info.name_index;
        char* supername = NULL;
        strcpy(supername,search_utf8(aux->constant_pool, supernamei));
        aux = check_class(e,supername);
        if(aux) { //superclasse encontrada
            for(field_info* a = aux->fields;a<aux->fields+aux->fields_count;++a) {
                char* descriptor = search_utf8(aux->constant_pool,a->descriptor_index);
                char* name = search_utf8(aux->constant_pool,a->name_index);
                if(is_static(aux->access_flags)) {
                    f[i].descriptor = descriptor;
                    f[i].name = name;
                    f[i++].value.Long = 0;
                }
            }
        } else {
            printf("ERRO in field_init: Not Found super.\n");
            exit(1);
        }
    }
}

int new_(execution *e){
    u2 classi = u2ReadFrame(e->frame);
    u2 classnamei = e->frame->constant_pool[classi-1].info.Class_info.name_index;
    char* classname = search_utf8(e->frame->constant_pool,classnamei);
    ClassFile* cf = check_class(e,classname);
    object* o = (object*) malloc(sizeof(object));
    o->num_fields = num_fields(e,cf);
     
    if (o->num_fields > 0) { //Caso existam fields, carrega-los
        o->fields = (field*) malloc(sizeof(field)*o->num_fields);
        field_init(e,cf,o->fields);
    }

    operand_type op;
    op.Ref = o;
    push_op(&(e->frame->top),op,1);
	return 0;
} 

int newarray(execution *e){
    vector* v = (vector*) malloc(sizeof(vector));
    v->type = u1ReadFrame(e->frame);
    operand_type c = pop_op(&(e->frame->top));
    v->size = c.Int;
    switch(v->type) {
        case BOOLEANTYPE:
        case CHARTYPE:
        case FLOATTYPE:
        case DOUBLETYPE:
        case BYTETYPE:
        case SHORTTYPE:
        case INTEGERTYPE:
        case LONGTYPE:
            v->array = (operand_type*) calloc(v->size,sizeof(operand_type));
            break;
        default:
            printf("NEWARRAY ERROR.\n");
            exit(1);
    }
    operand_type arr;
    arr.Ref = v;
    push_op(&(e->frame->top),arr,1);
	return 0;
} 
int anewarray(execution *e){
    vector* v = (vector*)calloc(1,sizeof(vector));
    u2 i = u2ReadFrame(e->frame);   //indexbyte da instrucao nao esta sendo utilizado --REVER LOGICA
    								//"The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class"
    operand_type c = pop_op(&(e->frame->top));
    if(c.Int<0) {
        printf("ANEWARRAY ERROR. size < 0.\n");
        exit(1);
    }
    v->size = c.Int;
    v->type = INTEGERTYPE;
    v->array = (operand_type*)calloc(v->size,sizeof(operand_type));
    operand_type arr;
    arr.Ref = v;
    push_op(&(e->frame->top),arr,1);
	return 0;
} 
int arraylength(execution *e){
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    operand_type l;
    l.Int = v->size;
    push_op(&(e->frame->top),l,1);
	return 0;
}

vector* alocate_multirray(int dim, int* aux) {
    vector* a = (vector*) malloc(sizeof(vector));
    vector* a2;
    if(dim == 1) {
        a->size = aux[0];
        a->type = INTEGERTYPE;
        a->array = (operand_type*) calloc(aux[0],sizeof(operand_type));
    } else {
        a->size = aux[0];
        a->type = INTEGERTYPE;
        a->array = (operand_type*) calloc(aux[0],sizeof(operand_type));
        int* auxTam = (int*)calloc(dim-1,sizeof(int)); //cppcheck aponta vazamento de memoria no ponteiro
        for(int i=1;i<dim;++i) {
            auxTam[i-1] = aux[i];
        }
        for(int i=0;i<aux[0];++i) {
            a2 = alocate_multirray(dim-1,auxTam);
            a->array[i].Ref = a2;
        }
        free(auxTam);
    }
    return a;
}

int multianewarray(execution *e){

    u2 i = u2ReadFrame(e->frame);   //indexbyte da instrucao nao esta sendo utilizado --REVER LOGICA
    								//"The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class"
    u1 dim = u1ReadFrame(e->frame);
    operand_type c[dim], arrref;
    if(dim <=0) { // CHECANDO SE UMA VARIAVEL UNSIGNED EH < 0 ?
        printf("ERRO: Dimensao invalida em multianewarray.\n");
        exit(1);
    } else {
        int aux[dim];
        for(int j=0;j<dim;++j) {
            c[j] = pop_op(&(e->frame->top));
            aux[j] = c[j].Int;
        }
        vector* v = alocate_multirray(dim,aux);
        arrref.Ref = v;
        push_op(&(e->frame->top),arrref,1);
    }
	return 0;
} 
int ifnull(execution *e){
    u2 off = u2ReadFrame(e->frame);
    operand_type op = pop_op(&(e->frame->top));
    off -= 3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(op.Ref == NULL)
    	e->frame->pc += off;
	return 0;
} 
int ifnonnull(execution *e){
    u2 off = u2ReadFrame(e->frame);
    operand_type op = pop_op(&(e->frame->top));
    off -= 3; //a leitura 'ReadFrame' de 2 bytes ja avancou pc 2 vezes; a leitura da instrucao avancou mais 1 vez
    if(op.Ref != NULL)
    	e->frame->pc += off;
	return 0;
} 
int goto_w(execution *e){ //por que nao usar u4ReadFrame?
    u2 o1 = u2ReadFrame(e->frame);
    u2 o2 = u2ReadFrame(e->frame);
    unsigned int off = o1;
    off <<= 16;
    off |= o2;
    off-=5; //a leitura 'ReadFrame' de 4 bytes ja avancou pc 4 vezes; a leitura da instrucao avancou mais 1 vez
    e->frame->pc += off;
	return 0;
} 
int jsr_w(execution *e){ //por que nao usar u4ReadFrame?
    u2 o1 = u2ReadFrame(e->frame);
    u2 o2 = u2ReadFrame(e->frame);
    unsigned int off = o1;
    off <<= 16;
    off |= o2;
    off-=5; //a leitura 'ReadFrame' de 4 bytes ja avancou pc 4 vezes; a leitura da instrucao avancou mais 1 vez
    operand_type op1;
    op1.Ref = e->frame->pc;
    push_op(&(e->frame->top),op1,1);
    e->frame->pc += off;
	return 0;
}
