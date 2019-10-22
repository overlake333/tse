CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o
LIBFILE=../lib/libutils.a

all: $(OFILES)
	ar cr $(LIBFILE) $(OFILES)

%.o: %.c %.h
	gcc ${CFLAGS} -c $<


clean:
	rm -f $(OFILES) $(LIBFILE)
