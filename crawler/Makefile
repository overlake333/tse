CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl -lpthread

all: clean crawler pcrawler

crawler:
	gcc $(CFLAGS) crawler2.c $(LIBS) -o $@

pcrawler:
	gcc $(CFLAGS) pcrawler.c $(LIBS) -o $@

#
#run:
#		rm -f ../pages/*
#		crawler https://thayer.github.io/engs50/ pages 2

run:
		pcrawler https://thayer.github.io/engs50/ fpages 3 100

grind:
		rm -f ../pages/*
		crawlerValgrind.sh

clean:
	rm -f crawler pcrawler
