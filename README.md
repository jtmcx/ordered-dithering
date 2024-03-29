# Ordered Dithering Example

This is a small program written to apply [ordered dithering][1] to
images. It was written to better understand how ordered dithering
works, and also to test its viability for displaying images in a
one-bit space (for another project).

There are two programs: `bayer` and `dither`. The `bayer` program
generates threshold matrices for the ordered dithering algorithm.
The `dither` program reads farbfeld images from stdin, and writes
dithered farbfeld images to stdout.

To run a test program, run: 

	$ make clean test N=3

The test program requires that `farbfeld` and `feh` are installed
on the machine. These are available in most package managers.

## Example

Input                      | Output
:-------------------------:|:-------------------------:
![](example/input.png)    | ![](example/output.png)

[1]: https://en.wikipedia.org/wiki/Ordered_dithering
