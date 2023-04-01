SHELL = /bin/bash
CC = gcc
CFLAGS = -g -pthread
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@
	sudo chown root $@
	sudo chmod +s $@

clean:
	rm ${EXE}