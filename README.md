# CFAF240240A Family Demo Code

This example code is written for the Crystalfontz CFAF240240A 1.3" TFT Family of displays. These displays use the [ST7789H2](https://www.crystalfontz.com/controllers/Sitronix/ST7789H2/449) display controller and bring out the [SPI](https://www.crystalfontz.com/blog/glossary/spi) interface for communication.

For more information about other TFT options, see our full list [here](https://www.crystalfontz.com/c/tft-lcd-displays/25).

## Display information
Here are links to our active displays:\
[CFAF240240A3-013TN](https://www.crystalfontz.com/product/cfaf240240a3013tn)

Here are the breakout boards and dev kits:\
[CFAF240240A3-013TN-E1](https://www.crystalfontz.com/product/cfaf240240a3013tne1) display and adapter board \
[CFAF240240A3-013TN-E1-2](https://www.crystalfontz.com/product/cfaf240240a3013tne12) full display dev kit

An example schematic can be downloaded from [here](https://www.crystalfontz.com/products/document/4386/CFAF240240A1013T_0v1_CAM.zip).
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


## Additional Accessories
Additional accessories for the products can be found at the bottom of each of the product pages. 