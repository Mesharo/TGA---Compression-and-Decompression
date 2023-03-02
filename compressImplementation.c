#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "TGA.h"
#include "Data.h"
#include "compress.h"

bool correctArgumentsCompress(int argc, char** argv)  {
    if (argc != 10)  {
        return false;
    }

    // Possibility of multiplicity.
    int countInput = 0; int countOutput = 0; int countBlockSize = 0; int countMaxDiff = 0;
    for (int current = 2; current < argc; current += 2)  {
        if (strcmp(argv[current], "--input") == 0)  {
            countInput++; continue;
        }
        if (strcmp(argv[current], "--output") == 0)  {
            countOutput++; continue;
        }
        if (strcmp(argv[current], "--block-size") == 0)  {
            countBlockSize++; continue;
        }
        if (strcmp(argv[current], "--max-diff") == 0)  {
            countMaxDiff++; continue;
        }
        return false;
    }

    if (countInput == 1 && countOutput == 1 && countBlockSize == 1 && countMaxDiff == 1)  {
        return true;
    }
    return false;
}

InitialDataCompress getInitialDataCompress(int argc, char** argv)  {
    InitialDataCompress my;
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
        if (strcmp(argv[current], "--block-size") == 0)  {
            my.blockSize = atoi(argv[current + 1]);
            continue;
        }
        if (strcmp(argv[current], "--max-diff") == 0)  {
            my.maxDiff = atoi(argv[current + 1]);
        }
    }
    return my;
}



Pixel* loadPixels(FILE* file, int width, int height) {
    Pixel* pixels = (Pixel*) malloc(sizeof(Pixel) * width * height);

    assert(fread(pixels, sizeof(Pixel) * width * height, 1, file) == 1);

    return pixels;
}


Image loadImage(char* inputPath)  {
    FILE* file = fopen(inputPath, "rb");
    assert(file);

    TGAHeader header = {};
    assert(fread(&header, sizeof(TGAHeader), 1, file) == 1);

    int width = 0;
    int height = 0;
    
    memcpy(&width, header.width, 2);
    memcpy(&height, header.height, 2);

    Pixel* myPixels = loadPixels(file, width, height);

    Image myImage;

    memcpy(&myImage.header, &header, sizeof(TGAHeader));

    myImage.pixels = malloc(sizeof(Pixel) * width * height);
    memcpy(myImage.pixels, myPixels, sizeof(Pixel) * width * height);

    myImage.height = height;
    myImage.width = width;

    fclose(file);
    return myImage;
}

bool isSimilar(Pixel myPixelCurrent, Pixel myPixelNew, int maxDiff)  {
    if ((abs(myPixelCurrent.blue - myPixelNew.blue) + abs(myPixelCurrent.green - myPixelNew.green) + abs(myPixelCurrent.red - myPixelNew.red)) <= maxDiff)  {
        return true;
    }
    return false;
}


Sequence* getSequences(Image myImage, InitialDataCompress myData, int* sizeFinal)  {
    if (((myImage.width % myData.blockSize) != 0) || ((myImage.height % myData.blockSize) != 0))  {
        return NULL;
    }

    Sequence* array;
    int size = 0;
    Sequence currentSequence = {myImage.pixels[0], 1};

    int width = 0; 
    memcpy(&width, &myImage.width, 2);

    for (int x = 0; x < myImage.height; x += myData.blockSize)  {  
        for (int y = 0; y < myImage.width; y += myData.blockSize)  {   
            for (int smallSquareX = 0; smallSquareX < myData.blockSize; smallSquareX++)  {
                for (int smallSquareY = 0; smallSquareY < myData.blockSize; smallSquareY++)  {  
                    if (x == 0 && y == 0 && smallSquareX == 0 && smallSquareY == 0)  {
                        continue;
                    }

                    if ((smallSquareX == (myData.blockSize - 1)) && (smallSquareY == (myData.blockSize - 1)))  {
                        if (size == 0)  {
                            array = malloc(sizeof(Sequence) * (size + 1));
                        }  else  {
                            array = realloc(array, sizeof(Sequence) * (size + 1));
                        }

                        memcpy(&array[size], &currentSequence, sizeof(Sequence));
                        size++;
                        
                        currentSequence.pixel = myImage.pixels[(x + smallSquareX) * width + y + smallSquareY];
                        currentSequence.length = 1;
                        continue;
                    }    


                    if (isSimilar(currentSequence.pixel, myImage.pixels[(x + smallSquareX) * width + y + smallSquareY], myData.maxDiff) == true)  {
                        currentSequence.length += 1;
                    }  else  {
                        if (size == 0)  {
                            array = malloc(sizeof(Sequence) * (size + 1));
                        }  else  {
                            array = realloc(array, sizeof(Sequence) * (size + 1));
                        }

                        memcpy(&array[size], &currentSequence, sizeof(Sequence));
                        size++;
                        
                        currentSequence.pixel = myImage.pixels[(x + smallSquareX) * width + y + smallSquareY]; 
                        currentSequence.length = 1;
                    }
                }
            }
        }
    }
    
    *sizeFinal = size;
    return array;
}



void saveSequences(Sequence* mySequences, int size, Image myImage, char* outputPath, int blockSize)  {
    FILE* file = fopen(outputPath, "wb");
    assert(file);

    TGAHeader myHeader;
    memcpy(&myHeader, &myImage.header, sizeof(TGAHeader));

    fwrite(&myHeader, sizeof(TGAHeader), 1, file);

    fwrite(&blockSize, sizeof(int), 1, file); 

    // Max length of one sequence.              // HERE OPT
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

    for (int i = 0; i < size; i++)  {
        byte blue = 0; byte green = 0; byte red = 0; int length = 0;   // HERE  2^8 -> 2^32
        memcpy(&blue, &mySequences[i].pixel.blue, 1);
        memcpy(&green, &mySequences[i].pixel.green, 1);
        memcpy(&red, &mySequences[i].pixel.red, 1);
        memcpy(&length, &mySequences[i].length, flag);        // HERE

        fwrite(&blue, sizeof(byte), 1, file);
        fwrite(&green, sizeof(byte), 1, file); 
        fwrite(&red, sizeof(byte), 1, file);
        fwrite(&length, flag, 1, file);   // HERE
    }

    fclose(file);
}

long getFileSize(char* path)  {
    long size = 0;
    FILE* file = fopen(path, "rb");
    assert(file);
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    return size;
}

void printFileSizeDiff(char* inputPath, char* outputPath)  {
    long inputSize = getFileSize(inputPath);
    long outputSize = getFileSize(outputPath);

    printf("Input size: %ld bytes, Output size: %ld bytes, Ratio in percentage: %.2f\n", inputSize, outputSize, (((float)outputSize / (float)inputSize) * 100));
}


bool compress(int argc, char** argv)  {
    InitialDataCompress myData = getInitialDataCompress(argc, argv);
    Image myImage = loadImage(myData.inputPath);

    int size = 0;
    Sequence* mySequences = getSequences(myImage, myData, &size);

    if (mySequences == NULL)  {
        printf("Wrong command line arguments - incompatible with dimensions.\n");
        return false;
    }

    saveSequences(mySequences, size, myImage, myData.outputPath, myData.blockSize);
    printFileSizeDiff(myData.inputPath, myData.outputPath);

    free(myImage.pixels);
    free(mySequences);
    return true;
}