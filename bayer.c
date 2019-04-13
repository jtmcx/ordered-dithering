#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

static void
usage(char *argv0)
{
	fprintf(stderr, "usage: %s [-p] -s size\n", argv0);
	exit(1);
}

static int
interleave(int n, int x, int y)
{
	int i, j, s, v = 0;

	for (i = 0, j = 0; i < n; i++) {
		s = n - i - 1;
		v |= ((y >> s) & 1) << j++;
		v |= ((x >> s) & 1) << j++;
	}
	return v;
}

static void
dump(int size, int pretty)
{
	int i, j, n, v;

	n = 1 << size;
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			v = interleave(size, i ^ j, j);
			printf(pretty ? "%4d " : "%d\n", v); 
		}
		if (pretty)
			printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int opt, size, pretty;

	size = -1;
	pretty = 0;

	while ((opt = getopt(argc, argv, "ps:")) != -1) {
		switch (opt) {
		case 'p':
			pretty = 1;
			break;
		case 's': 
			size = atoi(optarg);
			break;
		default:
			usage(argv[0]);
		}
	}

	if (size < 0)
		usage(argv[0]);

	dump(size, pretty);
	return 0;
}
