#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "types.h"

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