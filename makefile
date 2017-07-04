#CFLAGS=-Wall -Wextra
CFLAGS = -ggdb -std=c99 -lm
ifdef RELEASE
	CFLAGS += -O2
else
	#CFLAGS += -lm -ggdb3 -fsanitize=address
endif

all: jvm

jvm: main.o classfile.o execution.o frame.o heap.o instruction_set.o types.o util.o
	gcc classfile.o execution.o frame.o heap.o instruction_set.o types.o util.o main.o $(CFLAGS) -o jvm

main.o: main.c
	gcc $(CFLAGS) -c main.c

classfile.o: classfile.c
	gcc $(CFLAGS) -c classfile.c

execution.o: execution.c
	gcc $(CFLAGS) -c execution.c

frame.o: frame.c
	gcc $(CFLAGS) -c frame.c

heap.o: heap.c
	gcc $(CFLAGS) -c heap.c

instruction_set.o: instruction_set.c
	gcc $(CFLAGS) -c instruction_set.c

types.o: types.c
	gcc $(CFLAGS) -c types.c

util.o: util.c
	gcc $(CFLAGS) -c util.c

clean:
	find . -name '*.o' -exec rm '{}' \;
	find . -name 'jvm' -exec rm '{}' \;
