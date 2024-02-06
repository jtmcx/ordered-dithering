#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/* 'DIM' is the number of rows (and columns; it's a square) in the input
 * matrix.  Note the 'N' passed in from the command line, and should be
 * the same value that was used in './bayer -s <N>'.
 */
#define DIM (1 << N)

typedef struct Pixel Pixel;

struct Pixel {
	uint16_t r;  /* red */
	uint16_t g;  /* green */
	uint16_t b;  /* blue */ 
	uint16_t a;  /* alpha */
};

/* This array is generated in the makefile. It's the output of running
 * './bayer -s N' with some awk magic to normalize the matrix.
 */
static uint16_t bayer[] = {
#include "bayer.inc"
};

static void
panic(char *msg)
{
	fprintf(stderr, "panic: %s\n", msg);
	exit(1);
}

static void
readmagic(FILE *in)
{
	char magic[8];

	fread(magic, 1, 8, in);
	if (strncmp(magic, "farbfeld", 8) != 0)
		panic("bad magic");
}

static void
readdim(FILE *in, uint32_t *w, uint32_t *h)
{
	fread(w, 1, 4, in);
	fread(h, 1, 4, in);
	*w = ntohl(*w);
	*h = ntohl(*h);
}

static void
readpixel(FILE *in, Pixel *p)
{
	fread(p, 1, 8, in);
	p->r = ntohs(p->r);
	p->g = ntohs(p->g);
	p->b = ntohs(p->b);
	p->a = ntohs(p->a);
}

static void
writemagic(FILE *out)
{
	fwrite("farbfeld", 1, 8, out);
}

static void
writedim(FILE *out, uint32_t w, uint32_t h)
{
	w = htonl(w);
	h = htonl(h);
	fwrite(&w, 1, 4, out);
	fwrite(&h, 1, 4, out);
}

static void
writepixel(FILE *out, Pixel p)
{
	p.r = htons(p.r);
	p.g = htons(p.g);
	p.b = htons(p.b);
	p.a = htons(p.a);
	fwrite(&p, 1, 8, out);
}

static uint16_t
quantize(uint16_t x)
{
	uint16_t v = (x >> 12) & 0xE;
	return (v << 0) | (v << 4) | (v << 8) | (v << 12);
}

static Pixel 
togreyscale(Pixel p, uint32_t row, uint32_t col)
{
	Pixel q;
	uint16_t thresh, m = 0;

	/* Set m to a 16-bit greyscale value. */
	m += 0.21 * p.r;
	m += 0.72 * p.g;
	m += 0.07 * p.b;
	m = quantize(m);

	/* Get the threshold from the bayer matrix. */
	thresh = bayer[(row % DIM) * DIM + (col % DIM)];

	/* Only enable the pixel if we're passed the threshold. */
	m = m > thresh ? m : 0;

	q.r = q.g = q.b = m;
	q.a = 0xFFFF;		   /* Ignore alpha channel. */
	return q;
}

int
main(int argc, char *argv[])
{
	Pixel p;
	uint32_t x, y;
	uint32_t w, h;

	readmagic(stdin);
	readdim(stdin, &w, &h);

	writemagic(stdout);
	writedim(stdout, w, h);

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			readpixel(stdin, &p); 
			p = togreyscale(p, y, x);
			writepixel(stdout, p);
		}
	}
	return 0;
}


