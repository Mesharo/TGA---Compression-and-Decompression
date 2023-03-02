# Compression/Decompression of .TGA

Simple compression of **TGA** images using block compression method, followed up by decompression of previously compressed TGA image in the form of **.jbk** file.

## Functionality

RLE compression divides the image into individual blocks of size (entered by user), and then, in each block, it clusters *similar* pixels into *sequences*.

- *similar*	= two pixels are similar, when the absolute value of a division between their RGB values is less than, or equal to maximal division value (entered by user)
- *sequence*	= contains RGB value of a pixel, and number of similar pixels in the original image


Program accepts command line parameters (in more detail in documentation).
During compression, the program verifies whether user's input is correct, or it should abort and display a warning. Then, it loads and saves information about the TGA from its header, saves the image's pixels and verifies whether it's possible to split the image into the blocks. Finally, the program calculates the correct sequences, saves the TGA head and sequences into a new .jbk file, and prints statistics about the compression.

During decompression, the program saves all the information needed from the compressed .jbk file, calculates for all pixels in each sequence their proper coordinates in the decompressed image, and creates it by writing a TGA header and its pixels into a new TGA file.

## Compiling

### Windows

In console, using gcc. First command might look like:

`gcc main.c compressedImplementation.c decompressedImplementation.c -o final`

where you can give your *.exe* file any name you want instead of *final*. Next, if you want to compress a TGA image, the its template is:

`.\<name>.exe compress --input <yourTGAimage> --output <nameForJBK> --max-diff <numberForSequences> --block-size <sizeOfBlocks>`

Decompression is very similar:

`\<name>.exe decompress --input <yourJBKfile> --output --<nameForTGA>

### Linux

Using Makefile with the command *make*:

`make final`

followed by:

`./final <same parameter pattern like mentioned above>`

Thanks to:

`make clean` 

you can delete all .o files after you are done with compiling.