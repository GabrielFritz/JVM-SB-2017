#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "classloader.h"

void load_magic(ClassFile* cf, FILE* fd) {
    cf->magic = u4Read(fd);
    /*if (cf->magic != 0xcafebabe)
        EXIT; //arquivo não eh .class
    */
}

void load_versions(ClassFile* cf, FILE* fd) {
    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
}

void load_constantpool(ClassFile* cf, FILE* fd) {
    cf->constant_pool_count = u2Read(fd);
    if (cf->constant_pool_count <= 1) {
        cf->constant_pool = NULL;
        return;
    }
    cf->constant_pool = (cp_info*) calloc((cf->constant_pool_count - 1), sizeof(cp_info));
    cp_info *cp;
    for (cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; ++cp) {
        cp->tag = u1Read(fd);
        switch (cp->tag) {
        case CLASS:
            cp->info.Class_info.name_index = u2Read(fd);
            break;
        case FIELDREF:
            cp->info.Fieldref_info.class_index = u2Read(fd);
            cp->info.Fieldref_info.name_and_type_index = u2Read(fd);
            break;
        case METHOD:
            cp->info.Method_info.class_index = u2Read(fd);
            cp->info.Method_info.name_and_type_index = u2Read(fd);
            break;
        case INTERFACE:
            cp->info.Interface_info.class_index = u2Read(fd);
            cp->info.Interface_info.name_and_type_index = u2Read(fd);
            break;
        case NAMEANDTYPE:
            cp->info.NameAndType_info.name_index = u2Read(fd);
            cp->info.NameAndType_info.descriptor_index = u2Read(fd);
            break;
        case UTF8:
            cp->info.Utf8_info.length = u2Read(fd);
            cp->info.Utf8_info.bytes = (u1*)calloc(cp->info.Utf8_info.length+1, sizeof(u1)); //length diz o numero de bytes UTF8 desse cp_info
            u1* b;
            for (b = cp->info.Utf8_info.bytes ; b < cp->info.Utf8_info.bytes + cp->info.Utf8_info.length ; ++b) { //laco para leitura desses bytes
                *b = u1Read(fd);
            }
            break;
        case STRING:
            cp->info.String_info.string_index = u2Read(fd);
            break;
        case INTEGER:
            cp->info.Integer_info.bytes = u4Read(fd);
            break;
        case FLOAT:
            cp->info.Float_info.bytes = u4Read(fd);
            break;
        case LONG:
            cp->info.Long_info.high_bytes = u4Read(fd); //estrutura de 64 bytes
            cp->info.Long_info.low_bytes = u4Read(fd);
            break;
        case DOUBLE:
            cp->info.Double_info.high_bytes = u4Read(fd); //estrutura de 64 bytes
            cp->info.Double_info.low_bytes = u4Read(fd);
            break;
        }
        if (cp->tag == LONG || cp->tag == DOUBLE) {
            ++cp;
            cp->tag = -1;
        }
    }
}

void load_classdata(ClassFile* cf, FILE* fd) {
    cf->access_flags = u2Read(fd);
    cf->this_class = u2Read(fd);
    cf->super_class = u2Read(fd);
}

void load_interfaces(ClassFile* cf, FILE* fd) {
    cf->interfaces_count = u2Read(fd);
    if (cf->interfaces_count == 0) {
        cf->interfaces = NULL;
        return;
    }
    cf->interfaces = (u2*) calloc(cf->interfaces_count, sizeof(u2));
    u2* bytes;
    for (bytes = cf->interfaces; bytes < cf->interfaces + cf->interfaces_count; ++bytes) {
        *bytes = u2Read(fd);
    }
}

void load_constantvalue_attr(attribute_info* att, FILE* fd) {
    att->type.ConstantValue.constantvalue_index = u2Read(fd);
}

void load_code_attr(attribute_info* att, ClassFile* cf, FILE* fd) {
    att->type.Code.max_stack = u2Read(fd);
    att->type.Code.max_locals = u2Read(fd);
    att->type.Code.code_length = u4Read(fd);
    if (att->type.Code.code_length == 0) {
        att->type.Code.code = NULL;
    } else {
        att->type.Code.code = (u1*)calloc(att->type.Code.code_length, sizeof(u1));
        u1* byte;
        for (byte = att->type.Code.code; byte < att->type.Code.code + att->type.Code.code_length; ++byte) {
            *byte = u1Read(fd);
        }
    }
    att->type.Code.exception_table_length = u2Read(fd);
    if (att->type.Code.exception_table_length == 0) {
        att->type.Code.exception_table = NULL;
    } else {
        att->type.Code.exception_table = (exception_table_info*)calloc(att->type.Code.exception_table_length,sizeof(exception_table_info));
        exception_table_info* exp_aux;
        for (exp_aux = att->type.Code.exception_table; exp_aux < att->type.Code.exception_table + att->type.Code.exception_table_length; ++exp_aux) {
            exp_aux->start_pc = u2Read(fd);
            exp_aux->end_pc = u2Read(fd);
            exp_aux->handler_pc = u2Read(fd);
            exp_aux->catch_type = u2Read(fd);
        }
    }
    att->type.Code.attributes_count = u2Read(fd);
    if (att->type.Code.attributes_count == 0) {
        att->type.Code.attributes = NULL;
    } else {
        att->type.Code.attributes = (attribute_info*)calloc(att->type.Code.attributes_count,sizeof(attribute_info));
        attribute_info* att_aux;
        for (att_aux = att->type.Code.attributes; att_aux < att->type.Code.attributes + att->type.Code.attributes_count; ++att_aux) {
            load_attribute(att_aux, cf, fd);
        }
    }
}

void load_exceptions_attr(attribute_info* att, FILE* fd) {
    att->type.Exceptions.number_of_exceptions = u2Read(fd);
    if (att->type.Exceptions.number_of_exceptions == 0) {
        att->type.Exceptions.exception_index_table = NULL;
    } else {
        att->type.Exceptions.exception_index_table = (u2*) calloc(att->type.Exceptions.number_of_exceptions,sizeof(u2));
        u2* bytes;
        for (bytes = att->type.Exceptions.exception_index_table; bytes < att->type.Exceptions.exception_index_table + att->type.Exceptions.number_of_exceptions; ++bytes) {
            *bytes = u2Read(fd);
        }
    }
}

void load_innerclasses_attr(attribute_info* att, FILE* fd) {
    att->type.InnerClasses.number_of_classes = u2Read(fd);
    if (att->type.InnerClasses.number_of_classes == 0) {
        att->type.InnerClasses.classes = NULL;
        return;
    }
    att->type.InnerClasses.classes = (classtype_info*) calloc(att->type.InnerClasses.number_of_classes,sizeof(classtype_info));
    classtype_info* classtype_aux;
    for (classtype_aux = att->type.InnerClasses.classes; classtype_aux < att->type.InnerClasses.classes + att->type.InnerClasses.number_of_classes; ++classtype_aux) {
        classtype_aux->inner_class_info_index = u2Read(fd);
        classtype_aux->outer_class_info_index = u2Read(fd);
        classtype_aux->inner_name_index = u2Read(fd);
        classtype_aux->inner_class_access_flags = u2Read(fd);
    }
}

void load_other_attr(attribute_info* att, FILE* fd) {
    if (att->attribute_length == 0) {
        att->type.Other.bytes = NULL;
        return;
    }
    att->type.Other.bytes = (u1*) calloc(att->attribute_length,sizeof(u1));
    u1* bytes;
    for (bytes = att->type.Other.bytes; bytes < att->type.Other.bytes + att->attribute_length; ++bytes) {
        *bytes = u1Read(fd);
    }
}

void load_attribute(attribute_info* att, ClassFile* cf, FILE* fd) {
    char* type;
    att->attribute_name_index = u2Read(fd);
    att->attribute_length = u4Read(fd);
    type = (char*)calloc(cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1,sizeof(char));
      strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
      int i = findtype(type);
      switch (i) {
      case CONSTANTVALUE:
          load_constantvalue_attr(att, fd);
          break;
      case CODE:
          load_code_attr(att, cf, fd);
          break;
      case EXCEPTIONS:
          load_exceptions_attr(att, fd);
          break;
      case INNERCLASSES:
          load_innerclasses_attr(att, fd);
          break;
      case OTHER:
          load_other_attr(att, fd);
          break;
      }
      free(type);
  }

  void load_fields(ClassFile* cf, FILE* fd) {
      cf->fields_count = u2Read(fd);
      if (cf->fields_count == 0) {
          cf->fields = NULL;
          return;
      }
      cf->fields = (field_info*)calloc(cf->fields_count,sizeof(field_info));
      field_info* field_aux;
      for (field_aux = cf->fields; field_aux < cf->fields + cf->fields_count; ++field_aux) {
          field_aux->access_flags = u2Read(fd);
          field_aux->name_index = u2Read(fd);
          field_aux->descriptor_index = u2Read(fd);
          field_aux->attributes_count = u2Read(fd);
          field_aux->attributes = calloc(field_aux->attributes_count,sizeof(attribute_info));
          attribute_info* attribute_aux;
          for (attribute_aux = field_aux->attributes; attribute_aux < field_aux->attributes + field_aux->attributes_count; ++attribute_aux) {
              load_attribute(attribute_aux, cf, fd);
          }
      }
  }

  void load_methods(ClassFile* cf, FILE* fd) {
      cf->method_count = u2Read(fd);
      if (cf->method_count == 0) {
          cf->methods = NULL;
          return;
      }
      cf->methods = (method_info*) calloc(cf->method_count, sizeof(method_info));
      method_info* method_aux;
      for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
          method_aux->access_flags = u2Read(fd);
          method_aux->name_index = u2Read(fd);
          method_aux->descriptor_index = u2Read(fd);
          method_aux->attributes_count = u2Read(fd);
          method_aux->attributes = (attribute_info*) calloc(method_aux->attributes_count, sizeof(attribute_info));
          attribute_info* att_aux;
          for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux) {
              load_attribute(att_aux, cf, fd);
          }
      }
  }

  void load_attributes(ClassFile* cf, FILE* fd) {
      cf->attributes_count = u2Read(fd);
      if (cf->attributes_count == 0) {
          cf->attributes = NULL;
          return;
      }
      cf->attributes = (attribute_info*) calloc(cf->attributes_count, sizeof(attribute_info));
      attribute_info* att_aux;
      for (att_aux = cf->attributes; att_aux < cf->attributes + cf->attributes_count; ++att_aux) {
          load_attribute(att_aux, cf, fd);
      }
  }

ClassFile* readClass(FILE* fd) {
  ClassFile* cf = (ClassFile*) calloc(1,sizeof(ClassFile));
  load_magic(cf, fd);
  load_versions(cf, fd);
  load_constantpool(cf, fd);
  load_classdata(cf, fd);
  load_interfaces(cf, fd);
  load_fields(cf, fd);
  load_methods(cf, fd);
  load_attributes(cf, fd);
  return cf;
}


int is_true(int code, int id) {
    return code & (1 << id);
}

void print_permissions(int code, FILE* fout) {
    fprintf(fout, "[");
    if (is_true(code, 0)) fprintf(fout, "public ");
    else if (is_true(code, 1)) fprintf(fout, "private ");
    else if (is_true(code, 2)) fprintf(fout, "protected ");

    if (is_true(code, 3)) fprintf(fout, "static ");
    if (is_true(code, 4)) fprintf(fout, "final ");
    if (is_true(code, 5)) fprintf(fout, "super ");
    if (is_true(code, 6)) fprintf(fout, "volatile ");
    if (is_true(code, 7)) fprintf(fout, "transient ");
    if (is_true(code, 8)) fprintf(fout, "native ");
    if (is_true(code, 9)) fprintf(fout, "interface ");
    if (is_true(code, 10)) fprintf(fout, "abstract ");
    fprintf(fout, "]");
}

void print_magic(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MAGIC: %x\n\n", cf->magic);
}

void print_versions(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MINOR VERSION: %d\n", cf->minor_version);
    char *nome_versao =  show_version(cf->major_version);
    fprintf(fout, "MAJOR VERSION: %d - %s\n", cf->major_version, nome_versao);
    free(nome_versao);

    fprintf(fout, "CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);

    fprintf(fout, "ACCESS_FLAGS: %x ", cf->access_flags);
    print_permissions(cf->access_flags, fout);
    fprintf(fout, "\n");
    fprintf(fout, "THIS_CLASS: %d\n", cf->this_class);
    fprintf(fout, "SUPER_CLASS: %d\n", cf->super_class);

    fprintf(fout, "INTERFACES_COUNT: %d\n", cf->interfaces_count);
    fprintf(fout, "FIELDS_COUNT: %d\n", cf->fields_count);
    fprintf(fout, "METHODS_COUNT: %d\n", cf->method_count);
    fprintf(fout, "ATTRIBUTES_COUNT: %d\n\n", cf->attributes_count);

}

void print_constantpool(ClassFile* cf, FILE* fout) {
    int i = 1;
    long long Long;
    fprintf(fout, "CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);
    fprintf(fout, "CONSTANT_POOL:\n");
    cp_info* cp;
    for (cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; ++cp) {
        fprintf(fout, "[%d]\n", i++);
        switch (cp->tag) {
        case CLASS:
            fprintf(fout, "\tCP_INFO: CLASS\n");
            fprintf(fout, "\tNAME_INDEX: %d: %s\n\n", cp->info.Class_info.name_index, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes);
            break;
        case FIELDREF:
            fprintf(fout, "\tCP_INFO: FIELDREF\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Fieldref_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Fieldref_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case METHOD:
            fprintf(fout, "\tCP_INFO: METHOD\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Method_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Method_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case INTERFACE:
            fprintf(fout, "\tCP_INFO: INTERFACE\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Interface_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Interface_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case NAMEANDTYPE:
            fprintf(fout, "\tCP_INFO: NAMEANDTYPE\n");
            fprintf(fout, "\tNAME_INDEX: %d: %s\n", cp->info.NameAndType_info.name_index, (char*)cf->constant_pool[cp->info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n\n", cp->info.NameAndType_info.descriptor_index, (char*)cf->constant_pool[cp->info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case UTF8:
            fprintf(fout, "\tCP_INFO: UTF8\n");
            fprintf(fout, "\tLENGTH: %d\n", cp->info.Utf8_info.length);
            fprintf(fout, "\tVALUE: %s\n\n", (char*)cp->info.Utf8_info.bytes);
            break;
        case STRING:
            fprintf(fout, "\tCP_INFO: STRING\n");
            fprintf(fout, "\tSTRING_INDEX: %d: %s\n\n", cp->info.String_info.string_index, (char*)cf->constant_pool[cp->info.String_info.string_index - 1].info.Utf8_info.bytes);
            break;
        case INTEGER:
            fprintf(fout, "\tCP_INFO: INTEGER\n");
            fprintf(fout, "\tBYTES: %x\n", cp->info.Integer_info.bytes);
            fprintf(fout, "\tVALUE: %u\n\n", cp->info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(fout, "\tCP_INFO: FLOAT\n");
            fprintf(fout, "\tBYTES: %x\n", cp->info.Float_info.bytes);
            u4tofloat.U4 = cp->info.Float_info.bytes;
            fprintf(fout, "\tVALUE: %f\n\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(fout, "\tCP_INFO: LONG\n");
            fprintf(fout, "\tHIGH: %x\n", cp->info.Long_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cp->info.Long_info.low_bytes);
            Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        case DOUBLE:
            fprintf(fout, "\tCP_INFO: DOUBLE\n");
            fprintf(fout, "\tHIGH: %x\n", cp->info.Double_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cp->info.Double_info.low_bytes);
            Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        }
    }

}

void print_classdata(ClassFile* cf, FILE* fout) {
    fprintf(fout, "ACCESS_FLAGS: %x ", cf->access_flags);
    print_permissions(cf->access_flags, fout);
    fprintf(fout, "\n");
    fprintf(fout, "THIS_CLASS: %d\n", cf->this_class);
    fprintf(fout, "SUPER_CLASS: %d\n\n", cf->super_class);
}

void print_interfaces(ClassFile* cf, FILE* fout) {
    fprintf(fout, "INTERFACES_COUNT: %d\n", cf->interfaces_count);
    if (cf->interfaces_count == 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "INTERFACES:\n");
    u2* interface_aux;
    for (interface_aux = cf->interfaces; interface_aux < cf->interfaces + cf->interfaces_count; ++interface_aux) {
        fprintf(fout, "\tINTERFACE: %d\n\n", *interface_aux);
    }
}

void print_attribute(ClassFile* cf, attribute_info* att, FILE* fout) {

char* tiponewarray_conteudo[] = {NULL, NULL, NULL, NULL, "T_BOOLEAN", "T_CHAR", "T_FLOAT", "T_DOUBLE", "T_BYTE", "T_SHORT","T_INT", "T_LONG"};

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

enum instrucoes_code { //10 instrucoes por linha
    nop, aconst_null, iconst_m1, iconst_0, iconst_1, iconst_2, iconst_3, iconst_4, iconst_5, lconst_0, //0 ao 9
    lconst_1, fconst_0, fconst_1, fconst_2, dconst_0, dconst_1, bipush, sipush, ldc, ldc_w,
    ldc2_w, iload, lload, fload,dload, aload, iload_0, iload_1, iload_2, iload_3,
    lload_0, lload_1, lload_2, lload_3, fload_0, fload_1, fload_2, fload_3, dload_0, dload_1,
    dload_2, dload_3, aload_0, aload_1, aload_2, aload_3, iaload, laload, faload, daload,
    aaload, baload, caload, saload, istore, lstore, fstore, dstore, astore, istore_0,
    istore_1, istore_2, istore_3, lstore_0, lstore_1, lstore_2, lstore_3, fstore_0, fstore_1, fstore_2,
    fstore_3, dstore_0, dstore_1, dstore_2, dstore_3, astore_0, astore_1, astore_2, astore_3, iastore,
    lastore, fastore, dastore, aastore, bastore, castore, sastore, pop, pop2, dup,
    dup_x1, dup_x2, dup2, dup2_x1, dup2_x2, swap, iadd, ladd, fadd, dadd,
    isub, lsub, fsub, dsub, imul, lmul, fmul, dmul, idiv, ldiv, //100 ao 109
    fdiv, ddiv, irem, lrem, frem, drem, ineg, lneg, fneg, dneg,
    ishl, lshl, ishr, lshr, iushr, lushr, iand, land, ior, lor,
    ixor, lxor, iinc, i2l, i2f, i2d, l2i, l2f, l2d, f2i,
    f2l, f2d, d2i, d2l, d2f, i2b, i2c, i2s, lcmp, fcmpl,
    fcmpg, dcmpl, dcmpg, ifeq, ifne, iflt, ifge,ifgt, ifle, if_icmpeq, //150 ao 159
    if_icmpne, if_icmplt, if_icmpge, if_icmpgt, if_icmple, if_acmpeq, if_acmpne, goto2, jsr, ret,
    tableswitch, lookupswitch, ireturn, lreturn, freturn, dreturn, areturn, return2, getstatic, putstatic,
    getfield, putfield, invokevirtual, invokespecial, invokestatic, invokeinterface, invokedynamic, new, newarray, anewarray,
    arraylength, athrow, checkcast, instanceof, monitorenter, monitorexit, wide, multianewarray, ifnull, ifnonnull,
    goto_w, jsr_w, breakpoint, impdep1 = 254, impdep2 = 255 //250 ao 255
};

    long long Long;
    char* type;
    type = (char*)malloc(sizeof(char) * cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1);
    strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    int i = findtype(type);
    fprintf(fout, "\tATTRIBUTE_NAME_INDEX: %d : %s\n", att->attribute_name_index, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    fprintf(fout, "\tATTRIBUTE_LENGTH: %u\n\n", att->attribute_length);
    switch (i) {
    case CONSTANTVALUE:
        fprintf(fout, "\tTYPE: CONSTANT_VALUE\n");
        fprintf(fout, "\tCONSTANTVALUE_INDEX: %d\n\n", att->type.ConstantValue.constantvalue_index);
        switch (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].tag) {
        case INTEGER:
            fprintf(fout, "\tCP_INFO: INTEGER\n");
            fprintf(fout, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            fprintf(fout, "\tVALUE: %u\n\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(fout, "\tCP_INFO: FLOAT\n");
            fprintf(fout, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes);
            u4tofloat.U4 = cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes;
            fprintf(fout, "\tVALUE: %f\n\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(fout, "\tCP_INFO: LONG\n");
            fprintf(fout, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        case DOUBLE:
            fprintf(fout, "\tCP_INFO: DOUBLE\n");
            fprintf(fout, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        }
        break;
    case CODE:
        fprintf(fout, "\tTYPE: CODE\n");
        fprintf(fout, "\tMAX_STACK: %d\n", att->type.Code.max_stack);
        fprintf(fout, "\tMAX_LOCALS: %d\n", att->type.Code.max_locals);
        fprintf(fout, "\tCODE_LENGTH: %u\n", att->type.Code.code_length);
        fprintf(fout, "\tCODE:\n");
        u1* code;
        cp_info* cp;
        for (code = att->type.Code.code; code < att->type.Code.code + att->type.Code.code_length; ++code) {
            //fprintf(fout, "\t\t%d | %02x | ", (int) (code - (att->type.Code.code)), *code); //printa o codigo em hexa da instrucao e a instrucao em questao
            fprintf(fout, "\t\t%d | ", (int) (code - (att->type.Code.code))); //printa a instrucao sem o codigo em hexa

            fprintf(fout, "%s ", instrucoes_nomes[*code]); //printa a instrucao
            int8_t byte_aux;
            int16_t half_aux;
            int32_t word_aux;
            
            switch (*code) { //TRATAR INSTRUCOES QUE ARMAZENAM OPERANDOS NA PILHA
                case aload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case anewarray:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case astore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case bipush:
                    fprintf(fout, "%d", *(++code)); //byte
                    break;
                case checkcast:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case dload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case dstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case fload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case fstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case getfield:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case getstatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case goto2: //REVER - MOSTRANDO VALOR INESPERADO
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case goto_w:
                    word_aux = *(++code); //byte1 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte2 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte3 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte4 de branch
                    fprintf(fout, "%d", word_aux); //print branchbyte

                    break;
                case if_acmpeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_acmpne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmple:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpgt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpge:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmplt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifgt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case iflt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifle:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifge:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifnonnull:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifnull:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case iinc:
                    byte_aux = *(++code); //byte de index
                    fprintf(fout, "%d by %d", byte_aux, *(++code)); //print branchbyte
                    break;
                case iload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case instanceof:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case invokedynamic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    break;
                case invokeinterface:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    fprintf(fout, " count %d", *(++code)); //count
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    break;
                case invokespecial:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case invokestatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case invokevirtual:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case istore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case jsr:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case jsr_w:
                    word_aux = *(++code); //byte1 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte2 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte3 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte4 de branch
                    fprintf(fout, "%d", word_aux); //print branchbyte
                    break;
                case ldc:
                    byte_aux = *(++code); //index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d", half_aux); //CONTEUDO PODE SER STRING, FLOAT...
                    break;
                case ldc_w:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case ldc2_w:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case lload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case lstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case multianewarray:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    fprintf(fout, " dim %d", *(++code)); //dimensions
                    break;
                case new:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d, <%s>", half_aux, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes); //print indexbyte
                    break;
                case newarray:
                    fprintf(fout, "%s", tiponewarray_conteudo[*(++code)]); //string referente a codificacao do tipo em questao
                    break;
                case putfield:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index

                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case putstatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case ret:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case sipush:
                    half_aux = *(++code); //byte1
                        half_aux = half_aux << 8; //shift
                        half_aux += *(++code); //byte2
                        fprintf(fout, "%d", half_aux); //print
                    break;
                case wide:
                    if ((*(++code)) == iinc) { //caso o proximo byte seja iinc
                        half_aux = *(++code); //byte1 de index
                        half_aux = half_aux << 8; //shift de index
                        half_aux += *(++code); //byte2 de index
                        fprintf(fout, "#%d", half_aux); //print indexbyte
                        
                        half_aux = *(++code); //byte1 de constante
                        half_aux = half_aux << 8; //shift de constante
                        half_aux += *(++code); //byte2 de constante
                        fprintf(fout, " const %d", half_aux); //print constante
                    }
                    else {
                        half_aux = *(++code); //byte1 de index
                        half_aux = half_aux << 8; //shift de index
                        half_aux += *(++code); //byte2 de index
                        fprintf(fout, "#%d", half_aux); //print indexbyte
                    }
                    break;

                case lookupswitch: //REVER - ESTUDAR SOBRE
                    break;
                case tableswitch: //REVER - ESTUDAR SOBRE
                    break;
            }


            fprintf(fout, "\n");
        }
        fprintf(fout, "\tEXCEPTION_TABLE_LENGTH: %d\n", att->type.Code.exception_table_length);
        exception_table_info* exp_aux;
        for (exp_aux = att->type.Code.exception_table; exp_aux < att->type.Code.exception_table + att->type.Code.exception_table_length; ++exp_aux) {
            fprintf(fout, "\tEXCEPTION:\n");
            fprintf(fout, "\t\tSTART_PC: %d\n", exp_aux->start_pc);
            fprintf(fout, "\t\tEND_PC: %d\n", exp_aux->end_pc);
            fprintf(fout, "\t\tHANDLER_PC: %d\n", exp_aux->handler_pc);
            fprintf(fout, "\t\tCATCH_TYPE: %d\n\n", exp_aux->catch_type);
        }
        fprintf(fout, "\tATTRIBUTES_COUNT: %d\n", att->type.Code.attributes_count);
        attribute_info* att_aux;
        for (att_aux = att->type.Code.attributes; att_aux < att->type.Code.attributes + att->type.Code.attributes_count; ++att_aux) {
            print_attribute(cf, att_aux, fout);
        }
        break;
    case EXCEPTIONS:
        fprintf(fout, "\tTYPE: EXCEPTIONS\n");
        fprintf(fout, "\tNUMBER_OF_EXCEPTIONS: %d\n", att->type.Exceptions.number_of_exceptions);
        u2* expt_aux;
        for (expt_aux = att->type.Exceptions.exception_index_table; expt_aux < att->type.Exceptions.exception_index_table + att->type.Exceptions.number_of_exceptions; ++expt_aux) {
            fprintf(fout, "\tEXCEPTION:\n");
            fprintf(fout, "\t\tCLASS: %d\n\n", *expt_aux);
        }
        break;
    case INNERCLASSES:
        fprintf(fout, "\tTYPE: INNER CLASSES:\n");
        fprintf(fout, "\tNUMBER_OF_CLASSES: %d\n", att->type.InnerClasses.number_of_classes);
        classtype_info* classtype_aux;
        for (classtype_aux = att->type.InnerClasses.classes; classtype_aux < att->type.InnerClasses.classes + att->type.InnerClasses.number_of_classes; ++classtype_aux) {
            fprintf(fout, "\tINNER CLASS:\n");
            fprintf(fout, "\t\tINNER CLASS: %d\n", classtype_aux->inner_class_info_index);
            fprintf(fout, "\t\tOUTER CLASS: %d\n", classtype_aux->outer_class_info_index);
            fprintf(fout, "\t\tINNER NAME: %d\n", classtype_aux->inner_name_index);
            fprintf(fout, "\t\tINNER CLASS ACCESS FLAGS: %x ", classtype_aux->inner_class_access_flags);
            print_permissions(classtype_aux->inner_class_access_flags, fout);
            fprintf(fout, "\n\n");
            fprintf(fout, "\n");
        }
        break;
    case OTHER:
        break;
    }
    free(type);
}

void print_fields(ClassFile* cf, FILE* fout) {
    int i1 = 0, i2 = 0;
    fprintf(fout, "FIELDS_COUNT: %d\n", cf->fields_count);
    if (cf->fields_count == 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "FIELDS:\n");
    field_info* field_aux;
    for (field_aux = cf->fields; field_aux < cf->fields + cf->fields_count; ++field_aux) {
        fprintf(fout, "\t[%d]\n", i1++);
        fprintf(fout, "\tNAME_INDEX: %d: %s\n", field_aux->name_index, (char*)cf->constant_pool[field_aux->name_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n", field_aux->descriptor_index, (char*)cf->constant_pool[field_aux->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tACCESS_FLAGS: %x ", field_aux->access_flags);
        print_permissions(field_aux->access_flags, fout);
        fprintf(fout, "\n");
        fprintf(fout, "\tATTRIBUTE_COUNT: %d\n\n", field_aux->attributes_count);
        attribute_info* att_aux;
        for (att_aux = field_aux->attributes; att_aux < field_aux->attributes + field_aux->attributes_count; ++att_aux) {
            fprintf(fout, "[%d] FIELD_ATTRIBUTE:\n", i2++);
            print_attribute(cf, att_aux, fout);
        }
    }
}

void print_methods(ClassFile* cf, FILE* fout) {
    int i1 = 0, i2 = 0;
    fprintf(fout, "METHODS_COUNT: %d\n", cf->method_count);
    if (cf->method_count == 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "METHODS:\n");
    method_info* method_aux;
    for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
        fprintf(fout, "[%d]\n", i1++);
        fprintf(fout, "\tNAME_INDEX: %d: %s\n", method_aux->name_index, (char*)cf->constant_pool[method_aux->name_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n", method_aux->descriptor_index, (char*)cf->constant_pool[method_aux->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tACCESS_FLAGS: %x ", method_aux->access_flags);
        print_permissions(method_aux->access_flags, fout);
        fprintf(fout, "\n\n");
        fprintf(fout, "\tATTRIBUTE_COUNT: %d\n\n", method_aux->attributes_count);
        attribute_info* att_aux;
        for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux) {
            fprintf(fout, "\t[%d] METHOD_ATTRIBUTE:\n", i2++);
            print_attribute(cf, att_aux, fout);
        }
    }
}

void print_attributes(ClassFile* cf, FILE* fout) {
    int i = 0;
    fprintf(fout, "ATTRIBUTES_COUNT: %d\n", cf->attributes_count);
    if (cf->attributes_count == 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "ATTRIBUTES:\n");
    attribute_info* att_aux;
    for (att_aux = cf->attributes; att_aux < cf->attributes + cf->attributes_count; ++att_aux) {
        fprintf(fout, "[%d] ATTRIBUTE:\n", i++);
        print_attribute(cf, att_aux, fout);
    }
}

void print_class(ClassFile* cf, char* nomearquivo, FILE* fout) {
    fprintf(fout, "Nome do .class: %s\n\n", nomearquivo);
    fprintf(fout, "----------------------------------------------\n\n");
    print_magic(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_versions(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    // print_classdata(cf, fout);
    // fprintf(fout, "----------------------------------------------\n\n");
    print_constantpool(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_interfaces(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_fields(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_methods(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_attributes(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
}

char* show_version(int code) {
//Minor Version funciona como subversão
    
    char* nome_versao;
    switch (code) {
        case 45:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.1");
            break;
        case 46:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.2");
            break;
        case 47:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.3");
            break;
        case 48:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.4");
            break;
        case 49:
            nome_versao = (char*) malloc(sizeof(char) * 12);
            strcpy(nome_versao, "Java SE 5.0");
            break;
        case 50:
            nome_versao = (char*) malloc(sizeof(char) * 12);
            strcpy(nome_versao, "Java SE 6.0");
            break;
        case 51:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 7");
            break;
        case 52:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 8");
            break;
        case 53:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 9");
            break;
        default:
            nome_versao = (char*) malloc(sizeof(char) * 21);
            strcpy(nome_versao, "Java nao reconhecido");
            break;
    }
    return nome_versao;
}


void load_opcodes()
{
    opcodes = (opcode*)malloc(sizeof(opcode)*256);

    strcpy(opcodes[0x00].sign, "nop");
    strcpy(opcodes[0x01].sign, "aconst_null");
    strcpy(opcodes[0x02].sign, "iconst_m1");
    strcpy(opcodes[0x03].sign, "iconst_0");
    strcpy(opcodes[0x04].sign, "iconst_1");
    strcpy(opcodes[0x05].sign, "iconst_2");
    strcpy(opcodes[0x06].sign, "iconst_3");
    strcpy(opcodes[0x07].sign, "iconst_4");
    strcpy(opcodes[0x08].sign, "iconst_5");
    strcpy(opcodes[0x09].sign, "lconst_0");
    strcpy(opcodes[0x0a].sign, "lconst_1");
    strcpy(opcodes[0x0b].sign, "fconst_0");
    strcpy(opcodes[0x0c].sign, "fconst_1");
    strcpy(opcodes[0x0d].sign, "fconst_2");
    strcpy(opcodes[0x0e].sign, "dconst_0");
    strcpy(opcodes[0x0f].sign, "dconst_1");
    strcpy(opcodes[0x10].sign, "bipush");
    strcpy(opcodes[0x11].sign, "sipush");
    strcpy(opcodes[0x12].sign, "ldc");
    strcpy(opcodes[0x13].sign, "ldc_w");
    strcpy(opcodes[0x14].sign, "ldc2_w");
    strcpy(opcodes[0x15].sign, "iload");
    strcpy(opcodes[0x16].sign, "lload");
    strcpy(opcodes[0x17].sign, "fload");
    strcpy(opcodes[0x18].sign, "dload");
    strcpy(opcodes[0x19].sign, "aload");
    strcpy(opcodes[0x1a].sign, "iload_0");
    strcpy(opcodes[0x1b].sign, "iload_1");
    strcpy(opcodes[0x1c].sign, "iload_2");
    strcpy(opcodes[0x1d].sign, "iload_3");
    strcpy(opcodes[0x1e].sign, "lload_0");
    strcpy(opcodes[0x1f].sign, "lload_1");
    strcpy(opcodes[0x20].sign, "lload_2");
    strcpy(opcodes[0x21].sign, "lload_3");
    strcpy(opcodes[0x22].sign, "fload_0");
    strcpy(opcodes[0x23].sign, "fload_1");
    strcpy(opcodes[0x24].sign, "fload_2");
    strcpy(opcodes[0x25].sign, "fload_3");
    strcpy(opcodes[0x26].sign, "dload_0");
    strcpy(opcodes[0x27].sign, "dload_1");
    strcpy(opcodes[0x28].sign, "dload_2");
    strcpy(opcodes[0x29].sign, "dload_3");
    strcpy(opcodes[0x2a].sign, "aload_0");
    strcpy(opcodes[0x2b].sign, "aload_1");
    strcpy(opcodes[0x2c].sign, "aload_2");
    strcpy(opcodes[0x2d].sign, "aload_3");
    strcpy(opcodes[0x2e].sign, "iaload");
    strcpy(opcodes[0x2f].sign, "laload");
    strcpy(opcodes[0x31].sign, "daload");
    strcpy(opcodes[0x32].sign, "aaload");
    strcpy(opcodes[0x33].sign, "baload");
    strcpy(opcodes[0x34].sign, "caload");
    strcpy(opcodes[0x35].sign, "saload");
    strcpy(opcodes[0x36].sign, "istore");
    strcpy(opcodes[0x37].sign, "lstore");
    strcpy(opcodes[0x38].sign, "fstore");
    strcpy(opcodes[0x39].sign, "dstore");
    strcpy(opcodes[0x3a].sign, "astore");
    strcpy(opcodes[0x3b].sign, "istore_0");
    strcpy(opcodes[0x3c].sign, "istore_1");
    strcpy(opcodes[0x3d].sign, "istore_2");
    strcpy(opcodes[0x3e].sign, "istore_3");
    strcpy(opcodes[0x3f].sign, "lstore_0");
    strcpy(opcodes[0x40].sign, "lstore_1");
    strcpy(opcodes[0x41].sign, "lstore_2");
    strcpy(opcodes[0x42].sign, "lstore_3");
    strcpy(opcodes[0x43].sign, "fstore_0");
    strcpy(opcodes[0x44].sign, "fstore_1");
    strcpy(opcodes[0x45].sign, "fstore_2");
    strcpy(opcodes[0x46].sign, "fstore_3");
    strcpy(opcodes[0x47].sign, "dstore_0");
    strcpy(opcodes[0x48].sign, "dstore_1");
    strcpy(opcodes[0x49].sign, "dstore_2");
    strcpy(opcodes[0x4a].sign, "dstore_3");
    strcpy(opcodes[0x4b].sign, "astore_0");
    strcpy(opcodes[0x4c].sign, "astore_1");
    strcpy(opcodes[0x4d].sign, "astore_2");
    strcpy(opcodes[0x4e].sign, "astore_3");
    strcpy(opcodes[0x4f].sign, "iastore");
    strcpy(opcodes[0x50].sign, "lastore");
    strcpy(opcodes[0x51].sign, "fastore");
    strcpy(opcodes[0x52].sign, "dastore");
    strcpy(opcodes[0x53].sign, "aastore");
    strcpy(opcodes[0x54].sign, "bastore");
    strcpy(opcodes[0x55].sign, "castore");
    strcpy(opcodes[0x56].sign, "sastore");
    strcpy(opcodes[0x57].sign, "pop");
    strcpy(opcodes[0x58].sign, "pop2");
    strcpy(opcodes[0x59].sign, "dup");
    strcpy(opcodes[0x5a].sign, "dup_x1");
    strcpy(opcodes[0x5b].sign, "dup_x2");
    strcpy(opcodes[0x5c].sign, "dup2");
    strcpy(opcodes[0x5d].sign, "dup2_x1");
    strcpy(opcodes[0x5e].sign, "dup2_x2");
    strcpy(opcodes[0x60].sign, "iadd");
    strcpy(opcodes[0x61].sign, "ladd");
    strcpy(opcodes[0x62].sign, "fadd");
    strcpy(opcodes[0x63].sign, "dadd");
    strcpy(opcodes[0x64].sign, "isub");
    strcpy(opcodes[0x65].sign, "lsub");
    strcpy(opcodes[0x66].sign, "fsub");
    strcpy(opcodes[0x67].sign, "dsub");
    strcpy(opcodes[0x68].sign, "imul");
    strcpy(opcodes[0x69].sign, "lmul");
    strcpy(opcodes[0x6a].sign, "fmul");
    strcpy(opcodes[0x6b].sign, "dmul");
    strcpy(opcodes[0x6c].sign, "idiv");
    strcpy(opcodes[0x6d].sign, "ldiv");
    strcpy(opcodes[0x6e].sign, "fdiv");
    strcpy(opcodes[0x6f].sign, "ddiv");
    strcpy(opcodes[0x70].sign, "irem");
    strcpy(opcodes[0x71].sign, "lrem");
    strcpy(opcodes[0x72].sign, "frem");
    strcpy(opcodes[0x73].sign, "drem");
    strcpy(opcodes[0x74].sign, "ineg");
    strcpy(opcodes[0x75].sign, "lneg");
    strcpy(opcodes[0x76].sign, "fneg");
    strcpy(opcodes[0x77].sign, "dneg");
    strcpy(opcodes[0x78].sign, "ishl");
    strcpy(opcodes[0x79].sign, "lshl");
    strcpy(opcodes[0x7a].sign, "ishr");
    strcpy(opcodes[0x7b].sign, "lshr");
    strcpy(opcodes[0x7c].sign, "iushr");
    strcpy(opcodes[0x7d].sign, "lushr");
    strcpy(opcodes[0x7e].sign, "iand");
    strcpy(opcodes[0x7f].sign, "land");
    strcpy(opcodes[0x80].sign, "ior");
    strcpy(opcodes[0x81].sign, "lor");
    strcpy(opcodes[0x82].sign, "ixor");
    strcpy(opcodes[0x83].sign, "lxor");
    strcpy(opcodes[0x84].sign, "iinc");
    strcpy(opcodes[0x85].sign, "i2l");
    strcpy(opcodes[0x86].sign, "i2f");
    strcpy(opcodes[0x87].sign, "i2d");
    strcpy(opcodes[0x88].sign, "l2i");
    strcpy(opcodes[0x89].sign, "l2f");
    strcpy(opcodes[0x8a].sign, "l2d");
    strcpy(opcodes[0x8b].sign, "f2i");
    strcpy(opcodes[0x8c].sign, "f2l");
    strcpy(opcodes[0x8d].sign, "f2d");
    strcpy(opcodes[0x8e].sign, "d2i");
    strcpy(opcodes[0x8f].sign, "d2l");
    strcpy(opcodes[0x90].sign, "d2f");
    strcpy(opcodes[0x91].sign, "i2b");
    strcpy(opcodes[0x92].sign, "i2c");
    strcpy(opcodes[0x93].sign, "i2s");
    strcpy(opcodes[0x94].sign, "lcmp");
    strcpy(opcodes[0x95].sign, "fcmpl");
    strcpy(opcodes[0x96].sign, "fcmpg");
    strcpy(opcodes[0x97].sign, "dcmpl");
    strcpy(opcodes[0x98].sign, "dcmpg");
    strcpy(opcodes[0x99].sign, "ifeq");
    strcpy(opcodes[0x9a].sign, "ifne");
    strcpy(opcodes[0x9b].sign, "iflt");
    strcpy(opcodes[0x9c].sign, "ifge");
    strcpy(opcodes[0x9d].sign, "ifgt");
    strcpy(opcodes[0x9e].sign, "ifle");
    strcpy(opcodes[0x9f].sign, "if_icmpeq");
    strcpy(opcodes[0xa0].sign, "if_icmpne");
    strcpy(opcodes[0xa1].sign, "if_icmplt");
    strcpy(opcodes[0xa2].sign, "if_icmpge");
    strcpy(opcodes[0xa3].sign, "if_icmpgt");
    strcpy(opcodes[0xa4].sign, "if_icmple");
    strcpy(opcodes[0xa5].sign, "if_acmpeq");
    strcpy(opcodes[0xa6].sign, "if_acmpne");
    strcpy(opcodes[0xa7].sign, "goto");
    strcpy(opcodes[0xa8].sign, "jsr");
    strcpy(opcodes[0xa9].sign, "ret");
    strcpy(opcodes[0xaa].sign, "tableswitch");
    strcpy(opcodes[0xab].sign, "lookupswitch");
    strcpy(opcodes[0xac].sign, "ireturn");
    strcpy(opcodes[0xad].sign, "lreturn");
    strcpy(opcodes[0xae].sign, "freturn");
    strcpy(opcodes[0xaf].sign, "dreturn");
    strcpy(opcodes[0xb0].sign, "areturn");
    strcpy(opcodes[0xb1].sign, "return");
    strcpy(opcodes[0xb2].sign, "getstatic");
    strcpy(opcodes[0xb3].sign, "putstatic");
    strcpy(opcodes[0xb4].sign, "getfield");
    strcpy(opcodes[0xb5].sign, "putfield");
    strcpy(opcodes[0xb6].sign, "invokevirtual");
    strcpy(opcodes[0xb7].sign, "invokespecial");
    strcpy(opcodes[0xb8].sign, "invokestatic");
    strcpy(opcodes[0xb9].sign, "invokeinterface");
    strcpy(opcodes[0xbb].sign, "new");
    strcpy(opcodes[0xbc].sign, "newarray");
    strcpy(opcodes[0xbd].sign, "anewarray");
    strcpy(opcodes[0xbe].sign, "arraylength");
    strcpy(opcodes[0xc4].sign, "wide");
    strcpy(opcodes[0xc5].sign, "multianewarray");
    strcpy(opcodes[0xc6].sign, "ifnull");
    strcpy(opcodes[0xc7].sign, "ifnonnull");
    strcpy(opcodes[0xc8].sign, "goto_w");
    strcpy(opcodes[0xc9].sign, "jsr_w");
    opcodes[0x00].count = 0;
    opcodes[0x01].count = 0;
    opcodes[0x02].count = 0;
    opcodes[0x03].count = 0;
    opcodes[0x04].count = 0;
    opcodes[0x05].count = 0;
    opcodes[0x06].count = 0;
    opcodes[0x07].count = 0;
    opcodes[0x08].count = 0;
    opcodes[0x09].count = 0;
    opcodes[0x0a].count = 0;
    opcodes[0x0b].count = 0;
    opcodes[0x0c].count = 0;
    opcodes[0x0d].count = 0;
    opcodes[0x0e].count = 0;
    opcodes[0x0f].count = 0;
    opcodes[0x10].count = 0;
    opcodes[0x11].count = 0;
    opcodes[0x12].count = 0;
    opcodes[0x13].count = 0;
    opcodes[0x14].count = 0;
    opcodes[0x15].count = 0;
    opcodes[0x16].count = 0;
    opcodes[0x17].count = 0;
    opcodes[0x18].count = 0;
    opcodes[0x19].count = 0;
    opcodes[0x1a].count = 0;
    opcodes[0x1b].count = 0;
    opcodes[0x1c].count = 0;
    opcodes[0x1d].count = 0;
    opcodes[0x1e].count = 0;
    opcodes[0x1f].count = 0;
    opcodes[0x20].count = 0;
    opcodes[0x21].count = 0;
    opcodes[0x22].count = 0;
    opcodes[0x23].count = 0;
    opcodes[0x24].count = 0;
    opcodes[0x25].count = 0;
    opcodes[0x26].count = 0;
    opcodes[0x27].count = 0;
    opcodes[0x28].count = 0;
    opcodes[0x29].count = 0;
    opcodes[0x2a].count = 0;
    opcodes[0x2b].count = 0;
    opcodes[0x2c].count = 0;
    opcodes[0x2d].count = 0;
    opcodes[0x2e].count = 0;
    opcodes[0x2f].count = 0;
    opcodes[0x31].count = 0;
    opcodes[0x32].count = 0;
    opcodes[0x33].count = 0;
    opcodes[0x34].count = 0;
    opcodes[0x35].count = 0;
    opcodes[0x36].count = 0;
    opcodes[0x37].count = 0;
    opcodes[0x38].count = 0;
    opcodes[0x39].count = 0;
    opcodes[0x3a].count = 0;
    opcodes[0x3b].count = 0;
    opcodes[0x3c].count = 0;
    opcodes[0x3d].count = 0;
    opcodes[0x3e].count = 0;
    opcodes[0x3f].count = 0;
    opcodes[0x40].count = 0;
    opcodes[0x41].count = 0;
    opcodes[0x42].count = 0;
    opcodes[0x43].count = 0;
    opcodes[0x44].count = 0;
    opcodes[0x45].count = 0;
    opcodes[0x46].count = 0;
    opcodes[0x47].count = 0;
    opcodes[0x48].count = 0;
    opcodes[0x49].count = 0;
    opcodes[0x4a].count = 0;
    opcodes[0x4b].count = 0;
    opcodes[0x4c].count = 0;
    opcodes[0x4d].count = 0;
    opcodes[0x4e].count = 0;
    opcodes[0x4f].count = 0;
    opcodes[0x50].count = 0;
    opcodes[0x51].count = 0;
    opcodes[0x52].count = 0;
    opcodes[0x53].count = 0;
    opcodes[0x54].count = 0;
    opcodes[0x55].count = 0;
    opcodes[0x56].count = 0;
    opcodes[0x57].count = 0;
    opcodes[0x58].count = 0;
    opcodes[0x59].count = 0;
    opcodes[0x5a].count = 0;
    opcodes[0x5b].count = 0;
    opcodes[0x5c].count = 0;
    opcodes[0x5d].count = 0;
    opcodes[0x5e].count = 0;
    opcodes[0x60].count = 0;
    opcodes[0x61].count = 0;
    opcodes[0x62].count = 0;
    opcodes[0x63].count = 0;
    opcodes[0x64].count = 0;
    opcodes[0x65].count = 0;
    opcodes[0x66].count = 0;
    opcodes[0x67].count = 0;
    opcodes[0x68].count = 0;
    opcodes[0x69].count = 0;
    opcodes[0x6a].count = 0;
    opcodes[0x6b].count = 0;
    opcodes[0x6c].count = 0;
    opcodes[0x6d].count = 0;
    opcodes[0x6e].count = 0;
    opcodes[0x6f].count = 0;
    opcodes[0x70].count = 0;
    opcodes[0x71].count = 0;
    opcodes[0x72].count = 0;
    opcodes[0x73].count = 0;
    opcodes[0x74].count = 0;
    opcodes[0x75].count = 0;
    opcodes[0x76].count = 0;
    opcodes[0x77].count = 0;
    opcodes[0x78].count = 0;
    opcodes[0x79].count = 0;
    opcodes[0x7a].count = 0;
    opcodes[0x7b].count = 0;
    opcodes[0x7c].count = 0;
    opcodes[0x7d].count = 0;
    opcodes[0x7e].count = 0;
    opcodes[0x7f].count = 0;
    opcodes[0x80].count = 0;
    opcodes[0x81].count = 0;
    opcodes[0x82].count = 0;
    opcodes[0x83].count = 0;
    opcodes[0x84].count = 0;
    opcodes[0x85].count = 0;
    opcodes[0x86].count = 0;
    opcodes[0x87].count = 0;
    opcodes[0x88].count = 0;
    opcodes[0x89].count = 0;
    opcodes[0x8a].count = 0;
    opcodes[0x8b].count = 0;
    opcodes[0x8c].count = 0;
    opcodes[0x8d].count = 0;
    opcodes[0x8e].count = 0;
    opcodes[0x8f].count = 0;
    opcodes[0x90].count = 0;
    opcodes[0x91].count = 0;
    opcodes[0x92].count = 0;
    opcodes[0x93].count = 0;
    opcodes[0x94].count = 0;
    opcodes[0x95].count = 0;
    opcodes[0x96].count = 0;
    opcodes[0x97].count = 0;
    opcodes[0x98].count = 0;
    opcodes[0x99].count = 0;
    opcodes[0x9a].count = 0;
    opcodes[0x9b].count = 0;
    opcodes[0x9c].count = 0;
    opcodes[0x9d].count = 0;
    opcodes[0x9e].count = 0;
    opcodes[0x9f].count = 0;
    opcodes[0xa0].count = 0;
    opcodes[0xa1].count = 0;
    opcodes[0xa2].count = 0;
    opcodes[0xa3].count = 0;
    opcodes[0xa4].count = 0;
    opcodes[0xa5].count = 0;
    opcodes[0xa6].count = 0;
    opcodes[0xa7].count = 0;
    opcodes[0xa8].count = 0;
    opcodes[0xa9].count = 0;
    opcodes[0xaa].count = 0;
    opcodes[0xab].count = 0;
    opcodes[0xac].count = 0;
    opcodes[0xad].count = 0;
    opcodes[0xae].count = 0;
    opcodes[0xaf].count = 0;
    opcodes[0xb0].count = 0;
    opcodes[0xb1].count = 0;
    opcodes[0xb2].count = 0;
    opcodes[0xb3].count = 0;
    opcodes[0xb4].count = 0;
    opcodes[0xb5].count = 0;
    opcodes[0xb6].count = 0;
    opcodes[0xb7].count = 0;
    opcodes[0xb8].count = 0;
    opcodes[0xb9].count = 0;
    opcodes[0xbb].count = 0;
    opcodes[0xbc].count = 0;
    opcodes[0xbd].count = 0;
    opcodes[0xbe].count = 0;
    opcodes[0xc4].count = 0;
    opcodes[0xc5].count = 0;
    opcodes[0xc6].count = 0;
    opcodes[0xc7].count = 0;
    opcodes[0xc8].count = 0;
    opcodes[0xc9].count = 0;
    opcodes[0x10].count = 1;
    opcodes[0x11].count = 2;
    opcodes[0x12].count = 1;
    opcodes[0x13].count = 2;
    opcodes[0x14].count = 2;
    opcodes[0x15].count = 1;
    opcodes[0x16].count = 1;
    opcodes[0x17].count = 1;
    opcodes[0x18].count = 1;
    opcodes[0x19].count = 1;
    opcodes[0x36].count = 1;
    opcodes[0x37].count = 1;
    opcodes[0x38].count = 1;
    opcodes[0x39].count = 1;
    opcodes[0x3a].count = 1;
    opcodes[0x84].count = 2;
    opcodes[0x99].count = 2;
    opcodes[0x9a].count = 2;
    opcodes[0x9b].count = 2;
    opcodes[0x9c].count = 2;
    opcodes[0x9d].count = 2;
    opcodes[0x9e].count = 2;
    opcodes[0x9f].count = 2;
    opcodes[0xa7].count = 2;
    opcodes[0xa0].count = 2;
    opcodes[0xa1].count = 2;
    opcodes[0xa2].count = 2;
    opcodes[0xa3].count = 2;
    opcodes[0xa4].count = 2;
    opcodes[0xa5].count = 2;
    opcodes[0xa6].count = 2;
    opcodes[0xa8].count = 2;
    opcodes[0xa9].count = 1;
    opcodes[0xaa].count = 14;
    opcodes[0xab].count = 10;
    opcodes[0xb2].count = 2;
    opcodes[0xb4].count = 2;
    opcodes[0xbd].count = 2;
    opcodes[0xb9].count = 4;
    opcodes[0xb7].count = 2;
    opcodes[0xb8].count = 2;
    opcodes[0xb6].count = 2;
    opcodes[0xbc].count = 1;
    opcodes[0xb5].count = 2;
    opcodes[0xb3].count = 2;
    opcodes[0xbb].count = 2;
    opcodes[0xc8].count = 4;
    opcodes[0xc7].count = 2;
    opcodes[0xc6].count = 2;
    opcodes[0xc9].count = 4;
    opcodes[0xc5].count = 3;
    opcodes[0xc4].count = 7;
}

u1 u1Read(FILE* fd) {
    u1 byte;
    fread(&byte, sizeof(u1), 1, fd);
    return byte;
}

u2 u2Read(FILE* fd) {
    u2 toReturn = 0;
    u1 byte1, byte2;
    fread(&byte1, sizeof(u1), 1, fd);
    fread(&byte2, sizeof(u1), 1, fd);
    toReturn = byte1 << 8;
    toReturn |= byte2;
    return toReturn;
}

u4 u4Read(FILE* fd) {
    u4 toReturn = u2Read(fd) << 16;
    toReturn |= u2Read(fd);
    return toReturn;
}

int findtype(char* type) {
    if (!strcmp(type, "ConstantValue")) {
        return CONSTANTVALUE;
    } else if (!strcmp(type, "Code")) {
        return CODE;
    } else if (!strcmp(type, "Exceptions")) {
        return EXCEPTIONS;
    } else if (!strcmp(type, "InnerClasses")) {
        return INNERCLASSES;
    } else {
        return OTHER;
    }
}

void free_cte_pool(ClassFile *cf) {
    cp_info *cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
       if (cp->tag == UTF8)
         free(cp->info.Utf8_info.bytes);
    }
    free(cf->constant_pool);
}


void free_methods(ClassFile *cf) {
  /*method_info* method_aux;
  char *type;

  for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
    attribute_info* att_aux;
    for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux) {
      type = (char*)calloc(cf->constant_pool[att_aux->attribute_name_index - 1].info.Utf8_info.length,sizeof(char));
      strcpy(type, (char*)cf->constant_pool[att_aux->attribute_name_index - 1].info.Utf8_info.bytes);
      int i = findtype(type);
      if (i == CODE){
        if (att_aux->type.Code.code_length > 0) {
          free(att_aux->type.Code.code);
        }
        if (att_aux->type.Code.exception_table_length > 0) {
          free(att_aux->type.Code.exception_table);
        }
      }
      free(type);
      //free_attr(cf, method_aux);
    }
    free(method_aux->attributes);
  }*/
  free(cf->methods);
}
void free_clFile(ClassFile* cf) {
  if (!cf)
    return;
  free_cte_pool(cf);
  if (cf->fields)
    free(cf->fields);
  if (cf->interfaces)
    free(cf->interfaces);
  free_methods(cf);
  /*if (cf->methods)
    free(cf->methods);
  if (cf->attributes)
    free(cf->attributes);*/
  free(cf);
}

void shutdown(FILE *fd, FILE *fout, ClassFile* cf) {
    free_clFile(cf);
    fclose(fout);
    fclose(fd);
    printf("Finalizado!\n");
}


FILE* open_file(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo, "rb");
    if (!fp) {
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}

FILE * io_handler(int argc, char *argv[], char *nomearquivo, FILE **fout)
{
  FILE *fd = NULL;
  char saidaarquivo[1024] = "output_class.txt";
  if (argc == 1) {
    do {
      printf("Digite o nome do arquivo: ");
      scanf("%s", nomearquivo);
      fflush(stdin);
      fd = open_file(nomearquivo);
    } while (!fd);
  } else if (argc == 3) {
    strcpy(nomearquivo, argv[1]);
    fd = open_file(nomearquivo);
    if (!fd) {
      printf("Arquivo de entrada não encontrado!\n");
      exit(0);
    }
    strcpy(saidaarquivo, argv[2]);

  } else {
    printf("Uso do programa: ./leitorexibidor [nome-do-class nome-da-saída]\n");
    exit(0);
  }
  if (!(*fout = fopen(saidaarquivo, "w+"))) {
    fprintf(stderr, "Erro %d na abertura do arquivo de saida\n", errno);
    exit(0);
  }
  return fd;
}

/**************************************************************************************************
                                    CÓDIGO ADICIONADO PARA JVM
**************************************************************************************************/

char* read_utf8(cp_info* cp,u2 index) { return (char*)cp[index].info.Utf8_info.bytes; }
