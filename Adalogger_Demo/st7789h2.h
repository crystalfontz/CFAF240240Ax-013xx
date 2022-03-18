#ifndef __ST7789H2_H__
#define __ST7789H2_H__
/*
 * ST7789H2.h
 *
 * Created: 2015-08-18 9:16:09 AM
 *  Author: max
 */ 

#include "atsamd21g18.h"

//==============================================================================
//Select your desired rotation here
//==============================================================================
#define ROTATE 0
//==============================================================================
//These are for comparison against the desired rotation
#define ROTATE_0 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3
//==============================================================================
//Based on the selection above set some values
//==============================================================================
#if (ROTATE == ROTATE_0)
  #define X_START (0)
  #define X_END (0)
  #define Y_START (80)
  #define Y_END (319)
  #define MY_ROTATED 1
  #define MX_ROTATED 0
  #define MV_ROTATED 0
#elif (ROTATE == ROTATE_90)
  #define X_START (0)
  #define X_END (0)
  #define Y_START (0)
  #define Y_END (239)
  #define MY_ROTATED 0
  #define MX_ROTATED 0
  #define MV_ROTATED 1
#elif (ROTATE == ROTATE_180)
  #define X_START (0)
  #define X_END (0)
  #define Y_START (0)
  #define Y_END (239)
  #define MY_ROTATED 0
  #define MX_ROTATED 1
  #define MV_ROTATED 0
#elif (ROTATE == ROTATE_270)
  #define X_START (80)
  #define X_END (319)
  #define Y_START (00)
  #define Y_END (239)
  #define MY_ROTATED 1
  #define MX_ROTATED 1
  #define MV_ROTATED 1
#endif
//==============================================================================
#define XS15_08 (X_START >> 8)
#define XS07_00 (X_START & 0x00FF)
#define XE15_08 (X_END >> 8)
#define XE07_00 (X_END & 0x00FF)
#define YS15_08 (Y_START >> 8)
#define YS07_00 (Y_START & 0x00FF)
#define YE15_08 (Y_END >> 8)
#define YE07_00 (Y_END & 0x00FF)

// color definitions
const color_t BLACK   = {0x00, 0x00, 0x00}; //#define	BLACK   0x000000
const color_t BLUE    = {0x00, 0x00, 0xFF}; //#define	BLUE    0x0000FF
const color_t GREEN   = {0x00, 0xFF, 0x00}; //#define	GREEN   0x00FF00
const color_t CYAN    = {0x00, 0xFF, 0xFF}; //#define CYAN    0x00FFFF
const color_t RED     = {0xFF, 0x00, 0x00}; //#define	RED     0xFF0000
const color_t MAGENTA = {0xFF, 0x00, 0xFF}; //#define MAGENTA 0xFF00FF
const color_t YELLOW  = {0xFF, 0xFF, 0x00}; //#define YELLOW  0xFFFF00
const color_t WHITE   = {0xFF, 0xFF, 0xFF}; //#define WHITE   0xFFFFFF

// ********************************************************
// Defines for the ST7789 registers.
// ref: https://www.crystalfontz.com/products/document/3277/ST7735_V2.1_20100505.pdf
// ********************************************************
#define ST7789_SLPIN      (0x10)  //Sleep in
#define ST7789_SLPOUT     (0x11)  //Sleep Out
#define ST7789_INVOFF     (0x20)  //Display Inversion Off
#define ST7789_INVON      (0x21)  //Display Inversion On
#define ST7789_DISPOFF    (0x28)  //Display Off
#define ST7789_DISPON     (0x29)  //Display On
#define ST7789_CASET      (0x2A)  //Column Address Set
#define ST7789_RASET      (0x2B)  //Row Address Set
#define ST7789_RAMWR      (0x2C)  //Memory Write
#define ST7789_TEOFF      (0x34)  //Tearing Effect Line OFF
#define ST7789_TEON       (0x35)  //Tearing Effect Line On
#define ST7789_MADCTL     (0x36)  //Memory Data Access Control
#define ST7789_COLMOD     (0x3A)  //Interface Pixel Format
#define ST7789_STE        (0x44)  //Set Tear Scanline
#define ST7789_WRDISBV    (0x51)  //Write Display Brightness
#define ST7789_WRCTRLD    (0x53)  //Write CTRL Display
#define ST7789_WRCACE     (0x55)  //Write Content Adaptive Brightness Control and Color Enhancement
#define ST7789_WRCABCMB   (0x5E)  //Write CABC minimum brightness
#define ST7789_PORCTRL    (0xB2)  //Porch control
#define ST7789_GCTRL      (0xB7)  //Gate Control
#define ST7789_VCOMS      (0xBB)  //Gate on timing adjustment
#define ST7789_LCMCTRL    (0xC0)  //LCM Control
#define ST7789_VDVVRHEN   (0xC2)  //VDV and VRH Command Enable
#define ST7789_VRHS       (0xC3)  //VRH Set
#define ST7789_VDVSET     (0xC4)  //VDV Set
#define ST7789_FRCTR2     (0xC6)  //Frame Rate Control in Normal Mode
#define ST7789_CABCCTRL   (0xC7)  //CABC Control
#define ST7789_PWCTRL1    (0xD0)  //Power Control 1
#define ST7789_PVGAMCTRL  (0xE0)  //Positive Voltage Gamma Control
#define ST7789_NVGAMCTRL  (0xE1)  //Negative Voltage Gamma Control
// ********************************************************
void displayInit(void);
void enterSleep (void);
void exitSleep (void);
void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, color_t color);
void Fill_Display_Gamma_Gradient(uint8_t height, uint8_t width);
void fillScreen(color_t color);
void LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color);
void Put_Pixel(uint8_t x, uint8_t y, color_t color);
void setDisplayWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void setInterface(void);
void writeColor(color_t color);
void writeColorBars(uint8_t width, uint8_t height);

//
// Customer code after this line
//

#endif /* __ST7789H2_H__ */
