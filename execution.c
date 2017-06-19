#include "execution.h"
#include "classfile.h"
#include "frame.h"
#include "types.h"

ClassFile* check_class(execution* e, char* name) {
    
}

void execute_method(execution* e) {
    int flag =0;
    while(!flag) {
        u1 i = u1ReadFrame(e->frame);
        flag = instr_array[i](e);
    }
}