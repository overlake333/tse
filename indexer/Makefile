CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean indexer

indexer:
	gcc $(CFLAGS) indexer.c $(LIBS) -o $@

run:

grind:

clean:
	rm -f indexer
