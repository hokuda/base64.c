LDFLAGS	= -shared
CFLAGS	= -fPIC -g

all:		libbase64.so test

clean:
	rm -f base64.o libbase64.so test

base64.o:	base64.c base64.h
	$(CC) $(CFLAGS) base64.c -c

libbase64.so:	base64.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o libbase64.so base64.o

test:		test.c libbase64.so
	$(CC) -L. -lbase64 $(CFLAGS) test.c -o test

run:		test
	LD_LIBRARY_PATH=. ./test
