// ==================================================================
// Functienaam : -
// Filenaam    : misc.h
// Auteur      : E. van de Logt
// Datum       : 13-03-2002
// Versie      : V0.2
// ------------------------------------------------------------------
// Doel : This include file contains defines for misc.c
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// 13-03-02 LGT    Initial version, derived from brew.c
// 27-07-02 LGT    moving_average() function added
// ==================================================================
#ifndef MISC_H
#define MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#define FALSE        (0)
#define TRUE    (!FALSE)
#define SLEN       (255)
#define MAX_MS      (10)
#define NOT_STARTED (-1)

#define ALIVE_TICKS (10) // timer for 0.5 sec. LED

// Bit assignments of IO port (LSB_IO)
//------------------------------------
#define HEATERb (0x01)
#define ALIVEb  (0x02)
#define PUMPb   (0x04)

typedef struct _maisch_schedule
{
   double time;  /* time (min.) to remain at this temperature */
   double temp;  /* temperature (Celsius) to hold */
   int    timer; /* timer, init. to NOT_STARTED */
} maisch_schedule;

typedef struct _sparge_struct
{
   int    sp_batches;      // Total number of sparge batches
   int    sp_time;         // Time between two sparge batches in minutes
   int    mash_vol;        // Total mashing volume in litres
   int    sp_vol;          // Total sparge volume in litres
   int    boil_time;       // Total boiling time in minutes
   int    sp_time_ticks;   // sp_time in TS ticks
   int    boil_time_ticks; // boil_time in TS ticks
   double sp_vol_batch;    // Sparge volume of 1 batch = sp_vol / sp_batches
} sparge_struct;

typedef struct _timer_vars
{
   int  htimer;    // Timer to count time that heater is on
   int  ltimer;    // Timer to count time that heater is off
   int  isrstate;  // State of the Interrupt Service Routine
   int  alive;     // Alive bit (bit 1 of IO port)
   int  alive_tmr; // Toggle to indicate program is still alive
   int  time_high; // The number of clock-ticks to set the heater on
   int  time_low;  // The number of clock-ticks to set the heater off
   int  pid_tmr;   // Timer for when to run PID controller
} timer_vars;

#define MAX_MA (20)
typedef struct _ma
{
   double T[MAX_MA]; // array with delayed values of input signal
   int    index;     // index in T[] where to store the new input value
   int    N;         // The order of the MA filter. Note that N < MAX_MA
   double sum;       // The running sum of the MA filter
} ma;

// Defines for State Transition Diagram.
// The STD is called every second => 1 tick == 1 second.
//------------------------------------------------------
#define INITIALISATION        (0)
#define WAIT_FOR_HLT_TEMP     (1)
#define FILL_MLT              (2)
#define MASH_IN_PROGRESS      (3)
#define BYPASS_HEAT_EXCHANGER (4)
#define SPARGING_REST         (5)
#define PUMP_FROM_MLT_TO_BOIL (6)
#define PUMP_FROM_HLT_TO_MLT  (7)
#define DELAY_1SEC            (8)
#define EMPTY_MLT             (9)
#define BOILING               (10)
#define EMPTY_HEAT_EXCHANGER  (11)
#define CHILL                 (12)

#define TMLT_HLIMIT           (0.5)
#define TMLT_LLIMIT           (0.0)
#define TIMEOUT_1SEC          (1)
#define VMLT_EMPTY            (3.0)
#define TIMEOUT3              (300)
#define TIMEOUT4              (20)

//--------------------------------------------------------------------------
// #defines for the valves. Each valve can be set manually or automatically
// by the STD. Bit-values are for the variable 'valves'.
//--------------------------------------------------------------------------
#define V7M  (0x8000)
#define V6M  (0x4000)
#define V5M  (0x2000)
#define V4M  (0x1000)
#define V3M  (0x0800)
#define V2M  (0x0400)
#define V1M  (0x0200)
#define V7b  (0x0080)
#define V6b  (0x0040)
#define V5b  (0x0020)
#define V4b  (0x0010)
#define V3b  (0x0008)
#define V2b  (0x0004)
#define V1b  (0x0002)
#define V0b  (0x0001)

#define V71MTXT "V7 ON (M)"
#define V70MTXT "V7 OFF (M)"
#define V71ATXT "V7 ON (A)"
#define V70ATXT "V7 OFF (A)"
#define V61MTXT "V6 ON (M)"
#define V60MTXT "V6 OFF (M)"
#define V61ATXT "V6 ON (A)"
#define V60ATXT "V6 OFF (A)"
#define V51MTXT "V5 ON (M)"
#define V50MTXT "V5 OFF (M)"
#define V51ATXT "V5 ON (A)"
#define V50ATXT "V5 OFF (A)"
#define V41MTXT "V4 ON (M)"
#define V40MTXT "V4 OFF (M)"
#define V41ATXT "V4 ON (A)"
#define V40ATXT "V4 OFF (A)"
#define V31MTXT "V3 ON (M)"
#define V30MTXT "V3 OFF (M)"
#define V31ATXT "V3 ON (A)"
#define V30ATXT "V3 OFF (A)"
#define V21MTXT "V2 ON (M)"
#define V20MTXT "V2 OFF (M)"
#define V21ATXT "V2 ON (A)"
#define V20ATXT "V2 OFF (A)"
#define V11MTXT "V1 ON (M)"
#define V10MTXT "V1 OFF (M)"
#define V11ATXT "V1 ON (A)"
#define V10ATXT "V1 OFF (A)"

int read_input_file(char *inf, maisch_schedule ms[], int *count, double ts);
void update_tset(double *tset, double temp, double offset,
                 maisch_schedule ms[], int *ms_idx, int ms_total);
int  update_std(double vmlt, double tmlt, unsigned int *kleppen,
                 maisch_schedule ms[], int ms_idx, int ms_total,
                 sparge_struct *sps, int pid_on);
void init_ma(ma *p, int N);
double moving_average(ma *p, double x);

#ifdef __cplusplus
};
#endif
#endif
