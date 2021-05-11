#include "oled.h"

//------------------------------------------------------------------------------
const u8 *m_font;    // Current font.
u8 m_invertMask = 0; // font invert mask
u8 m_magFactor = 1;  // Magnification factor.

u8 m_letterSpacing = 1; // Letter-spacing in pixels.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
u8 m_colOffset = 0; // Column offset RAM to SEG.
u8 m_displayWidth = 128;
#ifdef Adafruit128x32
u8 m_displayHeight = 32;
#endif
#ifdef Adafruit128x64
u8 m_displayHeight = 64;
#endif
u8 m_col; // Cursor column.
u8 m_row; // Cursor RAM row.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef Adafruit128x32
const u8 Adafruit128x32init[] = {
    // Init sequence for Adafruit 128x32 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80, // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 0x1F,       // ratio 32
    SSD1306_SETDISPLAYOFFSET, 0x0,    // no offset
    SSD1306_SETSTARTLINE | 0x0,       // line #0
    SSD1306_CHARGEPUMP, 0x14,         // internal vcc
    SSD1306_MEMORYMODE, 0x02,         // page mode
    SSD1306_SEGREMAP | 0x1,           // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,               // column scan direction reversed
    SSD1306_SETCOMPINS, 0x02,         // sequential COM pins, disable remap
    SSD1306_SETCONTRAST, 0x7F,        // contrast level 127
    SSD1306_SETPRECHARGE, 0xF1,       // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x40,      // vcomh regulator level
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON};
#endif
#ifdef Adafruit128x64
const u8 Adafruit128x64init[] = {
    // Init sequence for Adafruit 128x64 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80, // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 0x3F,       // ratio 64
    SSD1306_SETDISPLAYOFFSET, 0x0,    // no offset
    SSD1306_SETSTARTLINE | 0x0,       // line #0
    SSD1306_CHARGEPUMP, 0x14,         // internal vcc
    SSD1306_MEMORYMODE, 0x02,         // page mode
    SSD1306_SEGREMAP | 0x1,           // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,               // column scan direction reversed
    SSD1306_SETCOMPINS, 0x12,         // alt COM pins, disable remap
    SSD1306_SETCONTRAST, 0x7F,        // contrast level 127
    SSD1306_SETPRECHARGE, 0xF1,       // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x40,      // vcomh regulator level
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON};
#endif
//------------------------------------------------------------------------------

void set1X() { m_magFactor = 1; }
void set2X() { m_magFactor = 2; }
u8 letterSpacing() { return m_magFactor * m_letterSpacing; }
u16 fontSize() { return (readFontByte(m_font) << 8) | readFontByte(m_font + 1); }
void setFont(const u8 *font) { m_font = font; }
u8 oledFontRows() { return m_font ? m_magFactor * ((readFontByte(m_font + FONT_HEIGHT) + 7) / 8) : 0; }
void ssd1306WriteCmd(u8 c) { writeDisplay(c, SSD1306_MODE_CMD); }
u8 displayRows() { return m_displayHeight / 8; }
u8 displayWidth() { return m_displayWidth; }

void oledFill()
{
  for (u8 row = 0; row <= displayRows(); row++)
  {
    setRow(row);
    for (u8 col = 0; col < 128; col++)
    {
      setCol(col);
      ssd1306WriteRamBuf(0xFF);
    }
  }
}

#define TestBit(A, k) (A[((k) / 8)] & (1 << ((k) % 8)))
#define SetBit(A, k) (A[((k) / 8)] |= (1 << ((k) % 8)))
#define ClearBit(A, k) (A[((k) / 8)] &= ~(1 << ((k) % 8)))

void oledDraw(u8 x, u8 y, u8 icon[])
{
  u8 ff[] = {0x00};
  setCursor(x, y);
  for (u8 col = 0; col < 8; col++)
  {
    for (u8 row = 0; row < 8; row++)
    {
      if (TestBit(icon, row * 8 + col))
      {
        SetBit(ff, row);
      }
      else
      {
        ClearBit(ff, row);
      }
    }
    setCol(8 - col + x);
    ssd1306WriteRamBuf(ff[0]);
  }
}

void ssd1306WriteRamBuf(u8 c)
{
  if (m_col < m_displayWidth)
  {
    writeDisplay(c ^ m_invertMask, SSD1306_MODE_RAM_BUF);
    m_col++;
  }
}

void writeDisplay(u8 b, u8 mode)
{
  s32 ret;
  mode = (mode == SSD1306_MODE_CMD ? 0X00 : 0X40);
  u8 AB[2] = {mode, b};

  ret = Ql_IIC_Write(0, I2C_ADDRESS, AB, 2);
}

void oledClear()
{
  for (u8 r = 0; r <= displayRows() - 1; r++)
  {
    setCursor(0, r);
    for (u8 c = 0; c <= displayWidth() - 1; c++)
    {
      // Insure clear() writes zero. result is (m_invertMask^m_invertMask).
      ssd1306WriteRamBuf(m_invertMask);
    }
  }
  setCursor(0, 0);
}

void setCol(u8 col)
{
  if (col < m_displayWidth)
  {
    m_col = col;
    col += m_colOffset;
    ssd1306WriteCmd(SSD1306_SETLOWCOLUMN | (col & 0XF));
    ssd1306WriteCmd(SSD1306_SETHIGHCOLUMN | (col >> 4));
  }
}

void setRow(u8 row)
{
  if (row < displayRows())
  {
    m_row = row;
    ssd1306WriteCmd(SSD1306_SETSTARTPAGE | m_row);
  }
}

void setCursor(u8 col, u8 row)
{
  setCol(col);
  setRow(row);
}

void oledPrint(char str[])
{
  for (u8 i = 0; i < strlen(str); i++)
  {
    oledWrite(str[i]);
  }
}
void oledInit()
{
#ifdef Adafruit128x32
  const u8 *table = Adafruit128x32init;
  for (u8 i = 0; i < sizeof(Adafruit128x32init); i++)
  {
#endif
#ifdef Adafruit128x64
    const u8 *table = Adafruit128x64init;
    for (u8 i = 0; i < sizeof(Adafruit128x64init); i++)
    {
#endif
      ssd1306WriteCmd(readFontByte(table + i));
    }
  }

  u8 oledWrite(u8 ch)
  {
    if (!m_font)
    {
      return 0;
    }
    u8 w = readFontByte(m_font + FONT_WIDTH);
    u8 h = readFontByte(m_font + FONT_HEIGHT);
    u8 nr = (h + 7) / 8;
    u8 first = readFontByte(m_font + FONT_FIRST_CHAR);
    u8 count = readFontByte(m_font + FONT_CHAR_COUNT);
    const u8 *base = m_font + FONT_WIDTH_TABLE;

    if (ch < first || ch >= (first + count))
    {
      if (ch == '\r')
      {
        setCol(0);
        return 1;
      }
      if (ch == '\n')
      {
        setCol(0);
        u8 fr = m_magFactor * nr;

        setRow(m_row + fr);

        return 1;
      }
      return 0;
    }
    ch -= first;
    u8 s = letterSpacing();
    u8 thieleShift = 0;
    if (fontSize() < 2)
    {
      // Fixed width font.
      base += nr * w * ch;
    }
    else
    {
      if (h & 7)
      {
        thieleShift = 8 - (h & 7);
      }
      u16 index = 0;
      for (u8 i = 0; i < ch; i++)
      {
        index += readFontByte(base + i);
      }
      w = readFontByte(base + ch);
      base += nr * index + count;
    }
    u8 scol = m_col;
    u8 srow = m_row;

    for (u8 r = 0; r < nr; r++)
    {
      for (u8 m = 0; m < m_magFactor; m++)
      {
        if (r || m)
        {
          setCursor(scol, m_row + 1);
        }
        for (u8 c = 0; c < w; c++)
        {
          u8 b = readFontByte(base + c + r * w);
          if (thieleShift && (r + 1) == nr)
          {
            b >>= thieleShift;
          }
          if (m_magFactor == 2)
          {
            b = m ? b >> 4 : b & 0XF;
            b = readFontByte(scaledNibble + b);
            ssd1306WriteRamBuf(b);
          }
          ssd1306WriteRamBuf(b);
        }
        for (u8 i = 0; i < s; i++)
        {
          ssd1306WriteRamBuf(0);
        }
      }
    }
    setRow(srow);
    return 1;
  }
