/* Welcome to the libusb part of this code, this part includes functions from the original main.cpp code */
/* like emulator.cpp/hpp, this is used to shrink and unclutter the main.cpp code                         */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "libusb.hpp"
#include "libusb.h"

bool logLUSB = false;

//libusb
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


int hw_init(libusb_device_handle *device) {
    if (libusb_reset_device(device) != 0) {
        libusb_release_interface(device, HID_IF);
        printf("[libusb] Error: Failed to reset the device\n");
        return 0;
    }

    if (libusb_kernel_driver_active(device, HID_IF) == 1 && libusb_detach_kernel_driver(device, HID_IF) != 0) {
        //printf(INFO);
        printf("[libusb] Error: Failed to activate and detach\n");
        return 0;
    }

    if (libusb_set_configuration(device, 1) != 0) {
        libusb_release_interface(device, HID_IF);
        //printf(INFO);
        printf("[libusb] Error: Failed to set configuration\n");
        return 0;
    }

    if (libusb_claim_interface(device, HID_IF) != 0) {
        //printf(INFO);
        printf("[libusb] Error: Failed to claim interface\n");
        return 0;
    }

    if (libusb_set_interface_alt_setting(device, HID_IF, HID_AS) != 0) {
        libusb_release_interface(device, HID_IF);
        //printf(INFO);
        printf("[libusb] Error: Failed to set alt setting\n");
        return 0;
    }


    if (libusb_clear_halt(device, HID_WR_EP) != 0) {
        //printf(INFO);
        printf("[libusb] Error: Failed to clear WR halt\n");
        return 0;
    }


    if (libusb_clear_halt(device, HID_RD_EP) != 0) {
        libusb_release_interface(device, HID_IF);
        //printf(INFO);
        printf("[libusb] Error: Failed to clear RD halt\n");
        return 0;
    }
    
	//no errors encoutered, returning 1 and continuing
    return 1;
}

//toto zatiaľ vypadá jako nevyužitý kód ktorý sa "teoreticky" na neskór zíde
void hw_write(struct libusb_transfer *xfer) {
    /* free the data */
    free(xfer->buffer);
    libusb_free_transfer(xfer);
}

int dev_read(libusb_device_handle *device, uint8_t *data, unsigned int length, bool button = false) {
    int read = 0;
    if (libusb_bulk_transfer(device, HID_RD_EP, data, length, &read, 2000) != 0) {
    	if(!button)
        	printf("[libusb] Error reading data (try reconnecting your ScreenDUO)\n");
        return read;
    }
    return read;
}

uint32_t flip32(uint32_t v) {
    return ((v & 0xff000000) >> 24) | ((v & 0xff0000) >> 8) | ((v & 0xff00) << 8) | ((v & 0xff) << 24);
}

int get_buttons(libusb_device_handle *device) {
    int r, wrote;
    int pos = 0;
    int button = -1; //setting to -1 as default to not trigger the enter button
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
        printf("[libusb] Error writing CBW header\n");
        exit(-1);
    }

    int r_len = dev_read(device, tmp, 256, true);
    int tmp_r_len;
    if (r_len > 0) {
        while ((tmp_r_len = dev_read(device, tmp2, 512, true)) > 0) {
            // this never seems to happen
        }
        if (tmp[0] == 3 && tmp[1] == 0 && tmp[2] == 8 && tmp[3] == 0) {
            if (tmp[4] == r_len) {
                int n_buttons = tmp[4] - 8;
                for (int cur_button = 0; cur_button < n_buttons; ++cur_button) {
                    if(logLUSB)
                    	printf("[libusb] button: ");
                    button = tmp[8 + cur_button];
                }
            }
        }
    }

    // set fields for the setup packet as needed              
    uint8_t       bmReqType = 0x02;   // the request type (direction of transfer)
    uint8_t            bReq = 1;   // the request field for this packet
    uint16_t           wVal = 0;   // the value field for this packet
    uint16_t         wIndex = 0x81;   // the index field for this packet
    unsigned char*   dft;   // the data buffer for the in/output data
    uint16_t           wLen = 0;   // length of this setup packet 
    unsigned int     to = 0;       // timeout duration (if transfer fails)

    // finished
    int config = libusb_control_transfer(device,bmReqType,bReq,wVal,wIndex,dft,wLen,to);
    // a final read, do nothing with result
    dev_read(device, tmp2, 512, true);

    return button;
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
            //printf(INFO);
        	printf("[libusb] There was an error writing the CBW header (try reconnecting your ScreenDUO)\n");
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
            //printf(INFO);
        	printf("[libusb] There was an error writing the CBW footer at %d (try reconnecting your ScreenDUO)\n", wrote);
            exit(-1);
        }

        ++block;
    }
    return pos;
}

/*
int hw_open(int r,libusb_context *ctx, libusb_device_handle *device) {
	if ((r = libusb_init(&ctx)) < 0)
    	    return r;
	
    	device = libusb_open_device_with_vid_pid(ctx, 0x1043, 0x3100);
    	if (!device) {
    	    printf("[libusb] Error: Failed to open the device (is your ScreenDUO connected?)\n");
    	    return -1;
    	} //libusb was unable to find ScreenDuo connected to your host system, returning -1 and exiting

    	if (!_lusb::hw_init(device)) {
    	    libusb_close(device);
    	    libusb_exit(ctx);
    	    return -1;
	}
}*/