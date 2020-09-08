--ASUS ScreenDUO - screenduo-userspace lua based settings--

--Use SDL emulated mode? Disable for libusb mode for use with a real ScreenDUO--
SDL = true

--Text mode SDL delay to not quit the window right away as written pixel data stays on duo infinitely until rewrite(default: 5000)--
SDLTextDelay = 5000

--Use both libusb and SDL? To try both at once to see if everything behaves as it should--
SDLandlibusb = false

--Show log info? Errors will be shown regardless of the setting--
logINFO = true

--Show info text? Best for text mode and not applicable for usage output or errors--
showInfo = true
