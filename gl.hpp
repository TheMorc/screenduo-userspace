//So you are looking at the header of my graphics lib
//As said before, it is used to move functions from main.cpp here

#ifndef _GL
#define _GL
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
//#include "font8x8_extended.h"

void putbmp(uint8_t *data, int x, int y, const char *filename, bool transparency = false, bool compatibility = false);
void puttext(uint8_t *data, int Xp, int Yp, const char *text);
void putpixelxl(uint8_t *data, int x, int y, int r, int g, int b);
void putpixel(uint8_t *data, int x, int y, char r, char g, char b);
void putchar(uint8_t *data, int x, int y, char letter, int red = 255, int green = 255, int blue = 255);
void fillscreen(uint8_t *data, char color);
void clearscreen(uint8_t *data);
void dimscreen(uint8_t *data, int amount = 25);
void putbmpbg(uint8_t *data, const char *filename);
void putpng(uint8_t *data, int x, int y, const char *filename, int blendmode = 0);

#endif