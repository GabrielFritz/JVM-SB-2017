#include <math.h>
#include "instruction_set.h"
#include "execution.h"
#include "classfile.h"
#include "types.h"
#include "frame.h"
#include "heap.h"

int (*instr_array[])(execution *p) = {
    nop, // 0x00
    aconst_null,// 0x1
    iconst_m1,// 0x2
    iconst_0,// 0x3
    iconst_1,// 0x4
    iconst_2,// 0x5
    iconst_3,// 0x6
    iconst_4,// 0x7
    iconst_5,// 0x8
    lconst_0,// 0x9
    lconst_1,// 0xA
    fconst_0,// 0xB
    fconst_1,// 0xC
    fconst_2,// 0xD
    dconst_0,// 0xE
    dconst_1,// 0xF
    bipush,// 0x10
    sipush,// 0x11
    ldc,// 0x12
    ldc_w,// 0x13
    ldc2_w,// 0x14
    iload,// 0x15
    lload,// 0x16
    fload,// 0x17
    dload,// 0x18
    aload,// 0x19
    iload_0,// 0x1A
    iload_1,// 0x1B
    iload_2,// 0x1C
    iload_3,// 0x1D
    lload_0,// 0x1E
    lload_1,// 0x1F
    lload_2,// 0x20
    lload_3,// 0x21
    fload_0,// 0x22
    fload_1,// 0x23
    fload_2,// 0x24
    fload_3,// 0x25
    dload_0,// 0x26
    dload_1,// 0x27
    dload_2,// 0x28
    dload_3,// 0x29
    aload_0,// 0x2A
    aload_1,// 0x2B
    aload_2,// 0x2C
    aload_3,// 0x2D
    iaload,// 0x2E
    laload,// 0x2F
    faload,// 0x30
    daload,// 0x31
    aaload,// 0x32
    iaload,// 0x33
    iaload,// 0x34
    iaload,// 0x35
    store,// 0x36
    store,// 0x37
    store,// 0x38
    store,// 0x39
    store,// 0x3A
    store_0,// 0x3B
    store_1,// 0x3C
    store_2,// 0x3D
    store_3,// 0x3E
    store_0,// 0x3F
    store_1,// 0x40
    store_2,// 0x41
    store_3,// 0x42
    store_0,// 0x43
    store_1,// 0x44
    store_2,/// 0x45
    store_3,// 0x46
    store_0,// 0x47
    store_1,// 0x48
    store_2,/// 0x49
    store_3,// 0x4A
    store_0,// 0x4B
    store_1,// 0x4C
    store_2,/// 0x4D
    store_3,// 0x4E
    iastore,// 0x4F
    lastore,// 0x50
    fastore,// 0x51
    dastore,// 0x52
    aastore,// 0x53
    iastore,// 0x54
    iastore,// 0x55
    iastore,// 0x56
    pop,// 0x57
    pop2,// 0x58
    dup,//0x59
    dup_x1,//0x5A
    dup_x2,//0x5B
    dup2,// 0x5C
    dup2_x1,// 0x5D
    dup2_x2,// 0x5E
    swap,// 0x5F
    iadd,// 0x60
    ladd,// 0x61
    fadd,// 0x62
    dadd,// 0x63
    isub,// 0x64
    lsub,// 0x65
    fsub,// 0x66
    dsub,// 0x67
    imul,// 0x68
    lmul,// 0x69
    fmul,// 0x6A
    dmul,// 0x6B
    idiv,// 0x6C
    ldiv_,// 0x6D
    fdiv,// 0x6E
    ddiv,// 0x6F
    irem,// 0x70
    lrem,// 0x71
    frem,// 0x72
    drem_,// 0x73
    ineg,// 0x74
    lneg,// 0x75
    fneg,// 0x76
    dneg,// 0x77
    ishl,// 0x78
    lshl,// 0x79
    ishr,// 0x7A
    lshr,// 0x7B
    iushr,// 0x7C
    lushr,// 0x7D
    iand,// 0x7E
    land,// 0x7F
    ior,// 0x80
    lor,// 0x81
    ixor,// 0x82
    lxor,// 0x83
    iinc,// 0x84
    i2l,// 0x85
    i2f,// 0x86
    i2d,// 0x87
    l2i,// 0x88
    l2f,// 0x89
    l2d,// 0x8A
    f2i,// 0x8B
    f2l,// 0x8C
    f2d,// 0x8D
    d2i,// 0x8E
    d2l,// 0x8F
    d2f,// 0x90
    i2c,// 0x91
    i2c,// 0x92
    i2s,// 0x93
    lcmp,// 0x94
    fcmpl,// 0x95
    fcmpg,// 0x96
    dcmpl,// 0x97
    dcmpg,// 0x98
    ifeq,// 0x99
    ifne,// 0x9A
    iflt,// 0x9B
    ifge,// 0x9C
    ifgt,// 0x9D
    ifle,// 0x9E
    if_icmpeq,// 0x9F
    if_icmpne,// 0xA0
    if_icmplt,// 0xA1
    if_icmpge,// 0xA2
    if_icmpgt,// 0xA3
    if_icmple,// 0xA4
    if_acmpeq,// 0XA5
    if_acmpne,// 0xA6
    goto_,// 0xA7
    jsr,// 0xA8
    ret,// 0xA9
    tableswitch,// 0xAA
    lookupswitch,// 0xAB
    ireturn,// 0xAC
    lreturn,// 0xAD
    freturn,// 0xAE
    dreturn,// 0xAF
    areturn,// 0xB0
    return_,// 0xB1
    getstatic,// 0xB2
    putstatic,// 0xB3
    getfield,// 0xB4
    putfield,// 0xB5
    invokevirtual,// 0xB6
    invokespecial,// 0xB7
    invokestatic,// 0xB8
    nop,//nop,// 0xB9
    nop,//nop,// 0xBA
    new_,// 0xBB
    newarray,// 0xBC
    anewarray,// 0xBD
    arraylength,// 0xBE
    nop,//athrow,// 0xBF
    nop,//checkcast,// 0xC0
    nop,//instanceof,// 0xC1
    nop,//nop,// 0xC2
    nop,//nop,// 0xC3
    nop,//wide,// 0xC4
    multianewarray,// 0xC5
    ifnull,// 0xC6
    ifnonnull,// 0xC7
    goto_w,// 0xC8
    jsr_w// 0xC9
};

u1 u1ReadFrame(frame* f) { return *(++f->pc) }

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

int nop(execution* e) {
    return 0;
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
	push_op(&(e->frame->top),op,1);
	return 0;
} 
int sipush(execution *e){
    operand_type op;
    op.Int = u2ReadFrame(e->frame);
	push_op(&(e->frame->top),op,1);
	return 0;
}
u1 search_tag(cp_info* cp, u2 i) { return cp[i].tag; }
int search_int(cp_info* cp, u2 i) { return (int)cp[i].info.Integer_info.bytes;}
int search_float(cp_info* cp, u2 i) {
    u4tofloat.U4 = cp[i].info.Float_info.bytes;
    return u4tofloat.Float;
}
int ldc(execution *e){
    operand_type op;
    u1 i = u1ReadFrame(e->frame);
    switch (search_tag(e->frame->constant_pool,i)) {
        case INTEGERTYPE:
            op.Int = search_int(e->frame->constant_pool,i);
        break;
        case FLOAT:
            op.Float = search_float(e->frame->constant_pool,i);
        break;
        case STRING:
            op.Ref = (char*) search_utf8(e->frame->constant_pool,
                                        e->frame->constant_pool[i].info.String_info.string_index);
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
        case INTEGERTYPE:
            op.Int = search_int(e->frame->constant_pool,i);
        break;
        case FLOAT:
            op.Float = search_float(e->frame->constant_pool,i);
        break;
        case STRING:
            op.Ref = (char*) search_utf8(e->frame->constant_pool,
                                        e->frame->constant_pool[i].info.String_info.string_index);
        break;
        default:
            printf("Invalid index for LDC.\n");
            exit(1);

    }
    push_op(&(e->frame->top),op,1);
    return 0;
}

double search_double(cp_info* cp, u2 i) {
    longtodouble.Long = ((long long) cp[i].info.Double_info.high_bytes << 32) | (cp[i].info.Double_info.low_bytes);
    return longtodouble.Double;
}

long long search_long(cp_info* cp, u2 i) {
    long long Long = ((long long) cp[i].info.Long_info.high_bytes << 32) | (cp[i].info.Long_info.low_bytes);
    return Long;
}

int ldc2_w(execution *e){
    operand_type op;
    u2 i = u2ReadFrame(e->frame);
    switch (search_tag(e->frame->constant_pool,i)) {
        case DOUBLE:
            op.Int = search_double(e->frame->constant_pool,i);
        break;
        case LONG:
            op.Float = search_long(e->frame->constant_pool,i);
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
    vector v = arr_ref.Ref;
	push_op(&(e->frame->top),v->arr[i.Int],1);
	return 0;
} 
int laload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector v = arr_ref.Ref;
    push_op(&(e->frame->top),v->arr[i.Int],2);
	return 0;
} 
int faload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector v = arr_ref.Ref;
    push_op(&(e->frame->top),v->arr[i.Int],1);
	return 0;
} 
int daload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector v = arr_ref.Ref;
    push_op(&(e->frame->top),v->arr[i.Int],2);
	return 0;
} 
int aaload(execution *e){
	operand_type i = pop_op(&(e->frame->top));
    operand_type arr_ref = pop_op(&(e->frame->top));
    vector v = arr_ref.Ref;
    push_op(&(e->frame->top),v->arr[i.Int],1);
    return 0;
} 
int store(execution *e){
    e->frame->local_arr[u1ReadFrame(e->frame)] = pop_op(&(e->frame->top));
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
    v->arr[i.Int].Int = value.Int;
    return 0;
} 
int lastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->arr[i.Int].Long = value.Long;
	return 0;
} 
int fastore(execution *e){
	operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->arr[i.Int].Float = value.Float;
    return 0;
} 
int dastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->arr[i.Int].Double = value.Double;
	return;
} 
int aastore(execution *e){
    operand_type value = pop_op(&(e->frame->top));
    operand_type i = pop_op(&(e->frame->top));
    operand_type ref = pop_op(&(e->frame->top));
    vector* v = ref.Ref;
    v->arr[i.Int].Ref = value.Ref;
	return;
} 
int pop(execution *e){
	if(e->frame->top->type==1) pop_op(&(e->frame->top));
    else {
        printf("Type 2 POP Error.\n");
        exit(1);
    }
    return 0;
} 
int pop2(execution *e){
	if(e->frame->top->type==1 && e->frame->top->below->type==1) {
        pop_op(&(p->frame->top));
        pop_op(&(p->frame->top));
    }
    else {
        pop_op(&(p->frame->top));
    }
    return 0;
} 
int dup(execution *e){
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
    push_op(&(e->frame->top),op1,type);
    push_op(&(e->frame->top),op3,type);
    push_op(&(e->frame->top),op2,type);
    push_op(&(e->frame->top),op1,type);
	return 0;
} 
int dup2(execution *e){
    int type1 = e->frame->top->type;
    operand_type op1 = pop_op(&(e->frame->top));
    if(type1==1) {
        int type2 = e->frame->top->type;
        operand_type op2 = pop_op(&(e->frame->top));
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
    if(type1==1) {
        int type2 = e->frame->top->type;
        operand_type op2 = pop_op(&(e->frame->top));
    }
    int type3 = e->frame->top->type;
    operand_type op3 = pop_op(&(e->frame->top)); 
    if(type1==1) {
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type2);
        push_op(&(e->frame->top),op2,type2);
        push_op(&(e->frame->top),op1,type1);
    } else {
        push_op(&(e->frame->top),op1,type1);
        push_op(&(e->frame->top),op3,type1);
        push_op(&(e->frame->top),op1,type1);
    }
	return 0;
} 
int dup2_x2(execution *e){
    int type1 = e->frame.top.type;
    operand_type op1 = pop_op(&(e->frame->top));
    if(type1==1) {
        int type2 = e->frame.top.type;
        operand_type op2 = pop_op(&(e->frame->top));
    }
    int type3 = e->frame.top.type;
        operand_type op3 = pop_op(&(e->frame->top)); 
    if(type3 == 1) {
        int type4 = e->frame.top.type;
        operand_type op4 = pop_op(&(e->frame->top));  
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
        push_op(&(e->frame->top),op2,type);
        push_op(&(e->frame->top),op1,type);
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
    op1.Int -= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long -= op2.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int fsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float -= op2.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int dsub(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Double -= op2.Double;
    push_op(&(e->frame->top),op1,2);
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
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int /= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int ldiv_(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long /= op2.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int fdiv(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float /= op2.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int ddiv(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Double /= op2.Double;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int irem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int %= op2.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lrem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long %= op2.Long;
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int frem(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float = remainder(op1.Float,op2.Float);
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int drem_(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Double = remainder(op1.Double,op2.Double);
    push_op(&(e->frame->top),op1,2);
	return 0;
}  
int ineg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int = 0 - op1.Int;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int lneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Long = 0 - op1.Long;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int fneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Float = 0 - op1.Float;
    push_op(&(e->frame->top),op1,1);
	return 0;
}  
int dneg(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
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
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2 = pop_op(&(e->frame->top));
    op1.Int &= 0x1F;
    int n=0;
    if(op2.Int<0) {
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
    int n=0;
    if(op2.Long<0) {
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
    char c = u1ReadFrame(e->frame);
    e->frame->local_arr[i].Int++;
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
    op2.Float = (double) op1.Int;
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
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
int i2s(execution *e){
    operand_type op1 = pop_op(&(e->frame->top));
    operand_type op2;
    op2.Int = (short)op1.Int;
    push_op(&(e->frame->top),op2,2);
	return 0;
}  
// int lcmp(execution *e){
// 	return;
// }  
// int fcmpl(execution *e){
// 	return;
// }  
// int fcmpg(execution *e){
// 	return;
// }  
// int dcmpl(execution *e){
// 	return;
// }  
// int dcmpg(execution *e){
// 	return;
// }  
int ifeq(execution *e) {
 	operand_type value = pop_op(&(e->frame->top));
    int8_t o1 = u1ReadFrame(e->frame);
    int8_t o2 = u1ReadFrame(e->frame);
    int16_t off = o1;
    
    if (value.Int == 0) { //unica diferenca entre ifeq, ifne, iflt, ifge, ifgt, ifle sera essa comparacao
    	off <<= 8;
    	off |= o2;
    	off-=3; //por que 3?
    	e->frame->pc += off; // (valor -2 para voltar para a instrucao ifeq)
    }
    else {
		e->frame->pc++; //incremento de instrucao
    }
	return 0;
}
// int ifne(execution *e){
// 	return;
// }  
// int iflt(execution *e){
// 	return;
// }  
// int ifge(execution *e){
// 	return;
// }  
// int ifgt(execution *e){
// 	return;
// }  
// int ifle(execution *e){
// 	return;
// }  
// int if_icmpeq(execution *e){ //SEMELHANTE A ifeq MAS FAZ POP EM OUTRO OPERANDO E COMPARA OS DOIS, EM VEZ DE COMPARAR COM 0
// 	return;
// }  
// int if_icmpne(execution *e){
// 	return;
// }  
// int if_icmplt(execution *e){
// 	return;
// }  
// int if_icmpge(execution *e){
// 	return;
// }  
// int if_icmpgt(execution *e){
// 	return;
// }  
// int if_icmple(execution *e){
// 	return;
// }  
// int if_acmpeq(execution *e){
// 	return;
// }  
// int if_acmpne(execution *e){
// 	return;
// }  
int goto_(execution *e){
	int8_t o1 = u1ReadFrame(e->frame);
    int8_t o2 = u1ReadFrame(e->frame);
    int16_t off = o1;
    off <<= 8;
    off |= o2;
    off-=3; //por que 3?
    e->frame->pc += off;
	return 0;
} 
// int jsr(execution *e){
// 	return;
// } 
// int ret(execution *e){
// 	return;
// } 
// int tableswitch(execution *e){
// 	return;
// } 
// int lookupswitch(execution *e){
// 	return;
// } 
// int ireturn(execution *e){
// 	return;
// }  
// int lreturn(execution *e){
// 	return;
// }  
// int freturn(execution *e){
// 	return;
// }  
// int dreturn(execution *e){
// 	return;
// }  
// int areturn(execution *e){
// 	return;
// }  
// int return_(execution *e){
// 	return;
// }  
// int getstatic(execution *e){
// 	return;
// } 
// int putstatic(execution *e){
// 	return;
// } 
// int getfield(execution *e){
// 	return;
// } 
// int putfield(execution *e){
// 	return;
// } 
// int invokevirtual(execution *e){
// 	return;
// } 
// int invokespecial(execution *e){
// 	return;
// }
// int invokestatic(execution *e){
// 	return;
// }
// int new_(execution *e){
// 	return 0;
// } 
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
    u2 i = u2ReadFrame(e->frame);
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
        int* auxTam = (int*)calloc(dim-1,sizeof(int));
        for(int i=1;i<dim;++i) {
            auxTam[i-1] = aux[i];
        }
        for(int i=0;i<aux[0];++i) {
            a2 = alocate_multirray(dim-1,auxTam);
            a->array[i].Ref = a2;
        }
    }
    return a;
}

int multianewarray(execution *e){

    int i = u2ReadFrame(e->frame);
    u1 dim = u1ReadFrame(e->frame);
    operand_type c[dim], arrref;
    int aux[dim];
    if(dim <=0) {
        printf("ERRO: Dimensao invalida em multianewarray.\n");
        exit(1);
    } else {
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
    off -= 3;
    if(op.Ref == NULL) e->frame->pc += off;
	return 0;
} 
int ifnonnull(execution *e){
    u2 off = u2ReadFrame(e->frame);
    operand_type op = pop_op(&(e->frame->top));
    off -= 3;
    if(op.Ref != NULL) e->frame->pc += off;
	return 0;
} 
int goto_w(execution *e){ //por que nau usar u4ReadFrame?
    u2 o1 = u2ReadFrame(e->frame);
    u2 o2 = u2ReadFrame(e->frame);
    unsigned int off = o1;
    off <<= 16;
    off |= o2;
    off-=5;
    e->frame->pc += off;
	return 0;
} 
int jsr_w(execution *e){ //por que nau usar u4ReadFrame?
    u2 o1 = u2ReadFrame(e->frame);
    u2 o2 = u2ReadFrame(e->frame);
    unsigned int off = o1;
    off <<= 16;
    off |= o2;
    off-=5;
    operand_type op1;
    op1.Ref = e->frame->pc;
    push_op(&(e->frame->top),op1,1);
    e->frame->pc += off;
	return 0;
} 