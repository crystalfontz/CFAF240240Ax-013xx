#ifndef __ST7789H2_H__
#define __ST7789H2_H__
/*
 * ST7789H2.h
 *
 * Created: 2015-08-18 9:16:09 AM
 *  Author: max
 */ 

#include <Arduino.h>
#include <SPI.h>
#include "atmega328.h"

// color definitions
#define	BLACK   color_t {0x00,0x00,0x00}
#define	BLUE    color_t {0x00,0x00,0xFF}
#define	GREEN   color_t {0x00,0xFF,0x00}
#define CYAN    color_t {0x00,0xFF,0xFF}
#define	RED     color_t {0xFF,0x00,0x00}
#define MAGENTA color_t {0xFF,0x00,0xFF}
#define YELLOW  color_t {0xFF,0xFF,0x00}
#define WHITE   color_t {0xFF,0xFF,0xFF}


class color_t
{
public:
  uint8_t r, g, b;
};


void clearScreen(void);
void displayHome(void);
void displayInit(void);
void enterSleep (void);
void exitSleep (void);
void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, uint8_t R, uint8_t G, uint8_t B);
void Fill_OLED_Gamma_Gradient(uint8_t height, uint8_t width);
void Fill_LCD(uint8_t R, uint8_t G, uint8_t B);
void fillScreen(uint32_t color);
void fillScreen(color_t color);
void LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t R, uint8_t G, uint8_t B);
void OLED_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t r, uint8_t g, uint8_t b);
void Put_Pixel(uint8_t x, uint8_t y, uint8_t R, uint8_t G, uint8_t B);
//void Set_LCD_for_write_at_X_Y(uint8_t x, uint8_t y);
void setDisplayWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void setInterface(void);
void writeColor(uint32_t color);
void writeColor(color_t color);
void writeColorBars(uint8_t height, uint8_t width);
void SPI_sendCommand(uint8_t command);
void SPI_sendData(uint8_t data);


#endif /* __ST7789H2_H__ */
