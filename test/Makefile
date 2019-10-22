CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean testPageio

testPageio:
	gcc $(CFLAGS) testPageio.c $(LIBS) -o $@

clean:
	rm -f testPageio
