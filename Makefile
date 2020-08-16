print-%: @echo $* is $($*)
CC=g++
SRC = main.cpp
IDIR=/usr/local/Cellar/libusb/1.0.21/include/libusb-1.0
CFLAGS=-I$(IDIR) -lusb-1.0

OBJ = duo
LIBS=$(-lusb-1.0,-L/usr/local/lib)
#CC=gcc
#SRC = main.c
#BTNSRC = button.c
# %.o: %.c $(DEPS)
#       $(CC) -c -o $@ $< $(CFLAGS)

all: duo

duo: duo
	$(CC) $(SRC) -o $@ $(CFLAGS) -v -L/usr/local/lib  -lSDL2 -O2
	install_name_tool -change /usr/local/opt/libusb/lib/libusb-1.0.0.dylib "@loader_path/libusb-1.0.0.dylib" ./duo #relink dylibs to same folder as binary
	install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib "@loader_path/libSDL2-2.0.0.dylib" ./duo

# original C
#	gcc main.c -o $@ -I/usr/local/Cellar/libusb/1.0.23/include/libusb-1.0 -lusb-1.0

#duobtn: $(OBJ)
#	$(CC) $(BTNSRC) -o $@ $(CFLAGS) $(LIBS)

clean: 
	rm $(OBJ)