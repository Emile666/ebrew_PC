// ==================================================================
// Functienaam : -
// Filenaam    : i2c_dll.h
// Auteur      : E. van de Logt
// Datum       : 01-07-2002
// Versie      : V1.04
// ------------------------------------------------------------------
// Doel : External declaration for I2C DLL, which can be used by both
//        Visual C++ and Visual Basic. It is meant to directly access
//        the I2C Hardware.
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// 14-06-02 LGT    V1.00: First version for Ronald Rakke
// 01-07-02 LGT    V1.01: init_adc()/read_adc(): 4 channels working
//                        set_led(): visibility [1..7] parameter added
// 02-07-02 LGT    V1.02: - eewrite(): i2c_stop()+i2c_start() added to
//                                     initiate write to eeprom
//                        - eewrite(), eeread(): page xover (>16) check added
//			  - TO_VAL: 500 -> 50
// 14-09-02 LGT    V1.03  - VREF_INIT from 900 -> 930
//                        - DAC enabled (dac added to adda_t struct,
//                          ADDA_CONTROL_BYTE from 0x04 -> 0x44
//                        - PortTalk interface added (nt_in(), nt_out())
// 11-11-02 LGT    V1.04  - MAX6626 constants replaced by LM76 constants
//                        - lm76_read function prototype added
// ==================================================================
#ifndef _I2C_DLL_H
#define _I2C_DLL_H

// I2C2 Error Messages
//---------------------
#define I2C_NOERR       (0)
#define I2C_BERR       (-1)
#define I2C_BB         (-2)
#define I2C_TIMEOUT    (-3)
#define I2C_ARGS       (-4)
#define I2C_PT         (-5)
#define I2C_BERR_MSG   "I2C Bus-Error"
#define I2C_BB_MSG     "I2C Bus-Busy Error"
#define I2C_TO_MSG     "I2C Time-Out"
#define I2C_ARGS_MSG   "Wrong argument"
#define I2C_PT_MSG     "PortTalk Error"

// The LM92 sign bit is normally bit 12. The value read from the
// LM92 is SHL3. Therefore the sign bit is at bit 15
// Same for the Full Scale value, normally 2^12 SHL3 = 2^15.
#define LM92_SIGNb     (0x8000)
#define LM92_FS        (32768)
#define LM92_ERR       (99.99)

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE  (!FALSE)
#endif

#define LSB_IO (0)
#define MSB_IO (1)

#define VREF_INIT         (930.0)

//------------------------------
// Base addresses of I2C Devices
//
// LCD Display        : PCF 8574
// Digital IO         : PCF 8574
// LED1..LED4 Displays: SAA 1064
// AD/DA Converter    : PCF 8591 / LM92 / ADS7828
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
#define LM92_1_BASE      (0x92)
#define LM92_2_BASE      (0x94)
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
#define LM92_1_OK        (0x0100)
#define LM92_2_OK        (0x0200)
#define ADS7828_OK       (0x0400)
#define FM24C08_OK       (0x0800)
#define ALL_OK           (0x0FFF)

#define NOT_TXT          "NOT"
#define YES_TXT          "IS "
#define LCD_TXT          "LCD Display\tPCF 8574\t0x40 : %s Present\n"
#define DIG_IO_LSB_TXT   "Dig. IO LSB\tPCF 8574\t0x42 : %s Present\n"
#define DIG_IO_MSB_TXT   "Dig. IO MSB\tPCF 8574\t0x44 : %s Present\n"
#define LED1_TXT         "LED1 Display\tSAA 1064\t0x70 : %s Present\n"
#define LED2_TXT         "LED2 Display\tSAA 1064\t0x72 : %s Present\n"
#define LED3_TXT         "LED3 Display\tSAA 1064\t0x74 : %s Present\n"
#define LED4_TXT         "LED4 Display\tSAA 1064\t0x76 : %s Present\n"
#define ADDA_TXT         "AD/DA Converter\tPCF 8591\t0x90 : %s Present\n"
#define LM92_1_TXT       "Temp. Sensor 1\tLM92\t0x92 : %s Present\n"
#define LM92_2_TXT       "Temp. Sensor 2\tLM92\t0x94 : %s Present\n"
#define ADS7828_TXT      "12-bit 8 ch. ADC\tADS7828\t0x96 : %s Present\n"
#define FM24C08_TXT      "EEPROM Device\tFM24C08\t0xA0 : %s Present\n"

//------------------------------------------------------------------
// This struct is needed for the PCF8591 ADC/DAC. For every channel,
// there is a corresponding conversion factor and reference voltage.
//------------------------------------------------------------------
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
   int    dac;   // Value for DA Converter
} adda_t;

extern "C" __declspec(dllexport) int    __stdcall i2c_init(int address, byte win_ver);
extern "C" __declspec(dllexport) int    __stdcall i2c_start(void);
extern "C" __declspec(dllexport) int    __stdcall i2c_address(byte address);
extern "C" __declspec(dllexport) int    __stdcall i2c_write(byte address, byte *p, int bytes);
extern "C" __declspec(dllexport) int    __stdcall i2c_read(byte address, byte *p, int bytes);
extern "C" __declspec(dllexport) int    __stdcall i2c_stop(void);
extern "C" __declspec(dllexport) int    __stdcall set_led(int number, int dp, int which_led, int visibility);
extern "C" __declspec(dllexport) void   __stdcall check_i2c_hw(int *HW_present);
extern "C" __declspec(dllexport) void   __stdcall init_adc(adda_t *p);
extern "C" __declspec(dllexport) int    __stdcall read_ads7828(int channel, int *value);
extern "C" __declspec(dllexport) int    __stdcall read_adc(adda_t *p);
extern "C" __declspec(dllexport) int    __stdcall WriteIOByte(byte value, byte LorH);
extern "C" __declspec(dllexport) int    __stdcall eewrite(int addr, byte *p, byte nr);
extern "C" __declspec(dllexport) int    __stdcall eeread(int addr, byte *p, byte nr);
extern "C" __declspec(dllexport) double __stdcall lm92_read(byte dvc);
#endif