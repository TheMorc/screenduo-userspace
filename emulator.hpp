//oh hi, you are looking at my first ever header file, emulator.hpp
//this is just a complementary file for emulator.cpp to get it to compile

#ifndef _EMU
#define _EMU
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "SDL2/SDL.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *sdlTexture;
extern Uint32 * pixels;

extern bool logEMU;
extern int SDLTextDelay;

void Init();
int KeyHandler();
void QuitKeyHandler(); //mainly used for screen mode which does not have any other functions
void Render(uint8_t *data, bool textmode = false, bool both = false); //forcing textmode to false because we use it only sometimes(more like only for text mode)
void SetTitlebar(const char *text);

#endif