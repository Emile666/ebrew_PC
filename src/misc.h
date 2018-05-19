// ==================================================================
// Filenaam    : $Id$
// Auteur      : E. van de Logt
// ------------------------------------------------------------------
// Purpose : This include file contains defines for misc.c
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
#define MAX_SP      (10) 
#define NOT_STARTED (-1)

#define LOG_HDR_SIZE (10)
#define LOG_LAST_LINE (3)
#define COLON         ","

typedef struct _log_struct
{
   char          brew_date[20];  /* Brew date */
   unsigned int  bline;          /* First line with valid information */
   unsigned int  eline;          /* Last line with valid information */
   char          btime[20];      /* Begin time of brew session */
   char          etime[20];      /* End time of brew session */
   int           lms_idx;        /* Last known value of ms_idx */
   int           tmr_ms_idx;     /* Timer value of ms_idx timer */
   int           std_val;        /* Last known value of std_state */
   unsigned int  start_lstd;     /* The start line number of the latest ebrew_std */
   unsigned int  start_lmtmr;    /* The start line number of the start of the latest mash timer */
   int           lsp_idx;        /* Last known value of sp_idx */
   unsigned int  mashing_start[MAX_MS];  /* Start line of Mashing */
   unsigned int  sparging_start[MAX_SP]; /* Start line of Sparging */
   unsigned int  sparging_start2[MAX_SP]; /* Start line of Sparging */
   unsigned int  boil_start;     /* Start line of Boiling */
   unsigned int  chill_start;    /* Start line of Chilling */
   double        time_period;    /* Time [sec.] between 2 lines in the log-file */
} log_struct;

typedef struct _maisch_schedule
{
   double time;           /* time (min.) to remain at this temperature */
   double temp;           /* temperature (Celsius) to hold */
   int    timer;          /* timer, init. to NOT_STARTED */
   int    preht;          /* preheat timer */
   char   time_stamp[20]; /* time when timer was started */
} maisch_schedule;

typedef struct _sparge_struct
{
   /* Mash Settings */
   double temp_offset0;    // Offset to add to Tset to compensate for dough-in losses
   double temp_offset;     // Offset to add to Tset to compensate for HLT-MLT losses
   double temp_offset2;    // Offset to add to Tmlt for early start of mash timers
   int    ph_time;         // Copy of Registry var. PREHEAT_TIME
   int    use_dpht;        // 1= use Dynamic preheat timing instead of fixed timing
   int    hlt_bcap;        // HLT Burner Capacity in seconds per °C
   int    leave_pumps_on;  // After MLT Temp and before malt is added.
   /* Sparge Settings */
   int    sp_batches;      // Total number of sparge batches
   int    sp_time;         // Time between two sparge batches in minutes
   int    mash_vol;        // Total mashing volume in litres
   int    sp_vol;          // Total sparge volume in litres
   int    sp_time_ticks;   // sp_time in TS ticks
   int    boil_time_ticks; // boil_time in TS ticks
   double sp_vol_batch;    // Sparge volume of 1 batch = sp_vol / sp_batches
   double sp_vol_batch0;   // Sparge volume of first batch
   /* Sparge Settings */
   int    boil_min_temp;   // Min. Temp. for Boil-Kettle to enable PID controller
   int    boil_time;       // Total boiling time in minutes
   int    sp_preboil;      // Setpoint Preboil Temperature
   double boil_detect;     // Boiling-Detection minimum Temperature (Celsius)
   int    sp_boil;         // Setpoint Boil Temperature
   int    pid_ctrl_boil_on;// 1= enable PID-controller for Boil-Kettle 
   /* Time-stamps for Sparge, Boil and Chilling*/
   char   mlt2boil[MAX_SP][40]; // MAX_SP strings for time-stamp moment of MLT -> BOIL
   char   hlt2mlt[MAX_SP][40];  // MAX_SP strings for time-stamp moment of HLT -> MLT
   char   boil[2][40];          // Boil-start and Boil-End time-stamps
   char   chill[2][40];         // Chill-start and Chill-End time-stamps
} sparge_struct;

typedef struct _std_struct
{
   int    ebrew_std; // Current state of STD
   int    ms_tot;    // tot. nr. of valid temp & time values
   int    ms_idx;    // index in ms[] array
   int    sp_idx;    // Sparging index [0..sps->sp_batches-1]
   int    timer1;    // Timer for state 'Sparging Rest'
   int    timer2;    // Timer for state 'Delay_xSEC'
   int    timer3;    // Timer for state 'Pump Pre-Fill'
   int    mrest_tmr; // Timer for state 'Mast Rest 5 Min.'
   int    brest_tmr; // Timer for state 'Boiling finished, prepare Chiller'
   int    timer5;    // Timer for state 'Boiling'
   int    mash_rest; // 1 = mash rest after malt is added
   int    cip_tmr1;  // Timer for CIP process
   int    cip_circ;  // Counter for CIP circulations
   int    boil_rest; // 1 = let wort rest after malt is added
} std_struct;

#define MAX_MA (50)
typedef struct _ma
{
   double T[MAX_MA]; // array with delayed values of input signal
   int    index;     // index in T[] where to store the new input value
   int    N;         // The order of the MA filter. Note that N < MAX_MA
   double sum;       // The running sum of the MA filter
} ma;

#define VBOIL_START   (0.0)
typedef struct _volume_struct
{
   double Vhlt;       // Volume of HLT in litres
   double Vmlt;       // Volume of MLT in litres
   double Vboil;      // Volume of Boil kettle in litres
   double Vhlt_old;   // Prev. value of Vhlt, used in STD
   double Vmlt_old;   // Prev. value of Vmlt, used in STD
   double Vboil_old;  // Prev. value of Vboil, used in STD
   double Flow_hlt_mlt;       // Flow1
   double Flow_mlt_boil;      // Flow2
   double Flow_cfc_out;       // Flow3
   double Flow4;              // Flow4: Future Use
   double Flow_hlt_mlt_old;
   double Flow_mlt_boil_old;
   double Flow_cfc_out_old;
   double Flow4_old;
   double Flow_rate_hlt_mlt;
   double Flow_rate_mlt_boil;
   double Flow_rate_cfc_out;
   double Flow_rate4;
   double Flow_cfc_out_reset_value;
   int    min_flowrate_mlt_perc;
   int    min_flowrate_boil_perc;
} volume_struct;

typedef struct _flow_rate_low_struct
{
	int    frl_std;         // STD state number
	int    frl_tmr;         // Timer value
	double frl_det_lim;     // Lower-limit for flowrate
	double frl_min_det_lim; // Minimum flowrate: sensor-check
	int    frl_perc;        // Percentage of max flowrate
} flow_rate_low_struct;

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
#define S14_PUMP_PREFILL          (14)
#define S02_FILL_MLT               (2)

#define S03_WAIT_FOR_MLT_TEMP      (3)
#define S15_ADD_MALT_TO_MLT       (15)
#define S04_MASH_TIMER_RUNNING     (4)
#define S13_MASH_PREHEAT_HLT      (13)

#define S05_SPARGE_TIMER_RUNNING   (5)
#define S06_PUMP_FROM_MLT_TO_BOIL  (6)
#define S07_PUMP_FROM_HLT_TO_MLT   (7)
#define S08_DELAY_xSEC             (8)

#define S09_EMPTY_MLT              (9)
#define S10_WAIT_FOR_BOIL         (10)
#define S11_BOILING               (11)
#define S12_BOILING_FINISHED      (12)
#define S16_CHILL_PUMP_FERMENTOR  (16)
#define S17_FINISHED              (17)
#define S18_MASH_REST_5_MIN       (18)
#define S19_RDY_TO_ADD_MALT       (19)

#define S20_CIP_INIT              (20)
#define S21_CIP_HEAT_UP           (21)
#define S22_CIP_CIRC_5_MIN        (22)
#define S23_CIP_REST_5_MIN        (23)
#define S24_CIP_DRAIN_BOIL1       (24)
#define S25_CIP_DRAIN_BOIL2       (25)
#define S26_CIP_FILL_HLT          (26)
#define S27_CIP_CLEAN_OUTPUT_V7   (27)
#define S28_CIP_CLEAN_OUTPUT_V6   (28)
#define S29_CIP_CLEAN_OUTPUT_V4   (29)
#define S30_CIP_CLEAN_INPUT_V3    (30)
#define S31_CIP_CLEAN_INPUT_V1    (31)
#define S32_CIP_END               (32)

//----------------------------------
// Defines for User Interaction
//----------------------------------
#define UI_PID_ON             (0x0001)
#define UI_MALT_ADDED_TO_MLT  (0x0002)
#define UI_BOILING_STARTED    (0x0004)
#define UI_START_CHILLING     (0x0008)
#define UI_CHILLING_FINISHED  (0x0010)
#define UI_CIP_INIT           (0x0020)
#define UI_CIP_BOIL_FILLED    (0x0040)
#define UI_CIP_HOSES_IN_DRAIN (0x0080)
#define UI_CIP_HLT_FILLED     (0x0100)
#define UI_ADDING_MALT_TO_MLT (0x0200)

//-------------------------------------------------------------
// Hard-coded Timers.
// task update_std() runs every second, so 1 second is 1 tick.
//-------------------------------------------------------------
#define TMR_PREFILL_PUMP       (60)
#define TMR_DELAY_xSEC         (10)
#define TMR_MASH_REST_5_MIN   (300)
#define TMR_BOIL_REST_5_MIN   (300)
#define TMR_CIP_CIRC_TIME     (300)
#define TMR_CIP_REST_TIME     (300)
#define TMR_CIP_CLEAN_OUTPUTS  (60)
#define TMR_CIP_CLEAN_INPUTS   (60)

#define CIP_TEMP_SETPOINT     (65.0)

//--------------------------------------------------------------------------
// #defines for the valves. Each valve can be set manually or automatically
// by the STD. Bit-values are for the variable 'valves'.
//--------------------------------------------------------------------------
#define P1M  (0x02000000)
#define P0M  (0x01000000)
#define V8M  (0x00800000)
#define V7M  (0x00400000)
#define V6M  (0x00200000)
#define V5M  (0x00100000)
#define V4M  (0x00080000)
#define V3M  (0x00040000)
#define V2M  (0x00020000)
#define V1M  (0x00010000)

#define P1b  (0x00000200)
#define P0b  (0x00000100)
#define V8b  (0x00000080)
#define V7b  (0x00000040)
#define V6b  (0x00000020)
#define V5b  (0x00000010)
#define V4b  (0x00000008)
#define V3b  (0x00000004)
#define V2b  (0x00000002)
#define V1b  (0x00000001)

#define ALL_VALVES (V1b | V2b | V3b | V4b | V5b | V6b | V7b | V8b)
#define ALL_PUMPS  (P0b | P1b)
#define ALL_MANUAL (V1M | V2M | V3M | V4M | V5M | V6M | V7M | V8M | P0M | P1M)

#define P11MTXT "PUMP2 ON (M)"
#define P10MTXT "PUMP2 OFF (M)"
#define P11ATXT "PUMP2 ON (A)"
#define P10ATXT "PUMP2 OFF (A)"
#define V81MTXT "V8 ON (M)"
#define V80MTXT "V8 OFF (M)"
#define V81ATXT "V8 ON (A)"
#define V80ATXT "V8 OFF (A)"
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

void add_seconds(char *s, int seconds);
int decode_log_file(FILE *fd, log_struct p[]);
int read_input_file(char *inf, maisch_schedule ms[], int *count, double ts, int init, int *vmash, int *vsparge);
int update_std(volume_struct *vol, double thlt, double tmlt, double tboil,
               double *tset_hlt, double *tset_mlt, double *tset_boil,
               unsigned int *kleppen, maisch_schedule ms[],
               sparge_struct *sps, std_struct *std, int ui, int std_fx);
void   init_ma(ma *p, int N, double init_val);
double moving_average(ma *p, double x);
void   init_sample_delay(ma *p, int TD);
double sample_delay(ma *p, double x);
void   slope_limiter(const double lim, const double Told, double *Tnew);

#ifdef __cplusplus
};
#endif
#endif /* MISC_H */
