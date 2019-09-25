# ScreenDUO4macOS (fork of mayorbobster's screenduo4linux driver)
WIP thing, most likely to be unstable

```
ASUS ScreenDUO
FOSS Application based driver for ASUS ScreenDUO on macOS

Usage: ./duo <anytext>

Formatting characters:
\n - Newline
\c<0-E> - Text Color
\a<FFFFFF> - Text Color in HEX format supported in lower and also uppercase (example: \aFEA4dC)
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
- **[Ash Hughes](https://github.com/ashh87)** for making (not used now) button code
