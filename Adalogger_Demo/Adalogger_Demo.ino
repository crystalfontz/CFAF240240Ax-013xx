//===========================================================================
//
//  Code written for Seeeduino set to 3.3v (important!)
//
//  CRYSTALFONTZ 240X240 SPI COLOR 1.3" TFT Family
//  ref: https://www.crystalfontz.com/product/cfaf240240a4013tn
//
//  This code uses 4-wire SPI mode.
//
// The controller is a Sitronix ST7789H2
//   http://www.crystalfontz.com/controllers/Sitronix/ST7789H2
//
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//
//============================================================================
#include <SPI.h>
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.cpp
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.h

#include <SD.h>
//#include <avr/io.h>
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.h

#include "atsamd21g18.h"
#include "st7789h2.h"
#include "fonts.h"
#include "font_08x08.h"
#include "font_12x16.h"

//============================================================================
void circleDemo(uint8_t width, uint8_t height)
{
  uint8_t w_section = width / 6;
  uint8_t h_section = height / 6;

  //Draw a cyan circle
  LCD_Circle(w_section * 3, h_section * 3, (w_section * 3) - 1, CYAN);
  //Draw a green circle
  LCD_Circle(w_section * 3, h_section * 3, w_section - 1, GREEN);
  //Draw a white circle
  LCD_Circle(w_section * 1, h_section * 3, w_section - 1, WHITE);
  //Draw a red circle
  LCD_Circle(w_section * 5, h_section * 3, w_section - 1, RED);
  //Draw a purple circle
  LCD_Circle(w_section * 3, h_section * 5, w_section - 1, MAGENTA);
  //Draw a orange circle
  LCD_Circle(w_section * 3, h_section * 1, w_section - 1, color_t {0xFF, 0xA5, 0x00});
  delay(1000);

  Serial.println("draw some circles");
  for(uint8_t i = 2; i < (w_section * 3); i += 2)
  {
    LCD_Circle(i + 2, (h_section * 3), i, color_t {i<<2, 0xFF-(i<<2),0x00});
  }
}
//============================================================================
void setup()
{
  //debug console
  Serial.begin(9600);
  Serial.println("setup()");

  hostInit();

  // For the Seeduino library I am using, the default speed of SPI_HALF_SPEED
  // set in C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
  // results in a 4MHz clock.
  //
  // If you change this function call in SDClass::begin() of SD.cpp
  // from:
  //
  //  return card.init(SPI_HALF_SPEED, csPin) &&
  //         volume.init(card) &&
  //         root.openRoot(volume);
  //
  // to:
  //
  //  return card.init(SPI_FULL_SPEED, csPin) &&
  //         volume.init(card) &&
  //         root.openRoot(volume);
  //
  // That appears to make the SD library talk at 8MHz.
  //

  //if (!SD.begin(uSD_CS))
  if (!SD.begin(12000000, 4))
  {
    Serial.println("Card failed to initialize, or not present");
    //Reset the SPI clock to fast. SD card library does not clean up well.
    //Bump the clock to 8MHz. Appears to be the maximum.
  }
  else
  {
    Serial.println("Card initialized.");
  }

  //Bump the clock to 8MHz. Appears to be the maximum at 12MHz
  //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));  //WORKS
  //SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));  //WORKS
  SPI.beginTransaction(SPISettings(32000000, MSBFIRST, SPI_MODE0));  //WORKS

  //Initialize the LCD controller
  displayInit();

  Serial.println("Writing a white screen");
  fillScreen(WHITE);
  delay(500);

  Serial.println("Writing a red screen");
  fillScreen(RED);
  delay(500);

  Serial.println("Writing a green screen");
  fillScreen(GREEN);
  delay(500);

  Serial.println("Writing a blue screen");
  fillScreen(BLUE);
  delay(500);
}
//============================================================================
void loop()
{
  Serial.println("--> Top of the Loop <--");

#if(1)
  writeColorBars(240, 240);
  delay(1000);

  Fill_Display_Gamma_Gradient(240, 240);
  delay(1000);
#endif
#if(1)  // Enable to demonstrate basic tests

  Serial.println("Writing a black screen");
  fillScreen(BLACK);
  delay(500);

  Serial.println("Circle tests");
  circleDemo(240, 240);
  delay(1000);
#endif
#if(1)  // Enable to demonstrate writing fonts to the screen
  font.reset();
  font.font_set = &f_12x16;
  font.background = BLACK;
  Serial.println("Writing 12x16");
  LCD_Character(12+(12*0),223,'B');
  LCD_Character(12+(12*1),223,'i');
  LCD_Character(12+(12*2),223,'g');

  font.transparent = true;
  LCD_String(12, 206, "Big Font");

  Serial.println("Writing 08x08");
  font.reset();
  font.font_set = &f_08x08;
  font.background = GREEN;
  LCD_String(152, 1, "Little Font");
  // LCD_Character(120,160,'A');
  // LCD_Character(120+8,160,'!');
  // LCD_Character(120+8+8,160,'C');

  // LCD_String(40, 40, "Test String Little");

  font.reset();
  font.proportional = true;
  font.foreground = RED;
  LCD_String(150, 50, "proportional");

  font.reset();
  font.font_set = &f_12x16;
  for(uint8_t y=0;y<=75;y++)
  {
    #if(0)
    LCD_Character(63+(12*0), y+(16*2), '2');
    LCD_Character(63+(12*1), y+(16*2), '1');
    LCD_Character(63+(12*2), y+(16*2), '0');
    LCD_Character(63+(12*3), y+(16*2), '0');
    LCD_Character(63+(12*4), y+(16*2), '0');

    LCD_Character(63+(12*0), y+(16*1), '2');
    LCD_Character(63+(12*1), y+(16*1), '2');
    LCD_Character(63+(12*2), y+(16*1), '0');
    LCD_Character(63+(12*3), y+(16*1), '0');
    LCD_Character(63+(12*4), y+(16*1), '0');

    LCD_Character(63+(12*0), y+(16*0), '2');
    LCD_Character(63+(12*1), y+(16*0), '3');
    LCD_Character(63+(12*2), y+(16*0), '0');
    LCD_Character(63+(12*3), y+(16*0), '0');
    LCD_Character(63+(12*4), y+(16*0), '0');
    #else
    LCD_String(63+(12*0), y+(16*2), "21000");
    LCD_String(63+(12*0), y+(16*1), "22000");
    LCD_String(63+(12*0), y+(16*0), "23000");
    #endif
  }

#endif

#if(1)
  show_BMPs_in_root();
#endif
  delay(1000);
} // void loop()
//============================================================================
