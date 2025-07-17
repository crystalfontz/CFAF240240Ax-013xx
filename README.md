# CFAF240240A Family Demo Code

This example code is written for the Crystalfontz CFAF240240A 1.3" TFT Family of displays. These displays use the [ST7789H2](https://www.crystalfontz.com/controllers/Sitronix/ST7789H2/449) display controller and bring out the [SPI](https://www.crystalfontz.com/blog/glossary/spi) interface for communication.

For more information about other TFT options, see our full list [here](https://www.crystalfontz.com/c/tft-lcd-displays/25).

## Display information
Here are links to our active displays:\
[CFAF240240A4-013TN](https://www.crystalfontz.com/product/cfaf240240a4013tn)

Here are the breakout boards and dev kits:\
[CFAF240240A4-013TN-E1](https://www.crystalfontz.com/product/cfaf240240a4013tne1) display and adapter board \
[CFAF240240A4-013TN-E1-2](https://www.crystalfontz.com/product/cfaf240240a4013tnkit) full display dev kit\

Some demos use files stored on an SD card. Our [CFA10112 micro SD card adapter](https://www.crystalfontz.com/product/cfa10112) can be used. Use of an SD card only needed if using the bmp_demo to show images.

An example schematic can be downloaded from [here](https://www.crystalfontz.com/products/document/4764/10107_h0v4_Build_Files.zip).

Note that the CFAF240240A family of displays use 3.3V logic. Using 5V logic can permanently damage the display. We use a [Seeeduino with switchable 3.3V logic](https://crystalfontz.com/product/cfapn15062), but you could alternately step the logic lines down to 3.3V.

## Demo Code

This repo includes two demo code folders and a folder for images:
*Arduino_Demo includes code for the ATMEGA328 (e.g., Seeeduino or Arduino Uno) 
*Adalogger_Demo includes code for the ATSAMD21G18 (e.g., Adalogger)
* SD_CARD_CONTENTS contains images that can be loaded onto an SD card and displayed from there. Use of this folder is optional.

### Included Demos

There are six separate demos included in the code. Each one can be enabled or disabled by defining it as either 1 (enabled) or 0 (disabled). This is located just above the loop.

#define fullscreen_demo     1
#define colorbar_demo       1
#define gammagradient_demo  1
#define circle_demo         0
#define font_demo           0
#define bmp_demo            0

When fullscreen_demo is enabled, the display will fill the screen with white, then red, then green, then blue for 0.5 seconds each.

When colorbar_demo is enabled, eight different colors will be displayed on the screen in rows.

When gammagradient_demo is enabled, three different colors will be displayed on the screen in rows.

When circle_demo is enabled, a ceries of circles will be drawn.

When font_demo is enabled, a different font styles will be shown, including using foreground and backgorund colors and moving text on the display.

When bmp_demo is enabled, BMP images that are exactly 172854 bits and stored on an attached SD card will be displayed. BMP must be 240x240 24-bit.

## Connection Details
#### Connection details 

| CFAF240240A Pin | Seeeduino Pin  | Pin Function | Seeeduino Pin SPI | 
|-----------------|----------------|--------------|---------------------------------- |
| 1               | N/A            | K (LED-)     | A 20mA LED driver should be used   |
| 2               | N/A            | A (LED+)     | A 20mA LED driver should be used   |
| 3               | DNC            | CABC PWM     | PWM out to control the backlight. Leave open if not in use.|
| 4               | DNC            | CABC EN      | Enable pin to control the backlight. Leave open if not in use.|
| 5               | GND            | GND          | Ground                                        |
| 6               | 3V3            | VDD          | Supply voltage                                |
| 7               | 3V3            | VDDIO        | I/O voltage                                   |
| 8               | GND            | GND          | Ground                                        |
| 9               | D13            | SCLK         | Serial clock                                  |
| 10              | D11            | SDA          | Serial input                                  |
| 11              | D12            | SDO          | Serial output                                 |
| 12              | D8             | D/C          | Data / Command                                |
| 13              | D10            | /CS          | Chip select                                   |
| 14              | GND            | GND          | Ground                                        |
| 15              | D9             | /RST         | Reset Pin                                     |
| 16              | 3V3            | IM3          | Interface Mode                                |
| 17              | DNC            | TE           | Tearing effect pin. Leave open if not in use. |
| 18              | GND            | GND          | Ground                                        |


If using the CFAF240240A4-013TN-E1 which includes the CFA10107 adapter board

| Adapter Pin  | Seeeduino Pin  | Pin Function | Description       | 
|--------------|----------------|--------------|------------------ |
| 1 3v3        | 3V3            | VDD          | Supply voltage    |
| 2 GND        | GND            | GND          | Ground            |
| 3 SDA        | D11            | SDA          | Serial input      |
| 4 SCLK       | D13            | SCLK         | Serial clock      |
| 5 D/C        | D8             | D/C          | Data / Command    |
| 6 SDO        | D12            | SDO          | Serial output     |
| 7 /RST       | D9             | /RST         | Reset Pin         |
| 8 /CS        | D10            | /CS          | Chip select       |    
| 9 PWM        | D6             | PWM          | CABC PWM control  |
| 10 TE        | DNC            | TE           | Tearing effect pin. Leave open if not in use. |      



A CFA10112 micro SD card adapter should be used: https://www.crystalfontz.com/product/cfa10112
| microSD Pin | Seeeduino Pin | Connection Description |
|-------------|---------------|------------------------|
| 1 (3V3/VCC) | 3V3           | 3.3V power             |
| 2 (GND)     | GND           | Ground                 |
| 3 (SCLK)    | 13            | SD Clock               |
| 4 (DAT2)    |  -            | No Connection          |
| 5 (CS)      | 7             | SD Chip Select         |
| 6 (DI)      | 11            | SD Data In             |
| 7 (DO)      | 12            | SD Data Out            |
| 8 (DAT1)    | -             | No Connection          |

(microSD use is optional, but required for DEMO_BMPIMAGES)

## Additional Accessories
Additional accessories for the products can be found at the bottom of each of the product pages. 
