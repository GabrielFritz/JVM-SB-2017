CFLAGS=-Wall -Wextra
ifdef RELEASE
	CFLAGS += -O2
else
	#CFLAGS += -lm -ggdb3 -fsanitize=address
	CFLAGS += -lm -ggdb -std=c99
endif

all: jvm

jvm: main.o
	gcc $(CFLAGS) main.o -o jvm

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	find . -name '*.o' -exec rm '{}' \;
	find . -name 'jvm' -exec rm '{}' \;
