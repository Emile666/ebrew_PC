// ==================================================================
// Functienaam : -
// Filenaam    : i2c.h
// Auteur      : E. van de Logt
// Datum       : 12-03-2000
// Versie      : V0.1
// ------------------------------------------------------------------
// Doel : Deze file bevat de basisroutines voor de I2C bus. Deze
//	  file is afgeleid van de Elektuur i2c.h file.
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// 13-03-02 LGT    - #I2C_H defines added
//                 - Function prototype for hw_err() added
// ==================================================================
#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c2.h"

// LCD Display: Data-Register (DR) or Instruction-Register (IR)
//-------------------------------------------------------------
#define DR (0)
#define IR (1)

#define LSB_IO (0)

#define STRLEN (50)

//------------------------------
// Base addresses of I2C Devices
//
// LCD Display        : PCF 8574
// Digital IO         : PCF 8574
// LED1..LED4 Displays: SAA 1064
// AD/DA Converter    : PCF 8591 / MAX6626 / ADS7828
// EEPROM memory      : FM24C08
//------------------------------
#define LCD_BASE         (0x40)
#define DIG_IO_LSB_BASE  (0x42)
#define DIG_IO_MSB_BASE  (0x44)
#define LED1_BASE        (0x70)
#define LED2_BASE        (0x72)
#define LED3_BASE        (0x74)
#define LED4_BASE        (0x76)
#define ADDA_BASE        (0x90)
#define MAX6626_1_BASE   (0x92)
#define MAX6626_2_BASE   (0x94)
#define ADS7828_BASE     (0x96)
#define FM24C08_BASE     (0xA0)

#define LCD_OK           (0x0001)
#define DIG_IO_LSB_OK    (0x0002)
#define DIG_IO_MSB_OK    (0x0004)
#define LED1_OK          (0x0008)
#define LED2_OK          (0x0010)
#define LED3_OK          (0x0020)
#define LED4_OK          (0x0040)
#define ADDA_OK          (0x0080)
#define MAX6626_1_OK     (0x0100)
#define MAX6626_2_OK     (0x0200)
#define ADS7828_OK       (0x0400)
#define FM24C08_OK       (0x0800)
#define ALL_OK           (0x0FFF)

#define NOT_TXT          "NOT"
#define YES_TXT          ""
#define LCD_TXT          "LCD Display\tPCF 8574, 0x40 : %s Present\n"
#define DIG_IO_LSB_TXT   "Dig. IO LSB\tPCF 8574, 0x42 : %s Present\n"
#define DIG_IO_MSB_TXT   "Dig. IO MSB\tPCF 8574, 0x44 : %s Present\n"
#define LED1_TXT         "LED1 Display\tSAA 1064, 0x70 : %s Present\n"
#define LED2_TXT         "LED2 Display\tSAA 1064, 0x72 : %s Present\n"
#define LED3_TXT         "LED3 Display\tSAA 1064, 0x74 : %s Present\n"
#define LED4_TXT         "LED4 Display\tSAA 1064, 0x76 : %s Present\n"
#define ADDA_TXT         "AD/DA Converter\tPCF 8591, 0x90 : %s Present\n"
#define MAX6626_1_TXT    "Temp. Sensor 1\tMAX 6626, 0x92 : %s Present\n"
#define MAX6626_2_TXT    "Temp. Sensor 2\tMAX 6626, 0x94 : %s Present\n"
#define ADS7828_TXT      "12-bit 8 ch. ADC\tADS7828, 0x96 : %s Present\n"
#define FM24C08_TXT      "EEPROM Device\tFM24C08, 0xA0 : %s Present\n"

//--------------------------------------------------
// Max. Waiting Time in milliseconds for LCD Display
// Max. #bytes on 1 line of the LCD Display
// Internal LCD address of 1st line to display
// Internal LCD address of 2nd line to display
//--------------------------------------------------
#define LCD_TIMEOUT  (10)
#define LCD_MAX_CHAR (16)
#define LCD_LINE1_ADDR (0x80)
#define LCD_LINE2_ADDR (0xC0)

//----------------------------
// !BL  = 1 (P7)  DB7 = 1 (P3)
// E    = 0 (P6)  DB6 = 1 (P2)
// D/!I = 1 (P5)  DB5 = 1 (P1)
// R/!W = 0 (P4)  DB4 = 1 (P0)
//----------------------------
#define LCD_INITVALUE (0xAF)
#define LCD_BUSY_FLAG (0x08)

//-------------------------------------------------------------------
// The LED instruction byte initializes the auto-increment pointer of
// the SAA1064. Bits b2..b0 determine to which register the data
// byte following the instruction byte will be written.
// After each transmission, the pointer is incremented automatically
// or wraps around from 7 to 0.
//
// b2 b1 b0  write data to...       (b7..b3 = 0)
//  0  0  0  control register
//  0  0  1  LD1 latch
//  0  1  0  LD2 latch
//  0  1  1  LD3 latch
//  1  0  0  LD4 latch
//  1  0  1  not used
//  1  1  0  not used
//  1  1  1  not used
//
// Set to 0x00 => select control register
//-------------------------------------------------------------------
#define LED_INSTR_BYTE (0x00)

//-------------------------------------------------------------------
// The LED control byte for the SAA1064
//
// Function of control bits when set to one:
//  b7 = reserved
//  b6 = adds 12 mA to segment output current
//  b5 = adds  6 mA to segment output current
//  b4 = adds  3 mA to segment output current
//  b3 = segment test (all outputs are switched on)
//  b2 = enable digits 2 + 4
//  b1 = enable digits 1 + 3
//  b0 = dynamic mode (multiplex digits).}
//
// Set to 0x37 => dynamic mode, current = 12 mA, all digits enabled
//-------------------------------------------------------------------
#define LED_CONTROL_BYTE (0x47)

//--------------------------------------------------------
// LED_DP   : Decimal point for LED Display
// LED_DUMMY: Dummy byte for LED Display
// LED_BUF  : Buffer size in bytes to write to LED Display
//--------------------------------------------------------
#define LED_DP    (0x80)
#define LED_DUMMY (0x00)
#define LED_BUF   (9)

//-------------------------------------------------------------------
// The ADDA control byte for the PCF 8591
//
// Function of control bits when set to one:
//  b7     = reserved (0!)
//  b6     = analog output enable flag
//  b5, b4 = input mode
//  b3     = reserved (0!)
//  b2     = auto-increment flag
//  b1, b0 = AD/DA channel selection
//
// Set to 0x04 => DAC disabled         , auto-increment on,
//                4 single-ended inputs, start at channel 0
//
// ADDA_BUF  : The size of the buffer that is read from the ADDA device
// ADDA_VREF : The reference voltage in mV
// ADDA_CONV : Constant used to compute temperature in degree Celsius
//-------------------------------------------------------------------
#define ADDA_CONTROL_BYTE (0x04)
#define ADDA_BUF          (0x05)
#define VREF_INIT         (900.0)


typedef struct
{
   double ad1; // The measured temperature (ADC1) in E-1 °C
   double ad2; // The measured temperature (ADC2) in E-1 °C
   double ad3; // The measured temperature (ADC3) in E-1 °C
   double ad4; // The measured temperature (ADC4) in E-1 °C
   double ad1c; // Conversion factor for AD1
   double ad2c; // Conversion factor for AD2
   double ad3c; // Conversion factor for AD3
   double ad4c; // Conversion factor for AD4
   int    vref1; // Reference voltage for AD1 in mV
   int    vref2; // Reference voltage for AD2 in mV
   int    vref3; // Reference voltage for AD3 in mV
   int    vref4; // Reference voltage for AD4 in mV
} adda_t;

//---------------------
// General I2C Routines
//---------------------
void check_i2c_hw(int *HW_present); // Check if all HW devices are present
//int         hw_err(int errval,int ax,int bp,int si); // custom error function

//---------------------
// LCD Display Routines
//---------------------
int  WriteCharLCD(byte Data);
int  ReadCharLCD(byte *Data);
int  WriteInstrLCD(byte Data);
byte GetAddrCntLCD(byte *addr_cntr);
int  InitLCD(void);
int  print_lcd(char *s);
int  read_lcd(void);

//---------------------
// LED Display Routines
//---------------------
int set_led(int number, int dp, int which_led);

//---------------------
// ADDA Routines
//---------------------
void init_adc(adda_t *p);
int read_adc(adda_t *p);
int read_ads7828(int channel, int *value);

//---------------------
// IO Routines
//---------------------
int WriteIOByte(byte value, byte LorH);

#ifdef __cplusplus
};
#endif
#endif
