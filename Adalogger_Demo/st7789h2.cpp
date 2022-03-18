#include "st7789h2.h"

/*******************************************************************************
 * \brief Function to configure the display controller for this display.
 ******************************************************************************/
void displayInit(void)
{
  SET_RESET;  //Take the display controller out of reset
  delay(10);  //10µS min
  CLR_RESET;  //Put the display controller into reset
  delay(10);  //10µS min
  SET_RESET;  //Take the display controller out of reset
  delay(130); //130mS max

  //Sleep Out ("Sleep Out" should be called "wake")
  //The DC/DC converter is enabled, Internal display oscillator
  //is started, and panel scanning is started.
  writeCommand(ST7789_SLPOUT);
  delay(120);

  //-----------------------------Display setting--------------------------------
  writeCommand(ST7789_MADCTL); //Page 215
  // Bit D7 - MY - Page Address Order
  // “0” = Top to Bottom (When MADCTL D7=”0”).
  // “1” = Bottom to Top (When MADCTL D7=”1”).
  #define MY MY_ROTATED
  // Bit D6 - MX - Column Address Order
  // “0” = Left to Right (When MADCTL D6=”0”).
  // “1” = Right to Left (When MADCTL D6=”1”).
  #define MX MX_ROTATED
  // Bit D5 - MV - Page/Column Order
  // “0” = Normal Mode (When MADCTL D5=”0”).
  // “1” = Reverse Mode (When MADCTL D5=”1”)
  // Note: Bits D7 to D5, alse refer to section 8.12 Address Control
  #define MV MV_ROTATED
  // Bit D4 - ML - Line Address Order
  // “0” = LCD Refresh Top to Bottom (When MADCTL D4=”0”)
  // “1” = LCD Refresh Bottom to Top (When MADCTL D4=”1”)
  #define ML 0
  // Bit D3 - RGB - RGB/BGR Order
  // “0” = RGB (When MADCTL D3=”0”)
  // “1” = BGR (When MADCTL D3=”1”)
  #define RGB 0
  // Bit D2 - MH - Display Data Latch Data Order
  // “0” = LCD Refresh Left to Right (When MADCTL D2=”0”)
  // “1” = LCD Refresh Right to Left (When MADCTL D2=”1”)  
  #define MH 0
  writeData((MY << 7) | (MX << 6) | (MV << 5) | (ML << 4) | (RGB << 3) | (MH << 2));

  writeCommand(ST7789_COLMOD); //Interface pixel format Pg 224
  writeData(0x06);
  
  //writeCommand(ST7789_INVOFF);
  writeCommand(ST7789_INVON);

  writeCommand(ST7789_CASET);
  writeData(XS15_08);
  writeData(XS07_00);
  writeData(XE15_08);
  writeData(XE07_00);

  writeCommand(ST7789_RASET);
  writeData(YS15_08);
  writeData(YS07_00);
  writeData(YE15_08);
  writeData(YE07_00);

  //------------------------- Frame rate setting-------------------
  writeCommand(ST7789_PORCTRL);	// Porch Control
	writeData(0x0C);
	writeData(0x0C);
	writeData(0x00);
	writeData(0x33);
	writeData(0x33);

	writeCommand(ST7789_GCTRL);	//Gate Control
	writeData(0x35);							//VGH:13.26/VGL:-10.43

  //----------------------- Power setting-------------------
	writeCommand(ST7789_VCOMS);	//VCOM Setting
  writeData(0x1F);     //VCOM = 0.875V

  writeCommand(ST7789_LCMCTRL);  //LCM Control
  writeData(0x2C);     //Power On Sequence default

	writeCommand(ST7789_VDVVRHEN);  //VDV and VRH Command Enable
  writeData(0x01);

	writeCommand(ST7789_VRHS);  //VRH Set
  writeData(0x12);     //4.45+( vcom+vcom offset+0.5vdv)

	writeCommand(ST7789_VDVSET);  //VDV Set
  writeData(0x20);     //VDV = 0V

	writeCommand(ST7789_FRCTR2);  //Frame Rate Control in Normal Mode
  writeData(0x0F);     //60HZ

	writeCommand(ST7789_PWCTRL1);   //Power Control 1
  writeData(0xA4);
  writeData(0xA1);     //VDS=2.3V/AVCL = -4.8V /AVDD=6.8V

  //Set the Gamma Tables
	writeCommand(ST7789_PVGAMCTRL);
  writeData(0xD0);
  writeData(0x08);
  writeData(0x11);
  writeData(0x08);
  writeData(0x0C);
  writeData(0x15);
  writeData(0x39);
  writeData(0x33);
  writeData(0x50);
  writeData(0x36);
  writeData(0x13);
  writeData(0x14);
  writeData(0x29);
  writeData(0x2D);
  
  //--------------------
	writeCommand(ST7789_NVGAMCTRL);
  writeData(0xD0);
  writeData(0x08);
  writeData(0x10);
  writeData(0x08);
  writeData(0x06);
  writeData(0x06);
  writeData(0x39);
  writeData(0x44);
  writeData(0x51);
  writeData(0x0B);
  writeData(0x16);
  writeData(0x14);
  writeData(0x2F);
  writeData(0x31);

  //Disable Tearing
  writeCommand(ST7789_TEOFF);

  //Enable Tearing
	// writeCommand(ST7789_TEON);
  // writeData(0x00); // ON
  // When TEM =’0’: The Tearing Effect output line consists of V-Blanking information only
  // When TEM =’1’: The Tearing Effect output Line consists of both V-Blanking and H-Blanking information

  //CABC Setup
  //Write Display Brightness
  writeCommand(ST7789_WRDISBV); //Defaults to 0000h
  writeData(0xFF); // 8 bit display brightness 0x00 = lowest, 0xFF = highest

  //Write CTRL Display
  writeCommand(ST7789_WRCTRLD);
  // BCTRL: Brightness Control Block On/Off, This bit is always used to switch brightness for display.
  // 0 = Off (Brightness register are 00h, DBV[7:0])
  // 1 = On (Brightness register are active, according to the other parameters.)
  #define BCTRL 1
  // DD: Display Dimming (Only for manual brightness setting)
  // DD = 0: Display Dimming is off.
  // DD = 1: Display Dimming is on.
  #define DD    1
  // BL: Backlight Control On/Off
  // 0 = Off (Completely turn off backlight circuit. Control lines must be low.)
  // 1 = On
  #define BL    1
  writeData((BCTRL << 5) | (DD << 3) | (BL << 2));

  //Content Adaptive Brightness controller
	writeCommand(ST7789_CABCCTRL); //Defaults to 0x00
  // LEDONREV: Reverse the status of LED_ON:
  // “0”: keep the status of LED_ON.
  // “1”: reverse the status of LED_ON.
  #define LEDONREV 0
  // DPOFPWM: initial state control of LEDPWM.
  // “0”: The initial state of LEDPWM is low.
  // “1”: The initial state of LEDPWM is high.
  #define DPOFPWM  0
  // PWMFIX: LEDPWM fix control.
  // “0”: LEDPWM control by CABC.
  // “1”: fix LEDPWM in “ON” status.
  #define PWMFIX   0
  // PWMPOL: LEDPWM polarity control.
  // “0”: polarity high.
  // “1”: polarity low.
  #define PWMPOL   0
  writeData((LEDONREV << 3) | (DPOFPWM << 2) | (PWMFIX << 1) | (PWMPOL << 0));

	writeCommand(ST7789_WRCACE); //Defaults to 0x00
  // CECTRL: Color Enhancement Control Bit:
  // CECTRL=0: Color Enhancement Off.
  // CECTRL=1: Color Enhancement On.
  #define CECTRL 1

  // |CE1 | CE0 | Color enhancement level
  // ------------------------------------
  // | 0  | 0   | Low enhancement
  // | 0  | 1   | Medium enhancement
  // | 1  | 1   | High enhancement
  // ------------------------------------
  #define CE1 1
  #define CE0 1
  
  // | C1 | C0 | Function
  // ---------------------------------
  // | 0  | 0  | Off
  // | 0  | 1  | User Interface Mode
  // | 1  | 0  | Still Picture
  // | 1  | 1  | Moving Image
  // ---------------------------------
  #define C1 1
  #define C0 1
  writeData((CECTRL << 7) | (CE1 << 5) | (CE0 << 4) | (C1 << 1) | (C0 << 0));

  //
  writeCommand(ST7789_WRCABCMB); //Defaults to 0x00
  //  - This command is used to set the minimum brightness value of the display for CABC function.
  //  -In principle relationship is that 00h value means the lowest brightness for CABC and FFh value means the brightness for CABC  
  writeData(0x00);

  setDisplayWindow(0, 0, 239, 239);
  //--------------------
	exitSleep();
}
/*******************************************************************************
 * \brief Send command to turn off the display and go to sleep
 ******************************************************************************/
void enterSleep (void)
{
	writeCommand(ST7789_DISPOFF);	// Display Off
	delay(20);
	writeCommand(ST7789_SLPIN);	// Sleep In (Low power mode)
	delay(120);
}
/*******************************************************************************
 * \brief Send command to exit sleep mode and turn on the display
 ******************************************************************************/
void exitSleep (void)
{
	writeCommand(ST7789_SLPOUT); // Exit Sleep Mode
	delay(120);
	writeCommand(ST7789_DISPON); // Display on
}
/*******************************************************************************
 * \brief Macro two swap two variables
 ******************************************************************************/
#define mSwap(a,b,t)\
  {\
  t=a;\
  a=b;\
  b=t;\
  }\
/*******************************************************************************
 * \brief Draws a straight line as fast as possible
 * 
 * Works by setting a display window 1 pixel high from x0 to x1 and writing
 * pixels into it.
 ******************************************************************************/
void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, color_t color)
{
  if(x1 < x0)
  {
    uint16_t temp;
    mSwap(x0, x1, temp);
  }
  setDisplayWindow(x0, y, x1, y);

  while(x0 <= x1)
  {
    //Write the single pixel's worth of data
    M_SPI_WRITE_WAIT(color.r);
    M_SPI_WRITE_WAIT(color.g);
    M_SPI_WRITE_WAIT(color.b);
    x0++;
  }
  while(0==(REG_SERCOM4_SPI_INTFLAG&0x2));
  //SET_CS; // Deselect the display controller
}
/*******************************************************************************
 * \brief Set the display window to the entire screen and fill it up with the
 * specified color
 * 
 * Optimized for the Feather M0
 ******************************************************************************/
void fillScreen(color_t color)
{
	unsigned int i,j;

  setDisplayWindow(0, 0, 239, 239);
  
  SET_RS; // Select the data register
	for(i=0;i<240;i++)
	{
		for(j=0;j<240;j++)
		{
      //Send the command via SPI:
      M_SPI_WRITE_WAIT(color.r);
      M_SPI_WRITE_WAIT(color.g);
      M_SPI_WRITE_WAIT(color.b);
		}
	}
  while(0==(REG_SERCOM4_SPI_INTFLAG&0x2));
  //SET_CS; // Deselect the display controller
}
/*******************************************************************************
 * \brief Used to set the window to write data into
 * 
 * Optimized for the Feather M0
 ******************************************************************************/
void setDisplayWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	CLR_CS; // Select the display controller
  
  //Write the "column address set" command to the LCD
  //Correct the X coordinates based on our current start value
  x0 += X_START;
  x1 += X_START;
  //CASET (2Ah): Column Address Set
  // The value of XS [15:0] and XE [15:0] are referred when RAMWR
  //   command comes.
  // Each value represents one column line in the Frame Memory.
  // XS [15:0] always must be equal to or less than XE [15:0]
  CLR_RS; // Select the command register
  M_SPI_WRITE_DONE(ST7789_CASET); //Column address set

  //Write the parameters for the "column address set" command
  SET_RS; // Select the data register
  M_SPI_WRITE_WAIT(x0>>8);  //Start MSB = XS[15:8]
  M_SPI_WRITE_WAIT(x0);     //Start LSB = XS[ 7:0]
  M_SPI_WRITE_WAIT(x1>>8);  //End MSB   = XE[15:8]
  M_SPI_WRITE_DONE(x1);     //End LSB   = XE[ 7:0]
  
  //Write the "row address set" command to the LCD
  //Correct the Y coordinates based on our current start value
  y0 += Y_START;
  y1 += Y_START;
  //RASET (2Bh): Row Address Set
  // The value of YS [15:0] and YE [15:0] are referred when RAMWR
  //   command comes.
  // Each value represents one row line in the Frame Memory.
  // YS [15:0] always must be equal to or less than YE [15:0]
  CLR_RS; // Select the command register
  M_SPI_WRITE_DONE(ST7789_RASET); //Row address set
  
  //Write the parameters for the "row address set" command
  SET_RS; // Select the data register
  M_SPI_WRITE_WAIT(y0>>8);  //Start MSB = YS[15:8]
  M_SPI_WRITE_WAIT(y0);     //Start LSB = YS[ 7:0]
  M_SPI_WRITE_WAIT(y1>>8);  //End MSB   = YE[15:8]
  M_SPI_WRITE_DONE(y1);     //End LSB   = YE[ 7:0]
  //Write the "write data" command to the LCD
  //RAMWR (2Ch): Memory Write
  CLR_RS; // Select the command register
  M_SPI_WRITE_DONE(ST7789_RAMWR); //write data

  //Intentionally NOT setting RS. The next function is responsible to set it

  //Intentionally NOT setting CS. This function is design to be called 
  // right before a data write
}
/*******************************************************************************
 * \brief Writes a color triple to the display
 ******************************************************************************/
void writeColor(color_t color)
{
  SET_RS; // Select the data register
  //Send the command via SPI:
  M_SPI_WRITE_WAIT(color.r);
  M_SPI_WRITE_WAIT(color.g);
  M_SPI_WRITE_DONE(color.b);
  // Deselect the LCD controller
}
/*******************************************************************************
 * \brief Fills the screen with color bars
 * 
 * Accepts two inputs, the width and height of the display in pixels
 ******************************************************************************/
void writeColorBars(uint8_t width, uint8_t height)
{
  uint8_t barHeight = height / 8; // 240 / 8 = 30
	uint8_t i, j;

  Serial.print("Writing color bars for ");
  Serial.print(width, DEC);
  Serial.print("x");
  Serial.println(height, DEC);

  setDisplayWindow(0, 0, width-1, height-1);

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(i>(barHeight * 7))       // > 210
			{
				writeColor(BLACK);
			}
			else if(i>(barHeight * 6))  // > 180
			{
				writeColor(BLUE);
			}
			else if(i>(barHeight * 5))  // > 150
			{
				writeColor(GREEN);
			}
			else if(i>(barHeight * 4))  // > 120
			{
				writeColor(CYAN);
			}
			else if(i>(barHeight * 3))  // > 90
			{
				writeColor(RED);
			}
			else if(i>(barHeight * 2))  // > 60
			{
				writeColor(MAGENTA);
			}
			else if(i>(barHeight * 1))  // > 30
			{
				writeColor(YELLOW);
			}
			else                        // > 0
			{
				writeColor(WHITE);
			}
		}
	}
}
/*******************************************************************************
 * \brief Fills the display with a tri-color gamma gradient
 * 
 * Used to test the gamma of a display
 ******************************************************************************/
void Fill_Display_Gamma_Gradient(uint8_t height, uint8_t width)
{
  uint16_t x;
  uint8_t barHeight = height / 3;

  Serial.println("Gamma Gradient");
  
  setDisplayWindow(0, 0, width - 1, height - 1);

  for(x=0; x < width; x++)
  {
    //Red
    drawLine(x,(barHeight*0),
              x,(barHeight*1) - 1,
              color_t {x,0,0});
    //Green
    drawLine(x,(barHeight*1),
              x,(barHeight*2) - 1,
              color_t {0,x,0});
    //Blue
    drawLine(x,(barHeight*2),
              x,(barHeight*3) - 1,
              color_t{0,0,x});
  }
}
/*******************************************************************************
 * \brief Draws a line of a specified color from Point [x0, y0] to [x1, y1]
 * 
 * From: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
 ******************************************************************************/
void drawLine(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1, color_t color)
{
  //General case
  if (y0 != y1)
  {
    int16_t dx = abs((int16_t )x1 - (int16_t )x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = abs((int16_t )y1 - (int16_t )y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = (dx > dy ? dx : -dy) / 2;

    for (;;)
    {
      Put_Pixel(x0, y0, color);
      if ((x0 == x1) && (y0 == y1))
      {
        break;
      }
      int16_t e2 = err;
      if (e2 > -dx)
      {
        err -= dy;
        x0 = (uint16_t)((int16_t) x0 + sx);
      }
      if (e2 < dy)
      {
        err += dx;
        y0 = (uint16_t)((int16_t) y0 + sy);
      }
    }
  }
  else
  {
    //Optimized for this display
    Fast_Horizontal_Line(x0, y0, x1, color);
  }
}
/*******************************************************************************
 * \brief places at single pixel of a specified color at [x, y]
 * 
 * Optimized for the Feather M0
 ******************************************************************************/
void Put_Pixel(uint8_t x, uint8_t y, color_t color)
{
  setDisplayWindow(x, y, x, y);

  SET_RS; // Select the data register
  M_SPI_WRITE_WAIT(color.r);
  M_SPI_WRITE_DONE(color.g);
  M_SPI_WRITE_WAIT(color.b);
}
/*******************************************************************************
 * \brief Draws a circle of a specified color centered at [x, y] with a specified radius
 * 
 * From: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 ******************************************************************************/
void LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color)
{
  uint8_t x = radius;
  uint8_t y = 0;
  int16_t radiusError = 1 - (int16_t) x;

  while (x >= y)
  {
    //11 O'Clock
    Put_Pixel(x0 - y, y0 + x, color);
    //1 O'Clock
    Put_Pixel(x0 + y, y0 + x, color);
    //10 O'Clock
    Put_Pixel(x0 - x, y0 + y, color);
    //2 O'Clock
    Put_Pixel(x0 + x, y0 + y, color);
    //8 O'Clock
    Put_Pixel(x0 - x, y0 - y, color);
    //4 O'Clock
    Put_Pixel(x0 + x, y0 - y, color);
    //7 O'Clock
    Put_Pixel(x0 - y, y0 - x, color);
    //5 O'Clock
    Put_Pixel(x0 + y, y0 - x, color);

    y++;
    if (radiusError < 0)
    {
      radiusError += (int16_t)(2 * y + 1);
    }
    else
    {
      x--;
      radiusError += 2 * (((int16_t) y - (int16_t) x) + 1);
    }
  }
}
