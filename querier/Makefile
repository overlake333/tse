CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: clean querier

querier:
	gcc $(CFLAGS) query.c $(LIBS) -o $@

clean:
	rm -f querier
