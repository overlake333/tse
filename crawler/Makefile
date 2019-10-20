CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean crawler

crawler:
	gcc $(CFLAGS) crawler2.c $(LIBS) -o $@
clean:
	rm -f crawler
