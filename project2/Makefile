CC	= gcc
CFLAGS	= -g -Wall
LDFLAGS	=
PROGS	= unique-unsorted unique-sorted parity-unsorted parity-sorted

all:			$(PROGS)

clean:;			$(RM) $(PROGS) *.o core

unique-unsorted:	unique.o unsorted.o
			$(CC) -o $@ $(LDFLAGS) unique.o unsorted.o

unique-sorted:		unique.o sorted.o
			$(CC) -o $@ $(LDFLAGS) unique.o sorted.o

parity-unsorted:	parity.o unsorted.o
			$(CC) -o $@ $(LDFLAGS) parity.o unsorted.o

parity-sorted:		parity.o sorted.o
			$(CC) -o $@ $(LDFLAGS) parity.o sorted.o
