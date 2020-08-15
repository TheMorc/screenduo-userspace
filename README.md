# screenduo-userspace
This is a userspace driver based on libusb that is used as a replacement for the original Windows XP/Vista ASUS ScreenDUO driver. It features a text mode, pseudoscreen capture mode, UI mode and a "emulator" to develop stuff for this driver without having access to real ScreenDUO. 

### Usage output
```
ASUS ScreenDUO opensource userspace based driver written in C++ for Windows/macOS/Linux

Options:
        <anytext>     Text mode (no args needed, use quotes)
        --ui          UI mode
        --screen      Screen mode, provide screen.bmp file in exec folder
        --SDL         SDL emulator mode
        --oldSDL      Screen capture test mode using SDL using window buffer (deprecated, will be deleted in future)

Formatting guide for text mode:
\n - Newline
\c<0-E> - Text Color
\a<FFFFFF> - Text Color in HEX format in both lower and uppercase format (ex: FEA4dC)
\p<X>,<Y>, - Puts pixel in current color at X pos <0-320> and Y pos <0-240> Last comma is required.
\b<X1>,<X2>,<Y1>,<Y2>, - Draws a box from X1, Y1 to X2, Y2 pos. Last comma is required.

Examples:
./duo "\affffff\p20,130," - Puts a white pixel at X:20, Y:130
./duo "Fred\n\c3Barney" - Prints Fred in white color and Barney in blue color on new line
```
### Dependencies and other stuff
To get this driver to compile you need to install:
- libusb and sdl2


I got this driver to compile using g++ on:
* **macOS 10.16 Beta 1** - with no backwards compatibility whatsoever(for now)
  * using libusb and sdl2 from brew and using the included makefile
* **Mac OS X 10.7** - with compatibility to the newest macOS
  * using libusb and sdl2 from tigerbrew and by modifying include paths for SDL and libusb(due to older libs)
* **Windows 7** - with backwards compatibility up to Windows XP
  * using libusb and sdl2-devel from original sites and by adding flags and modifying paths to get it to compile on TDM GCC(MinGW)
* **Raspbian** - to try it to compile on Linux and ARM 
  * by using libusb-1.0-dev and modifying paths
*(i will add these flags with identifiying to makefile as soon I will need to)*


## Some incomplete credits:

- **[Dmitry Zaitsev](https://github.com/hhrhhr)** for creating original code
- **[Geoffrey McRae](https://github.com/gnif)** also possibly for creating the original code (not sure which one of them)
- **[Andrei Sokolov](https://youtu.be/I2bF9IQN76U)** for adding putpixel, ~~putbigpixel~~ putpixelxl
- **[mayorbobster or Bob Gill](https://github.com/mayorbobster/screenduo4linux)** for improving the original code by adding multiline color text support(originally forked from this)
- **[Ash Hughes](https://github.com/ashh87)** for making the relatively good and working button code that I modified and added
