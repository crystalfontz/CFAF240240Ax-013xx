#include <SPI.h>
#include "st7789h2.h"
#include "atsamd21g18.h"

/*******************************************************************************
 * \brief Initialize the microprocessor
 * 
 * Readies the micro to write to a display.
 ******************************************************************************/
void hostInit(void)
{
  REG_PORT_DIRSET0 = (LCD_CS | LCD_RESET);
  REG_PORT_DIRSET1 = (LCD_IM3 | LCD_RS);
  
  SET_IM3;  //SDI/SDO on different pins
  //CLR_IM3; //SDI/SDO share MISO
  
  //Drive the ports to a reasonable starting state.
  CLR_RESET;  //Put the display controller into reset
  SET_CS; // Deselect the display controller
  CLR_RS; // Select the command register
}
/*******************************************************************************
 * \brief Cycles the display's reset pin to ensure the display is ready
 ******************************************************************************/
void reset_display(void)
{
	CLR_RESET;  //Put the display controller into reset
	delay(10);
	SET_RESET;  //Take the display controller out of reset
	delay(10);
}
/*******************************************************************************
 * \brief Reads the images on the SD card and writes them to the display
 * 
 * Requires tweaking to ensure the file size is correct for the display
 * being written to
 ******************************************************************************/
void show_BMPs_in_root(void)
{
  Serial.println("Showing BMPs from uSD Card");

  File root_dir;
  root_dir = SD.open("/");
  if(0 == root_dir)
  {
    Serial.println("show_BMPs_in_root: Can't open \"root\"");
  }
  else
  {
    File bmp_file;
    
    do
    {

      bmp_file = root_dir.openNextFile();
      if (0 == bmp_file)
      {
        // no more files, break out of while()
        // root_dir will be closed below.
        break;
      }
      //Skip directories (what about volume name?)
      else if(0 == bmp_file.isDirectory())
      {
        //The file name must include ".BMP"
        if(0 != strstr(bmp_file.name(),".BMP"))
        {
          //The BMP must be exactly 172854 long
          //(this is correct for 240x240, 24-bit + 54)
          if((172854 == bmp_file.size()) || (172856 == bmp_file.size()))
          {
            Serial.print("Displaying ");
            Serial.println(bmp_file.name());
            //Jump over BMP header. BMP must be 240x240 24-bit
            bmp_file.seek(54);
            //Bitmap file happens to match the LCD 1:1. Nice.
            
            setDisplayWindow(0,0,239,239);
            SET_CS; // Deselect the display controller

            //Since we are limited in memory, break the image up
            //into 24 sections, each one 10 lines.
            // 240 * 3 * 10 = 7200
            const uint8_t width = 240;
            const uint8_t height = 240;
            const uint8_t lines_per_section = 10;
            static uint8_t uSDLine[width*3*lines_per_section];
            for(uint8_t line = 0; line < height; line+=lines_per_section)
              {
              //Get 10 lines worth of pixels
              // This takes about 26mS to read in the 7200 bytes
              // ~2,215,384 Hz Thoughput
              bmp_file.read(uSDLine, width*3*lines_per_section);
              //Now write this section to the TFT
              SPI_send_pixels_666(width*lines_per_section, uSDLine);
              }
          }
          else
          {
            Serial.print("This BMP is ");
            Serial.print(bmp_file.size());
            Serial.println(" bytes");
          }
        }
      }
      //Release the BMP file handle
      bmp_file.close();
      delay(2000);  // Wait between each image
    } while(0 == bmp_file);
  }
  //Release the root directory file handle
  root_dir.close();
}
/*******************************************************************************
 * \brief Writes an array to the display in 565 mode
 * 
 * The information starts at data_ptr and continues for a number of pixels
 * equal to pixel_count
 ******************************************************************************/
void SPI_send_pixels_565(uint8_t pixel_count, uint8_t *data_ptr)
{
  // This function transfers data, in one stream. Slightly
  // optimized to do index operations during SPI transfers.
  color_t color;
  uint8_t first_half, second_half;

  SET_RS; // Select the data register
  CLR_CS; // Select the display controller
  
  //Load the first pixel. BMPs BGR format
  color.b=*data_ptr;
  data_ptr++;
  color.g=*data_ptr;
  data_ptr++;
  color.r=*data_ptr;
  data_ptr++;

  //The display takes two bytes (565) RRRRR GGGGGG BBBBB 
  //to show one pixel.
  first_half=(color.r&0xF8) | (color.g >> 5);
  second_half=((color.g << 3)&0xE0) | (color.b >> 3);

  while(pixel_count)
    {
    //Send the first half of this pixel out
    REG_SERCOM4_SPI_DATA = first_half;
    //Load the next pixel while that is transmitting
    color.b=*data_ptr;
    data_ptr++;
    color.g=*data_ptr;
    data_ptr++;
    color.r=*data_ptr;
    data_ptr++;
    //Calculate the next first half while that is transmitting
    // ~1.9368us -0.1256 us = 1.8112uS
    first_half=(color.r&0xF8) | (color.g >> 5);
    //At 8MHz SPI clock, the transfer is done by now, so we
    //do not need this:
    // while (!(SPSR & _BV(SPIF))) ;
    //Send the second half of the this pixel out
    REG_SERCOM4_SPI_DATA = second_half;
    //Calculate the next second half
    second_half=((color.g << 3)&0xE0) | (color.b >> 3);
    //Done with this pixel
    pixel_count--;
    //At 8MHz SPI clock, the transfer is done by now, so we
    //do not need this:
    // while (!(SPSR & _BV(SPIF))) ;
    }
  //Wait for the final transfer to complete before we bang on CS.
  while (0 == (REG_SERCOM4_SPI_INTFLAG & 0x2));
  //SET_CS; // Deselect the display controller
}
/*******************************************************************************
 * \brief Writes an array to the display in 666 mode
 * 
 * The information starts at data_ptr and continues for a number of pixels
 * equal to pixel_count
 ******************************************************************************/
void SPI_send_pixels_666(uint16_t pixel_count, uint8_t *data_ptr)
{
  // This function transfers data, in one stream.
  color_t color;

  // Ensure this display is selected after having just read from the uSD card.
  CLR_CS; // Select the display controller

  SET_RS; // Select the data register

  // This takes 4.82mS to ship out the 7200 bytes
  // ~11,950,207 Hz
  while(pixel_count)
  {

    //Load the byte
    color.b=*data_ptr++;
    color.g=*data_ptr++;
    color.r=*data_ptr++;
    
    M_SPI_WRITE_WAIT(color.r);
    M_SPI_WRITE_WAIT(color.g);
    M_SPI_WRITE_WAIT(color.b);

    //count this pixel
    pixel_count--;
  }
  
  while (0 == (REG_SERCOM4_SPI_INTFLAG & 0x2));
  SET_CS; // Deselect the display controller
}
/*******************************************************************************
 * \brief Writes a command byte to the display
 ******************************************************************************/
void writeCommand(uint8_t command)
{
	CLR_CS; // Select the display controller
	CLR_RS; // Select the command register

	//Send the command via SPI:
	SPI.transfer(command);
	//SET_CS; // Deselect the display controller
}
/*******************************************************************************
 * \brief Writes a data byte to the display
 ******************************************************************************/
void writeData(uint8_t data)
{
	CLR_CS; // Select the display controller
	SET_RS; // Select the data register
  //Send the command via SPI:
	SPI.transfer(data);

	//SET_CS; // Deselect the display controller
}
//================================================================================
// extern "C" char *sbrk(int i);
//
// int FreeRam () {
//   char stack_dummy = 0;
//   return &stack_dummy - sbrk(0);
// }
//================================================================================
