TARG = dither
OBJS = dither.o

N=2
CFLAGS = -Wall -std=c89 -pedantic -DN=$(N)

dither: dither.o
	$(CC) $(LDFLAGS) -o $@ dither.o

dither.o: bayer.inc

bayer.inc: bayer
	./bayer -s $(N) | \
		awk '{ D=2^$N; printf "0x%04X,\n", $$1 * 2^16 / D^2 }' \
	       	> bayer.inc

bayer: bayer.o
	$(CC) $(LDFLAGS) -o $@ bayer.o

clean:
	rm -f dither bayer *.o bayer.inc

test: dither
	cat obama.jpg | 2ff | ./$(TARG) | ff2jpg | feh -
