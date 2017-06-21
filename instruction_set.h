#ifndef INSTR_H
#define INSTR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execution.h"
#include "util.h"
int nop(execution *e);
int aconst_null(execution *e); 
int iconst_m1(execution *e); 
int iconst_0(execution *e); 
int iconst_1(execution *e); 
int iconst_2(execution *e); 
int iconst_3(execution *e); 
int iconst_4(execution *e); 
int iconst_5(execution *e); 
int lconst_0(execution *e); 
int lconst_1(execution *e); 
int fconst_0(execution *e); 
int fconst_1(execution *e); 
int fconst_2(execution *e); 
int dconst_0(execution *e); 
int dconst_1(execution *e); 
int bipush(execution *e); 
int sipush(execution *e);
int ldc(execution *e); 
int ldc_w(execution *e); 
int ldc2_w(execution *e); 
int iload(execution *e); 
int lload(execution *e); 
int fload(execution *e); 
int dload(execution *e); 
int aload(execution *e); 
int iload_0(execution *e); 
int iload_1(execution *e); 
int iload_2(execution *e); 
int iload_3(execution *e); 
int lload_0(execution *e); 
int lload_1(execution *e); 
int lload_2(execution *e); 
int lload_3(execution *e); 
int fload_0(execution *e); 
int fload_1(execution *e); 
int fload_2(execution *e); 
int fload_3(execution *e); 
int dload_0(execution *e); 
int dload_1(execution *e); 
int dload_2(execution *e); 
int dload_3(execution *e); 
int aload_0(execution *e); 
int aload_1(execution *e); 
int aload_2(execution *e); 
int aload_3(execution *e); 
int iaload(execution *e); 
int laload(execution *e); 
int faload(execution *e); 
int daload(execution *e); 
int aaload(execution *e); 
int store(execution *e); 
int store_0(execution *e); 
int store_1(execution *e); 
int store_2(execution *e); 
int store_3(execution *e); 
int iastore(execution *e); 
int lastore(execution *e); 
int fastore(execution *e); 
int dastore(execution *e); 
int aastore(execution *e); 
int pop(execution *e); 
int pop2(execution *e); 
int dup(execution *e); 
int dup_x1(execution *e); 
int dup_x2(execution *e); 
int dup2(execution *e); 
int dup2_x1(execution *e); 
int dup2_x2(execution *e); 
int swap(execution *e);  
int iadd(execution *e);  
int ladd(execution *e);  
int fadd(execution *e);  
int dadd(execution *e);  
int isub(execution *e);  
int lsub(execution *e);  
int fsub(execution *e);  
int dsub(execution *e);  
int imul(execution *e);  
int lmul(execution *e);  
int fmul(execution *e);  
int dmul(execution *e);  
int idiv(execution *e);  
int ldiv_(execution *e);  
int fdiv(execution *e);  
int ddiv(execution *e);  
int irem(execution *e);  
int lrem(execution *e);  
int frem(execution *e);  
int drem_(execution *e);  
int ineg(execution *e);  
int lneg(execution *e);  
int fneg(execution *e);  
int dneg(execution *e);  
int ishl(execution *e);  
int lshl(execution *e);  
int ishr(execution *e);  
int lshr(execution *e);  
int iushr(execution *e); 
int lushr(execution *e); 
int iand(execution *e);  
int land(execution *e);  
int ior(execution *e);  
int lor(execution *e);  
int ixor(execution *e);  
int lxor(execution *e);  
int iinc(execution *e); 
int i2l(execution *e);  
int i2f(execution *e);  
int i2d(execution *e);  
int l2i(execution *e);  
int l2f(execution *e);  
int l2d(execution *e);  
int f2i(execution *e);  
int f2l(execution *e);  
int f2d(execution *e);  
int d2i(execution *e);  
int d2l(execution *e);  
int d2f(execution *e);  
int i2c(execution *e);  
int i2s(execution *e);  
int lcmp(execution *e);  
int fcmpl(execution *e);  
int fcmpg(execution *e);  
int dcmpl(execution *e);  
int dcmpg(execution *e);  
int ifeq(execution *e);  
int ifne(execution *e);  
int iflt(execution *e);  
int ifge(execution *e);  
int ifgt(execution *e);  
int ifle(execution *e);  
int if_icmpeq(execution *e);  
int if_icmpne(execution *e);  
int if_icmplt(execution *e);  
int if_icmpge(execution *e);  
int if_icmpgt(execution *e);  
int if_icmple(execution *e);  
int if_acmpeq(execution *e);  
int if_acmpne(execution *e);  
int goto_(execution *e); 
int jsr(execution *e); 
int ret(execution *e); 
int tableswitch(execution *e); 
int lookupswitch(execution *e); 
int ireturn(execution *e);  
int lreturn(execution *e);  
int freturn(execution *e);  
int dreturn(execution *e);  
int areturn(execution *e);  
int return_(execution *e);  
int getstatic(execution *e); 
int putstatic(execution *e); 
int getfield(execution *e); 
int putfield(execution *e); 
// int invokevirtual(execution *e); 
// int invokespecial(execution *e); 
// int invokestatic(execution *e); 
int new_(execution *e); 
int newarray(execution *e); 
int anewarray(execution *e); 
int arraylength(execution *e); 
int multianewarray(execution *e); 
int ifnull(execution *e); 
int ifnonnull(execution *e); 
int goto_w(execution *e); 
int jsr_w(execution *e);
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
    // invokevirtual,// 0xB6
    // invokespecial,// 0xB7
    // invokestatic,// 0xB8
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
#endif