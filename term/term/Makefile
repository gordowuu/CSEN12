CC	= gcc
CFLAGS	= -g -Wall
PROGS	= maze radix qsort

all:	$(PROGS)

clean:;	$(RM) $(PROGS) *.o core

maze:	maze.o list.o
	$(CC) -o maze maze.o list.o -lcurses

radix:	radix.o list.o
	$(CC) -o radix radix.o list.o -lm

qsort:	qsort.o list.o
	$(CC) -o qsort qsort.o list.o
