/* Welcome to the emulator part of this code, this part includes functions from the original main.cpp code */
/* This is just used to make parts of the code more clear and not a cluttered mess until it was in one file*/
/*     This is also used for function declarations only for the emulator, that's why it is this small      */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "SDL2/SDL.h"

SDL_Window *window = NULL; //defining SDL things here so it will work across functions
SDL_Renderer *renderer = NULL;
SDL_Texture *sdlTexture = NULL;
Uint32 * pixels = new Uint32[76800];

int Emu_Init() {
    //
    //SDL Emulator - window init
    //
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
    
	//
    //SDL Emulator - window init
    //
    return 0;
}

int Emu_Message() {
	printf("Running in SDL emulated mode!\n\n");
    return 0;
}			
	
int Emu_KeyboardEvent() {	
	//
   	//SDL Emulator - quit and button handling
    //
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
                        printf("SDL emulator button: ");
                        main::buttonPress(1);
                        break;
                        
                        case SDLK_RIGHT:
                        printf("SDL emulator button: ");
                        buttonPress(2);
                        break;
                        
                        case SDLK_UP:
                        printf("SDL emulator button: ");
                        buttonPress(3);
                        break;

                        case SDLK_DOWN:
                        printf("SDL emulator button: ");
                        buttonPress(4);
                        break;
                        
                        
                        //enter and back
                        case SDLK_RETURN:
                        printf("SDL emulator button: ");
                        buttonPress(0);
                        break;
                        
                        case SDLK_BACKSPACE:
                        printf("SDL emulator button: ");
                        buttonPress(6);
                        break;
                        
                        
                        //app1 and app2
                        case SDLK_PERIOD:
                        printf("SDL emulator button: ");
                        buttonPress(13);
                        break;
                        
                        case SDLK_COMMA:
                        printf("SDL emulator button: ");
                        buttonPress(12);
                        break;
                    }
            	}
	//
    //SDL Emulator - quit and button handling
    //
    return 0;
}

int Emu_Render() {
	//
        	//SDL Emulator - framebuffer side
        	//
        	if(SDLemu){
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
			
			SDL_Delay(250); //libusb overhead time delay to relatively match speed
			}
        	//
        	//SDL Emulator - framebuffer side
        	//
    return 0;
}
		   	
int Emu_TextModeInit() {	
  
    	//
        //SDL Emulator - show only one frame for 5 seconds and exit, it's not useful for text mode to stay opened
        //
        if(SDLemu){
    	SDL_Event event;
        while (SDL_PollEvent(&event))
        {
           	if (event.type == SDL_QUIT) exit(0);
        }

		//copy of framebuffer side
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
		//copy of framebuffer side
		
        SDL_Delay(5000);
        }
        //
        //SDL Emulator - show only one frame for 5 seconds and exit, it's not useful for text mode to stay opened
        //
    
    return 0;
}