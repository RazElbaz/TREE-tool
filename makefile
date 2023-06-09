CC = gcc
CFLAGS = -Wall

stree: stree.c stree.h
	$(CC) $(CFLAGS) -o stree stree.c -lm

clean:
	rm -f stree
