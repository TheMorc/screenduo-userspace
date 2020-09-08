print-%: @echo $* is $($*)
OTHER1 = 
OTHER2 = 
OTHER3 = 
OTHER4 =
CC=g++
SRC = main.cpp emulator.cpp libusb.cpp gl.cpp
CFLAGS=-I/usr/local/Cellar/libusb/1.0.21/include/libusb-1.0 -lusb-1.0 -L/usr/local/lib -Iinclude -L. -llua52 -lSDL2

ifeq ($(OS),Windows_NT) #os detection thing to use the same makefile for windows, macos and linux
    CFLAGS = -I..\include -I..\lib -w -m32 -lmingw32 -lSDL2main -lSDL2 -llua52 -lusb-1.0
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS=-I/usr/include/libusb-1.0 -lusb-1.0 -L/usr/local/lib -lSDL2
    endif
    ifeq ($(UNAME_S),Darwin)
    	OTHER1 = ./bak.sh #this is my local backup script, nothing special
        CFLAGS=-I/usr/local/Cellar/libusb/1.0.21/include/libusb-1.0 -lusb-1.0 -L/usr/local/lib -Iinclude -L. -llua52 -lSDL2
		OTHER2 = install_name_tool -change /usr/local/opt/libusb/lib/libusb-1.0.0.dylib "@loader_path/libusb-1.0.0.dylib" ./duo #relink dylibs to same folder as binary
		OTHER3 = install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib "@loader_path/libSDL2-2.0.0.dylib" ./duo
		OTHER4 = install_name_tool -change liblua52.dylib "@loader_path/liblua52.dylib" ./duo
    endif
endif

.PHONY: duo  
duo:
	$(OTHER1)
	$(CC) $(SRC) -o $@ $(CFLAGS) -O2
	$(OTHER2)
	$(OTHER3)
	$(OTHER4)

# original C
#	gcc main.c -o $@ -I/usr/local/Cellar/libusb/1.0.23/include/libusb-1.0 -lusb-1.0

clean: 
	rm $@