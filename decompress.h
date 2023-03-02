#pragma once

/**
 * Function for verificating corectness of command line arguments.
 * Correct format of input has to contain 'decompress' as first word,
 * then only one input path, and only one output path, in non specific order,
 * with signatures '--input' and '--output', values following right after signature words.
 * Input file must be .jbk, output file must be .tga.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return bool. Decision whether parameters are correct or incorrect.
*/
bool correctArgumentsDecompress(int argc, char** argv);



/**
 * Function traversing through command line arguments, searching for 
 * input and output paths, which later are saved in a structure.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return InitialDataDecompress. Structure containing input and output paths.
*/
InitialDataDecompress getInitialDataDecompress(int argc, char** argv);



/**
 * Structure putting pixels into the right places in the image.
 * Allocates memory for array of pixels ready for saving into the final TGA image,
 * and fills it up with pixels from the sequences in .jbk by traversing through
 * the array by blocks, and copying pixels from sequences.
 * \param file FILE*. Already opened .jbk file.
 * \param width int. Width of tga image.
 * \param height int. Height of tga image.
 * \param blockSize int. Size of a single square block, on which the target image will be split.
 * \return Pixel*. Array of pixels of tga image..
*/
Pixel* computePixelPositions(FILE* file, int width, int height, int blockSize);



/**
 * Function processing given .jbk file.
 * Opens .jbk file, saves information about tga header, block size and count of pixels (width * height).
 * Calls computePixelPositions function and returns its result.
 * \param inputFile char*. Path to .jbk file.
 * \param myHeaderToSave TGAHeader*. Reference to struct, where tga head should be saved.
 * \param countPixelsToSave int*. Reference to int, where total pixel count should be saved.
 * \param blockSizeToSave int*. Reference to int, where block size should be saved.
 * \return Pixel*. Array of pixels of tga image.
*/
Pixel* loadCompressedFile(char* inputFile, TGAHeader* myHeaderToSave, int* countPixelsToSave, int* blockSizeToSave);



/**
 * Function writing all the information needed for a tga image into a file.
 * \param outputPath char*. Path where to save .tga file.
 * \param myHeader TGAHeader. Head of the final tga to be saved.
 * \param myPixels Pixel*. Pixels of the final tga to be saved.
 * \param count int. Number of pixels to be saved.
*/
void saveNewPicture(char* outputPath, TGAHeader myHeader, Pixel* myPixels, int count);



/**
 * Function serving as a wrapper for the entire process.
 * Calls other functions for successful decompression, and returns,
 * if the process went good or bad.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return bool. How decompression went.
*/
bool decompress(int argc, char** argv);