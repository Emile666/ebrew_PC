// ==================================================================
// Functienaam : WriteNibbleLCD(), ReadNibbleLCD(), BusyLCD(),
//               WriteCharLCD(),   ReadCharLCD(),   WriteInstrLCD(),
//               GetAddrCntLCD(),  InitLCD(),       set_led(),
//               print_lcd(),      check_i2c_hw(),  hw_err()
// Filenaam    : $Id$
// Auteur      : E. van de Logt
// Datum       : 12-03-2000
// Versie      : V0.1
// ------------------------------------------------------------------
// Doel : Deze file bevat de basisroutines voor de I2C bus. Deze
//	  file is afgeleid van de Elektuur i2c.c file.
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// 14-03-02 LGT    - check_I2C_hw(): few lines from main (brewery.c) added
//                 - function hw_err() added
// 19-03-02 LGT    - Adapted for use with i2c2.c instead of device driver.
// $Log$
// ==================================================================

// -------------------------- include files -------------------------
//#include <dos.h>
//#include <fcntl.h>
//#include <dos.h>
//#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>

#include "i2c.h"

// -------------------------- external section ----------------------
static int BackLight;             // Status of Backlight
static int lcd_lines_printed = 0; // #lines printed to LCD Display

//-------------------------------------------------------------------------
// Values needed to decode a decimal number into a byte for the LED Display
// b0..b6: segment a..g        a
// b7    : decimal point      f b
//                             g
//                            e c
//                             d .
//-------------------------------------------------------------------------
static int led_decode[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

int WriteNibbleLCD(byte Regi, byte DataNibble)
/*------------------------------------------------------------------
  Purpose  : Write one DataNibble to the LCD module
             Control and data lines of LCD module
             !BL  (P7)  DB7 (P3)
             E    (P6)  DB6 (P2)
             D/!I (P5)  DB5 (P1)
             R/!W (P4)  DB4 (P0)
             Px is I/O line of I2C-I/O port of PCF8574
             P7 is MSB, P0 is LSB

  Variables: Regi = DR: DataNibble is written into the dataregister
             Regi = IR: DataNibble is written into the instruction register
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
{
   byte to_io;   // byte to write to I2C
   int  res = 0; // return result

   to_io = DataNibble; // DB7..DB4 = data
   if (BackLight)
   {
      to_io &= 0x7F;
   }
   else
   {
      to_io |= 0x80;
   }
   to_io &= 0xBF;     // E = 0
   if (Regi == DR)
   {
      to_io |= 0x20;  // D/!I = 1, write to data register
   }
   else
   {
      to_io &= 0xDF;  // D/!I = 0, write to instruction register
   }
   to_io &= 0xEF;     // R/!W = 0
   res  = i2c_write(LCD_BASE,&to_io,1); // write 1 byte to LCD
   to_io |= 0x40;     // E = 1
   res |= i2c_write(LCD_BASE,&to_io,1); // write 1 byte to LCD
   to_io &= 0xBF;     // E = 0
   res |= i2c_write(LCD_BASE,&to_io,1); // write 1 byte to LCD
   return res;
} // WriteNibbleLCD()

int ReadNibbleLCD(byte Regi, byte *DataNibble)
/*------------------------------------------------------------------
  Purpose  : Read one DataNibble from the LCD module
  Variables: Regi = DR: DataNibble is written into the dataregister
             Regi = IR: DataNibble is written into the instruction register
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
{
   byte to_io;   // databyte to write to I2C
   byte from_io; // databyte to read from I2C
   int  res = 0; // return result

   to_io = 0x0F; // DB7..DB4 = 0x0F (data)
   if (BackLight)
   {
      to_io &= 0x7F;
   }
   else
   {
      to_io |= 0x80;
   }
   to_io &= 0xBF;     // E = 0
   if (Regi == DR)
   {
      to_io |= 0x20;  // D/!I = 1, write to data register
   }
   else
   {
      to_io &= 0xDF;  // D/!I = 0, write to instruction register
   }
   to_io   |= 0x10;     // R/!W = 1
   res      = i2c_write(LCD_BASE,&to_io,1);  // write 1 byte to LCD
   to_io   |= 0x40;     // E = 1
   res     |= i2c_write(LCD_BASE,&to_io,1);  // write 1 byte to LCD
   res     |= i2c_read(LCD_BASE | RWb,&from_io,1); // read DB7..DB4
   to_io   &= 0xBF;     // E = 0
   *DataNibble = from_io & 0x0F;
   res        |= i2c_write(LCD_BASE,&to_io,1); // write 1 byte to LCD
   return res;
} // ReadNibbleLCD()

int BusyLCD(void)
/*------------------------------------------------------------------
  Purpose  : Checks the busy flag of the LCD display. This busy flag
             must be checked before performing instructions or
             reading and writing data.
             The returned BusyLCD should be false: when it is true,
             there is probably something wrong with the LCD module
  Variables:
  Returns  : TRUE : LCD Display is still busy
             FALSE: LCD Display is ready
  ------------------------------------------------------------------*/
{
   byte DataH;           // High order 4 bits
   byte DataL;           // Low  order 4 bits
   int  Busy;            // Busy Flag, Boolean
   byte TimeCounter = 0; // init. timer
   int  res;             // return result

   do
   {
      res  = ReadNibbleLCD(IR,&DataH); // Read busy-flag of LCD
      if (res) return res; // exit in case of error
      res |= ReadNibbleLCD(IR,&DataL); // Complete data-transfer
      if (res) return res; // exit in case of error

      Busy = ((DataH & LCD_BUSY_FLAG) == LCD_BUSY_FLAG);
      if (Busy) // Busy flag set ?
      {
         delay(1);      // wait 1 msec.
         TimeCounter++; // increment timer
      }
   } while ((TimeCounter < LCD_TIMEOUT) && Busy);

   return Busy; // return BusyLCD
} // BusyLCD()

int WriteCharLCD(byte Data)
/*------------------------------------------------------------------
  Purpose  : Write a character-code-byte to the display data RAM (DD RAM)
             or to the character generator RAM (CG RAM). Wheter the DD
             or the CG RAM is to be written to, is determined by the
             previous LCD set address instruction (set DD RAM address
             or set CG RAM address).
  Variables: Data : byte to write to the LCD display
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int r1; // return-value

   if (!BusyLCD())
   {
      r1  = WriteNibbleLCD(DR,Data >> 4); // Transfer high order 4 bits
      r1 |= WriteNibbleLCD(DR,Data);      // Transfer low  order 4 bits
   }
   else
   {
      r1 = TRUE;
   }
   return r1;
} // WriteCharLCD()

int ReadCharLCD(byte *Data)
/*------------------------------------------------------------------
  Purpose  : Reads a character-code-byte from the display data RAM (DD RAM)
             or from the character generator RAM (CG RAM). Wheter the DD
             or the CG RAM is to be read, is determined by the
             previous LCD set address instruction (set DD RAM address
             or set CG RAM address).
  Variables: Data : byte that is read from the LCD display
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   byte DataH; // High data-nibble
   byte DataL; // Low data-nibble
   int r1;     // Return-value

   if (!BusyLCD())
   {
      r1  = ReadNibbleLCD(DR,&DataH); // Read high order 4 bits
      r1 |= ReadNibbleLCD(DR,&DataL); // Read low  order 4 bits
      *Data = (DataH << 4) | DataL;   // Add 2 nibbles together
   }
   else
   {
      r1 = TRUE;
   }
   return r1;
} // ReadCharLCD()

int WriteInstrLCD(byte Data)
/*------------------------------------------------------------------
  Purpose  : Write an instruction byte into the LCD instruction register
  Variables: Data : byte to write into the instruction register
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int r1;     // Return-value

   if (!BusyLCD())
   {
      r1  = WriteNibbleLCD(IR,Data >> 4); // Transfer high order 4 bits
      r1 |= WriteNibbleLCD(IR,Data);      // Transfer low  order 4 bits
   }
   else
   {
      r1 = TRUE;
   }
   return r1;
} // WriteInstrLCD()

byte GetAddrCntLCD(byte *addr_cntr)
/*------------------------------------------------------------------
  Purpose  : This function returns the state of the address counter.
             The address counter is used by the character generator
             RAM (CG RAM) as well as display data RAM (DD RAM), and
             it's value is determined by the previous instructions
             (set CG RAM address, set DD RAM address).
  Variables: -
  Returns  : Address Counter
  ------------------------------------------------------------------*/
{
   byte DataL; // Low order bytes of the Address Counter
   byte DataH; // High order bytes of the Address Counter
   int r1;     // 1st returnvalue

   r1  = ReadNibbleLCD(IR,&DataH); // Read high order 4 bits
   r1 |= ReadNibbleLCD(IR,&DataL); // Read low  order 4 bits
   *addr_cntr = (DataH << 4) | DataL;  // Add 2 nibbles together
   return r1;
} // GetAddrCntLCD()

int InitLCD(void)
/*------------------------------------------------------------------
  Purpose  : This function initializes the LCD Display.
  Variables: -
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int  r1;    // return-value
   byte to_io; // byte to send to LCD Display

   r1    = i2c_address(LCD_BASE); // Put LCD address on I2C bus
   to_io = LCD_INITVALUE;         // Init. LCD data and control lines
   r1    = i2c_write(LCD_BASE,&to_io,1); // write 1 byte to LCD
   BackLight = FALSE;            // Sync. Backlight status
   delay(15);
   r1 = WriteNibbleLCD(IR,0x03); // Switch LCD from 8-bit into 4-bit interface mode
   delay(5);
   r1 |= WriteNibbleLCD(IR,0x03);
   delay(1);
   r1 |= WriteNibbleLCD(IR,0x03);
   delay(1);
   r1 |= WriteNibbleLCD(IR,0x02);
   r1 |= WriteInstrLCD(0x28); // 4-bit data-length, 2 display lines, 5*7 character font
   r1 |= WriteInstrLCD(0x0F); // display on, cursor on, cursor blink
   r1 |= WriteInstrLCD(0x01); // Clear display
   r1 |= WriteInstrLCD(0x06); // auto incr./decr. RAM address, no display shift
   return r1;
} // InitLCD()

int set_led(int number, int dp, int which_led)
/*------------------------------------------------------------------
  Purpose  : This function sets a LED Display to a 4 digit number
  Variables: number   : a decimal number between 0 and 9999 (-1 = blank)
             dp       : [0..4], indicating which decimal point to set, 0=no dp
             which_led: [1..4], indicating which LED Display to address
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int   r1 = FALSE;       // return-value
   div_t temp;             // temp. variable
   byte  e1;               // duizentallen byte
   byte  e2;               // honderdtallen byte
   byte  e3;               // tientallen byte
   byte  e4;               // eenheden byte
   byte  buffer[LED_BUF];  // buffer to sent to LED display
   int   bidx = 0;         // Index into buffer[]

   buffer[bidx++] = LED_INSTR_BYTE;   // Send Intruction byte
   buffer[bidx++] = LED_CONTROL_BYTE; // Init. LED control register

   //-----------------------------------------
   // Convert digit into bytes for LED Display
   // Write result to LED display
   //-----------------------------------------
   if (number == -1)
   {
      buffer[bidx++] = 0x00;
      buffer[bidx++] = 0x00;
      buffer[bidx++] = 0x00;
      buffer[bidx++] = 0x00;
   }
   else
   {
      temp           = div(number,1000);
      e1             = temp.quot;
      temp           = div(temp.rem,100);
      e2             = temp.quot;
      temp           = div(temp.rem,10);
      e3             = temp.quot;
      e4             = temp.rem;
      buffer[bidx]   = led_decode[e1];
      dp == 1 ? buffer[bidx++] |= LED_DP : bidx++; // Add decimal point
      buffer[bidx]   = led_decode[e2];
      dp == 2 ? buffer[bidx++] |= LED_DP : bidx++; // Add decimal point
      buffer[bidx]   = led_decode[e3];
      dp == 3 ? buffer[bidx++] |= LED_DP : bidx++; // Add decimal point
      buffer[bidx]   = led_decode[e4];
      dp == 4 ? buffer[bidx++] |= LED_DP : bidx++; // Add decimal point
   } // else

   //-----------------------
   // Write to LED Display
   //-----------------------
   switch(which_led)
   {
      case 1 : r1  = i2c_address(LED1_BASE); //Put LED1 address on I2C bus
               r1 |= i2c_write(LED1_BASE,buffer,bidx); // write buffer to LED
               break;
      case 2 : r1  = i2c_address(LED2_BASE); //Put LED2 address on I2C bus
               r1 |= i2c_write(LED2_BASE,buffer,bidx); // write buffer to LED
               break;
      case 3 : r1  = i2c_address(LED3_BASE); //Put LED3 address on I2C bus
               r1 |= i2c_write(LED3_BASE,buffer,bidx); // write buffer to LED
               break;
      case 4 : r1  = i2c_address(LED4_BASE); //Put LED4 address on I2C bus
               r1 |= i2c_write(LED4_BASE,buffer,bidx); // write buffer to LED
               break;
      default: break;
   } // switch
   return r1; // return-value
} // set_led()

int print_lcd(char *s)
/*------------------------------------------------------------------
  Purpose  : This function prints a string to the LCD Display.
             If more than 2 lines are printed, then the lines are
             'scrolled'.
             If the string is smaller, then spaces are added.
  Variables: s[]              : the string to print to the LCD Display
             lcd_lines_printed: this is a static variable (in i2c.c)
                                that counts the #lines printed
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int  r1 = 0;       // return-value
   int  i;            // loop-counter
   byte B;            // byte to read from LCD Display
   char s1[STRLEN];   // temporary string
   char stmp[STRLEN]; // local copy of s[]

   r1 = i2c_address(LCD_BASE); // Put LCD address on I2C bus
   strcpy(stmp,s);        // copy s[] into stmp[]
   for (i = strlen(stmp); i < LCD_MAX_CHAR; i++)
   {
      stmp[i] = ' ';
   }
   stmp[i] = '\0';

   if (lcd_lines_printed == 0)
   {
      r1 = WriteInstrLCD(LCD_LINE1_ADDR); // set cursor to base address of 1st line
      for (i = 0; i < LCD_MAX_CHAR; i++)
      {
         r1 |= WriteCharLCD(stmp[i]);
      } // for
      lcd_lines_printed++;
   }
   else if (lcd_lines_printed == 1)
   {
      r1 = WriteInstrLCD(LCD_LINE2_ADDR); // set cursor to base address of 2nd line
      for (i = 0; i < LCD_MAX_CHAR; i++)
      {
         r1 |= WriteCharLCD(stmp[i]);
      } // for
      lcd_lines_printed++;
   }
   else
   {
      r1 = WriteInstrLCD(LCD_LINE2_ADDR); // Read string from line 2
      for (i = 0; i < LCD_MAX_CHAR; i++)
      {
         r1 |= ReadCharLCD(&B);
         s1[i] = B;
      } // for
      s1[i] = '\0';

      r1 = WriteInstrLCD(LCD_LINE1_ADDR); // Write string to line 1
      for (i = 0; i < LCD_MAX_CHAR; i++)
      {
         r1 |= WriteCharLCD(s1[i]);
      } // for

      r1 = WriteInstrLCD(LCD_LINE2_ADDR); // Write string to line 2
      for (i = 0; i < LCD_MAX_CHAR; i++)
      {
         r1 |= WriteCharLCD(stmp[i]);
      } // for
   }
   return r1;
} // print_lcd()

void check_i2c_hw(int *HW_present)
/*------------------------------------------------------------------
  Purpose  : This function checks all I2C Hardware if they are present
  Variables: HW_present: each bit set indicates that device is present.
                         See i2c.h for bit-details
  Returns  : nothing
  ------------------------------------------------------------------*/
{
   //----------------------------------------------------
   // Check the individual Hardware Devices.
   //----------------------------------------------------
   *HW_present = I2C_NOERR; // Assume no hardware devices are present
   if (i2c_address(LCD_BASE) == I2C_NOERR)
   {
      *HW_present |= LCD_OK;
      // InitLCD(); // Init. the LCD Display
   }
   if (i2c_address(DIG_IO_LSB_BASE) == I2C_NOERR)
   {
      *HW_present |= DIG_IO_LSB_OK;
   }
   if (i2c_address(DIG_IO_MSB_BASE) == I2C_NOERR)
   {
      *HW_present |= DIG_IO_MSB_OK;
   }
   if (i2c_address(LED1_BASE) == I2C_NOERR)
   {
      *HW_present |= LED1_OK;
   }
   if (i2c_address(LED2_BASE) == I2C_NOERR)
   {
      *HW_present |= LED2_OK;
   }
   if (i2c_address(LED3_BASE) == I2C_NOERR)
   {
      *HW_present |= LED3_OK;
   }
   if (i2c_address(LED4_BASE) == I2C_NOERR)
   {
      *HW_present |= LED4_OK;
   }
   if (i2c_address(ADDA_BASE) == I2C_NOERR)
   {
      *HW_present |= ADDA_OK;
   }
   if (i2c_address(MAX6626_1_BASE) == I2C_NOERR)
   {
      *HW_present |= MAX6626_1_OK;
   }
   if (i2c_address(MAX6626_2_BASE) == I2C_NOERR)
   {
      *HW_present |= MAX6626_2_OK;
   }
   if (i2c_address(ADS7828_BASE) == I2C_NOERR)
   {
      *HW_present |= ADS7828_OK;
   }
   if (i2c_address(FM24C08_BASE) == I2C_NOERR)
   {
      *HW_present |= FM24C08_OK;
   }
} // check_i2c_hw()

int read_lcd(void)
/*------------------------------------------------------------------
  Purpose  : This function reads the 2 lines from the LCD Display
  Variables: -
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   byte B;   // byte to read from LCD Display
   int  i;   // loop-counter
   int  res; // Result from ReadCharLCD()
   char s1[50];
   char s2[50];

   i2c_address(LCD_BASE);
   WriteInstrLCD(LCD_LINE1_ADDR);
   for (i = 0; i < LCD_MAX_CHAR; i++)
   {
      res = ReadCharLCD(&B); // read 1 char from LCD
      s1[i] = B;
   } // for
   s1[i] = '\0';

   WriteInstrLCD(LCD_LINE2_ADDR);
   for (i = 0; i < LCD_MAX_CHAR; i++)
   {
      res |= ReadCharLCD(&B); // read 1 char from LCD
      s2[i] = B;
   } // for
   s2[i] = '\0';
   printf("%s\n%s\n",s1,s2);
   return res;
} // read_lcd()

void init_adc(adda_t *p)
/*------------------------------------------------------------------
  Purpose  : This function calculates the conversion constant for the
             four AD Converters. Input is Vrefx (x = 1..4), output is
             adxc (x=1..4).
             Note that the ref. voltage is approx. 1.848 Volt for all ADC.
             But every LM35 output (10 mV/°C) is amplified by 2 approx.
             So the effective Vref would be close to 90 °C, but should be
             calibrated for every AD channel.
  Variables: p: pointer to a struct that will contain the values.
  Returns  : None
  ------------------------------------------------------------------*/
{
   // 8 bit AD = 256 steps; 10.0 -> E-1 mV
   p->ad1c = (double)(p->vref1) / 2560.0;
   p->ad2c = (double)(p->vref2) / 2560.0;
} // init_adc()

/*------------------------------------------------------------------
  Purpose  : This function reads one AD channel of the ADS7828.
             The ADS7828 first expects a command byte, bits 4-7 select
             the appropriate channel, while bits 2-3 select the power-down
             selection
  Variables: channel: [0..7] AD channel number to read value from
             value:   The 12 bit value is returned here
  Returns  : < 0: error
               0: everything OK
  ------------------------------------------------------------------*/
int read_ads7828(int channel, int *value)
{
   // Command Byte Bits 7 - 4
   // 1111 = Ch.7, 1110 = Ch.5, 1101 = Ch.3, 1100 = Ch.1
   // 1011 = Ch.6, 1010 = Ch.4, 1001 = Ch.2, 1000 = Ch.0
   // Bits 3 - 2: Power-down selection: 11 = Int. Vref on & ADC on
   byte cmd[8] = {0x8c,0xcc,0x9c,0xdc,0xac,0xec,0xbc,0xfc};

   byte to_ad;   // array to write to ADC
   byte from_ad; // array to write to ADC
   int  r1;      // Result from i2c_write() and i2c_read

   if ((channel > 7) || (channel < 0))
   {
      return I2C_ARGS;
   } // if
   r1    = i2c_address(ADS7828_BASE); // Put ADC address on I2C bus
   to_ad = cmd[channel];
   r1   |= i2c_write(ADS7828_BASE,&to_ad,1);    // send 1 byte to ADC

   //---------------------------------------------------------------
   // Now we switch to read mode and read 2 bytes from the ADS7828
   //---------------------------------------------------------------
   r1 |= i2c_read(ADS7828_BASE | RWb,&from_ad,1); // read MSB
   *value = (from_ad << 8);
   r1 |= i2c_read(ADS7828_BASE | RWb,&from_ad,1); // read LSB
   *value |= (from_ad & 0x0ff);
   return r1;
} // read_ads7828()

/*------------------------------------------------------------------
  Purpose  : This function reads the 4 AD-Converters from the PCF8591
  Variables: p: pointer to a struct that will contain the 4 ADC values.
                Values are in mV. If a LM35 temperature sensor is connected,
                then this is also the temperature in 0.1 degrees Celsius.
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
int read_adc(adda_t *p)
{
   byte to_ad;  // array to write to ADC
   int  r1;     // Result from write()
   byte buffer[ADDA_BUF+1]; // buffer to read from AD/DA converter

   r1    = i2c_address(ADDA_BASE); // Put ADC address on I2C bus
   to_ad = ADDA_CONTROL_BYTE;      // After addressed, the ADDA expects a control byte
   r1   |= i2c_write(ADDA_BASE,&to_ad,1);    // send 1 byte to ADC

   //---------------------------------------------------------------
   // The next byte to write to the ADDA would be stored in the DAC,
   // but now we switch to read mode.
   //---------------------------------------------------------------
   r1 |= i2c_read(ADDA_BASE | RWb,buffer,ADDA_BUF); // read dummy byte+AD1+AD2+AD3+AD4

   p->ad1 = (double)(buffer[1]) * p->ad1c; // convert to °C
   p->ad2 = (double)(buffer[2]) * p->ad2c; // convert to °C
   return r1;
} // read_adc()

/*------------------------------------------------------------------
  Purpose  : Write one byte to the PCF8574 IO chip
  Variables: value: The byte to write
             LorH : LSB_IO: write to the LSB chip at DIG_IO_LSB_BASE
                    MSB_IO: write to the MSB chip at DIG_IO_MSB_BASE
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
int WriteIOByte(byte value, byte LorH)
{
   int  res;   // return result

   if (LorH == LSB_IO)
   {
      res = i2c_address(DIG_IO_LSB_BASE);
      res |= i2c_write(DIG_IO_LSB_BASE,&value,1);
   }
   else
   {
      res = i2c_address(DIG_IO_MSB_BASE);
      res |= i2c_write(DIG_IO_MSB_BASE,&value,1);
   }
   return res;
} // WriteIOByte()

/*------------------------------------------------------------------
  Purpose  : Write a maximum of 16 bytes to EEPROM FM24C08. The max.
             amount of EEPROM memory is 2048 bytes (8 bit). This is
             organised in 2K page blocks.
             NOTE: It is not possible to write over a 16 byte page, a
                   roll-over will occur
             With 1 8Kbit device, the max. is 1023
  Variables: addr: the eeprom address to write to [0..1023].
               *p: Pointer to array of (max. 16) bytes to write
               nr: the number of bytes to write
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
int eewrite(int addr, byte *p, byte nr)
{
   int  res = I2C_NOERR;   // return result
   byte x; // temp. byte

   if ((addr < 0) || (addr > 1023) || (nr < 1) || (nr > 16))
   {
      return I2C_ARGS;
   } // if

   // Bit 9 and 8 of addr contain the page block address
   // Shift them to bit pos. 2 and 1 and add this to the address
   x    = ((addr >> 7) & 0x06);
   x   |= FM24C08_BASE;
   res |= i2c_address(x);     // select proper page block in eeprom
   x    = (byte)(addr & 0x0ff);
   res |= i2c_write(FM24C08_BASE,&x,1); // write byte address
   res |= i2c_write(FM24C08_BASE,p,nr); // write data
   return res;
} // eewrite()

/*------------------------------------------------------------------
  Purpose  : Reads a maximum of 16 bytes from EEPROM FM24C08. The max.
             amount of EEPROM memory is 2048 bytes (8 bit). This is
             organised in 2K page blocks.
             NOTE: It is not possible to read over a 16 byte page, a
                   roll-over will occur
             With 1 8Kbit device, the max. is 1023
  Variables: addr: the eeprom address to read from [0..1023].
               *p: Pointer to array of (max. 16) bytes to store the
                   bytes read. The calling program should make sure
                   that sufficient bytes are allocated!
               nr: the number of bytes to read
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
int eeread(int addr, byte *p, byte nr)
{
   int  res = I2C_NOERR;   // return result
   byte x; // temp. byte
   int i;

   if ((addr < 0) || (addr > 1023) || (nr < 1) || (nr > 16))
   {
      return I2C_ARGS;
   } // if

   // Bit 9 and 8 of addr contain the page block address
   // Shift them to bit pos. 2 and 1 and add this to the address
   x    = ((addr >> 7) & 0x06);
   x   |= FM24C08_BASE;
   res |= i2c_address(x);     // select proper page block in eeprom
   x    = (byte)(addr & 0x0ff);
   res |= i2c_write(FM24C08_BASE,&x,1);      // write byte address
   res |= i2c_read(FM24C08_BASE | RWb,p,nr); // read data
   return res;
} // eeread()


