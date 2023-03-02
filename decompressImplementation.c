#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "TGA.h"
#include "Data.h"
#include "decompress.h"

// Check the correctness of format.
bool correctArgumentsDecompress(int argc, char** argv)  {
    if (argc != 6)  {
        return false;
    }

    // Possibility of multiplicity.
    int countInput = 0; int countOutput = 0;
    for (int current = 2; current < argc; current += 2)  {
        if (strcmp(argv[current], "--input") == 0)  {
            countInput++; continue;
        }
        if (strcmp(argv[current], "--output") == 0)  {
            countOutput++; continue;
        }
        return false;
    }

    if (countInput == 1 && countOutput == 1)  {
        return true;
    }   
    return false;
}


InitialDataDecompress getInitialDataDecompress(int argc, char** argv)  {
    InitialDataDecompress my;
    for (int current = 2; current < argc; current += 2)  {
        if (strcmp(argv[current], "--input") == 0)  {
            my.inputPath = malloc(strlen(argv[current + 1]) * sizeof(char));
            strcpy(my.inputPath, argv[current + 1]);
            continue;
        }
        if (strcmp(argv[current], "--output") == 0)  {
            my.outputPath = malloc(strlen(argv[current + 1]) * sizeof(char));
            strcpy(my.outputPath, argv[current + 1]);
            continue;
        }
    }
    return my;
}


Pixel* computePixelPositions(FILE* file, int width, int height, int blockSize)  { 

    Pixel* myPixels = (Pixel*)malloc(sizeof(Pixel) * width * height);

    byte currentSequence[3] = {0,0,0}; // blue, green, red

    int length = 0;

    // Max length of one sequence.  // HERE OPT
    int temp = blockSize * blockSize;
    int count = 0;
    while(temp > (pow(2, count)))  {
        count++;
    }
    temp = pow(2, count);

    // Size of bytes needed to save the block.  // HERE OPT
    int flag = 0;
    if (temp < 255)  {
        flag = 1;
    } else if (temp < 65535)  {
        flag = 2;
    } else  {
        flag = 4;
    }

    for (int x = 0; x < height; x += blockSize)  {  
        for (int y = 0; y < width; y += blockSize)  {   
            for (int smallSquareX = 0; smallSquareX < blockSize; smallSquareX++)  {
                for (int smallSquareY = 0; smallSquareY < blockSize; smallSquareY++)  {  
                    if (length == 0)  {
                        fread(currentSequence, sizeof(byte), 3, file);
                        fread(&length, flag, 1, file);              // HERE
                    }
          
                    Pixel current;
                    current.blue = currentSequence[0];
                    current.green = currentSequence[1];
                    current.red = currentSequence[2];
                    myPixels[(x + smallSquareX) * width + y + smallSquareY] = current;

                    length--;

                }
            }
        }
    }

    return myPixels;
}


Pixel* loadCompressedFile(char* inputFile, TGAHeader* myHeaderToSave, int* countPixelsToSave, int* blockSizeToSave)  {
    FILE* file = fopen(inputFile, "rb");
    assert(file);

    TGAHeader my; int blockSize = 0;

    fread(&my, sizeof(TGAHeader), 1, file);
    fread(&blockSize, sizeof(int), 1, file);

    int width = 0; int height = 0;
    memcpy(&width, my.width, 2);
    memcpy(&height, my.height, 2);

    Pixel* myPixels = computePixelPositions(file, width, height, blockSize);

    memcpy(myHeaderToSave, &my, sizeof(TGAHeader));
    *countPixelsToSave = width * height;
    *blockSizeToSave = blockSize;

    fclose(file);
    return myPixels;
}


void saveNewPicture(char* outputPath, TGAHeader myHeader, Pixel* myPixels, int count)  {
    FILE* file = fopen(outputPath, "wb");  
    assert(file);  

    fwrite(&myHeader, sizeof(TGAHeader), 1, file);
    for (int i = 0; i < count; i++)  {
        fwrite(&myPixels[i], sizeof(Pixel), 1, file);
    }

    fclose(file);
}


bool decompress(int argc, char** argv)  {
    InitialDataDecompress myData = getInitialDataDecompress(argc, argv);

    TGAHeader myHeader = {}; int countPixels = 0; int blockSize = 0;
    Pixel* myPixels = loadCompressedFile(myData.inputPath, &myHeader, &countPixels, &blockSize);

    saveNewPicture(myData.outputPath, myHeader, myPixels, countPixels);

    return true;
}