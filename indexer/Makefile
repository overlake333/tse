CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean indexer


indexer:
	gcc $(CFLAGS) indexer.c $(LIBS) -o $@

indexer3:
	gcc $(CFLAGS) indexer3.c $(LIBS) -o $@

indexer4:
	gcc $(CFLAGS) indexer4.c $(LIBS) -o $@

run:

grind:

clean:
	rm -f indexer indexer3 indexer4
