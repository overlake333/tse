CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl -lpthread

all: clean testPageio indexioTest orTest lqueueTest lhashTest

testPageio:
	gcc $(CFLAGS) testPageio.c $(LIBS) -o $@

indexioTest:
	gcc $(CFLAGS) indexioTest.c $(LIBS) -o $@

orTest:
	gcc $(CFLAGS) orTest.c $(LIBS) -o $@

lqueueTest:
	gcc $(CFLAGS) lqueueTest.c $(LIBS) -o $@

lhashTest:
	gcc $(CFLAGS) lhashTest.c $(LIBS) -o $@

clean:
	rm -f testPageio indexioTest orTest lqueueTest lhashTest
