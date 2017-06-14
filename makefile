CFLAGS=-Wall -Wextra
ifdef RELEASE
	CFLAGS += -O2
else
	#CFLAGS += -lm -ggdb3 -fsanitize=address
	CFLAGS += -lm -ggdb -std=c99
endif

all: jvm

jvm: main.o leitor_exibidor.o exibidor.o util.o
	gcc $(CFLAGS) main.o leitor_exibidor.o exibidor.o util.o -o jvm

main.o: main.c
	gcc $(CFLAGS) -c main.c

leitor_exibidor.o: classloader/leitor_exibidor.c
	gcc $(CFLAGS) -c classloader/leitor_exibidor.c

exibidor.o: classloader/exibidor.c
	gcc $(CFLAGS) -c classloader/exibidor.c

util.o: classloader/util.c
	gcc $(CFLAGS) -c classloader/util.c

clean:
	find . -name '*.o' -exec rm '{}' \;
	find . -name 'leitorexibidor' -exec rm '{}' \;
