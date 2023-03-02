#pragma once

/**
 * Unsigned char with a more convenient name for easier writing to files and reading from them.
*/
typedef unsigned char byte;



/**
 * Structure representing the head of TGA image.
 * contains all the information about the image.
*/
typedef struct {
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;



/**
 * Structure representing one pixel in TGA image.
 * contains BGR values in range (0-255).
*/
typedef struct {
    byte blue;
    byte green;
    byte red;
} Pixel;



/**
 * Structure representing the TGA image intself.
 * consists of TGA header, array of pixels, and width and height of original picture.
*/
typedef struct {
    TGAHeader header;
    Pixel* pixels;
    int width;
    int height;
} Image;



/**
 * Structure representing a sequence written into .jbk file.
 * consists of one pixel and count how many times we will copy the pixel into the final image after decompression.
*/
typedef struct {
    Pixel pixel;
    int length;
} Sequence;