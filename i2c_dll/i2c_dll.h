// ======================================================================
// Filename : $Id$
// Author   : E. van de Logt
// Date     : $Date$
// Revision : $Revision$
// ----------------------------------------------------------------------
// Purpose : External declaration for I2C DLL, which can be used by both
//           Visual C++ and Visual Basic. It is meant to directly access
//           the I2C Hardware.
// ----------------------------------------------------------------------
// $Log$
// Revision 1.13  2005/10/23 12:45:18  Emile
// Several changes because of new hardware (MAX1238 instead of PCF8591):
// Changes to i2c_dll:
// - File reorganised into 4 layers with routines for more clarity
// - i2c_read/i2c_write: i2c_address() call added in VELLEMAN_CARD mode
// - i2c_address: i2c_start() call added in VELLEMAN_CARD mode
// - Routines added: get_analog_input() and max1238_read()
// - i2c_stop() changed into i2c_stop(enum pt_action pta) so that PortTalk
//   can be closed or remain open
// - init_adc() removed
//
// Revision 1.12  2005/08/28 22:17:41  Emile
// - DataGrapfForm: TTimer replaced again for TAnimTimer
// - Debug-code added for MA filter of Vmlt
// - 'H' key now toggles heater between 0% and 100%
// - Text Temp. Sensor 1 & 2 replaced by Temp. Sensor HLT & MLT
// - Printing of vmlt_unf (padc.ad4) removed again from log-file
//
// Revision 1.11  2005/03/26 13:42:01  Emile
// - Added functionality for Velleman card. i2c_init() is changed by this!!!
//
// Revision 1.10  2004/02/25 18:21:36  emile
// - Undo of previous revision. Porttalk was not the problem here.
//
// Revision 1.9  2004/02/25 13:09:10  emile
// - Bug-fix: only close PortTalk device when it is open. Introduced the
//            variable 'pt_opened' and the macro 'CLOSE_PORTTALK' for this.
//
// Revision 1.8  2004/02/25 09:40:18  emile
// - Several comments added and updated.
// - fscl_values[] + comments added to i2c_dll.cpp
//
// Revision 1.7  2004/02/22 10:03:46  emile
// - Added various CVS keywords to file-headers
// - ADS7828 code and defines replaced by LM92_3
// - Added clock_reg_val variable to i2c_init to allow for clock rate adjustments.
//   (this is not finished yet).
//
// Date    Author Version Description
// ----------------------------------------------------------------------
// 11-11-02 LGT    V1.04  - MAX6626 constants replaced by LM76 constants
//                        - lm76_read function prototype added
// 14-09-02 LGT    V1.03  - VREF_INIT from 900 -> 930
//                        - DAC enabled (dac added to adda_t struct,
//                          ADDA_CONTROL_BYTE from 0x04 -> 0x44
//                        - PortTalk interface added (nt_in(), nt_out())
// 02-07-02 LGT    V1.02: - eewrite(): i2c_stop()+i2c_start() added to
//                                     initiate write to eeprom
//                        - eewrite(), eeread(): page xover (>16) check added
//			  - TO_VAL: 500 -> 50
// 01-07-02 LGT    V1.01: init_adc()/read_adc(): 4 channels working
//                        set_led(): visibility [1..7] parameter added
// 14-06-02 LGT    V1.00: First version for Ronald Rakke
// ======================================================================
#ifndef _I2C_DLL_H
#define _I2C_DLL_H

typedef unsigned char byte;

//---------------------
// I2C2 Error Messages
//---------------------
#define I2C_NOERR       (0)
#define I2C_BERR       (-1)
#define I2C_BB         (-2)
#define I2C_TIMEOUT    (-3)
#define I2C_ARGS       (-4)
#define I2C_PT         (-5)
#define I2C_LM92_ERR   (-6)
#define I2C_BERR_MSG   "I2C Bus-Error"
#define I2C_BB_MSG     "I2C Bus-Busy Error"
#define I2C_TO_MSG     "I2C Time-Out"
#define I2C_ARGS_MSG   "Wrong argument"
#define I2C_PT_MSG     "PortTalk Error"
#define I2C_LM92_MSG   "LM92 Read Error"

//-----------------------------------------------------------------
// Defines for the VELLEMAN P8000 board. The I2C bus is directly
// connected to the parallel port pins in the following way:
// SCL             : /SELECT_IN: attached to Control register bit 3 (0x08)
// SDA to I2C bus  : /AUTO_FEED: attached to Control register bit 1 (0x02)
// SDA from I2C bus: SELECT    : attached to Status  register bit 4 (0x10)
// -----------------------------------------------------------------------
// SCL SDA Control_Register
//  0   0  14 = 0x0E
//  0   1  12 = 0x0C
//  1   0   6 = 0x06
//  1   1   4 = 0x04
//-----------------------------------------------------------------
#define I2C_SDA_IN    (0x10)
#define I2C_IDLE      (0x04)
#define I2C_SCL0_SDA0 (0x0E)
#define I2C_SCL0_SDA1 (0x0C)
#define I2C_SCL1_SDA0 (0x06)
#define I2C_SCL1_SDA1 I2C_IDLE

//---------------------------------------------------------------------------
// i2c_method: ISA PCB card of LPTx: parallel port PCB card or Velleman P8000
//---------------------------------------------------------------------------
#define WIN_9X        (0)
#define WIN_XP        (1)

#define RWb        (0x01)

#define ISA_CARD      (0)
#define LPT_CARD      (2)
#define VELLEMAN_CARD (4)
#define ALL_CARDS     (6)

// I2C slave HW responds with an ACK (0) or an NACK (1)
enum i2c_acks {I2C_ACK, I2C_NACK};

// Parameter value for i2c_stop to have PortTalk opened or to close it
enum pt_action {PT_OPEN, PT_CLOSE};

//-----------------------------------------------------------------
// The LM92 sign bit is normally bit 12. The value read from the
// LM92 is SHL3. Therefore the sign bit is at bit 15
// Same for the Full Scale value, normally 2^12 SHL3 = 2^15.
//-----------------------------------------------------------------
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

//---------------------------------------
// Base addresses of I2C Devices
//
// LCD Display        : PCF 8574
// Digital IO         : PCF 8574
// LED1..LED4 Displays: SAA 1064
// AD/DA Converter    : PCF 8591 / LM92
// EEPROM memory      : FM24C08
//---------------------------------------
#define LCD_BASE         (0x40)
#define DIG_IO_LSB_BASE  (0x42)
#define DIG_IO_MSB_BASE  (0x44)
#define MAX1238_BASE     (0x6A)
#define LED1_BASE        (0x70)
#define LED2_BASE        (0x72)
#define LED3_BASE        (0x74)
#define LED4_BASE        (0x76)
#define ADDA_BASE        (0x90)
#define LM92_1_BASE      (0x92)
#define LM92_2_BASE      (0x94)
#define LM92_3_BASE      (0x96)
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
#define LM92_3_OK        (0x0400)
#define FM24C08_OK       (0x0800)
#define MAX1238_OK       (0x1000)
#define ALL_OK           (0x1FFF)

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
#define LM92_1_TXT       "Temp. Sensor HLT\tLM92\t0x92 : %s Present\n"
#define LM92_2_TXT       "Temp. Sensor MLT\tLM92\t0x94 : %s Present\n"
#define LM92_3_TXT       "Temp. Sensor 3\tLM92\t0x96 : %s Present\n"
#define FM24C08_TXT      "EEPROM Device\tFM24C08\t0xA0 : %s Present\n"
#define MAX1238_TXT      "ADC, 12-bit 12 ch.\tMAX1238\t0x6A : %s Present\n"

//------------------------------------------------------------------
// This struct is needed for the PCF8591 ADC/DAC. For every channel,
// there is a corresponding conversion factor and reference voltage.
//------------------------------------------------------------------
typedef struct
{
   byte    ad1; // ADC1 value
   byte    ad2; // ADC2 value
   byte    ad3; // ADC3 value
   byte    ad4; // ADC4 value
   byte    dac; // Value for DA Converter
} adda_t;

//----------------------------------------------------------------------------
// List of all available AD-channels where analog values can be connected to
//----------------------------------------------------------------------------
enum i2c_adc {NONE, AIN0_PCF8591, AIN1_PCF8591, AIN2_PCF8591, AIN3_PCF8591,
                    AIN0_MAX1238, AIN1_MAX1238, AIN2_MAX1238, AIN3_MAX1238};

//----------------------------------------------------------------------------
// I2C_BUS_LAYER routines
//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int    __stdcall i2c_init(int address, byte control, byte clock_reg_val);
extern "C" __declspec(dllexport) int    __stdcall i2c_start(void);
extern "C" __declspec(dllexport) int    __stdcall i2c_address(byte address);
extern "C" __declspec(dllexport) int    __stdcall i2c_write(byte address, byte *p, int bytes);
extern "C" __declspec(dllexport) int    __stdcall i2c_read(byte address, byte *p, int bytes);
extern "C" __declspec(dllexport) int    __stdcall i2c_stop(enum pt_action);

//----------------------------------------------------------------------------
// I2C_DEVICE_LAYER routines
//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int    __stdcall set_led(int number, int dp, int which_led, int visibility);
extern "C" __declspec(dllexport) void   __stdcall check_i2c_hw(int *HW_present);
//extern "C" __declspec(dllexport) int    __stdcall read_ads7828(int channel, int *value);
extern "C" __declspec(dllexport) int    __stdcall read_adc(adda_t *p);
extern "C" __declspec(dllexport) int    __stdcall WriteIOByte(byte value, byte LorH);
extern "C" __declspec(dllexport) int    __stdcall eewrite(int addr, byte *p, byte nr);
extern "C" __declspec(dllexport) int    __stdcall eeread(int addr, byte *p, byte nr);
extern "C" __declspec(dllexport) double __stdcall lm92_read(byte dvc);
extern "C" __declspec(dllexport) int    __stdcall max1238_read(byte dvc);
extern "C" __declspec(dllexport) double __stdcall get_analog_input(int  hw_present,
                                                                   enum i2c_adc ad_channel,
                                                                   double a, double b, int *err);

i2c_acks i2c_output_bb(byte serdata);
byte     i2c_input_bb(bool last);
void     delay(int x);

#endif