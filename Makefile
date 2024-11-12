CC=gcc
CFLAGS=-g -Wall
LFLAGS=-pthread

all: treeMT.o treeMTTest

treeMT.o: treeMT.c
	$(CC) $(CFLAGS) $^ -c -o $@

treeMTTest: treeMT.o
	$(CC) $(CFLAGS) treeMTTest.c -o $@ $^ $(LFLAGS)

clean:
	rm -rf treeMT treeMTTest *.o