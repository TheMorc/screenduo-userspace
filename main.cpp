/* original program screenduo4linux written on or before Jun 1 2010 */
/* geoff@spacevs.com Geoffrey McRae (images)                        */
/* putpixel, putpixelxl written by Andrei Sokolov 7 Sep 2013        */ 
/* multiline color text support written by Bob Gill 19 Jun 2016     */
/* This program is licensed under the GNU GPL v2                    */
/* Morc 30.8.2019, čas: 00:10 prerábka pre macOS                    */
/* funkčný port, cca 00:20                                          */
/*						   update 20.8.2020:			   			*/
/*     this main.cpp is going to be only for the core functions     */
/*	  init of lua, settings and lua scripts is going to be here     */
/*   SDL functions are in emulator.cpp, to make this code prettier  */
/* libusb functions are in libusb.cpp and "graphics lib" in gl.cpp  */

//standard C++ libs
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>

//libusb, SDL2, lua libs
#include "libusb.h"
#include "SDL2/SDL.h"
#include "include/lua.hpp"

//screenduo-userspace own libs
#include "font8x8_extended.h"
#include "emulator.hpp"
#include "libusb.hpp"
#include "gl.hpp"

#define MAXCHAR 1000
#define INFO "ASUS ScreenDUO - opensource userspace driver written in C++ for Windows/macOS/Linux\n"

//struct for libusb image
typedef struct {
    uint8_t		u1;
    uint8_t		u2;
    uint16_t	u3;
    uint32_t	length;
    uint16_t	x, y, w, h;
    uint16_t	u4;
    uint16_t	u5;
    uint16_t	u6;
    uint16_t	u7;
    uint16_t	u8;
    uint16_t	u9;
    uint16_t	u10;
    uint16_t	u11;
} __attribute__ ((packed)) image_t;


//variable declarations for libusb and the image data
int r;
libusb_context *ctx;
libusb_device_handle *device;
uint8_t	image[sizeof(image_t) + (230400)];
uint8_t *data	= image + sizeof(image_t);

//Lua VM, thanks to javix9's Embedding Lua in C++ and the Automating Sequences via Lua Coroutines in C++ video!
lua_State *LVM = luaL_newstate(); //init the Lua VM 

bool processLua(lua_State *L, int r) //function to process the Lua code and print errors if necessary
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		printf("[Lua VM] %s\n", errormsg.c_str());
		return false;
	}
	return true;
}

bool SDLemu,logINFO,showInfo,SDLandlibusb = false; //These variables are changed using LVM handling the settings.lua

//LVM cross functions
int LVM_render(lua_State* L){
	if(lua_gettop(L) != 1) return -1;
	if(SDLandlibusb){ //write both
    	_lusb::dev_write(device, image, sizeof(image));
	   	_EMU::Render(data, lua_toboolean(L,1), true);
    }else if(!SDLemu) //write to dduo
    	_lusb::dev_write(device, image, sizeof(image));
    else
	   	_EMU::Render(data, lua_toboolean(L,1), false); //else render using SDL
	return 0;
}

int LVM_puticon(lua_State* L){
	if(lua_gettop(L) != 5) return -1;
	puticon(data,lua_tonumber(L,1),lua_tonumber(L,2),lua_tostring(L,3), lua_toboolean(L,4), lua_toboolean(L,5));
	return 0;
}

int LVM_putpixel(lua_State* L){
	if(lua_gettop(L) != 5) return -1;
	putpixel(data,lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5));
	return 0;
}

int LVM_putpixelxl(lua_State* L){
	if(lua_gettop(L) != 5) return -1;
	putpixelxl(data,lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5));
	return 0;
}

int LVM_putchar(lua_State* L){
	if(lua_gettop(L) != 6) return -1;
	putchar(data,lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5), lua_tonumber(L,6));
	return 0;
}

int LVM_putbg(lua_State* L){
	if(lua_gettop(L) != 1) return -1;
	putbg(data,lua_tostring(L,1));
	return 0;
}

int LVM_fillscreen(lua_State* L){
	if(lua_gettop(L) != 1) return -1;
	fillscreen(data,lua_tonumber(L,1));
	return 0;
}

int LVM_dimscreen(lua_State* L){
	if(lua_gettop(L) != 1) return -1;
	dimscreen(data,lua_tonumber(L,1));
	return 0;
}

int LVM_clearscreen(lua_State* L){
	if(lua_gettop(L) != 0) return -1;
	clearscreen(data);
	return 0;
}

int LVM_usleep(lua_State* L){
	if(lua_gettop(L) != 1) return -1;
	usleep(lua_tonumber(L,1));
	return 0;
}

//complementary function for lua button press events which is similiar to the original C++ one but reroutes button codes to lua
int luaButtonPress(char btn_code){
	if(btn_code != -1){ //only react when the button press code is not -1
		lua_getglobal(LVM, "buttonPress");
		if(lua_isfunction(LVM, -1)){
			lua_pushnumber(LVM, btn_code);
			if (lua_pcall(LVM, 1, 0, 0) != LUA_OK){
				std::string errormsg = lua_tostring(LVM, -1);
				printf("[Lua VM] %s\n", errormsg.c_str());
				return false;
			}
		}
	}
}

//common simple code for button handling
void buttonPress(char btn_code) { 
    switch(btn_code) {
    case -1: //just a thing to not react to the -1 returned from SDL or libusb
    	break; 
    case 0:
    	if(logINFO)
        	printf("enter pressed\n");
        break;
    case 1:
        if(logINFO)
        	printf("dpad left pressed\n");
        break;
    case 2:
        if(logINFO)
        	printf("dpad right pressed\n");
        break;
    case 3:
        if(logINFO)
        	printf("dpad up pressed\n");
        break;
    case 4:
        if(logINFO)
        	printf("dpad down pressed\n");
        break;
    case 6:
        if(logINFO)
        	printf("back pressed\n");
        break;
    case 12:
        if(logINFO)
        	printf("App 1 pressed\n");
        break;
    case 13:
        if(logINFO)
        	printf("App 2 pressed\n");
        break;
    default:
        if(logINFO)
        	printf("unknown button %d (report this to me)\n", btn_code);
        break;
	}
}

int x2d(char x) {
	if((x == 'a') || (x == 'A')) return 10;
	if((x == 'b') || (x == 'B')) return 11;
	if((x == 'c') || (x == 'C')) return 12; 
	if((x == 'd') || (x == 'D')) return 13;
	if((x == 'e') || (x == 'E')) return 14;
	if((x == 'f') || (x == 'F')) return 15; 
	else
	return (x-48);   
}

enum { MAXL = 64 };

int main(int argc, char **argv) {
    if (argc < 2) {
        printf(INFO);
    	printf("\nError: Too few arguments given.\n\nUsage:\n        <anytext>     Text mode (no args needed, use quotes)\n        --ui          UI mode\n        --screen      Screen mode, provide screen.bmp file in exec folder");
    	printf("\n\nSettings for SDL and others can be changed in settings.lua located in the Scripts folder.");
    	printf("\n\nFormatting guide for text mode:\n\\n - Newline\n\\c<0-E> - Text Color\n\\a<FFFFFF> - Text Color in HEX format in both lower and uppercase format (ex: \aFEA4dC)\n\\p<X>,<Y>, - Puts pixel in current color at X pos <0-320> and Y pos <0-240> Last comma is required.\n\\b<X1>,<X2>,<Y1>,<Y2>, - Draws a box from X1, Y1 to X2, Y2 pos. Last comma is required.\n");
    	printf("\nExamples:\n./duo \"\\affffff\\p20,130,\" - Puts a white pixel at X:20, Y:130\n./duo \"Fred\\n\\c3Barney\" - Prints Fred in white color and Barney in blue color on new line\n");
        return -1;
    } 

	//
	//Lua LVM - function registration, settings and other mandatory stuff
	//
	luaL_openlibs(LVM);
	
	lua_register(LVM, "render", LVM_render);
	lua_register(LVM, "puticon",LVM_puticon);
	lua_register(LVM, "putbg", LVM_putbg);
	lua_register(LVM, "putpixel",LVM_putpixel);
	lua_register(LVM, "putchar",LVM_putchar);
	lua_register(LVM, "putpixelxl", LVM_putpixelxl);
	lua_register(LVM, "fillscreen",LVM_fillscreen);
	lua_register(LVM, "dimscreen",LVM_dimscreen);
	lua_register(LVM, "clearscreen",LVM_clearscreen);
	lua_register(LVM, "usleep",LVM_usleep);
	
	if(processLua(LVM, luaL_dofile(LVM, "Scripts/settings.lua")))
	{ 
		lua_getglobal(LVM, "logINFO"); //gather settings for logINFO, to show or not show debug info
		if (lua_isboolean(LVM, -1)){
			logINFO = lua_toboolean(LVM, -1);
			_EMU::logEMU = logINFO;
			_lusb::logLUSB = logINFO;
		}
	
		lua_getglobal(LVM, "showInfo"); //gather settings for showInfo, to show or not show debug info
		if (lua_isboolean(LVM, -1)){
			showInfo = lua_toboolean(LVM, -1);
		}
		
		if(showInfo)
			printf(INFO);
	
		if(logINFO) //we need to gather logINFO first before sending Loading settings, this is a bit reversed, but fixed like this with this workaround
			printf("[Lua VM] Loading settings\n[Lua VM] logINFO is %d\n",logINFO);
		
		
		lua_getglobal(LVM, "SDL"); //gather settings for SDL, to usr or not use the SDL emulator
		if (lua_isboolean(LVM, -1)){
			SDLemu = lua_toboolean(LVM, -1);
			if(logINFO)
        		printf("[Lua VM] SDL is %d\n",SDLemu);
		}
		
		lua_getglobal(LVM, "SDLTextDelay"); //gather settings for SDLTextDelay, to usr or not use the SDL emulator
		if (lua_isnumber(LVM, -1)){
			_EMU::SDLTextDelay = lua_tonumber(LVM, -1);
			if(logINFO)
        		printf("[Lua VM] SDLTextDelay is %d\n",SDLTextDelay);
		}
		
		lua_getglobal(LVM, "SDLandlibusb"); //gather settings for SDLandlibusb, to usr or not use the SDL emulator
		if (lua_isboolean(LVM, -1)){
			SDLandlibusb = lua_toboolean(LVM, -1);
			if(logINFO)
        		printf("[Lua VM] SDLandlibusb is %d\n",SDLandlibusb);
		}
	}
	//
	//Lua LVM - functions settings and other stuff
	//
	

	if(SDLandlibusb){
    	if ((r = libusb_init(&ctx)) < 0)
    	    return r;
	
    	device = libusb_open_device_with_vid_pid(ctx, 0x1043, 0x3100);
    	if (!device) {
    	    printf("[libusb] Error: Failed to open the device (is your ScreenDUO connected?)\n");
    	    return -1;
    	} //libusb was unable to find ScreenDUO connected to your host system, exiting

    	if (!_lusb::hw_init(device)) {
    	    libusb_close(device);
    	    libusb_exit(ctx);
    	    return -1;
		}
    }else if(!SDLemu){
    	if ((r = libusb_init(&ctx)) < 0)
    	    return r;
	
    	device = libusb_open_device_with_vid_pid(ctx, 0x1043, 0x3100);
    	if (!device) {
    	    printf("[libusb] Error: Failed to open the device (is your ScreenDUO connected?)\n");
    	    return -1;
    	} //libusb was unable to find ScreenDUO connected to your host system, exiting

    	if (!_lusb::hw_init(device)) {
    	    libusb_close(device);
    	    libusb_exit(ctx);
    	    return -1;
		}
		
    }

    memset(image, 0, sizeof(image));

    image_t *header = (image_t*)image;
    
    header->u1	= 0x02;
    header->u2	= 0xf0;
    header->u3	= 0x20; /* bits per pixel? */
    header->x	= 0;
    header->y	= 0;
    header->w	= 320;
    header->h	= 240;
    header->length	= sizeof(image_t) + (230400);
    header->u5	= 0x01; /* no idea */

    memset(data, 0x00, sizeof(image) - sizeof(image_t));
    
    //original textmode ancient variables
    int x,y;
    int set;
    int mask;
    int c;
    int cc;
    int color = 1	; // 1-white 2-red 3-blue 4-green 5-cyan 6-yellow 7-magenta 8-black
    int red   = 255;
    int green = 255;
    int blue  = 255;
    int xpos  = 0;
    int ypos  = 0;
    int scale = 1;
    int ra    = 0;  // 6 variables for arbitrary colors
    int rb    = 0;
    int ga    = 0;
    int gb    = 0;
    int ba    = 0;
    int bb    = 0;
    int xa    = 0;  // 6 variables for box
    int xb    = 0;
    int ya    = 0;
    int yb    = 0;
    int lnx   = 0;
    int lny   = 0;
    int line  = 0;
    int nl    = 0;
    int nc    = 0;
    int maxlines = 13; /* maximum number of lines for the device (starting at 0) with linespace at 10 */
    int cx = 0;   
    int linespace = 8;  /* line spacing.  8 is small, 10 looks ok */
    
	if(logINFO)
    	if(SDLandlibusb) //check for SDL emu
			printf("[screenduo] Running in both SDL and libusb mode!\n");
		else if (SDLemu)
			printf("[screenduo] Running in SDL mode!\n");
		else
			printf("[screenduo] Running in libusb mode!\n");
		
	if(SDLandlibusb)
		_EMU::Init();
	else if(SDLemu)
		_EMU::Init();
			
    if(strcmp(argv[1],"--ui")==0)
	{
		if(logINFO)
        	printf("[screenduo] Lua UI mode\n");
        	
		if(processLua(LVM, luaL_dofile(LVM, "Scripts/ui.lua"))){ //open the ui.lua and do whatever it wants to do
			
			lua_getglobal(LVM, "preUI");
			if(lua_isfunction(LVM, -1)){
				if (lua_pcall(LVM, 0, 0, 0) != LUA_OK){
					std::string errormsg = lua_tostring(LVM, -1);
					printf("[Lua VM] %s\n", errormsg.c_str());
					return false;
				}
			}
			
			//if there is someone who does not get it why i am looping here
			//it is because i want the LVM to load the file and to save the variables
			//and then execute the UI function from C++ like update functions elsewhere
			while (1){
				if(SDLandlibusb){
					luaButtonPress(_lusb::get_buttons(device));
					luaButtonPress(_EMU::KeyHandler());
				}else if(!SDLemu){ //don't try to get_buttons from libusb during emulation
					luaButtonPress(_lusb::get_buttons(device)); //direct button events from libusb
				}
				else
		   			luaButtonPress(_EMU::KeyHandler()); //direct keyboard events from SDL
			
				lua_getglobal(LVM, "UI");
				if(lua_isfunction(LVM, -1)){
					if (lua_pcall(LVM, 0, 0, 0) != LUA_OK){
						std::string errormsg = lua_tostring(LVM, -1);
						printf("[Lua VM] %s\n", errormsg.c_str());
						return false;
					}
				}
			}
		}
		
	}
	else if(strcmp(argv[1],"--cat")==0)
	{
		if(logINFO)
        	printf("[screenduo] Muf\n");
		
		while (1) {
			if(SDLandlibusb)
				_EMU::QuitKeyHandler();
			else if(SDLemu)
				_EMU::QuitKeyHandler();
		
        	putbg(data, "Resources/Muf.bmp");
			
			if(SDLandlibusb){ //write using libusb, else SDL
				_lusb::dev_write(device, image, sizeof(image));
				_EMU::Render(data, false, true);
			}else if(!SDLemu)
				_lusb::dev_write(device, image, sizeof(image));
			else
				_EMU::Render(data);
    			
    	}
	}
	else if(strcmp(argv[1],"--screen")==0)
	{
		if(logINFO)
        	printf("[screenduo] Screen mode\n");
		
    	int size = 230400;
    	char *BMPdata = new char[230400]; // allocate 3 bytes per pixel
    	int i;
		while (1) {
			if(SDLandlibusb)
				_EMU::QuitKeyHandler();
			else if(SDLemu)
				_EMU::QuitKeyHandler();
		
        	putbg(data, "screen.bmp");
			
			if(SDLandlibusb){ //write using libusb, else SDL
				_lusb::dev_write(device, image, sizeof(image));
				_EMU::Render(data, false, true);
			}else if(!SDLemu)
				_lusb::dev_write(device, image, sizeof(image));
			else
				_EMU::Render(data);
    			
    	}
	}else{
    for (c=0; c < strlen(argv[1]); c++) {
		if(argv[1][c] == '\\') {         /* broken out for multiple \x items */
		c++;
		cc=argv[1][c];
	switch(cc) { 
		//text mode
		
		//nový riadok
		case('n') :
		          c++;  /* skip over the 'n' to the next character */ 
			  line+= linespace;  /* linefeed */
			  if (line > (linespace * maxlines)) line = 0;  /* newline screenwrap */
		          cx = 0;   /* carriage return */
			  nl = 1;
			  break;
		
		//farba
	    case('c') :		
			c++;  /* skip 1 character */
			// set text colors (feel free to mix your own)
			if(argv[1][c] == '1') { red=255;green=255;blue=255; }  // white
			if(argv[1][c] == '2') { red=255;green=0;blue=0; }      // red 
			if(argv[1][c] == '3') { red=0;green=0;blue=255; }      // blue
			if(argv[1][c] == '4') { red=0;green=255;blue=0; }      // green
			if(argv[1][c] == '5') { red=0;green=255;blue=255; }    // cyan
			if(argv[1][c] == '6') { red=255;green=255;blue=0; }    // yellow
			if(argv[1][c] == '7') { red=255;green=0;blue=255; }    // magenta
			if(argv[1][c] == '8') { red=0;green=0;blue=0; }        // black
			if(argv[1][c] == '9') { red=255;green=102;blue=0; }    // orange
        	if(argv[1][c] == 'a') { red=255;green=0;blue=153; }    // pink
            if(argv[1][c] == 'b') { red=51;green=0;blue=153; }     // navy
            if(argv[1][c] == 'c') { red=0;green=170;blue=0; }      // lime
            if(argv[1][c] == 'd') { red=102;green=51;blue=0; }     // dkbrown
            if(argv[1][c] == 'e') { red=153;green=102;blue=51; }   // ltbrown
			if(argv[1][c] == 'f') { red=128;green=128;blue=128; }  // gray
			c++;  // skip over the color number in the string
			nc = 1;
		break;

		//hex farba
		case('a') :               // arbitrary color (0-FF)x3
			  c++;ra=16*x2d(argv[1][c]);      // high red bits
			  c++;rb=x2d(argv[1][c]);ra+=rb;  // red low+red high 
			  c++;ga=16*x2d(argv[1][c]);      // high green bits
                          c++;gb=x2d(argv[1][c]);ga+=gb;  // green low+green high
			  c++;ba=16*x2d(argv[1][c]);      // blue red bits
                          c++;bb=x2d(argv[1][c]);ba+=bb;  // blue low+blue high
			  red=(ra % 256);green=(ga % 256);blue=(ba % 256);   // sanity checks
			  c++;
			  nc = 1;
			  break;

		//pixel
		case('p') :               // draw pixel
			 c++;xpos=0;scale=1;while((argv[1][c] != ',') && (xpos < 320)){xpos=xpos*10;xpos+=(argv[1][c]-48);c++;}
			 c++;ypos=0;scale=1;while((argv[1][c] != ',') && (ypos < 240)){ypos=ypos*10;ypos+=(argv[1][c]-48);c++;}
			 xpos=(xpos % 320);  // sanity checks
			 ypos=(ypos % 240);
		         set = 1;
			 putpixel(data,xpos,ypos,set ? red : 0,set ? green : 0,set ? blue : 0); 
			 c++;
			 nc = 1;
			 break;
			 
		//obdĺžnik
	    case('b') :               // draw box	
			c++;xa=0;scale=1;while((argv[1][c] != ',') && (xa < 320)){xa=xa*10;xa+=(argv[1][c]-48);c++;}
			c++;xb=0;scale=1;while((argv[1][c] != ',') && (xb < 320)){xb=xb*10;xb+=(argv[1][c]-48);c++;}
			c++;ya=0;scale=1;while((argv[1][c] != ',') && (ya < 200)){ya=ya*10;ya+=(argv[1][c]-48);c++;}
            c++;yb=0;scale=1;while((argv[1][c] != ',') && (yb < 200)){yb=yb*10;yb+=(argv[1][c]-48);c++;}
			// printf("\nya: %i yb: %i xa: %i xb: %i",ya,yb,xa,xb); 
			ya=(ya % 241);  // sanity checks
			yb=(yb % 241);
			xa=(xa % 321);
			xb=(xb % 321); 
			set = 1;
			for(lnx=xa;lnx<xb;lnx++)
				for(lny=ya;lny<yb;lny++)
					putpixel(data,lnx,lny,set ? red : 0,set ? green : 0,set ? blue : 0);
			c++;
			nc = 1;
			break;
			
		//náhodné pixely na test
		case('R') :               // random mess	
			c++;for(r = 0; r < header->w * header->h * 3; r++)
            	data[r] = rand()%255;
			break;
		}
						
		}
		else //písanie znakov, nepodporuje diakritiku
		{
			char *bitmap = font8x8_extended[argv[1][c]];
   	 	    //printf("%d\n", argv[1][c]);
   	 	    for (y=0; y < 8; y++) {
    	        for (x=0; x < 8; x++) {
    	            set = bitmap[y] & 1 << x;
    	            //putpixel(data,x+cx*8,y,set ? red : 0,set ? green : 0,set ? blue : 0);
    	            putpixelxl(data,x+cx*8,y+line,set ? red : 0,set ? green : 0,set ? blue : 0);
    	        }
			}
		}
		
		cx++;
		if ((cx % 20) == 0) line += linespace;  /* text end-of-line crlf */
		if (line > (linespace * maxlines)) line = 0; /* bottom of screen screenwrap */
		if (nc) {nc=0;c--;cx--;}
		if (nl) {nl=0;cx=0;c--;} 
    }//switch end
    	if(logINFO)
        	printf("[screenduo] Text mode\n");
        	
    	if(SDLandlibusb){ //write using libusb, else SDL
			_lusb::dev_write(device, image, sizeof(image));
			_EMU::Render(data, true, true);
		}else if(!SDLemu)
			_lusb::dev_write(device, image, sizeof(image));
		else
			_EMU::Render(data, true);
    }

	if(!SDLemu){
    	libusb_attach_kernel_driver(device, 0x0);
    	libusb_close(device);
    	libusb_exit(ctx);
    }
    lua_close(LVM);
    return 0;
}