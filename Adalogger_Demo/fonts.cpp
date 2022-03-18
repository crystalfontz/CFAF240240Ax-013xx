#include "fonts.h"
#include "st7789h2.h"
#include "atsamd21g18.h"

font_t font; // = &f_08x08;
  
/*******************************************************************************
 * \brief Writes a character to the display
 * 
 * Takes an 'x' and a 'y' value to specify the starting point
 * 'c' is the character to write
 ******************************************************************************/
void LCD_Character(uint8_t x0, uint8_t y0, uint8_t c)
{
  //singly, "register" saves 32
  register uint8_t x;
  //singly, "register" saves 32
  register uint8_t y;
  //singly, "register" saves 16
  register const uint8_t *bitmap_data;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t mask;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t line_data;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t top_rows_blank;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t cols_wide;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t left_cols_blank;

  //Make sure the character is in range of this font.
  //(hopefully the font has a '?' in it ;)
  if (
    (c < font.font_set->start_char) || 
    ((font.font_set->start_char + font.font_set->number_of_chars) < c)
  )
  {
    c = '?';
  }
  //Change the ASCII input to an index into the font
  c -= font.font_set->start_char;

  //Initialize pointer to this character's bitmap data
  bitmap_data = &font.font_set->fd[font.font_set->fc[c].data_index];

  //Bring in the deeply indexed pointers to simple ints
  top_rows_blank = font.font_set->fc[c].top_rows_blank;
  cols_wide = font.font_set->fc[c].cols_wide;
  //font.font_set->c[c].left_cols_blank is used to center the
  //character in the cell for fixed width strings. If we
  //are proportional, we will act like it is 0.
  left_cols_blank =
    font.proportional ? 0 : font.font_set->fc[c].left_cols_blank;

  //Screen is upside down compared to the font
  y0 += font.font_set->cell_max_height;
  CLR_CS; // Select the display controller

  if (font.transparent)
  {
    //Transparent background, do not draw background pixels
    for (y = 0; y < font.font_set->cell_max_height; y++)
    {
      //Skip blank lines
      /*lint --e(737)*/
      if ((top_rows_blank <= y)
              && (y < (top_rows_blank + font.font_set->fc[c].rows_high)))
      {
        //Now get the bitmap data and shove it out.
        line_data = *bitmap_data++;
        mask = 0x80;
        //If it is a two-byte character, get the second byte
        //of bitmap data.
        if (8 < cols_wide)
        {
          line_data <<= 8;
          line_data |= *bitmap_data++;
          mask = 0x8000;
        }
        for (x = 0; x < cols_wide; x++, mask >>= 1)
          if (line_data & mask)
            Put_Pixel(x0 + x + left_cols_blank, y0 - y,
                    font.foreground);
      }
    }
  }
  else
  {
    //Not transparent, so draw background pixels. Since we draw
    //every pixel, we can set the address once per line and
    //then just push the new data to the LCD controller, rather
    //than setting the address for every pixel.
    for (y = 0; y < font.font_set->cell_max_height; y++)
    {
      //Point the LCD to the start of this line
      //Set_LCD_for_write_at_X_Y(x0, y0 - y);
      //set_raw_XY(x0, y0 - y);

      //Check for blank lines
      /*lint --e(737)*/
      if ((y < top_rows_blank)
              || ((top_rows_blank + font.font_set->fc[c].rows_high) <= y))
      {
        //This is an entirely blank Line
        // This covers every blank line 
        for (x = 0; x < font.font_set->cell_max_width; x++)
        {
          //Background
          //Write the single pixel's worth of data
          Put_Pixel(x0 + x, y0 - y, font.background);
        }
      }
      else
      {
        //Not a blank line. Set or clear based on the bit data.
        //Push out the leading  blank columns
        for (x = 0; x < left_cols_blank; x++)
        {
          //Background
          //Write the single pixel's worth of data
          Put_Pixel(x0 + x, y0 - y, font.background);
        }
        //Now get the bitmap data and shove it out.
        line_data = *bitmap_data++;
        mask = 0x80;
        //If it is a two-byte character, get the second byte
        //of bitmap data.
        if (8 < cols_wide)
        {
          line_data <<= 8;
          line_data |= *bitmap_data++;
          mask = 0x8000;
        }
        //Conditionally set or clear the pixels based on the bitmap
        for (x = left_cols_blank; x < (left_cols_blank + cols_wide); x++, mask >>= 1)
        {
          if (line_data & mask)
          {
            //Foreground
            //Write the single pixel's worth of data
            Put_Pixel(x0 + x, y0 - y, font.foreground);
          }
          else
          {
            //Background
            //Write the single pixel's worth of data
            Put_Pixel(x0 + x, y0 - y, font.background);
          }
        }
        //Push out the trailing blank columns
        for (x = (left_cols_blank + cols_wide); x < font.font_set->cell_max_width;
                x++)
        {
          //Background
          //Write the single pixel's worth of data
          Put_Pixel(x0 + x, y0 - y, font.background);
        }

      }
    //SET_CS; // Deselect the display controller
    }
  }
}
/*******************************************************************************
 * \brief Writes a string to the display
 * 
 * Takes an 'x' and a 'y' value to specify the starting point
 * 's' is the string to write
 ******************************************************************************/
void LCD_String(uint8_t x, uint8_t y, const char *s)
{
  uint8_t c;
  while (0 != (c = (uint8_t) * s))
  {
    LCD_Character(x, y, c);
    //Move over, rover
    if (font.proportional)
      /*lint --e(737)*/
      x += font.font_set->fc[c - font.font_set->start_char].cols_wide + font.font_set->prop_space;
    else
      x += font.font_set->cell_max_width;
    //Move to the next character
    s++;
  }
}
/*******************************************************************************
 * \brief converts an integer less than 100 to a character array
 ******************************************************************************/
//#pragma GCC optimize ("-Os")
void dec_2_to_string(uint8_t input, char *output)
{
  char digit;
  if (99 < input)
    input = 99;
  digit = (char) (input / 10);
  output[0] = digit ? (digit + '0') : ' ';
  output[1] = input % 10 + '0';
  output[2] = 0;
}
#if 0
/*******************************************************************************
 * \brief prints a short to the display
 * 
 * \\TODO Not ported yet
 ******************************************************************************/
void LCD_put_dec_2_1(unsigned short input)
{
  unsigned char digit;
  digit = input / 100;
  if(digit)
    Write_LCD_Data(digit+'0');
  else
    Write_LCD_Data(' ');
  input %= 100;
  digit = input/10;
  Write_LCD_Data(digit + '0');
  Write_LCD_Data('.');
  Write_LCD_Data(input%10+'0');
}
#endif
/*******************************************************************************
 * \brief converts an integer less than 1000 to a character array
 ******************************************************************************/
//#pragma GCC optimize ("-Os")
void dec_3_to_string(uint16_t input, char *output)
{
  char digit;

  if (999 < input)
    input = 999;

  uint8_t no_blank;
  digit = (char) (input / 100);
  input %= 100;
  if (digit)
  {
    output[0] = digit + '0';
    no_blank = 1;
  }
  else
  {
    output[0] = ' ';
    no_blank = 0;
  }
  digit = (char) (input / 10);
  if (digit || no_blank)
    output[1] = digit + '0';
  else
    output[1] = ' ';
  output[2] = input % 10 + '0';
  output[3] = 0;
}
#if 0
/*******************************************************************************
 * \brief Print an integer less than 10000 to the display
 * 
 * \\TODO Not ported yet
 ******************************************************************************/
void LCD_put_dec_4(unsigned short input)
{
  unsigned char
  digit;
  unsigned char
  no_blank;
  if(9999<input)
  input=9999;
  no_blank=0;
  digit=input/1000;
  if(digit|no_blank)
  {
    Write_LCD_Data(digit+'0');
    no_blank=1;
  }
  else
  Write_LCD_Data(' ');
  input%=1000;
  digit=input/100;
  if(digit|no_blank)
  {
    Write_LCD_Data(digit+'0');
    no_blank=1;
  }
  else
  Write_LCD_Data(' ');
  input%=100;
  digit=input/10;
  if(digit|no_blank)
  Write_LCD_Data(digit+'0');
  else
  Write_LCD_Data(' ');
  Write_LCD_Data(input%10+'0');
}
#endif
#if 0
/*******************************************************************************
 * \brief Print an short less than 10000 to the display
 * 
 * \\TODO Not ported yet
 ******************************************************************************/
void LCD_put_dec_3_1(unsigned short input)
{
  unsigned char
  digit;
  unsigned char
  no_blank;
  if(9999<input)
  input=9999;
  no_blank=0;
  digit=input/1000;
  if(digit|no_blank)
  {
    Write_LCD_Data(digit+'0');
    no_blank=1;
  }
  else
  Write_LCD_Data(' ');
  input%=1000;
  digit=input/100;
  if(digit|no_blank)
  {
    Write_LCD_Data(digit+'0');
    no_blank=1;
  }
  else
  Write_LCD_Data(' ');
  input%=100;
  digit=input/10;
  Write_LCD_Data(digit+'0');
  Write_LCD_Data('.');
  Write_LCD_Data(input%10+'0');
}
#endif
/*******************************************************************************
 * \brief Converts a uint32 less than 100000 to a char array
 * 
 * \\TODO Not ported yet
 ******************************************************************************/
#if 0
#pragma GCC optimize ("-Os")
void dec_5_to_string(uint32 input, char *output)
{
  char
  digit;
  bool
  no_blank;
  if(99999<input)
  input=99999;
  no_blank=false;
  digit=(char)(input/10000);
  if(digit)
  {
    output[0]=digit+'0';
    no_blank=true;
  }
  else
  output[0]=' ';
  input%=10000;
  digit=(char)(input/1000);
  if(digit||no_blank)
  {
    output[1]=digit+'0';
    no_blank=true;
  }
  else
  output[1]=' ';
  input%=1000;
  digit=(char)(input/100);
  if(digit||no_blank)
  {
    output[2]=digit+'0';
    no_blank=true;
  }
  else
  output[2]=' ';
  input%=100;
  digit=(char)(input/10);
  if(digit||no_blank)
  output[3]=digit+'0';
  else
  output[3]=' ';
  output[4]=input%10+'0';
}
#endif
/*******************************************************************************
 * \brief Prints an unsigned char to the display
 * 
 * \\TODO Not ported yet
 ******************************************************************************/
#if 0
void LCD_put_hex(unsigned char input)
{
  unsigned char
  digit;
  digit=input>>4;
  if(9<digit)
  //Gets Error
  //Write_LCD_Data(digit-9 + 'A');
  //Write_LCD_Data(digit-9 + 65);
  Write_LCD_Data(digit+55);
  else
  Write_LCD_Data(digit + '0');
  digit=input&0x0F;
  if(9<digit)
  //Gets Error
  //Write_LCD_Data(digit-9 + 'A');
  //Write_LCD_Data(digit-9 + 65);
  Write_LCD_Data(digit+55);
  else
  Write_LCD_Data(digit + '0');
}
#endif