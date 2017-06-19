#include "types.h"
#include "classfile.h"
#include "util.h"
#include "heap.h"

void classheap_init(class_heap** ch) { *ch=NULL; }

int is_static(u2 access_flag) {
    return (access_flag & FSTATIC) != 0;
}

void push_class(class_heap** ch,ClassFile cf) {
    class_heap* aux1 = (class_heap*)malloc(sizeof(class_heap));
    aux1->cf = cf;
    aux1->num_static = 0;
    aux1->next = NULL;
    field_info* field_aux;
    for(field_aux=cf.fields;field_aux<cf.fields+cf.fields_count;++field_aux) {
        if(is_static(field_aux->access_flags)) {
            aux1->num_static++;
        }
    }

    aux1->static_fields = (field*) malloc(sizeof(field)*aux1->num_static);
    int static_index=0;
    for(field_aux=cf.fields;field_aux<cf.fields+cf.fields_count;++field_aux) {
        if(is_static(field_aux->access_flags)){
            aux1->static_fields[static_index].name = search_utf8(cf.constant_pool,field_aux->name_index);
            aux1->static_fields[static_index].name = search_utf8(cf.constant_pool,field_aux->descriptor_index);
            aux1->static_fields[static_index].value.Long =0;
            ++static_index;
        }
    }
    class_heap* move;
    aux1->objects = NULL;
    if(!(*ch)) *ch = aux1;
    else{
        move = *ch;
        while(move->next) move = move->next;
        move->next = aux1;
    }
}

ClassFile* search_classheap(class_heap* ch, char* name) {
    class_heap* aux = ch;
    while(aux) {
        if(!strcmp(name,
            search_utf8(aux->cf.constant_pool,
                        aux->cf.constant_pool[aux->cf.this_class].info.Class_info.name_index)))
        {
            return &(aux->cf);
        }
        aux = aux->next;
    }
    return NULL;
}
