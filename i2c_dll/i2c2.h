// ==================================================================
// Functienaam : -
// Filenaam    : i2c2.h
// Auteur      : E. van de Logt
// Datum       : 12-03-2000
// Versie      : V0.1
// ------------------------------------------------------------------
// Doel : Deze file bevat de basisroutines voor de I2C bus. Deze
//	  file is afgeleid van de Elektuur device driver I2CDRIV2.ASM.
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// ==================================================================
#ifndef I2C2_H
#define I2C2_H

#ifdef __cplusplus
extern "C" {
#endif

// Base address of I2C Printed Circuit Board in PC
//------------------------------------------------
//#define S023 (0x310)
//#define   S1 (S023 + 1)

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

// Control/Status Register S1
//---------------------------
#define PINb (0x80)
#define STSb (0x20)
#define BERb (0x10)
#define LRBb (0x08)
#define AASb (0x04)
#define LABb (0x02)
#define  BBb (0x01)

#define  RWb (0x01)

// I2C2 Error Messages
//---------------------
#define I2C_NOERR       (0)
#define I2C_BERR       (-1)
#define I2C_BB         (-2)
#define I2C_TIMEOUT    (-3)
#define I2C_ARGS       (-4)
#define I2C_BERR_MSG   "I2C Bus-Error"
#define I2C_BB_MSG     "I2C Bus-Busy Error"
#define I2C_TO_MSG     "I2C Time-Out"
#define I2C_ARGS_MSG   "Wrong argument"

#define FALSE (0)
#define TRUE  (!FALSE)

// Time-out value for wait_byte() routine, in pauze() ticks
//---------------------------------------------------------
#define TO_VAL (500)

typedef unsigned char byte;

// Function prototypes needed for external use
//--------------------------------------------
void delay(int x);
int  i2c_init(int address);
int  i2c_start(void);
int  i2c_address(byte address);
int  i2c_write(byte address, byte *p,int bytes);
int  i2c_read(byte address, byte *p, int bytes);
int  i2c_stop(void);

#ifdef __cplusplus
};
#endif
#endif
