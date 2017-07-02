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
int invokevirtual(execution *e); 
int invokespecial(execution *e); 
int invokestatic(execution *e); 
int new_(execution *e); 
int newarray(execution *e); 
int anewarray(execution *e); 
int arraylength(execution *e); 
int multianewarray(execution *e); 
int ifnull(execution *e); 
int ifnonnull(execution *e); 
int goto_w(execution *e); 
int jsr_w(execution *e);
extern int (*instr_array[])(execution *p) = {
    nop, 
    aconst_null,
    iconst_m1,
    iconst_0,
    iconst_1,
    iconst_2,
    iconst_3,
    iconst_4,
    iconst_5,
    lconst_0,
    lconst_1,
    fconst_0,
    fconst_1,
    fconst_2,
    dconst_0,
    dconst_1,
    bipush,
    sipush,
    ldc,
    ldc_w,
    ldc2_w,
    iload,
    lload,
    fload,
    dload,
    aload,
    iload_0,
    iload_1,
    iload_2,
    iload_3,
    lload_0,
    lload_1,
    lload_2,
    lload_3,
    fload_0,
    fload_1,
    fload_2,
    fload_3,
    dload_0,
    dload_1,
    dload_2,
    dload_3,
    aload_0,
    aload_1,
    aload_2,
    aload_3,
    iaload,
    laload,
    faload,
    daload,
    aaload,
    iaload,
    iaload,
    iaload,
    store,
    store,
    store,
    store,
    store,
    store_0,
    store_1,
    store_2,
    store_3,
    store_0,
    store_1,
    store_2,
    store_3,
    store_0,
    store_1,
    store_2,
    store_3,
    store_0,
    store_1,
    store_2,
    store_3,
    store_0,
    store_1,
    store_2,
    store_3,
    iastore,
    lastore,
    fastore,
    dastore,
    aastore,
    iastore,
    iastore,
    iastore,
    pop,
    pop2,
    dup,
    dup_x1,
    dup_x2,
    dup2,
    dup2_x1,
    dup2_x2,
    swap,
    iadd,
    ladd,
    fadd,
    dadd,
    isub,
    lsub,
    fsub,
    dsub,
    imul,
    lmul,
    fmul,
    dmul,
    idiv,
    ldiv_,
    fdiv,
    ddiv,
    irem,
    lrem,
    frem,
    drem_,
    ineg,
    lneg,
    fneg,
    dneg,
    ishl,
    lshl,
    ishr,
    lshr,
    iushr,
    lushr,
    iand,
    land,
    ior,
    lor,
    ixor,
    lxor,
    iinc,
    i2l,
    i2f,
    i2d,
    l2i,
    l2f,
    l2d,
    f2i,
    f2l,
    f2d,
    d2i,
    d2l,
    d2f,
    i2c,
    i2c,
    i2s,
    lcmp,
    fcmpl,
    fcmpg,
    dcmpl,
    dcmpg,
    ifeq,
    ifne,
    iflt,
    ifge,
    ifgt,
    ifle,
    if_icmpeq,
    if_icmpne,
    if_icmplt,
    if_icmpge,
    if_icmpgt,
    if_icmple,
    if_acmpeq,
    if_acmpne,
    goto_,
    jsr,
    ret,
    tableswitch,
    lookupswitch,
    ireturn,
    lreturn,
    freturn,
    dreturn,
    areturn,
    return_,
    getstatic,
    putstatic,
    getfield,
    putfield,
    invokevirtual,
    invokespecial,
    invokestatic,
    nop,
    nop,
    new_,
    newarray,
    anewarray,
    arraylength,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    multianewarray,
    ifnull,
    ifnonnull,
    goto_w,
    jsr_w
};
#endif
