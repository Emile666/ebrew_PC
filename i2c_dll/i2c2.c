// ==================================================================
// Functienaam : pauze()    , i2c_berr_check(), i2c_start(),
//               wait_byte(), terminate_read(), i2c_address(),
//               i2c_write(), i2c_read()
// Filenaam    : i2c2.c
// Auteur      : E. van de Logt
// Datum       : 12-03-2002
// Versie      : V0.1
// ------------------------------------------------------------------
// Doel : Deze file bevat de basisroutines voor de I2C bus. Deze
//	  file is afgeleid van de Elektuur file I2CDRIV2.ASM.
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// ==================================================================

// -------------------------- include files -------------------------
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "i2c2.h"

static byte last_start;   // TRUE = Last action on bus was start
static byte ic_adr;       // Currently addressed IC and R/W_ bit

static byte i2c_method;   // [ISA_CARD, LPT_CARD]
static int  S023;         // Registers S0, S2 and S3 of PCF8584
static int  S1;           // Register S1 of PCF8584 (= S023 + 1)
static int  lpt_data;     // Data register of LPTx: port
static int  lpt_ctrl;     // Control register of LPTx: port
static int  lpt_stat;     // Status register of LPTx: port
static byte lpt_ctrl_old; // Previous value of Control register of LPTx: port
static byte dataH;        // High nibble read from multiplexer
static byte dataL;        // Low nibble read from multiplexer

unsigned char inportb(unsigned short port)
/*------------------------------------------------------------------
  Purpose  : Implements the DOS Borland C function inportb(). This
             function was absent in Borland C++ Builder
  Variables: port: the port address to read a byte from
  Returns  : The byte read from the specified IO port
  ------------------------------------------------------------------*/
{
    asm     mov     dx,port
    asm     in      al,dx
    asm     xor     ah,ah
    return _AX;
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
    asm     mov     dx, port
    asm     mov     al, val
    asm     out     dx, al
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

int i2c_init(int address)
/*------------------------------------------------------------------
  Purpose  : Sets the proper base address for the PCF8584. This should
             be the first routine to call. The I2C bus can be interfaced
             in two ways:
             1) With an ISA PCB card: valid IO addresses are 0x300, 0x310,
                0x320, 0x330 or 0x340.
             2) With a I2C interface card connected to a parallel port.
                Base addresses are 0x378 (LPT1) and 0x278 (LPT2:)
  Variables: address: the IO base address for the PCF8584
  Returns  : Error: I2C_NOERR : No error
                    I2C_ARGS  : Wrong value for address
  ------------------------------------------------------------------*/
{
   int err = I2C_NOERR; // error return-value

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
   return err; // return error-code
} // i2c_init()

int i2c_start(void)
/*------------------------------------------------------------------
  Purpose  : Initialises the I2C bus.
  Variables: None
  Returns  : Error: I2C_NOERR : No error
                    I2C_BB    : I2C bus is still busy
                    I2C_BERR  : I2C bus error occurred
  ------------------------------------------------------------------*/
{
   int err = I2C_NOERR; // error return-value
   byte x; // temp. variable

   // Disable Serial Interface, next. reg. = SO' (own address)
   write_S1(0x00);
   pauze();
   // Set effective own address S0' to a non-zero value
   write_S023(0xFF);
   pauze();
   // Load next byte into clock-register S2
   write_S1(0x20);
   pauze();
   // fclk = 8 MHz (7.16 MHz on PCB), fscl = 11 kHz (9.8 kHz on PCB)
   write_S023(0x1A);
   pauze();
   // Enable Serial Interface, load next byte into S0 (Data)
   write_S1(0x40);
   pauze();
   // Check if I2C bus is busy (Bus-Busy bit is 1)
   x = read_S1();
   pauze();
   if ((x & BBb) == 0x00)
   {
      err = I2C_BB; // return error is I2C bus busy
   }
   else
   {  // Load S0 with address & R/W_ bit
      write_S023(0x00);
      ic_adr = 0x00; // save address & R/W_ bit
      pauze();
      // Load next byte in S0 (data), generate START + address
      // remain in MST/TRM mode
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

int i2c_address(byte address)
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

int i2c_write(byte address, byte *p, int bytes)
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

int i2c_read(byte address, byte *p, int bytes)
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
int i2c_stop(void)
{
   int err = I2C_NOERR; // error return-value

   err = wait_byte();   // wait for on-going actions
   if (err != I2C_NOERR)
   {
      return err; // exit if error
   } // if
   if ((ic_adr & RWb) == RWb) // Is slave still transmitting?
   {
      err = terminate_read(); // If so, terminate transmission
      if (err != I2C_NOERR)
      {
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
   return err;             // return error-code
} // i2c_stop()
