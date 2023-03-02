#pragma once

/**
 * Function for verificating corectness of command line arguments.
 * Correct format of input has to contain 'compress' as first word,
 * then only one input path, output path, block size and max difference in non specific order,
 * with signatures '--input', '--output', '--block-size' and '--max-diff', values following right after signature words.
 * Input file must be .tga, output file must be .jbk.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return bool. Decision whether parameters are correct or incorrect.
*/
bool correctArgumentsCompress(int argc, char** argv);



/**
 * Function traversing through command line arguments, searching for 
 * input and output paths, block size and max difference, which later are saved in a structure.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return InitialDataCompress. Structure containing input and output paths, block size and max difference.
*/
InitialDataCompress getInitialDataCompress(int argc, char** argv);



/**
 * Subfunction reading pixels from a file and saving them into a dynamic array.
 * \param file FILE*. Already opened tga image.
 * \param width int. Width of tga image.
 * \param height int. Height of tga image.
 * \return Pixel*. Array of pixels of original tga image.
*/
Pixel* loadPixels(FILE* file, int width, int height);



/**
 * Function saving all data about tga image.
 * Opens tga image, reads and saves header and pixels by
 * calling loadPixels function, and saves them into a 
 * struct for tga images with width and height.
 * \param inputPath char*. Path to original tga image.
 * \return Image. tga image.
*/
Image loadImage(char* inputPath);



/**
 * Subfunction to determine whether two pixels are similar or not.
 * Computes absolute values of differences between BGR values of pixels,
 * and sums them up. If the sum is less or equal to maximum difference given
 * in command line arguments, they are similar. Otherwise they are not similar.
 * \param myPixelCurrent Pixel. current pixel.
 * \param myPixelNew Pixel. adjacent pixel, and possibly similar.
 * \param maxDiff int. Maximum difference allowed.
 * \return bool. They are/aren't similar.
*/
bool isSimilar(Pixel myPixelCurrent, Pixel myPixelNew, int maxDiff);



/**
 * Function for computing sequences from pixels of original tga image.
 * Traverses array of pixels from tga image. If next pixel is similar to 
 * the current pixel, we increase length of sequence and move on.
 * If the next pixel either is not similar, or we are on the end of
 * the block, we save a sequence, consisting of current pixel and its length,
 * to dynamically allocated array, set the next pixel as current, and set length to 1.
 * \param myImage Image. Original tga image with its info and pixels.
 * \param myData InitialDataCompress. Structure with info about block size, width and height.
 * \param sizeFinal int*. Reference to int where final size of sequence array should be saved.
 * \return Sequence*. Array of computed sequences.
*/
Sequence* getSequences(Image myImage, InitialDataCompress myData, int* sizeFinal);



/**
 * Function that saves the compressed tga image into a .jbk file.
 * Creates a binary .jbk file, saves into it tga head, block size 
 * and all sequences.
 * \param mySequences Sequence*. Array of computed sequences.
 * \param size int. size of sequence array.
 * \param myImage Image. Original tga image with its info and pixels.
 * \param outputPath char*. Location where the new tga image should be stored.
 * \param blockSize int. Size of blocks we divide the image into.
*/
void saveSequences(Sequence* mySequences, int size, Image myImage, char* outputPath, int blockSize);



/**
 * Subfunction returning size of file in bytes.
 * Opens a file, moves the cursor at its end, and returns
 * the position cursor is at.
 * \param path char*. File from which we want to know its size.
 * \return long. Size of a file in bytes.
*/
long getFileSize(char* path);



/**
 * Function for computing ratio between the size of original tga image,
 * and compressed tga image in the form of .jbk file.
 * Calls getFileSize function on both files, computes
 * the ratio in % and prints it to the terminal.
 * \param inputPath char*. Original tga file.
 * \param outputPath char*. Compressed tga image - .jbk file.
*/
void printFileSizeDiff(char* inputPath, char* outputPath);



/**
 * Function serving as a wrapper for the entire process.
 * Calls other functions for successful compression, and returns,
 * if the process went good or bad.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return bool. How compression went.
*/
bool compress(int argc, char** argv);