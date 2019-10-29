CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean indexer indexer2

indexer:
	gcc $(CFLAGS) indexer.c $(LIBS) -o $@

indexer2:
	gcc $(CFLAGS) indexer2.c $(LIBS) -o $@

run:

grind:

clean:
	rm -f indexer indexer2
