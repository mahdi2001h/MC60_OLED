#ifndef oled_h
#define oled_h

#include "ql_type.h"
#include "ql_stdlib.h"
#include "ql_trace.h"
#include "ql_gpio.h"
#include "ql_iic.h"
#include "Adafruit5x7.h"

GLCDFONTDECL(scaledNibble) = {
    0X00, 0X03, 0X0C, 0X0F,
    0X30, 0X33, 0X3C, 0X3F,
    0XC0, 0XC3, 0XCC, 0XCF,
    0XF0, 0XF3, 0XFC, 0XFF};
    
//#define Adafruit128x64 
#define Adafruit128x32

//------------------------------------------------------------------------------
/** Fake read from flash. */
#define readFontByte(addr) (*(const unsigned char *)(addr))

#define FONT_LENGTH 0
/** Maximum character width. */
#define FONT_WIDTH 2
/** Font hight in pixels */
#define FONT_HEIGHT 3
/** Ascii value of first character */
#define FONT_FIRST_CHAR 4
/** count of characters in font. */
#define FONT_CHAR_COUNT 5
/** Offset to width table. */
#define FONT_WIDTH_TABLE 6
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/** Set Lower Column Start Address for Page Addressing Mode. */
#define SSD1306_SETLOWCOLUMN 0x00
/** Set Higher Column Start Address for Page Addressing Mode. */
#define SSD1306_SETHIGHCOLUMN 0x10
/** Set Memory Addressing Mode. */
#define SSD1306_MEMORYMODE 0x20
/** Set display RAM display start line register from 0 - 63. */
#define SSD1306_SETSTARTLINE 0x40
/** Set Display Contrast to one of 256 steps. */
#define SSD1306_SETCONTRAST 0x81
/** Enable or disable charge pump.  Follow with 0X14 enable, 0X10 disable. */
#define SSD1306_CHARGEPUMP 0x8D
/** Set Segment Re-map between data column and the segment driver. */
#define SSD1306_SEGREMAP 0xA0
/** Resume display from GRAM content. */
#define SSD1306_DISPLAYALLON_RESUME 0xA4
/** Force display on regardless of GRAM content. */
#define SSD1306_DISPLAYALLON 0xA5
/** Set Normal Display. */
#define SSD1306_NORMALDISPLAY 0xA6
/** Set Inverse Display. */
#define SSD1306_INVERTDISPLAY 0xA7
/** Set Multiplex Ratio from 16 to 63. */
#define SSD1306_SETMULTIPLEX 0xA8
/** Set Display off. */
#define SSD1306_DISPLAYOFF 0xAE
/** Set Display on. */
#define SSD1306_DISPLAYON 0xAF
/**Set GDDRAM Page Start Address. */
#define SSD1306_SETSTARTPAGE 0XB0
/** Set COM output scan direction normal. */
#define SSD1306_COMSCANINC 0xC0
/** Set COM output scan direction reversed. */
#define SSD1306_COMSCANDEC 0xC8
/** Set Display Offset. */
#define SSD1306_SETDISPLAYOFFSET 0xD3
/** Sets COM signals pin configuration to match the OLED panel layout. */
#define SSD1306_SETCOMPINS 0xDA
/** This command adjusts the VCOMH regulator output. */
#define SSD1306_SETVCOMDETECT 0xDB
/** Set Display Clock Divide Ratio/ Oscillator Frequency. */
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
/** Set Pre-charge Period */
#define SSD1306_SETPRECHARGE 0xD9
/** Deactivate scroll */
#define SSD1306_DEACTIVATE_SCROLL 0x2E
/** No Operation Command. */
#define SSD1306_NOP 0XE3
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/** Write to Command register. */
#define SSD1306_MODE_CMD 0
/** Write one byte to display RAM. */
#define SSD1306_MODE_RAM 1
/** Write to display RAM with possible buffering. */
#define SSD1306_MODE_RAM_BUF 2
//------------------------------------------------------------------------------


 
#define I2C_ADDRESS 0x78

void setFont(const u8 *font);
void oledFill();
void oledClear();
void ssd1306WriteRamBuf(u8 c);
void writeDisplay(u8 b, u8 mode);
void setRow(u8 row);
void setCol(u8 col);
void setCursor(u8 col, u8 row);
void oledPrint(char str[]);
void oledInit();
u8 oledWrite(u8 ch);
void oledDraw(u8 x, u8 y, u8 icon[]);

#endif
