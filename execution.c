#include "execution.h"
#include "classfile.h"
#include "frame.h"
#include "types.h"
#include "heap.h"
#include "instruction_set.h"

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

void execute_method(execution* e) {
    int flag =0;
    while(!flag) {
        u1 i = u1ReadFrame(e->frame);
        flag = instr_array[i](e);
    }
}

void init_methodexecution(execution* e,char* class,char* method, char* descriptor, int args){
    ClassFile* cf = check_class(e,class);
    push_frame(&(e->frame));
    frame_init(e->start,*cf,e->frame,method,descriptor);
    int sizeindex =0;
    if(e->frame->below) {
        operand_heap* opaux;
        init_opheap(&opaux);
        for(int i=0;i<args;++i) {
            int type = e->frame->below->top->type;
            operand_type new = pop_op(&(e->frame->below->top));
            push_op(&opaux,new,type);
            if(type==2) ++sizeindex;
            ++sizeindex;
        }
        for(int i=0;i<args;++i) {
            int type = opaux->type;
            operand_type new = pop_op(&opaux);
            push_op(&(e->frame->below->top),new,type);
        }
        for(int i=sizeindex-1;i>-1;--i) {
            if(e->frame->below->top->type == 2) --i;
            e->frame->local_arr[i] = pop_op(&(e->frame->below->top));
        }
    }
}

ClassFile* check_class(execution* e, char* name) {
    ClassFile* cf = search_classheap(e->start,name);
    if(!cf) {
        cf = load_ClassFile(name);
        push_class(&(e->start),*cf);

        if(search_method(cf,MNAME,MDESCR)) {
            init_methodexecution(e,name,MNAME,MDESCR,0);
            execute_method(e);
        }
    }    
}
