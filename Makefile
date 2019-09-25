print-%: @echo $* is $($*)
CC=gcc
IDIR=/usr/local/Cellar/libusb/1.0.22/include/libusb-1.0
CFLAGS=-I$(IDIR) -lusb-1.0
# DEPS = hellomake.h
DEPS = 
SRC = main.c
BTNSRC = button.c
OBJ = duo
LIBS=$(-lusb-1.0,-L/usr/local/lib)

# %.o: %.c $(DEPS)
#       $(CC) -c -o $@ $< $(CFLAGS)

all: duo duobtn

duo: $(OBJ)
	$(CC) $(SRC) -o $@ $(CFLAGS) $(LIBS)

#duobtn: $(OBJ)
#	$(CC) $(BTNSRC) -o $@ $(CFLAGS) $(LIBS)

clean: 
	rm $(OBJ)
