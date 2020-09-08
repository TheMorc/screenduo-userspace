//this is just a complementary file for libusb.cpp
//this includes main libusb features to shrink the main code

#ifndef _lusb
#define _lusb
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "libusb.h"


extern bool logLUSB;

int hw_init(libusb_device_handle *device);
void hw_write(struct libusb_transfer *xfer);
int dev_read(libusb_device_handle *device, uint8_t *data, unsigned int length, bool button);
//int btn_read(libusb_device_handle *device, uint8_t *data, unsigned int length);
int get_buttons(libusb_device_handle *device);
int dev_write(libusb_device_handle *device, uint8_t *data, unsigned int length);
//int hw_open(int r,libusb_context *ctx, libusb_device_handle *device);

#endif