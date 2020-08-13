# ScreenDUO4macOS (fork of mayorbobster's screenduo4linux driver)
Very, very, WIP thing, most likely to be unstable or to not even work at all...

```
ASUS ScreenDUO - screenduo4macOS
Opensource runtime app based driver for ASUS ScreenDUO

Options:
        duo <anytext>     Text mode
        duo --ui          UI mode
        duo --screen      Screen mode, provide screen.bmp file in exec folder

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
- **[Geoffrey McRae](https://github.com/gnif)** for creating original code
- **[Andrei Sokolov](https://youtu.be/I2bF9IQN76U)** for adding putpixel, ~~putbigpixel~~ putpixelxl
- **Bob Gill** for adding multiline color text support
- **[Ash Hughes](https://github.com/ashh87)** for making the relatively good and working button code
