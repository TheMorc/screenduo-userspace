/* original program screenduo4linux written on or before Jun 1 2010 */
/* geoff@spacevs.com Geoffrey McRae (images)                        */
/* putpixel, putbigpixel written by Andrei Sokolov 7 Sep 2013       */ 
/* multiline color text support written by Bob Gill 19 Jun 2016     */
/* This program is licensed under the GNU GPL v2                    */
/* Button Code by Ash Hughes (https://github.com/ashh87)*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "libusb.h"
#include "font8x8_extended.h"

#define DEV_VID		0x1043
#define DEV_PID		0x3100
#define HID_IF		0x0
#define HID_AS		0x0
#define HID_WR_EP	0x02
#define HID_RD_EP	0x81

typedef struct {
    uint32_t	dCBWSignature;
    uint32_t	dCBWTag;
    uint32_t	dCBWDataTransferLength;
    uint8_t		bmCBWFlags;
    uint8_t		bCBWLUN;
    uint8_t		bCBWCBLength;
} __attribute__ ((packed)) cbw_t;

typedef struct {
    uint16_t	u1;
    uint32_t	blockSize;
    uint32_t	totalSize;
    uint8_t		index; //?
    uint8_t		u2;
    uint8_t		unused[4];
} __attribute__ ((packed)) cb_header_t;

typedef struct {
    uint16_t	u1;
    uint32_t	blockSize;
    uint32_t	totalSize;
    uint8_t		index; //?
    uint8_t		u2;
    uint8_t		unused[485];
} __attribute__ ((packed)) cb_footer_t;

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

int hw_init(libusb_device_handle *device) {
    if (libusb_reset_device(device) != 0) {
        libusb_release_interface(device, HID_IF);
        printf("Failed to reset the device\n");
        return 0;
    }

    if (libusb_kernel_driver_active(device, HID_IF) == 1 && libusb_detach_kernel_driver(device, HID_IF) != 0) {
        printf("Failed to activate and detatch\n");
        return 0;
    }

    if (libusb_set_configuration(device, 1) != 0) {
        libusb_release_interface(device, HID_IF);
        printf("Failed to set configuration\n");
        return 0;
    }

    if (libusb_claim_interface(device, HID_IF) != 0) {
        printf("Failed to claim interface\n");
        return 0;
    }

    if (libusb_set_interface_alt_setting(device, HID_IF, HID_AS) != 0) {
        libusb_release_interface(device, HID_IF);
        printf("Failed to set alt setting\n");
        return 0;
    }


    if (libusb_clear_halt(device, HID_WR_EP) != 0) {
        printf("Failed to clear WR halt");
        return 0;
    }


    if (libusb_clear_halt(device, HID_RD_EP) != 0) {
        libusb_release_interface(device, HID_IF);
        printf("Failed to clear RD halt\n");
        return 0;
    }

    return 1;
}

void hw_write(struct libusb_transfer *xfer) {
    /* free the data */
    free(xfer->buffer);
    libusb_free_transfer(xfer);
}

int dev_read(libusb_device_handle *device, uint8_t *data, unsigned int length) {
    int read = 0;
    if (libusb_bulk_transfer(device, HID_RD_EP, data, length, &read, 9000) != 0) {
        //printf("error reading: %d\n", read);
        return read;
    }
    return read;
}

void dump(uint8_t *data, int len) {
    printf("Dump: %0x\n", len);
    int i;
    for(i = 0; i < len; ++i) {
        printf("%02x ", data[i]);
        if ((i+1) % 16 == 0) printf("\n"); 
    }
    printf("\n");
}

/* old */
/* uint32_t flip32(uint32_t v) { */
/*    return ((v & 0xff000000) >> 24) | ((v & 0xff0000) >> 8) | ((v & 0xff00) << 8) | ((v & 0xff) << 24); */
/* } */

/* New */
uint32_t flip32(uint32_t v) {
    return ((v & 0xff000000) >> 24) | ((v & 0xff0000) >> 8) | ((v & 0xff00) << 8) | ((v & 0xff) << 24);
}


int dev_write(libusb_device_handle *device, uint8_t *data, unsigned int length) {
    int r, wrote;
    int pos = 0;
    uint8_t tmp[0xff];

    cbw_t		*cbw;
    cb_header_t	*header;
    cb_footer_t	*footer;

    uint8_t h[sizeof(cbw_t) + sizeof(cb_header_t)];
    uint8_t f[sizeof(cbw_t) + sizeof(cb_footer_t)];
    cbw	= (cbw_t      *)&h[0];
    header	= (cb_header_t*)&h[sizeof(cbw_t)];
    footer  = (cb_footer_t*)&f[sizeof(cbw_t)];

    cbw->dCBWSignature		= 0x43425355;
    cbw->dCBWTag			= 0x843d84a0;
    cbw->bmCBWFlags			= 0x00;
    cbw->bCBWLUN			= 0x00;
    cbw->bCBWCBLength		= 0x0c;
    header->u1			= 0x02e6;
    header->totalSize		= flip32(length);
    header->index			= 0;

    footer->u1			= 0x02e6;
    footer->totalSize		= header->totalSize;
    memset(header->unused, 0, sizeof(header->unused));
    memset(footer->unused, 0, sizeof(footer->unused));

    int block = 0;
    while(length > 0) {
        int copy = length > 0x10000 ? 0x10000 : length;

        cbw->dCBWDataTransferLength = copy;
        header->blockSize	= flip32(copy);
        header->index		= block;
        header->u2		= 0x0;

        libusb_bulk_transfer(device, HID_WR_EP, h, sizeof(h), &wrote, 100);
        if (wrote != sizeof(h)) {
            printf("Error writing CBW header\n");
            exit(-1);
        }

        r = libusb_bulk_transfer(device, HID_WR_EP, data, copy, &wrote, 0);
        data	+= wrote;
        pos	+= wrote;
        length	-= wrote;
        dev_read(device, tmp, 0xd);

        /* the footer cbw is the same as the header */
        memcpy(f, cbw, sizeof(cbw_t));
        footer->blockSize	= flip32(copy);
        footer->index		= block;
        footer->u2		= 0x0;

        libusb_bulk_transfer(device, HID_WR_EP, f, sizeof(f), &wrote, 100);
        if (wrote != sizeof(f)) {
            printf("Error writing CBW footer %d\n", wrote);
            exit(-1);
        }

        ++block;
    }
    return pos;
}

void print_button(char btn_code)
{
    printf("Button: ");
    switch(btn_code) {
    case 0:
        printf("Enter\n");
        break;
    case 1:
        printf("Left\n");
        break;
    case 2:
        printf("Right\n");
        break;
    case 3:
        printf("Up\n");
        break;
    case 4:
        printf("Down\n");
        break;
    case 6:
        printf("Back\n");
        break;
    case 12:
        printf("App 1\n");
        break;
    case 13:
        printf("App 2\n");
        break;
    default:
        printf("Unknown: %d\n", btn_code);
        break;
	}
}

int get_buttons(libusb_device_handle *device) {
    int r, wrote;
    int pos = 0;
    uint8_t tmp[256];
    uint8_t tmp2[512];

    cbw_t		*cbw;
    cb_header_t	*header;
    cb_footer_t	*footer;

    uint8_t h[sizeof(cbw_t) + sizeof(cb_header_t)];
    uint8_t f[sizeof(cbw_t) + sizeof(cb_footer_t)];
    cbw	= (cbw_t      *)&h[0];
    header	= (cb_header_t*)&h[sizeof(cbw_t)];
    footer  = (cb_footer_t*)&f[sizeof(cbw_t)];

    cbw->dCBWSignature		= 0x43425355;
    cbw->dCBWTag			= 0xe6abb010;
    cbw->bmCBWFlags			= 0x80;
    cbw->bCBWLUN			= 0x00;
    cbw->bCBWCBLength		= 0x0c;
    header->u1			= 0x03e7;
    header->totalSize		= 0;
    header->index			= 0;

    footer->u1			= 0x03e7;
    footer->totalSize		= header->totalSize;
    memset(header->unused, 0, sizeof(header->unused));
    memset(footer->unused, 0, sizeof(footer->unused));

    int block = 0;

    int copy = 0x100;

    cbw->dCBWDataTransferLength = copy;
    header->blockSize	= flip32(copy);
    header->index		= block;
    header->u2		= 0x0;

    libusb_bulk_transfer(device, HID_WR_EP, h, sizeof(h), &wrote, 100);
    if (wrote != sizeof(h)) {
        printf("Error writing CBW header\n");
        exit(-1);
    }

    int r_len = dev_read(device, tmp, 256);
    int tmp_r_len;
    if (r_len > 0) {
        while ((tmp_r_len = dev_read(device, tmp2, 512)) > 0) {
            // this never seems to happen
        }
        if (tmp[0] == 3 && tmp[1] == 0 && tmp[2] == 8 && tmp[3] == 0) {
            if (tmp[4] == r_len) {
                int n_buttons = tmp[4] - 8;
                for (int cur_button = 0; cur_button < n_buttons; ++cur_button) {
                    print_button(tmp[8 + cur_button]);
                }
            }
        }
    }

    // set fields for the setup packet as needed              
    uint8_t       bmReqType = 0x02;   // the request type (direction of transfer)
    uint8_t            bReq = 1;   // the request field for this packet
    uint16_t           wVal = 0;   // the value field for this packet
    uint16_t         wIndex = 0x81;   // the index field for this packet
    unsigned char*   dft = "";   // the data buffer for the in/output data
    uint16_t           wLen = 0;   // length of this setup packet 
    unsigned int     to = 0;       // timeout duration (if transfer fails)

    // finished
    int config = libusb_control_transfer(device,bmReqType,bReq,wVal,wIndex,dft,wLen,to);
    // a final read, do nothing with result
    dev_read(device, tmp2, 512);

    return pos;
}

void putpixel(uint8_t *data, int x, int y, char r, char g, char b) {
    data[(x+y*320)*3] = r;
    data[(x+y*320)*3+1] = g;
    data[(x+y*320)*3+2] = b;
}

void putbigpixel(uint8_t *data, int x, int y, char r, char g, char b) {
    data[(x*2+y*2*320)*3] = r;
    data[(x*2+y*2*320)*3+1] = g;
    data[(x*2+y*2*320)*3+2] = b;
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

int main(int argc, char *argv[]) {
    int			r;
    libusb_context		*ctx;
    libusb_device_handle	*device;

    if ((r = libusb_init(&ctx)) < 0)
        return r;

    device = libusb_open_device_with_vid_pid(ctx, DEV_VID, DEV_PID);
    if (!device) {
        printf("Failed to open the device\n");
        return -1;
    }

    if (!hw_init(device)) {
        libusb_close(device);
        libusb_exit(ctx);
        return -1;
    }

    uint8_t	image[sizeof(image_t) + (320 * 240 * 3)];
    memset(image, 0, sizeof(image));

    image_t *header = (image_t*)image;
    uint8_t	*data	= image + sizeof(image_t);

    header->u1	= 0x02;
    header->u2	= 0xf0;
    header->u3	= 0x20; /* bits per pixel? */
    header->x	= 0;
    header->y	= 0;
    header->w	= 320;
    header->h	= 240;
    header->length	= sizeof(image_t) + (header->w * header->h * 3);
    header->u5	= 0x01; /* no idea */

    memset(data, 0x00, sizeof(image) - sizeof(image_t));

    uint8_t frame;
    /*for(frame = 0; frame <= 2; frame++){
        for(r = 0; r < header->w * header->h * 3; r++)
            data[r] = rand()%255;
        dev_write(device, image, sizeof(image));
    }*/
    int x,y;
    int set;
    int mask;
    int c;
    int cc;
    int color = 1; // 1-white 2-red 3-blue 4-green 5-cyan 6-yellow 7-magenta 8-black
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
    int maxlines = 10; /* maximum number of lines for the device (starting at 0) with linespace at 10 */
    int cx = 0;   
    int linespace = 10;  /* line spacing.  8 is small, 10 looks ok */
    for (c=0; c < strlen(argv[1]); c++) {
	if(argv[1][c] == '\\') {         /* broken out for multiple \x items */
		c++;
		cc=argv[1][c];
		switch(cc) { 
		case('n') :
		          c++;  /* skip over the 'n' to the next character */ 
			  line+= linespace;  /* linefeed */
			  if (line > (linespace * maxlines)) line = 0;  /* newline screenwrap */
		          cx = 0;   /* carriage return */
			  nl = 1;
			  break;
			
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
		}
			
	}
	else
	{
        char *bitmap = font8x8_basic[argv[1][c]];
        for (y=0; y < 8; y++) {
            for (x=0; x < 8; x++) {
                set = bitmap[y] & 1 << x;
                //putpixel(data,x+cx*8,y,set ? 255 : 0,set ? 255 : 0,set ? 255 : 0);
                putbigpixel(data,x+cx*8,y+line,set ? red : 0,set ? green : 0,set ? blue : 0);
            }
          }
	}
	cx++;
	if ((cx % 20) == 0) line += linespace;  /* text end-of-line crlf */
	if (line > (linespace * maxlines)) line = 0; /* bottom of screen screenwrap */
	if (nc) {nc=0;c--;cx--;}
	if (nl) {nl=0;cx=0;c--;} 
    }

    dev_write(device, image, sizeof(image));

    while (1) {
        get_buttons(device);
        usleep(5*200000);
    }

    libusb_attach_kernel_driver(device, HID_IF);
    libusb_close(device);
    libusb_exit(ctx);
    return 0;
}
