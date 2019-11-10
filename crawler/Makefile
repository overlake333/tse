CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean crawler

crawler:
	gcc $(CFLAGS) crawler2.c $(LIBS) -o $@

run:
		rm -f ../pages/*
		crawler https://thayer.github.io/engs50/ pages 2

grind:
		rm -f ../pages/*
		crawlerValgrind.sh

clean:
	rm -f crawler
