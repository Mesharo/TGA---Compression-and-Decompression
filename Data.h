#pragma once

/**
 * Structure for saving command line arguments for compression for future use.
*/
typedef struct {
    char* inputPath;    /**< Path to the TGA image we want to compress. */
    char* outputPath;   /**< Path to custom made .jbk file. */
    int blockSize;      /**< Size of a single square block, on which the target image will be split. */
    int maxDiff;        /**< A value to compare similarity of adjacent blocks. 1 */
} InitialDataCompress;


/**
 * Structure for saving command line arguments for decompression for future use.
*/
typedef struct {
    char* inputPath;    /**< Path to .jbk file, from which we will read information about original image. */
    char* outputPath;   /**< Path to location for decompressed TGA image. */
} InitialDataDecompress;
