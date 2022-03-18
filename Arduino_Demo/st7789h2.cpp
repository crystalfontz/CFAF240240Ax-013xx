#include "st7789h2.h"

//// ********************************************************
//// Defines for the ST7789 registers.
//// ref: https://www.crystalfontz.com/products/document/3277/ST7735_V2.1_20100505.pdf
//#define ST7789_SLPIN      (0x10)
//#define ST7789_SLPOUT     (0x11)
//#define ST7789_INVOFF     (0x20)
//#define ST7789_INVON      (0x21)
//#define ST7789_DISPOFF    (0x28)
//#define ST7789_DISPON     (0x29)
//#define ST7789_CASET      (0x2A)
//#define ST7789_RASET      (0x2B)
//#define ST7789_RAMWR      (0x2C)
//#define ST7789_MADCTL     (0x36)
//#define ST7789_COLMOD     (0x3A)
//#define ST7789_PORCTRL    (0xB2)
//#define ST7789_GCTRL      (0xB7)
//#define ST7789_VCOMS      (0xBB)
//#define ST7789_LCMCTRL    (0xC0)  //LCM Control
//#define ST7789_VDVVRHEN   (0xC2)  //VDV and VRH Command Enable
//#define ST7789_VRHS       (0xC3)  //VRH Set
//#define ST7789_VDVSET     (0xC4)  //VDV Set
//#define ST7789_FRCTR2     (0xC6)  //Frame Rate Control in Normal Mode
//#define ST7789_PWCTRL1    (0xD0)   //Power Control 1
//#define ST7789_PVGAMCTRL  (0xE0)
//#define ST7789_NVGAMCTRL  (0xE1)
//// **************************************************


// ********************************************************
// Defines for the ST7789 registers.
// ref: https://www.crystalfontz.com/products/document/3277/ST7735_V2.1_20100505.pdf
// ********************************************************
#define ST7789_01_SWRESET (0x01)  //SW_RESET
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
void displayInit(void)
{
  //Reset the LCD controller
  CLR_RESET;
  delay(1);//10µS min
  SET_RESET;
  delay(350);//120mS max

  //SLPOUT (11h): Sleep Out 
  //The DC/DC converter is enabled, Internal display oscillator
  //is started, and panel scanning is started.
  SPI_sendCommand(ST7789_SLPOUT);
  delay(120);
  SPI_sendCommand(ST7789_SLPIN);	// Sleep In (Low power mode)

  //-----------------------------Display setting--------------------------------
  SPI_sendCommand(ST7789_MADCTL); //Page 215
  //SPI_sendData(0x00); //DEFAULT
  SPI_sendData(0x48); //BGR
  //SPI_sendData(0x40); //RGB
  
  // Bit D7- Page Address Order
  // “0” = Top to Bottom (When MADCTL D7=”0”).
  // “1” = Bottom to Top (When MADCTL D7=”1”).
  // Bit D6- Column Address Order
  // “0” = Left to Right (When MADCTL D6=”0”).
  // “1” = Right to Left (When MADCTL D6=”1”).
  // Bit D5- Page/Column Order
  // “0” = Normal Mode (When MADCTL D5=”0”).
  // “1” = Reverse Mode (When MADCTL D5=”1”)
  // Note: Bits D7 to D5, alse refer to section 8.12 Address Control
  // Bit D4- Line Address Order
  // “0” = LCD Refresh Top to Bottom (When MADCTL D4=”0”)
  // “1” = LCD Refresh Bottom to Top (When MADCTL D4=”1”)
  // Bit D3- RGB/BGR Order
  // “0” = RGB (When MADCTL D3=”0”)
  // “1” = BGR (When MADCTL D3=”1”)
  // Bit D2- Display Data Latch Data Order
  // “0” = LCD Refresh Left to Right (When MADCTL D2=”0”)
  // “1” = LCD Refresh Right to Left (When MADCTL D2=”1”)  
  //Address control 
  SPI_sendCommand(ST7789_COLMOD); //Interface pixel format Pg 224
  SPI_sendData(0x06);
  
  //SPI_sendCommand(ST7789_INVOFF);
  SPI_sendCommand(ST7789_INVON);

  SPI_sendCommand(ST7789_CASET);
  SPI_sendData(0x00);
  SPI_sendData(0x00);
  SPI_sendData(0x00);
  SPI_sendData(0xEF);

  SPI_sendCommand(ST7789_RASET);
  SPI_sendData(0x00);
  SPI_sendData(0x00);
  SPI_sendData(0x00);
  SPI_sendData(0xEF);

  //------------------------- Frame rate setting-------------------
	SPI_sendCommand(ST7789_PORCTRL);	// Porch Control
	SPI_sendData(0x0C);
	SPI_sendData(0x0C);
	SPI_sendData(0x00);
	SPI_sendData(0x33);
	SPI_sendData(0x33);

	SPI_sendCommand(ST7789_GCTRL);	//Gate Control
	SPI_sendData(0x35);							//VGH:13.26/VGL:-10.43

  //----------------------- Power setting-------------------

	SPI_sendCommand(ST7789_VCOMS);	//VCOM Setting
  SPI_sendData(0x1F);     //VCOM = 0.875V

  SPI_sendCommand(ST7789_LCMCTRL);  //LCM Control
  SPI_sendData(0x2C);     //Power On Sequence default

	SPI_sendCommand(ST7789_VDVVRHEN);  //VDV and VRH Command Enable
  SPI_sendData(0x01);

	SPI_sendCommand(ST7789_VRHS);  //VRH Set
  SPI_sendData(0x12);     //4.45+( vcom+vcom offset+0.5vdv)

	SPI_sendCommand(ST7789_VDVSET);  //VDV Set
  SPI_sendData(0x20);     //VDV = 0V

	SPI_sendCommand(ST7789_FRCTR2);  //Frame Rate Control in Normal Mode
  SPI_sendData(0x0F);     //60HZ

	SPI_sendCommand(ST7789_PWCTRL1);   //Power Control 1
  SPI_sendData(0xA4);
  SPI_sendData(0xA1);     //VDS=2.3V/AVCL = -4.8V /AVDD=6.8V

  //               --------------------
  //               --------------------
  //               Set Gamma     for BOE 1.3
  //               --------------------
  // Set_Gamma: //  Is this a goto?

	SPI_sendCommand(ST7789_PVGAMCTRL);
  SPI_sendData(0xD0);
  SPI_sendData(0x08);
  SPI_sendData(0x11);
  SPI_sendData(0x08);
  SPI_sendData(0x0C);
  SPI_sendData(0x15);
  SPI_sendData(0x39);
  SPI_sendData(0x33);
  SPI_sendData(0x50);
  SPI_sendData(0x36);
  SPI_sendData(0x13);
  SPI_sendData(0x14);
  SPI_sendData(0x29);
  SPI_sendData(0x2D);
  
  //--------------------
	SPI_sendCommand(ST7789_NVGAMCTRL);
  SPI_sendData(0xD0);
  SPI_sendData(0x08);
  SPI_sendData(0x10);
  SPI_sendData(0x08);
  SPI_sendData(0x06);
  SPI_sendData(0x06);
  SPI_sendData(0x39);
  SPI_sendData(0x44);
  SPI_sendData(0x51);
  SPI_sendData(0x0B);
  SPI_sendData(0x16);
  SPI_sendData(0x14);
  SPI_sendData(0x2F);
  SPI_sendData(0x31);


  // //Disable Tearing
  //writeCommand(ST7789_TEOFF);

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
#define BCTRL 0

// DD: Display Dimming (Only for manual brightness setting)
// DD = 0: Display Dimming is off.
// DD = 1: Display Dimming is on.
#define DD    0

// BL: Backlight Control On/Off
// 0 = Off (Completely turn off backlight circuit. Control lines must be low.)
// 1 = On
#define BL    0

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
//*********************************************************
void enterSleep (void)
{
	writeCommand(ST7789_DISPOFF);	// Display Off
	delay(20);
	writeCommand(ST7789_SLPIN);	// Sleep In (Low power mode)
}
//*********************************************************
void exitSleep (void)
{
	writeCommand(ST7789_SLPOUT); // Exit Sleep Mode
	delay(120);
	writeCommand(ST7789_DISPON); // Display on

	//writeCommand(0x2c); // Memory write
	// When this command is accepted, the column register and the page
	// register are reset to the start column/start page positions.
}
// ********************************************************
void fillScreen(uint32_t color)
{
	unsigned int i,j;

  setDisplayWindow(0, 0, 240, 240);
	for(i=0;i<240;i++)
	{
		for(j=0;j<240;j++)
		{
			writeColor(color);
		}
	}
}
// ********************************************************
void fillScreen(color_t color)
{
  unsigned int i, j;


  setDisplayWindow(0, 0, 240, 240);
  for (i = 0; i < 240; i++)
  {
    for (j = 0; j < 240; j++)
    {
      writeColor(color);
    }
  }
}
//============================================================================
// void Set_LCD_for_write_at_X_Y(uint8_t x, uint8_t y)
// {
//   //CASET (2Ah): Column Address Set
//   // * The value of XS [15:0] and XE [15:0] are referred when RAMWR
//   //   command comes.
//   // * Each value represents one column line in the Frame Memory.
//   // * XS [15:0] always must be equal to or less than XE [15:0]
//   SPI_sendCommand(ST7789_CASET); //Column address set
//   //Write the parameters for the "column address set" command
//   SPI_sendData(0x00);     //Start MSB = XS[15:8]
//   SPI_sendData(0x00 + x); //Start LSB = XS[ 7:0]
//   SPI_sendData(0x00);     //End MSB   = XE[15:8]
//   SPI_sendData(0xEF);     //End LSB   = XE[ 7:0]
  
//   //Write the "row address set" command to the LCD
//   //RASET (2Bh): Row Address Set
//   // * The value of YS [15:0] and YE [15:0] are referred when RAMWR
//   //   command comes.
//   // * Each value represents one row line in the Frame Memory.
//   // * YS [15:0] always must be equal to or less than YE [15:0]
//   SPI_sendCommand(ST7789_RASET); //Row address set
//   //Write the parameters for the "row address set" command
//   SPI_sendData(0x00);     //Start MSB = YS[15:8]
//   SPI_sendData(0x00 + y); //Start LSB = YS[ 7:0]
//   SPI_sendData(0x00);     //End MSB   = YE[15:8]
//   SPI_sendData(0xEF);     //End LSB   = YE[ 7:0]
  
//   //Write the "write data" command to the LCD
//   //RAMWR (2Ch): Memory Write
//   SPI_sendCommand(ST7789_RAMWR); //write data
// }
//*********************************************************
void setDisplayWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  //CASET (2Ah): Column Address Set
  // * The value of XS [15:0] and XE [15:0] are referred when RAMWR
  //   command comes.
  // * Each value represents one column line in the Frame Memory.
  // * XS [15:0] always must be equal to or less than XE [15:0]
  writeCommand(ST7789_CASET);	// Column Address Set
  //Write the parameters for the "column address set" command
  writeData(x0>>8);	// X address start:
  writeData(x0);		// 0 <= XS <= X
  writeData(x1>>8);	// X address end:
  writeData(x1);		// S <= XE <= X

  //Write the "row address set" command to the LCD
  //RASET (2Bh): Row Address Set
  // * The value of YS [15:0] and YE [15:0] are referred when RAMWR
  //   command comes.
  // * Each value represents one row line in the Frame Memory.
  // * YS [15:0] always must be equal to or less than YE [15:0]
  SPI_sendCommand(ST7789_RASET); //Row address set
  //Write the parameters for the "row address set" command
  writeData(y0>>8);	// Y address start:
  writeData(y0);		// 0 <= YS <= Y
  writeData(y1>>8);	// Y address start:
  writeData(y1);		// S <= YE <= Y

  // //Write the "write data" command to the LCD
  // //RAMWR (2Ch): Memory Write
  SPI_sendCommand(ST7789_RAMWR); //write data
}
// ********************************************************
void setInterface(void)
{
	//Interface select
	uint8_t IM[4];

#ifdef MCU8BIT
	PORTA = 0x00;	//Pulled low if not in use
	IM[0] = 0;
	IM[1] = 1;
	IM[2] = 0;
	IM[3] = 0;
#endif
#ifdef MCU16BIT
	IM[0] = 0;
	IM[1] = 0;
	IM[2] = 0;
	IM[3] = 0;
#endif

	// PORTF is directly mapped to IM[0:3]
	//PORTF = (IM[3]<<3 | IM[2]<<2 | IM[1]<<1 | IM[0]<<0);
}
// **************************************************
void writeColor(uint32_t color)
{
// #ifdef MCU16BIT	
// 	SET_CD;
// 	PORTE=color>>8;
// 	PORTA=color;
// 	CLR_WR;
// 	SET_WR;
// #endif
// #ifdef MCU8BIT
// 	SET_CD;
//  	CLR_WR;
//  	PORTE=color>>8;
//  	SET_WR;
// 	PORTE=color;
// 	CLR_WR;
// 	SET_WR;
// #endif

  // Select the LCD controller
  CLR_CS;
  // Select the LCD's data register
  SET_DC;
  //Send the command via SPI:
  SPI.transfer(color >> 16);
  SPI.transfer(color >>  8);
  SPI.transfer(color >>  0);
  // Deselect the LCD controller
  SET_CS;

}
// **************************************************
void writeColor(color_t color)
{
  // #ifdef MCU16BIT	
  // 	SET_CD;
  // 	PORTE=color>>8;
  // 	PORTA=color;
  // 	CLR_WR;
  // 	SET_WR;
  // #endif
  // #ifdef MCU8BIT
  // 	SET_CD;
  //  	CLR_WR;
  //  	PORTE=color>>8;
  //  	SET_WR;
  // 	PORTE=color;
  // 	CLR_WR;
  // 	SET_WR;
  // #endif


    // Select the LCD controller
  CLR_CS;
  // Select the LCD's data register
  SET_DC;
  //Send the command via SPI:
  SPI.transfer(color.r);
  SPI.transfer(color.g);
  SPI.transfer(color.b);
  // Deselect the LCD controller
  SET_CS;
}
// ********************************************************
void writeColorBars(uint8_t height, uint8_t width)
{
  uint8_t barHeight = height / 8; // 240 / 8 = 30
	uint8_t i,j;

  Serial.println("Write color bars");

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
//============================================================================
#define mSwap(a,b,t)\
  {\
  t=a;\
  a=b;\
  b=t;\
  }\
//============================================================================
void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1,
                          uint8_t R, uint8_t G, uint8_t B)
{
  uint16_t temp;
  if(x1 < x0)
  {
    mSwap(x0, x1, temp);
  }
  setDisplayWindow(x0, y, x1, y);

  // Select the OLED controller
  CLR_CS;
  // Select the OLED's data register
  SET_DC;

  while(x0 <= x1)
  {
    //Write the single pixel's worth of data
    SPI.transfer(R);
    SPI.transfer(G);
    SPI.transfer(B);
    x0++;
  }
  // Deselect the OLED controller
  SET_CS;    
}
//============================================================================
//Gradient Bar fill
void Fill_OLED_Gamma_Gradient(uint8_t height, uint8_t width)
{
  uint16_t x;
  uint8_t barHeight = height / 3;

  Serial.println("Gamma Gradient");
  
  setDisplayWindow(0, 0, width - 1, height - 1);


  for(x=0; x < width; x++)
  {
    //Red
    OLED_Line(x,(barHeight*0),
              x,(barHeight*1) - 1,
              x,0,0);
    //Green
    OLED_Line(x,(barHeight*1),
              x,(barHeight*2) - 1,
              0,x,0);
    //Blue
    OLED_Line(x,(barHeight*2),
              x,(barHeight*3) - 1,
              0,0,x);
  }
}
//============================================================================
void Fill_LCD(uint8_t R, uint8_t G, uint8_t B)
{
  register int h, w;
 
  setDisplayWindow(0, 0, 239, 239);

  //Fill display with a given RGB value
  for (h = 0; h < 240; h++)
  {
    for (w = 0; w < (240); w++)
    {
      SPI_sendData(R); //Blue
      SPI_sendData(G); //Green
      SPI_sendData(B); //Red
    }
  }
}
//============================================================================
// From: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void OLED_Line(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1,
              uint8_t r, uint8_t g, uint8_t b)
{
  int16_t dx;
  int16_t sx;
  int16_t dy;
  int16_t sy;
  int16_t err;
  int16_t e2;

  //General case
  if (y0 != y1)
  {
    dx = abs((int16_t )x1 - (int16_t )x0);
    sx = x0 < x1 ? 1 : -1;
    dy = abs((int16_t )y1 - (int16_t )y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    for (;;)
    {
      Put_Pixel(x0, y0, r,g,b);
      if ((x0 == x1) && (y0 == y1))
      {
        break;
      }
      e2 = err;
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
    //Optimized for OLED
    Fast_Horizontal_Line(x0, y0, x1,r,g,b);
  }
}  
//============================================================================
void Put_Pixel(uint8_t x, uint8_t y, uint8_t R, uint8_t G, uint8_t B)
{
  setDisplayWindow (x, y, 239, 239);

  //Write the single pixel's worth of data
  SPI_sendData(B); //Blue
  SPI_sendData(G); //Green
  SPI_sendData(R); //Red
}
//============================================================================
// From: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t R, uint8_t G, uint8_t B)
  {
  uint8_t x = radius;
  uint8_t y = 0;
  int16_t radiusError = 1 - (int16_t) x;

  while (x >= y)
    {
    //11 O'Clock
    Put_Pixel(x0 - y, y0 + x, R, G, B);
    //1 O'Clock
    Put_Pixel(x0 + y, y0 + x, R, G, B);
    //10 O'Clock
    Put_Pixel(x0 - x, y0 + y, R, G, B);
    //2 O'Clock
    Put_Pixel(x0 + x, y0 + y, R, G, B);
    //8 O'Clock
    Put_Pixel(x0 - x, y0 - y, R, G, B);
    //4 O'Clock
    Put_Pixel(x0 + x, y0 - y, R, G, B);
    //7 O'Clock
    Put_Pixel(x0 - y, y0 - x, R, G, B);
    //5 O'Clock
    Put_Pixel(x0 + y, y0 - x, R, G, B);

    y++;
    if (radiusError < 0)
      radiusError += (int16_t)(2 * y + 1);
    else
      {
      x--;
      radiusError += 2 * (((int16_t) y - (int16_t) x) + 1);
      }
    }
  }

//============================================================================
void SPI_sendCommand(uint8_t command)
{
  // Select the LCD controller
  CLR_CS;
  // Select the LCD's command register
  CLR_DC;
  //Send the command via SPI:
  SPI.transfer(command);
  // Deselect the LCD controller
  SET_CS;
}
//----------------------------------------------------------------------------
void SPI_sendData(uint8_t data)
{
  // Select the LCD controller
  CLR_CS;
  // Select the LCD's data register
  SET_DC;
  //Send the command via SPI:
  SPI.transfer(data);
  // Deselect the LCD controller
  SET_CS;
}
