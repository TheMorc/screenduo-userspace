# ScreenDUO4macOS
Very, very, WIP thing, most likely to be unstable or to not even work at all...

```
ASUS ScreenDUO - screenduo4macOS
Opensource runtime app based driver for ASUS ScreenDUO

Options:
        <anytext>     Text mode
        --ui          UI mode
        --screen      Screen mode, provide screen.bmp file in exec folder
        --emu         SDL emulator/simulator thing used when you don't have a real ScreenDUO
        

Formatting guide for text mode:
\n - Newline
\c<0-E> - Text Color
\a<FFFFFF> - Text Color in HEX format supported in lower and also uppercase (example: FEA4dC)
\p<X>,<Y>, - Puts pixel in current color at X position <0-320> and Y position <0-240> Last comma is required.
\b<X1>,<X2>,<Y1>,<Y2>, - Draws a box from X1, Y1 to X2, Y2 position. Last comma is required.

Examples:
./duo "\affffff\p20,130," - Puts a white pixel at X:20, Y:130
./duo "Fred\n\c3Barney" - Prints Fred in white color and Barney in blue color on new line

```
# Credits:

- **[Dmitry Zaitsev](https://github.com/hhrhhr)** for creating original code
- **[Geoffrey McRae](https://github.com/gnif)** also possibly for creating the original code (not sure)
- **[Andrei Sokolov](https://youtu.be/I2bF9IQN76U)** for adding putpixel, ~~putbigpixel~~ putpixelxl
- **Bob Gill** for adding multiline color text support
- **[mayorbobster](https://github.com/mayorbobster/screenduo4linux)** for improving the original code (originally forked from this)
- **[Ash Hughes](https://github.com/ashh87)** for making the relatively good and working button code that I modified and added
