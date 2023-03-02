#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "TGA.h"
#include "Data.h"
#include "compress.h"
#include "decompress.h"


/*
FINAL VERSION
optimalizace, vsechno jede
*/


/**
 * Wrapper function to determine correctness of command line arguments.
 * \param argc int. Number of command line arguments.
 * \param argv char**. Array containing command line arguments.
 * \return bool. Decision whether parameters are correct or incorrect.
*/
bool correctArguments(int argc, char** argv)  {
    // No arguments given.
    if (argc <= 2)  {
        return false;
    }

    // Check if arguments are correct for compress variant.
    if (strcmp(argv[1], "compress") == 0)  {
        return (correctArgumentsCompress(argc, argv));
    }

    // Check if arguments are correct for decompress variant.
    if (strcmp(argv[1], "decompress") == 0)  {
        return (correctArgumentsDecompress(argc, argv));
    }

    // If other variant, we let the user know we don't support it and end the program.
    return false;
}


int main(int argc, char** argv)  {
    if (correctArguments(argc, argv) == false)  {
        printf("Wrong command line arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "compress") == 0)  {
        bool done = compress(argc, argv);
        if (done == false)  {
            return 1;
        }
    }  

    if (strcmp(argv[1], "decompress") == 0)  {
        bool done = decompress(argc, argv);
        if (done == false)  {
            return 1;
        }
    }

    return 0;
}