CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean testPageio indexioTest orTest

testPageio:
	gcc $(CFLAGS) testPageio.c $(LIBS) -o $@

indexioTest:
	gcc $(CFLAGS) indexioTest.c $(LIBS) -o $@

orTest:
	gcc $(CFLAGS) orTest.c $(LIBS) -o $@

clean:
	rm -f testPageio indexioTest orTest
