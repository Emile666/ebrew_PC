// ============================================================================
// Filename : $Id$
// Author   : E. van de Logt
// Date     : $Date$
// Revision : $Revision$
// ----------------------------------------------------------------------------
// Purpose  : I2C DLL, which can be used by both Visual C++ and Visual
//            Basic. It is meant to directly access the I2C Hardware.
//
//            The I2C DLL routines are meant for a PCF8584 I2C bus
//            controller chip. Two variations are possible here:
//            1) The PCF8584 is mounted on a ISA printed circuit board
//            2) The PCF8584 is mounted on a PCB connected to the parallel
//               port.
//            This is controlled by setting the proper variable in the
//            i2c_init() routine.
//
//            It is also possible to use the I2C DLL on various Windows
//            platform. Two variations again:
//            1) Win 3.1, 95, 98, ME
//            2) Win 2000, NT, XP
//            Again this is controlled by setting the proper variable in
//            the i2c_init() routine.
//            For option 2, the PortTalk DLL from www.beyondlogic.org is
//            used to directly access the parallel port. You should install
//            PortTalk first before using the routines in this DLL.
//
//            The I2C DLL supports the following I2C devices:
//            - PCF 8584 I2C bus controller
//            - PCF 8571 I2C 8-bit IO chip
//            - PCF 8591 I2C 8-bit AD-DA converter
//            - LM92 I2C 12 + 1 (sign) bit temperature sensor
//            - LM76 I2C 10 + 1 (sign) bit temperature sensor (identical to LM92)
//            - FM24C08 2048 * 8 bit EEPROM (routines not tested yet!)
//
//           The DLL is built with Borland C++ Builder 4.0.
// ----------------------------------------------------------------------------
// $Log$
// Revision 1.13  2004/02/22 10:16:02  emile
// - i2c_start(): clock_reg_S2 is used instead of hard-coded 0x1A value.
//
// Revision 1.12  2004/02/22 10:03:46  emile
// - Added various CVS keywords to file-headers
// - ADS7828 code and defines replaced by LM92_3
// - Added clock_reg_val variable to i2c_init to allow for clock rate adjustments.
//   (this is not finished yet).
//
// Revision 1.11  2004/02/01 14:24:26  emile
// - Setting the SCL clock to 90 kHz resulted in frequent lock-ups of the I2C
//   bus. fscl is now set back again to 9.8 kHz (0x1A).
//
// Revision 1.10  2004/01/06 19:09:30  emile
// - Changes in speed to allow correct operation on faster PCs
//   - TO_VAL from 50 to 100
//   - SCL clock frequency from 11 kHz to 81 kHz
//
// Revision 1.9  2003/09/24 21:01:26  emile
// - lm92_read() function: no second read if first read already returns with an error.
// - Reset_I2C_Bus() function added. Called when lm92_read() returns with an error.
// - LED Display update: time-slice now every second (was every TS seconds)
// - hw_status is set to 0 if i2c_start() returns with an error
//
// Revision 1.8  2003/09/15 20:37:31  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.7  2003/07/11 21:04:15  emile
// - Comments now in proper order.
//
// Revision 1.6  2003/07/11 21:01:20  emile
// Bad-fix solved: do NOT temp_int >>= 7, since this looses the numbers behind
//                 the comma. leave to (double)temp_int / 128.0 instead.
//
// Revision 1.5  2003/07/11 18:34:53  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm92_read())
//
// Revision 1.4  2002/12/09 21:25:14  emile
// - TRUE / FALSE now conditionally defined.
// - lm92_read() could not handle negative temperatures. Fixed.
//
// Revision 1.3  2002/11/18 18:34:02  emile
// - MAX6626 constants replaced by LM76 constants.
// - lm76_read() function added.
//
// Revision 1.2  2002/09/18 17:05:35  emile
// Added cvs tags
//
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
// 14-06-02 LGT    V1.00: First version for Ronald Rakké
// ============================================================================
#include <stdio.h>
#include <windows.h>
#include "pt_ioctl.c"
#include "i2c_dll.h"

#pragma hdrstop
#include <condefs.h>

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <time.h>

//---------------------------------------------------------------------------
//USERES("i2c_dll.res");
//---------------------------------------------------------------------------
#pragma argsused

//---------------------------------------------------------
// i2c_method: ISA PCB card of LPTx: parallel port PCB card
//---------------------------------------------------------
#define ISA_CARD (0)
#define LPT_CARD (1)

//--------------------------------------------------------------------------
// Bit defines for LPTx: Control Register
// Bit 0: /WR signal for PCF8584, negative strobe, Centronix strobe
// Bit 1: /CS signal for PCF8584, negative strobe, Centronix Auto Feed
// Bit 2: Switches multiplexer; positive logic: Centronix Reset
// Bit 3: Switches register of PCF8584, negative logic, Centronix Select In
//--------------------------------------------------------------------------
#define WR (0x01)
#define CS (0x02)
#define SW (0x04)
#define A0 (0x08)

//-------------------------------------------------------------
// Control/Status Register S1 of the PCF8584 I2C Bus controller
//-------------------------------------------------------------
#define PINb (0x80)
#define STSb (0x20)
#define BERb (0x10)
#define LRBb (0x08)
#define AASb (0x04)
#define LABb (0x02)
#define  BBb (0x01)

#define  RWb (0x01)

//---------------------------------------------------------------------------
// The PCF8584 uses register S2 to set the proper SCL frequency. It contains
// bytes S4,S3,S2 (control CLK frequency) and S1, S0 (control fscl).
// This results in the following modes (assume CLK = 12 MHz):
//
// Div_by     Reg_S2     fscl(kHz)     Div_by     Reg_S2     fscl(kHz)
//   8192   11111 (0x1F)   1.46           512   10110 (0x16)  22.44
//   5120   11011 (0x1B)   2.34           384   10010 (0x12)  31.25
//   4096   10111 (0x17)   2.93           256   00010 (0x02)  46.88
//   3072   10011 (0x13)   3.91           160   11001 (0x19)  75.00
//   2048   00011 (0x03)   5.86           128   10101 (0x15)  93.75
//   1024   11110 (0x1E)  11.72            96   10001 (0x11) 125.00
//    640   11010 (0x1A)  18.75
//---------------------------------------------------------------------------
byte fscl_values[] = {0x1F, 0x1B, 0x17, 0x13, 0x03, 0x1E, 0x1A,
                      0x16, 0x12, 0x02, 0x19, 0x15, 0x11};

//---------------------------------------------------------
// Time-out value for wait_byte() routine, in pauze() ticks
//---------------------------------------------------------
#define TO_VAL (100)

typedef unsigned char byte;

#define STRLEN (50)

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

#define BL (0x80)
#define EN (0x40)
#define DI (0x20)
#define RW (0x10)

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
static byte led_ctrl_byte[] = {0x00,0x17,0x27,0x37,0x47,0x57,0x67,0x77};

//--------------------------------------------------------
// LED_DP   : Decimal point for LED Display
// LED_DUMMY: Dummy byte for LED Display
// LED_BUF  : Buffer size in bytes to write to LED Display
//--------------------------------------------------------
#define LED_DP    (0x80)
#define LED_DUMMY (0x00)
#define LED_BUF   (9)

//-------------------------------------------------------------------------
// Values needed to decode a decimal number into a byte for the LED Display
// b0..b6: segment a..g        a
// b7    : decimal point      f b
//                             g
//                            e c
//                             d .
//-------------------------------------------------------------------------
static int led_decode[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

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
// Set to 0x44 => DAC enabled         , auto-increment on,
//                4 single-ended inputs, start at channel 0
//
// ADDA_BUF  : The size of the buffer that is read from the ADDA device
// ADDA_VREF : The reference voltage in mV
// ADDA_CONV : Constant used to compute temperature in degree Celsius
//-------------------------------------------------------------------
#define ADDA_CONTROL_BYTE (0x44)
#define ADDA_BUF          (0x05)


//---------------------------
// Internal i2c_dll variables
//---------------------------
static byte last_start;   // TRUE = Last action on bus was start
static byte ic_adr;       // Currently addressed IC and R/W_ bit

static byte win_nt;       // TRUE = Windows NT, 2000, XP; FALSE = Win 95,98,ME
static byte i2c_method;   // [ISA_CARD, LPT_CARD]
static byte clock_reg_S2; // Init. value for S2 clock register of PCF8584
static int  S023;         // Registers S0, S2 and S3 of PCF8584
static int  S1;           // Register S1 of PCF8584 (= S023 + 1)
static int  lpt_data;     // Data register of LPTx: port
static int  lpt_ctrl;     // Control register of LPTx: port
static int  lpt_stat;     // Status register of LPTx: port
static byte lpt_ctrl_old; // Previous value of Control register of LPTx: port
static byte dataH;        // High nibble read from multiplexer
static byte dataL;        // Low nibble read from multiplexer
char   *i2c_dll_revision = "$Revision$";

//----------------------------------------------------------------------------
// Start of i2c_dll routines
//----------------------------------------------------------------------------

unsigned char inportb(unsigned short port)
/*------------------------------------------------------------------
  Purpose  : Implements the DOS Borland C function inportb(). This
             function was absent in Borland C++ Builder
  Variables: port: the port address to read a byte from
  Returns  : The byte read from the specified IO port
  ------------------------------------------------------------------*/
{
   if (win_nt)
   {
      return nt_in(port); // Call the PortTalk input routine
   }
   else
   {
      asm     mov     dx,port
      asm     in      al,dx
      asm     xor     ah,ah
      return _AX;
   }
} // inportb()

void outportb(unsigned short port, unsigned char val)
/*------------------------------------------------------------------
  Purpose  : Implements the DOS Borland C function outportb(). This
             function was absent in Borland C++ Builder
  Variables: port: the port address to write a byte to
             val : the value to write to that port
  Returns  : None
  ------------------------------------------------------------------*/
{
   if (win_nt)
   {
      nt_out(port, val);  // Call the PortTalk output routine
   }
   else
   {
      asm     mov     dx, port
      asm     mov     al, val
      asm     out     dx, al
   }
} // outportb()

void write_S023(unsigned char val)
/*------------------------------------------------------------------
  Purpose  : Writes to the S0, S2 or S3 register of the PCF8584.
             Checks for the proper i2c_method (set in i2c_init())
  Variables: val : the value to write to the PCF8584 register
  Returns  : None
  ------------------------------------------------------------------*/
{
   if (i2c_method == ISA_CARD)
   {
      outportb(S023,val);
   }
   else
   {
      outportb(lpt_data, val);
      outportb(lpt_ctrl, A0 | WR); // Put data into PCF8584
      outportb(lpt_ctrl, A0 | WR | CS);
      outportb(lpt_ctrl, 0x00);
   } // else
} // write_S023()

void write_S1(unsigned char val)
/*------------------------------------------------------------------
  Purpose  : Writes to the S1 register of the PCF8584.
             Checks for the proper i2c_method (set in i2c_init())
  Variables: val : the value to write to the PCF8584 register
  Returns  : None
  ------------------------------------------------------------------*/
{
   if (i2c_method == ISA_CARD)
   {
      outportb(S1,val);
   }
   else
   {
      outportb(lpt_data, val);
      outportb(lpt_ctrl, WR); // Put data into PCF8584
      outportb(lpt_ctrl, WR | CS);
      outportb(lpt_ctrl, 0x00);
   } // else
} // write_S1()

void read_mpx(void)
/*------------------------------------------------------------------
  Purpose  : Reads the 4-bit multiplexer on the parallel port print
             The nibbles are returned in:
              dataH: /B7 - B6 B5 B4 - - -
              dataL: /B3 - B2 B1 B0 - - -
  Variables: None , dataH and dataL are updated
  Returns  : None
  ------------------------------------------------------------------*/
{
   outportb(lpt_ctrl,lpt_ctrl_old & ~SW); // /SW for lower nibble
   dataL = inportb(lpt_stat); // read lower nibble
   outportb(lpt_ctrl,lpt_ctrl_old | SW); // SW for lower nibble
   dataH = inportb(lpt_stat); // read upper nibble
} // read_mpx()

unsigned char convert_mpx(void)
/*------------------------------------------------------------------
  Purpose  : Convert the two nibbles read from the PCF8584 into one byte.
             The nibbles are:
              dataH: /B7 - B6 B5 B4 - - -
              dataL: /B3 - B2 B1 B0 - - -
  Variables: None
  Returns  : The byte read from the PCF8584 = B7 B6 B5 B4 B3 B2 B1 B0
  ------------------------------------------------------------------*/
{
   unsigned char Dummy1 = dataL;
   unsigned char Dummy2 = dataH;

   asm
   {
      push    ax
      push    bx
      push    cx

      mov     al,Dummy1
      and     al,80h
      xor     al,80h       // invert B3
      mov     cl,4
      shr     al,cl
      mov     bl,al        // B3 now at correct position in bl
      mov     al,Dummy1
      mov     cl,3
      shr     al,cl
      and     al,07h       // Isolate B2 B1 and B0
      or      al,bl        // Add B3. al now contains B3 B2 B1 B0
      mov     ah,Dummy2
      and     ah,80h
      xor     ah,80h       // Invert B7
      mov     bh,ah        // Save in bh
      mov     ah,Dummy2
      shl     ah,1         // B6 B5 B4 now in correct position
      and     ah,70h       // Isolate B6 B5 and B4
      or      ah,bh        // Add B7
      or      al,ah        // Add B7 .. B4 to B3 .. B0
      mov     Dummy1,al    // This is the desired value

      pop     cx
      pop     bx
      pop     ax
   }
   return Dummy1;
} // convert_mpx()

unsigned char read_S023(void)
/*------------------------------------------------------------------
  Purpose  : Reads from the S0, S2 or S3 register of the PCF8584.
             Checks for the proper i2c_method (set in i2c_init())
  Variables: None
  Returns  : The byte read from the PCF8584 register
  ------------------------------------------------------------------*/
{
   if (i2c_method == ISA_CARD)
   {
      return inportb(S023);
   }
   else
   {
      outportb(lpt_ctrl, A0);
      lpt_ctrl_old = A0 | CS; // save value of lpt_ctrl
      outportb(lpt_ctrl, lpt_ctrl_old);
      read_mpx();            // updates dataH and dataL
      outportb(lpt_ctrl,0x00);
      return convert_mpx();  // returns decoded byte
   } // else
} // read_S023()

unsigned char read_S1(void)
/*------------------------------------------------------------------
  Purpose  : Reads from the S1 register of the PCF8584.
             Checks for the proper i2c_method (set in i2c_init())
  Variables: None
  Returns  : The byte read from the PCF8584 register
  ------------------------------------------------------------------*/
{
   if (i2c_method == ISA_CARD)
   {
      return inportb(S1);
   }
   else
   {
      outportb(lpt_ctrl,0x00);
      lpt_ctrl_old = CS;     // save value of lpt_ctrl
      outportb(lpt_ctrl,lpt_ctrl_old);
      read_mpx();            // updates dataH and dataL
      outportb(lpt_ctrl,0x00);
      return convert_mpx();  // returns decoded byte
   } // else
} // read_S1()

void pauze(void)
/*------------------------------------------------------------------
  Purpose  : Create a small delay, approx. 18 usec.
  Variables: None
  Returns  : None
  ------------------------------------------------------------------*/
{
   int i;    // loop variable
   double j; // temp. variable

   for (i = 0; i < 512; i++)
   {
      j = i + 1; // do something
   } // for i
} // pauze()

void delay(int x)
/*------------------------------------------------------------------
  Purpose  : Create a msec. delay. Implementation of the DOS Borland C
             routine, which was absent in Borland C++ Builder
  Variables: x: the number of millisecond
  Returns  : None
  ------------------------------------------------------------------*/
{
   clock_t start;

   start = clock(); // return clock-tick in msec.
   while (clock() < start + x)
   {
      pauze(); // small delay
   } // while
} // delay()

int i2c_berr_check(void)
/*------------------------------------------------------------------
  Purpose  : Checks for a Bus Error on the I2C bus (test the BERR bit).
  Variables: None
  Returns  : Error: I2C_NOERR : No error
                    I2C_BERR  : I2C bus error occurred
  ------------------------------------------------------------------*/
{
   byte x; // temp. variable

   x = read_S1();
   pauze();
   if ((x & BERb) == BERb)
   {
      return I2C_BERR;
   }
   else
   {
      return I2C_NOERR;
   }
} // i2c_berr_check()

extern "C" __declspec(dllexport) int __stdcall i2c_init(int  address,
                                                        byte win_ver,
                                                        byte clock_reg_val)
/*------------------------------------------------------------------
  Purpose  : Sets the proper base address for the PCF8584. This should
             be the first routine to call. The I2C bus can be interfaced
             in two ways:
             1) With an ISA PCB card: valid IO addresses are 0x300, 0x310,
                0x320, 0x330 or 0x340.
             2) With a I2C interface card connected to a parallel port.
                Base addresses are 0x378 (LPT1) and 0x278 (LPT2:)
  Variables: address      : the IO base address for the PCF8584
             win_ver      : TRUE = Windows NT, 2000 or XP platform
             clock_reg_val: value for the clock register S2 of the PCF8584.
                            Should be one of the values from fscl_values[].
                            This byte controls the SCL frequency:
                             clock prescaler             SCL frequency
                            S24 S23 S22 Ratio            S21 S20 Ratio
                             0   X   X   /2               0   0    /16
                             1   0   0   /3               0   1    /32
                             1   0   1   /4               1   0   /128
                             1   1   0   /5               1   1  /1024
                             1   1   1   /8
  Returns  : Error: I2C_NOERR : No error
                    I2C_ARGS  : Wrong value for address
                    I2C_PT    : PortTalk error (NT, 2000, XP only)
             The static byte clock_reg_S2 is set
  ------------------------------------------------------------------*/
{
   int err  = I2C_NOERR; // error return-value

   win_nt = win_ver;
   if (win_nt && (OpenPortTalk() == -1))
   {
      return I2C_PT;
   }
   switch (address)
   {
      case 0x300: case 0x310:
      case 0x320: case 0x330:
      case 0x340:
           i2c_method = ISA_CARD; // ISA PCB card selected
           S023       = address;  // base address for PCF8584
           S1         = S023 + 1;
           break;
      case 0x378: case 0x278:
           i2c_method = LPT_CARD;    // LPTx: card selected
           lpt_data   = address;     // base address for LPTx: card
           lpt_stat   = address + 1; // status register
           lpt_ctrl   = address + 2; // control register
           outportb(lpt_ctrl,0x00);  // Init. /SW RD WR A0
           break;
      default:
           err = I2C_ARGS; // wrong input address
           break;
   } // switch
   clock_reg_S2 = clock_reg_val; // save S2 clock register value for i2c_start()
   return err; // return error-code
} // i2c_init()

extern "C" __declspec(dllexport) int __stdcall i2c_start(void)
/*------------------------------------------------------------------
  Purpose  : Initialises the I2C bus.
  Variables: None
             The static byte clock_reg_S2 is used
  Returns  : Error: I2C_NOERR : No error
                    I2C_BB    : I2C bus is still busy
                    I2C_BERR  : I2C bus error occurred
  ------------------------------------------------------------------*/
{
   int err = I2C_NOERR; // error return-value
   byte x; // temp. variable

   write_S1(0x00);   // Disable Serial Interface, next. reg. = SO' (own address)
   pauze();
   write_S023(0xFF); // Set effective own address S0' to a non-zero value
   pauze();
   write_S1(0x20);   // Load next byte into clock-register S2
   pauze();
   //--------------------------------------------------------------------
   // The var. clock_reg_S2 (set in i2c_init) contains the value for the
   // clock register S2 of the PCF8584. It controls the SCL frequency.
   //--------------------------------------------------------------------
   write_S023(clock_reg_S2);
   pauze();
   write_S1(0x40); // Enable Serial Interface, load next byte into S0 (Data)
   pauze();
   x = read_S1();  // Check if I2C bus is busy (Bus-Busy bit is 1)
   pauze();
   if ((x & BBb) == 0x00)
   {
      err = I2C_BB; // return error is I2C bus busy
   }
   else
   {
      write_S023(0x00); // Load S0 with address & R/W_ bit
      ic_adr = 0x00;    // save address & R/W_ bit
      pauze();
      // Load next byte in S0 (data), generate START + address
      // remain in MST/TRM (Master/Transmitter) mode
      write_S1(0x44);
      last_start = TRUE; // set flag: last action was START
      pauze();
      err = i2c_berr_check(); // check for bus-error
   } // else
   return err; // return-value
} // i2c_start()

int wait_byte(void)
/*------------------------------------------------------------------
  Purpose  : This function waits until a complete byte has been
             send or received. Time-out value is a multiple of
             the delay in pauze() and is approx. 0.5 second.
  Variables: None
      bytes: Number of bytes to write
  Returns  : Error: I2C_NOERR  : No error
                    I2C_TIMEOUT: I2C bus time-out
  ------------------------------------------------------------------*/
{
   int i = 0; // timer value

   while (((read_S1() & PINb) == PINb) && (i++ < TO_VAL))
   {
      pauze(); // loop until PIN bit is 0
   } // while
   pauze();
   if (i >= TO_VAL)
   {
      return I2C_TIMEOUT;
   }
   else
   {
      return I2C_NOERR;
   }
} // wait_byte()

int terminate_read(void)
/*------------------------------------------------------------------
  Purpose  : If an IC is waiting for a negative acknowledgement before
             it releases the SDA, we first turn off the ACK bit in the
             control register of the PCF8584. Then we read the last
             received byte, so the next received byte will not be
             acknowledged, and the slave transmitter stops transmitting
             and releases SDA. Finally we have to wait for reception of
             the last byte. Notice that the last two bytes read are LOST!
  Variables: None
  Returns  : Error: I2C_NOERR  : No error
                    I2C_TIMEOUT: I2C bus time-out
  ------------------------------------------------------------------*/
{
   write_S1(0x40);  // set ACK to negative acknowledgement
   pauze();
   read_S023();      // get one-before-last byte: goes LOST!
   pauze();
   return wait_byte(); // wait for last byte: goes LOST! Return result.
} // terminate_read()

extern "C" __declspec(dllexport) int __stdcall i2c_address(byte address)
/*------------------------------------------------------------------
  Purpose  : Send a 'REPEAT START' condition, followed by the address.
             Call this routine every time prior to addressing an I2C chip.
  Variables:
    address: the address to write to on the I2C bus
  Returns  : Error: I2C_NOERR   : No error
                    I2C_BB      : I2C bus is still busy
                    I2C_TIMEOUT : I2C bus time-out
  ------------------------------------------------------------------*/
{
   int err;         // error return-value
   byte x; // temp. variable

   err = wait_byte(); // wait for on-going actions
   if (err)
   {
      return err; // return in case of time-out
   } // if
   if ((ic_adr & RWb) == RWb) // Is slave still transmitting?
   {
      err = terminate_read(); // if yes, terminate transmission
      if (err)
      {
         return err; // return in case of time-out
      } // if
   } // if
   if (last_start)
   {
      // check if last action was START
      write_S1(0x47);   // load STOP/START condition
   }
   else
   {
      write_S1(0x45);   // load REPEAT START condition
   } // else
   pauze();

   last_start = TRUE;   // set flag: last action was START
   write_S023(address); // send REPEAT START & address & R/W_ bit
   ic_adr = address;    // save address and R/W_ bit
   pauze();
   err = wait_byte();   // wait till REPEAT START is send
   if (err)
   {
      return err;   // return in case of time-out
   } // if

   x = read_S1();  // test Last Received Bit (= ACK_)
   pauze();
   if ((x & LRBb) == LRBb)
   {
      return I2C_BB;  // Error: I2C bus busy
   }
   else if ((ic_adr & RWb) == RWb)
   {
      read_S023();  // action = READ
      pauze();      // set PIN bit to allow PCD8584 to receive data (dummy read)
   } // else if
   return I2C_NOERR;  // Return, no error
} // i2c_address()

extern "C" __declspec(dllexport) int __stdcall i2c_write(byte address, byte *p, int bytes)
/*------------------------------------------------------------------
  Purpose  : This function sends a number of bytes over the I2C bus
             The address on the I2C bus was set by a call to the
             function i2c_address(), prior to calling this function.
  Variables:
    address: The I2C address of the chip to write to
          p: Pointer to an array containing the bytes to write
      bytes: The number of bytes to write
  Returns  : Error: I2C_NOERR : No error
                    I2C_BB    : I2C bus is still busy
  ------------------------------------------------------------------*/
{
   int b = 0;           // counter for #bytes to write
   int err = I2C_NOERR; // error return-value
   int full;            // TRUE = IC accepts no more bytes

   // If the IC expects a read, we must first tell the addressed IC
   // that we want to write.
   err = wait_byte();
   if (err != I2C_NOERR)
   {
      return err; // exit if error
   } // if

   if ((ic_adr & RWb) == RWb)
   {
      ic_adr &= ~RWb;            // clear R/W_ bit
      err = i2c_address(address); // prepare bus for writing
   } // if

   full = FALSE;
   while ((b < bytes) && !full && (err == I2C_NOERR))
   {
      last_start = FALSE;
      write_S023(p[b++]); // send byte
      pauze();
      err  = wait_byte();    // wait until byte is sent
      full = ((read_S1() & LRBb) == LRBb); // test Last Received Bit (= ACK_)
      pauze();
   } // while
   return err; // return error code
} // i2c_write()

extern "C" __declspec(dllexport) int __stdcall i2c_read(byte address, byte *p, int bytes)
/*------------------------------------------------------------------
  Purpose  : This function reads a number of bytes from the I2C bus,
             The address on the I2C bus was set by a call to the
             function i2c_address(), prior to calling this function.
  Variables:
    address: The I2C address of the chip to read from
          p: Pointer to an array where the bytes read from the I2C
             bus can be stored. The calling program must allocate
             sufficient space!
      bytes: The number of bytes to read
  Returns  : Error: I2C_NOERR   : No error
                    I2C_BB      : I2C bus is still busy
                    I2C_TIMEOUT : I2C bus time-out error
  ------------------------------------------------------------------*/
{
   int b   = 0;         // counter for #bytes to read
   int err = I2C_NOERR; // error return-value

   // If the IC expects a write, we must first tell the addressed IC
   // that we want to read.
   err = wait_byte();
   if (err != I2C_NOERR)
   {
      return err; // exit if error
   } // if

   if ((ic_adr & RWb) == 0x00)
   {
      ic_adr |= RWb; // set R/W_ bit
      err = i2c_address(address); // prepare bus for reading
   } // if

   while ((b < bytes) && (err == I2C_NOERR))
   {
      last_start = FALSE;       // flag: last action was NOT start
      err        = wait_byte(); // wait on reception of next byte
      p[b++]     = read_S023(); // read byte
      pauze();
   } // while
   p[b] = '\0'; // close array properly
   return err; // return error-code
} // i2c_read()

/*------------------------------------------------------------------
  Purpose  : Closes the I2C bus.
  Variables: None
  Returns  : Error: I2C_NOERR : No error
                    I2C_BB    : I2C bus is still busy
                    I2C_BERR  : I2C bus error occurred
  ------------------------------------------------------------------*/
extern "C" __declspec(dllexport) int __stdcall i2c_stop(void)
{
   int err = I2C_NOERR; // error return-value

   err = wait_byte();   // wait for on-going actions
   if (err != I2C_NOERR)
   {
      if (win_nt) ClosePortTalk();
      return err; // exit if error
   } // if
   if ((ic_adr & RWb) == RWb) // Is slave still transmitting?
   {
      err = terminate_read(); // If so, terminate transmission
      if (err != I2C_NOERR)
      {
         if (win_nt) ClosePortTalk();
         return err; // exit if error
      } // if
   } // if
   write_S1(0x42);         // set STOP condition through S1 register
   pauze();
   write_S023(0x42);       // generate STOP condition
   pauze();
   err = i2c_berr_check(); // check for I2C bus-error
   write_S1(0x00);         // turn serial interface OFF
   pauze();
   if (win_nt) ClosePortTalk();
   return err;             // return error-code
} // i2c_stop()

extern "C" __declspec(dllexport) int __stdcall set_led(int number, int dp, int which_led, int visibility)
/*------------------------------------------------------------------
  Purpose  : This function sets a LED Display to a 4 digit number
  Variables: number   : a decimal number between 0 and 9999 (-1 = blank)
             dp       : [0..4], indicating which decimal point to set, 0=no dp
             which_led: [1..4], indicating which LED Display to address
             visibility: [1..7], visibility of LED display
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
{
   int   r1 = FALSE;       // return-value
   int   rem;              // temp. variable, remaining number
   byte  e1;               // duizentallen byte
   byte  e2;               // honderdtallen byte
   byte  e3;               // tientallen byte
   byte  e4;               // eenheden byte
   byte  buffer[LED_BUF];  // buffer to sent to LED display
   int   bidx = 0;         // Index into buffer[]

   buffer[bidx++] = LED_INSTR_BYTE;   // Send Intruction byte
   buffer[bidx++] = led_ctrl_byte[visibility]; // Init. LED control register

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
      e1             = number / 1000;
      rem            = number - e1 * 1000;  // remainder
      e2             = rem / 100;
      rem           -= e2 * 100;
      e3             = rem / 10;
      rem           -= e3 * 10;
      e4             = rem;
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

extern "C" __declspec(dllexport) void __stdcall check_i2c_hw(int *HW_present)
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
   if (i2c_address(LM92_1_BASE) == I2C_NOERR)
   {
      *HW_present |= LM92_1_OK;
   }
   if (i2c_address(LM92_2_BASE) == I2C_NOERR)
   {
      *HW_present |= LM92_2_OK;
   }
   if (i2c_address(LM92_3_BASE) == I2C_NOERR)
   {
      *HW_present |= LM92_3_OK;
   }
   if (i2c_address(FM24C08_BASE) == I2C_NOERR)
   {
      *HW_present |= FM24C08_OK;
   }
} // check_i2c_hw()

extern "C" __declspec(dllexport) void __stdcall init_adc(adda_t *p)
/*------------------------------------------------------------------
  Purpose  : This function calculates the conversion constant for the
             four AD Converters. Input is Vrefx (x = 1..4), output is
             adxc (x=1..4).
             Note that the ref. voltage is approx. 1.848 Volt for all ADCs.
             The value of every Vrefx is always as [E-1 unity], with unity
             being Litres or °C.

             AD1..AD3: Every LM35 output (10 mV/°C) is amplified by 2 approx.
                       So the effective Vref would be close to 90 °C, but
                       should be calibrated for every AD channel.
             AD4     : Pressure transducer,
  Variables: p: pointer to a struct that will contain the values.
  Returns  : None
  ------------------------------------------------------------------*/
{
   // 8 bit AD = 256 steps; 10.0 -> E-1
   p->ad1c = (double)(p->vref1) / 2560.0;
   p->ad2c = (double)(p->vref2) / 2560.0;
   p->ad3c = (double)(p->vref3) / 2560.0;
   p->ad4c = (double)(p->vref4) / 2560.0;
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
//extern "C" __declspec(dllexport) int __stdcall read_ads7828(int channel, int *value)
//{
   // Command Byte Bits 7 - 4
   // 1111 = Ch.7, 1110 = Ch.5, 1101 = Ch.3, 1100 = Ch.1
   // 1011 = Ch.6, 1010 = Ch.4, 1001 = Ch.2, 1000 = Ch.0
   // Bits 3 - 2: Power-down selection: 11 = Int. Vref on & ADC on
//   byte cmd[8] = {0x8c,0xcc,0x9c,0xdc,0xac,0xec,0xbc,0xfc};

//   byte to_ad;   // array to write to ADC
//   byte from_ad; // array to write to ADC
//   int  r1;      // Result from i2c_write() and i2c_read

//   if ((channel > 7) || (channel < 0))
//   {
//      return I2C_ARGS;
//   } // if
//   r1    = i2c_address(ADS7828_BASE); // Put ADC address on I2C bus
//   to_ad = cmd[channel];
//   r1   |= i2c_write(ADS7828_BASE,&to_ad,1);    // send 1 byte to ADC

   //---------------------------------------------------------------
   // Now we switch to read mode and read 2 bytes from the ADS7828
   //---------------------------------------------------------------
//   r1 |= i2c_read(ADS7828_BASE | RWb,&from_ad,1); // read MSB
//   *value = (from_ad << 8);
//   r1 |= i2c_read(ADS7828_BASE | RWb,&from_ad,1); // read LSB
//   *value |= (from_ad & 0x0ff);
//   return r1;
//} // read_ads7828()

/*------------------------------------------------------------------
  Purpose  : This function reads the 4 AD-Converters from the PCF8591
  Variables: p: pointer to a struct that will contain the 4 ADC values.
                Values are in mV. If a LM35 temperature sensor is connected,
                then this is also the temperature in 0.1 degrees Celsius.
  Returns  : TRUE : something went wrong
             FALSE: everything is OK
  ------------------------------------------------------------------*/
extern "C" __declspec(dllexport) int __stdcall read_adc(adda_t *p)
{
   byte to_ad[2];  // array to write to ADC
   int  r1;        // Result from write()
   byte buffer[ADDA_BUF+1]; // buffer to read from AD/DA converter

   r1       = i2c_address(ADDA_BASE); // Put ADC address on I2C bus
   to_ad[0] = ADDA_CONTROL_BYTE;      // Next byte should be a control byte
   to_ad[1] = (byte)(p->dac);         // Value for DA Converter
   r1   |= i2c_write(ADDA_BASE,to_ad,2); // send control byte + DAC value

   //-----------------------------
   // Now we switch to read mode.
   //-----------------------------
   r1 |= i2c_read(ADDA_BASE | RWb,buffer,ADDA_BUF); // read dummy byte+AD1+AD2+AD3+AD4

   p->ad1 = (double)(buffer[1]) * p->ad1c; // convert to °C
   p->ad2 = (double)(buffer[2]) * p->ad2c; // convert to °C
   p->ad3 = (double)(buffer[3]) * p->ad3c; // convert to °C
   p->ad4 = (double)(buffer[4]) * p->ad4c; // convert to °C
   return r1;
} // read_adc()

/*------------------------------------------------------------------
  Purpose  : Write one byte to the PCF8574 IO chip
  Variables: value: The byte to write
             LorH : LSB_IO: write to the LSB chip at DIG_IO_LSB_BASE
                    MSB_IO: write to the MSB chip at DIG_IO_MSB_BASE
  Returns  : I2Cerror
  ------------------------------------------------------------------*/
extern "C" __declspec(dllexport) int __stdcall WriteIOByte(byte value, byte LorH)
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
extern "C" __declspec(dllexport) int __stdcall eewrite(int addr, byte *p, byte nr)
{
   int  res = I2C_NOERR;   // return result
   byte x; // temp. byte

   if ((addr < 0) || (addr > 1023) || (nr < 1) || ((addr & 0x0f) + nr > 16))
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
   res |= i2c_stop(); // Gen. stop condition = start write to EEPROM
   res |= i2c_start(); // Gen. start condition again
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
extern "C" __declspec(dllexport) int __stdcall eeread(int addr, byte *p, byte nr)
{
   int  res = I2C_NOERR;   // return result
   byte x; // temp. byte

   if ((addr < 0) || (addr > 1023) || (nr < 1) || ((addr & 0x0f) + nr > 16))
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

extern "C" __declspec(dllexport) double __stdcall lm92_read(byte dvc)
/*------------------------------------------------------------------
  Purpose  : This function reads the LM92 13-bit Temp. Sensor and
             returns the temperature.
             Reading Register 0 of the LM92 results in the following bits:
              15   14  13 12      3   2    1   0
             Sign MSB B10 B9 ... B0 Crit High Low
  Variables:
       dvc : 0 = Read from the LM92 at 0x92/0x93 (LM92_1)
             1 = Read from the LM92 at 0x94/0x95 (LM92_2)
             2 = Read from the LM92 at 0x96/0x97 (LM92_3)
  Returns  : The temperature from the LM92
  ------------------------------------------------------------------*/
{
   int    res = I2C_NOERR;   // return result
   byte   buffer[2];         // array to store data from i2c_read()
   int    temp_int;          // the temp. from the LM92 as an integer
   int    sign;              // sign of temperature
   double temp;              // the temp. from the LM92 as a double

   if (dvc == 0)
   {
      res  = i2c_address(LM92_1_BASE);
      if (res == I2C_NOERR)
      {
         res = i2c_read(LM92_1_BASE | RWb,buffer,2); // read 2 bytes from LM92 register 0
      }
   }
   else if (dvc == 1)
   {
      res  = i2c_address(LM92_2_BASE);
      if (res == I2C_NOERR)
      {
         res = i2c_read(LM92_2_BASE | RWb,buffer,2); // read 2 bytes from LM92 register 0
      }
   } // else
   else
   {
      res  = i2c_address(LM92_3_BASE);
      if (res == I2C_NOERR)
      {
         res = i2c_read(LM92_3_BASE | RWb,buffer,2); // read 2 bytes from LM92 register 0
      }
   } // else

   if (res != I2C_NOERR)
   {
      temp = LM92_ERR; // return error number instead of real number
   }
   else
   {
      temp_int = buffer[0];      // store {Sign, MSB, bit 10..5} at bits temp_int bits 7..0
      temp_int <<= 8;            // SHL 8, Sign now at bit 15
      temp_int &= 0xff00;        // Clear bits 7..0
      temp_int |= buffer[1];     // Add bits D4..D0 to temp_int bits 7..3
      temp_int &= 0xFFF8;        // Clear Crit High & Low bits
      sign = ((temp_int & LM92_SIGNb) == LM92_SIGNb);
      if (sign)
      {
         temp_int &= ~LM92_SIGNb;        // Clear sign bit
         temp_int  = LM92_FS - temp_int; // Convert two complement number
      } // if
      temp = (double)temp_int / 128.0;   // SHR 3 + multiply with 0.0625 = 1/16
      if (sign)
      {
         temp = -temp; // negate number
      } // if
   } // else
   return temp;     // Return value now in °C
} // lm92_read()

//---------------------------------------------------------------------------
//   Important note about DLL memory management in a VCL DLL:
//
//
//
// If your DLL uses VCL and exports any functions that pass VCL String objects
// (or structs/classes containing nested Strings) as parameter or function
// results, you will need to build both your DLL project and any EXE projects
// that use your DLL with the dynamic RTL (the RTL DLL).  This will change your
// DLL and its calling EXE's to use BORLNDMM.DLL as their memory manager. In
// these cases, the file BORLNDMM.DLL should be deployed along with your DLL
// and the RTL DLL (CP3240MT.DLL). To avoid the requiring BORLNDMM.DLL in
// these situations, pass string information using "char *" or ShortString
// parameters and then link with the static RTL.
//
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}

