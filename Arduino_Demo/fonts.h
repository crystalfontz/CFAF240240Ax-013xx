#ifndef _FONTS_H_
#define _FONTS_H_

#include <Arduino.h>
#include <SD.h>
#include <avr/io.h>
//#include "atsamd21g18.h"
#include "atmega328.h"
#include "st7789h2.h"

//Header file information for font structure definitions.
#if 1
typedef struct
  {
  uint8_t
    cols_wide;
  uint8_t
    rows_high;
  uint8_t
    left_cols_blank;
  uint8_t
    top_rows_blank;
  uint16_t
	  data_index;
  }__attribute__ ((__packed__)) FONT_CHARACTER;
#else
//Bit fields should save data, but the pgm_read_* will not work
//Maybe could bring it in as a uint32_t then cast into a bitmap.	
typedef struct
  {
  unsigned int
    cols_wide:4;
  unsigned int
    rows_high:5;
  unsigned int
    left_cols_blank:4;
  unsigned int
    top_rows_blank:5;
  unsigned int
	  data_index:14;
  }__attribute__ ((__packed__)) FONT_CHARACTER;
#endif

typedef struct
  {
  uint8_t
    start_char;
  uint8_t
    number_of_chars;
  uint8_t
    cell_max_width;
  uint8_t
    cell_max_height;
  uint8_t
    prop_space;
  const FONT_CHARACTER
    *fc;
  const uint8_t
  	*fd;
  }__attribute__ ((__packed__)) FONT_HEADER;

typedef struct font_t
{
  color_t foreground = { 0xFF, 0xFF, 0xFF };
  color_t background = { 0x00, 0x00, 0x00 };
  bool transparent = false;
  bool proportional = false;

  const FONT_HEADER *font_set;

  void reset()
  {
    foreground = { 0xFF, 0xFF, 0xFF };
    background = { 0x00, 0x00, 0x00 };
    transparent = false;
    proportional = false;
  }
};
//------------
// Font Stuffs
//------------

//Header file declarations for font "08x08"
extern const FONT_HEADER f_08x08;

//Header file declarations for font "12x16"
extern const FONT_HEADER f_12x16;

//Global single font
extern font_t font;

void LCD_Character(uint8_t x0, uint8_t y0, uint8_t c);
void LCD_String(uint8_t x0, uint8_t y0, const char *s);

#endif // _FONTS_H_
