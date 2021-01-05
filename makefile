CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

clean:
	rm *.o

all: list.o

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o list.o
