// ==================================================================
// Filenaam    : $Id$
// Auteur      : E. van de Logt
// ------------------------------------------------------------------
// Purpose : This include file contains defines for misc.c
// ------------------------------------------------------------------
// Modification History :
// $Log$
// Revision 1.10  2004/05/05 15:44:16  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.9  2004/01/31 16:01:05  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.8  2003/09/15 20:37:22  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.7  2003/07/11 18:34:46  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm92_read())
//
// Revision 1.6  2003/06/01 13:40:45  emile
// - Bugfix: switch/fix for Tmlt and Thlt were in wrong time-slice. Corrected.
// - Switch/fix for std state added for easier testing
// - Vmash value added to 'Options|Sparge & STD Settings' dialog screen.
//
// Revision 1.5  2003/06/01 11:53:48  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
//
// Revision 1.4  2003/01/04 22:35:50  emile
// - Restore Settings function now restores all relevant variables (not just
//   the mashing variables). Several separate functions are created to
//   support this.
//
// Revision 1.3  2002/12/30 20:21:59  emile
// - Bug 2 29-12-02 solved: start mash timers if temp >= tref instead of >.
// - Bug 3 29-12-02 solved: deadlock in std_state 4 when mashing is finished.
// - Bug 4 29-12-02 solved: rush through STD. Sparging parameters were not
//   initialised. New function Init_Sparge_Settings() added.
// - Sparge variables now added to 'View Mash Progress' screen.
// - std_struct added for more flexibility of fixing STD values.
//
// Revision 1.2  2002/12/30 13:33:45  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// 27-07-02 LGT    moving_average() function added
// 13-03-02 LGT    Initial version, derived from brew.c
// ==================================================================
#ifndef MISC_H
#define MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifndef FALSE
#define FALSE        (0)
#endif
#ifndef TRUE
#define TRUE         (1)
#endif

#define SLEN       (255)
#define MAX_MS      (10)
#define NOT_STARTED (-1)

#define LOG_HDR_SIZE (10)
#define LOG_LAST_LINE (3)
#define COLON         ","

#define ALIVE_TICKS (10) // timer for 0.5 sec. LED

// Bit assignments of IO port (LSB_IO)
//------------------------------------
#define HEATERb (0x01)
#define ALIVEb  (0x02)
#define PUMPb   (0x04)

typedef struct _log_struct
{
   char brew_date[20];  /* Brew date */
   int  bline;          /* First line with valid information */
   int  eline;          /* Last line with valid information */
   char btime[20];      /* Begin time of brew session */
   char etime[20];      /* End time of brew session */
   int  lms_idx;        /* Last known value of ms_idx */
   int  tmr_ms_idx;     /* Timer value of ms_idx timer */
   int  std_val;        /* Last known value of std_state */
   int  start_lstd;     /* The start line number of the latest ebrew_std */
   int  sp_idx;         /* Number of Sparging cycles */
   int  mashing_start;  /* Start line of Mashing */
   int  sparging_start; /* Start line of Sparging */
   int  boil_start;     /* Start line of Boiling */
   int  chill_start;    /* Start line of Chilling */
   int  max_std;        /* Max. value of ebrew_std */
   double vmash;        /* Value of vmash = Vmlt if ebrew_std 3 -> 5 */
} log_struct;

typedef struct _maisch_schedule
{
   double time;  /* time (min.) to remain at this temperature */
   double temp;  /* temperature (Celsius) to hold */
   int    timer; /* timer, init. to NOT_STARTED */
} maisch_schedule;

typedef struct _sparge_struct
{
   /* Mash Settings */
   double temp_offset;     // Offset to add to Tset to compensate for HLT-MLT losses
   double temp_offset2;    // Offset to add to Tmlt for early start of mash timers
   int    ph_timer;        // Mash pre-heat timer
   /* Sparge Settings */
   int    sp_batches;      // Total number of sparge batches
   int    sp_time;         // Time between two sparge batches in minutes
   int    mash_vol;        // Total mashing volume in litres
   int    sp_vol;          // Total sparge volume in litres
   int    boil_time;       // Total boiling time in minutes
   int    sp_time_ticks;   // sp_time in TS ticks
   int    boil_time_ticks; // boil_time in TS ticks
   double sp_vol_batch;    // Sparge volume of 1 batch = sp_vol / sp_batches
   /* STD Settings */
   double vmlt_empty;      // MLT is empty below this volume
   int    to_xsec;         // Timeout value for state 8
   int    to3;             // Timeout value for state 10 -> 11
   int    to4;             // Timout value for state 11 -> 10
} sparge_struct;

typedef struct _std_struct
{
   int    ebrew_std; // Current state of STD
   int    ms_tot;    // tot. nr. of valid temp & time values
   int    ms_idx;    // index in ms[] array
   int    sp_idx;    // Sparging index [0..sps->sp_batches-1]
   int    timer1;    // Timer for state 'Sparging Rest'
   int    timer2;    // Timer for state 'Delay_1SEC'
   int    timer3;    // Timer for transition to state 'Empty Heat Exchanger'
   int    timer4;    // Timer for state 'Empty Heat Exchanger'
   int    timer5;    // Timer for boiling time
} std_struct;

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

#define VBOIL_START (0.0)
typedef struct _volume_struct
{
   double Vhlt;       // Volume of HLT in litres
   double Vmlt;       // Volume of MLT in litres
   double Vboil;      // Volume of Boil kettle in litres
   int    Vhlt_start; // Starting volume of HLT
   double Vhlt_old;   // Prev. value of Vhlt, used in STD
   double Vmlt_old;   // Prev. value of Vmlt, used in STD
   double Vboil_old;  // Prev. value of Vboil, used in STD
   // Assumption is made here that Vmlt is always measured.
   int    Vhlt_simulated;  // true = Vhlt is not measured, but calculated
   int    Vboil_simulated; // true = Vboil is not measured, but calculated
} volume_struct;

//------------------------------
// Defines for read_input_file()
//------------------------------
#define INIT_TIMERS    (1)
#define NO_INIT_TIMERS (0)

//------------------------------------------------------
// Defines for State Transition Diagram.
// The STD is called every second => 1 tick == 1 second.
//------------------------------------------------------
#define S00_INITIALISATION         (0)
#define S01_WAIT_FOR_HLT_TEMP      (1)
#define S02_FILL_MLT               (2)
#define S03_MASH_IN_PROGRESS       (3)
#define S04_MASH_TIMER_RUNNING     (4)
#define S13_MASH_PREHEAT_HLT      (13)
#define S05_SPARGING_REST          (5)
#define S06_PUMP_FROM_MLT_TO_BOIL  (6)
#define S07_PUMP_FROM_HLT_TO_MLT   (7)
#define S08_DELAY_xSEC             (8)
#define S09_EMPTY_MLT              (9)
#define S10_BOILING               (10)
#define S11_EMPTY_HEAT_EXCHANGER  (11)
#define S12_CHILL                 (12)

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
#define P0M  (0x0100)
#define V7b  (0x0080)
#define V6b  (0x0040)
#define V5b  (0x0020)
#define V4b  (0x0010)
#define V3b  (0x0008)
#define V2b  (0x0004)
#define V1b  (0x0002)
#define P0b  (0x0001)

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
#define P01MTXT "PUMP ON (M)"
#define P00MTXT "PUMP OFF (M)"
#define P01ATXT "PUMP ON (A)"
#define P00ATXT "PUMP OFF (A)"

int decode_log_file(FILE *fd, log_struct p[]);
int read_input_file(char *inf, maisch_schedule ms[], int *count, double ts, int init);
//double update_tset(double *tset, double temp, double offset, double offset2,
//                   maisch_schedule ms[], int *ms_idx, int ms_total);
int update_std(volume_struct *vol, double tmlt, double thlt, double *tset_mlt,
               double *tset_hlt, unsigned int *kleppen, maisch_schedule ms[],
               sparge_struct *sps, std_struct *std, int pid_on, int std_fx);
void init_ma(ma *p, int N, double init_val);
double moving_average(ma *p, double x);

#ifdef __cplusplus
};
#endif
#endif
