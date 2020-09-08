/* Welcome to the emulator part of this code, this part includes functions from the original main.cpp code */
/* This is just used to make parts of the code more clear and not a cluttered mess until it was in one file*/
/*  This is also used for function declarations only for the emulator, that's why it this is a bit small   */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "SDL2/SDL.h"
#include "emulator.hpp"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *sdlTexture = NULL;
Uint32 * pixels = new Uint32[76800];

bool logEMU = false;
int SDLTextDelay = 5000;

//
//SDL Emulator - window init
//
void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("ASUS ScreenDUO - SDL emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        320, 240,
        SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetWindowMinimumSize(window, 320, 240);

    SDL_RenderSetLogicalSize(renderer, 320, 240);

    sdlTexture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
        320, 240);
    
    if(logEMU)
    	printf("[SDL] SDL emulator init\n");
}		
	
	
//
//SDL Emulator - the whole quit and button handling
//
int KeyHandler() {	
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      	if (event.type == SDL_QUIT) exit(0);
       	if( event.type == SDL_KEYDOWN )
        {
        	switch( event.key.keysym.sym )
        	{
            	//ESC quit
            	case SDLK_ESCAPE:
                	exit(0);
                    break;
                    
                    
                //arrow keys
                case SDLK_LEFT:
                	if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(1);
                    break;
                        
                case SDLK_RIGHT:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(2);
                    break;
                        
                case SDLK_UP:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(3);
                    break;

                case SDLK_DOWN:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                   	return(4);
                	break;
                        
                        
                //enter and back
                case SDLK_RETURN:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(0);
                    break;
                        
                case SDLK_BACKSPACE:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(6);
                    break;
                        
                        
                //app1 and app2
                case SDLK_PERIOD:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(13);
                    break;
                        
                case SDLK_COMMA:
                    if(logEMU)
                		printf("[SDL] emulator button: ");
                    return(12);
                    break;
            }
        }
    }
    return -1;
}


//
//SDL Emulator - quit handling
//
void QuitKeyHandler() {	
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      	if (event.type == SDL_QUIT) exit(0);
       	if( event.type == SDL_KEYDOWN )
        {
        	if(event.key.keysym.sym == SDLK_ESCAPE){
                	exit(0);
                    break;
            }
        }
    }
}


//
//SDL Emulator - framebuffer rendering
//
void Render(uint8_t *data, bool textmode, bool both) {
	//SDL event handler for text mode (looks like it doesn't show the window without this)
	if(textmode){
		SDL_Event event;
    	while (SDL_PollEvent(&event))
    	{
      		if (event.type == SDL_QUIT) exit(0);
    	}
    }

	for (int y = 0; y < 240; ++y)
	{
		for (int x = 0; x < 320; ++x)
		{
			pixels[x + y * 320] = ((data[(x+y*320)*3]<<16) | (data[(x+y*320)*3+1]<<8) | data[(x+y*320)*3+2]);
		}
	}
        	
	//update and write
	SDL_UpdateTexture(sdlTexture, NULL, pixels, 320 * 4);
	SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	if(!both) //do not delay when running with both sdl and libusb
		if(!textmode)
			SDL_Delay(250); //time delay to relatively match overhead of the libusb write speed
		else
			SDL_Delay(SDLTextDelay); //5 sec delay to even see the text mode message and to not have it dissappear right away	
}


//
//SDL Emulator - change window name, convenience feature, nothing special.
//
void SetTitlebar(const char *text){
	SDL_SetWindowTitle(window, text); 
}