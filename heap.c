#include "types.h"
#include "classfile.h"
#include "util.h"
#include "heap.h"

void free_fields(field* static_fields){
}

void free_classheap(class_heap* ch){
  //tem que ser para cada no do heap
  free_clFile(&(ch->cf));
  free(ch->static_fields);
  //free_objects(ch->objects);
}

/*!
 * Inicializa o heap da classe
 * @param[in] ch  Heap a ser inicializado
 *
 * Atribui NULL à heap ch.
 * */
void classheap_init(class_heap** ch) { *ch=NULL; }

/*!
 * Verifica se o metodo e' estatico
 * @param[in]   access_flag   Flag de acesso do metodo
 * @param[out]  bool          Verdadeiro ou falso
 *
 * Verifica se a flag do metodo e' FSTATIC.
 * */
int is_static(u2 access_flag) {
    return (access_flag & FSTATIC) != 0;
}

/*!
 * A classe e' colocada na heap de classes carregadas.
 * @param[in] ch  Heap com as classes carregadas
 * @param[in] cf  Estrutra que contem os dados do arquivo ponto Class
 *
 * O algoritmo consiste em duas etapas. Na primera, os dados do cf sao passados
 * para uma variavel auxiliar do tipo Class_heap, e na segunda, procura-se o
 * final da heap, onde insere-se essa variavel auxiliar
 * */
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
            aux1->static_fields[static_index].descriptor = search_utf8(cf.constant_pool,field_aux->descriptor_index);
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

/*!
 * Procura por uma classe no heap de classes ja' carregadas.
 * @param[in]   ch          Ponteiro para o topo da heap de classes carregadas
 * @param[in]   name        Nome da classe a ser carregada
 * @param[out]  ClassFile   Ponteiro para o descritor da classe encontrada
 *
 * O heap ch e' percorrido ate que seja encontrada a classe com o nome desejado.
 * E' retornado nulo, caso contrario.
 * */
ClassFile* 
search_classheap(class_heap* ch, char* name) {
    class_heap* aux = ch;
    while(aux) {
        if(!strcmp(name,
            search_utf8(aux->cf.constant_pool,aux->cf.constant_pool[(aux->cf.this_class)-1].info.Class_info.name_index)
                ))
        {
            return &(aux->cf); //retorna a classe pesquisada
        }
        aux = aux->next;
    }
    return NULL;
}

field* search_field(char* name, char* descr, object* o) {
    field* aux;
    for(aux = o->fields;aux<o->fields+o->num_fields;++aux) {
        if(!strcmp(name,aux->name)
            && !strcmp(descr,aux->descriptor)) {
            return aux;
        }
    }
    return NULL;
}

field* search_staticfield(class_heap* start, char* class, char* field) {
    class_heap* aux = start;
    while(aux) {
        if(!strcmp(class,search_utf8(aux->cf.constant_pool,
            aux->cf.constant_pool[(aux->cf.this_class)-1].info.Class_info.name_index))) { //classe encontrada
            for(int i=0;i<aux->num_static;++i) {
                if(!strcmp(field,aux->static_fields[i].name))
                    return &(aux->static_fields[i]);
            }
            return NULL;
        }
        aux=aux->next;
    }
    return NULL;
}
