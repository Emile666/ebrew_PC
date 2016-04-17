/*==================================================================
  File name    : $Id$
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains the defines for the PID controller.
  ------------------------------------------------------------------
  $Log$
  Revision 1.14  2016/04/09 12:58:50  Emile
  - First version for new V3.30 PCB HW. Now support 4 temperatures, 4 flowsensors
    and Boil-Kettle PID-Controller. Various changes to User Interface, Registry
    parameters and scheduler/tasks.
  - Only 6 parameters left to send to HW. In line with firmware R1.23.
  - New switched/fixes added for tset_boil, gamma_boil and Tboil.

  Revision 1.13  2013/07/24 14:00:00  Emile
  - Version ready for Integration Testing with Ebrew HW R1.07!
  - Writing parameters to Ebrew HW now works with new task writing_pars
  - HW Revision is now displayed in statusbar if comm. is established
  - Task hw_debug added to display S1, S2 & S3 command info from Ebrew HW.
  - Key-press 'D' now initiates hw_debug task

  Revision 1.12  2011/05/29 20:56:26  Emile
  - New Registry variables added: STC_N, STC_TD and STC_ADF
  - PID Settings Dialog screen extended with new parameters for self-tuning
    controller: possibility to set the system order N, an estimate for the
    time-delay and a boolean whether or not to use adaptive dir. forgetting.
  - PID Settings Dialog screen: parameters enabled/disabled when a
    specific PID controller is chosen.
  - New functions time_delay() and init_time_delay() added
  - Changes made in init_pid2() function header.
  - Unit-test cases updated and extended with tests for new functions.

  Revision 1.11  2011/05/14 14:02:19  Emile
  - Unit test set updates, test-case 16 added
  - Self-Tuning controller N=1 and N=2 added to PID dialog screen
  - PID debug label extended with (Kc Ti Td) and sys. id. parameters
  - Max. sample-time extended to SIXTY_SECONDS (was 20 seconds)
  - Help file updated with version history

  Revision 1.10  2011/05/06 11:09:42  Emile
  - pid_reg1(), pid_reg2(), init_pid1(), init_pid2() removed.
  - pid_reg4() changed into pure Takahashi PID controller, no D-filtering anymore.
  - PID dialog updated to reflect changes.

  Revision 1.9  2007/07/06 22:23:02  Emile
  - The real time between two lines from a log-file is now used instead of a
    fixed 5 sec. time when reading a log-file.
  - Leading-zero bug solved in Mash Progress screen
  - i2c_stop() only called with PT_CLOSE in case of exit of program
  - System Identification library functions added (but not used yet).

  Revision 1.8  2005/03/26 13:53:21  Emile
  - During State "Mash Preheat" pump is set to ON (again)
  - Added a burner_on option (bit 4 on LSB_IO). For this two new registry
    variables are introduced (BURNER_HHYST and BURNER_LHYST)
  - Various screens a little enlarged (no scrollbars visible anymore)

  Revision 1.7  2004/05/13 20:51:00  emile
  - Main loop timing improved. Only 99 (of 100) cycles were executed. Loop
    timing is now reset after 100 loops (5 seconds)
  - TS parameter now only works on PID-controller time-slice. Max. is 20 sec.
  - Bug-fix in init_ma() filter when init. to a value (should be /N).
  - LPF for D-term of PID controller added. New reg. var. K_LPF
  - PID Debug label added + checkbox in PID screen. Default off (NO reg var).
  - Statusbar object added
  - Start made with network functionality. Not operational yet.

  Revision 1.6  2004/05/10 20:54:30  emile
  - Bug-fix: log-file header: '\n' was removed, is corrected now
  - Hints added to PID dialog screen
  - Now four PID controllers to choose from. Old ebrew version is still listed,
    but should not be used anymore. Simulation showed stability problems.
    Preferably use the type C controller.

  Revision 1.5  2004/05/08 14:52:52  emile
  - Mash pre-heat functionality added to STD. New registry variable PREHEAT_TIME.
    tset_hlt is set to next mash temp. if mash timer >= time - PREHEAT_TIME
  - View mash progress screen: reorganised, pre-heat timers added, timers are now
    in seconds instead of minutes.
  - update_tset() function removed. Now incorporated in STD, states 3-5 + (new state) 13.
  - THLT_HLIMIT and THLT_LLIMIT and state 4 'Bypass Heat Exchanger' removed
  - Reorganisation of several variables (e.g. ms_idx, ms_tot) into (other) structs.
  - 'Apply' Button added to Fix parameters dialog screen.
  - 'Edit mash scheme' no longer resets the (running) mash timers
  - 'Mash progress controlled by' function removed. Registry var 'mash_control' now
    also removed.
  - Changing init. volume of HLT did not result in an update on screen. Corrected.

  Revision 1.4  2004/05/05 15:44:16  emile
  - Main Screen picture update
  - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
  - STD update: calculation of volumes should be correct now
  - Parameter added for early start of mash timer. Registry var. TOffset2
  - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
  - Some screens updated with hints (also of labels)
  - Bug-fix: unnecessary delay after change in gamma. Is corrected now
  - Help via menu now also works

  Revision 1.3  2004/04/25 14:02:17  emile
  - Added a 'type C' PID controller, function pid_reg3(). Possible to select
    this from the PID settings dialog screen. Left the old one in for
    compatibility. New registry variable PID_Model.
  - Gamma added to log-file, so that the PID controller can be tuned.

  Revision 1.2  2002/12/30 13:33:45  emile
  - Headers with CVS tags added to every source file
  - Restore Settings function is added
  - "ebrew" registry key now in a define REGKEY

  V0.1 060302 First version
  ==================================================================
*/
#ifndef PID_REG_H
#define PID_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#define PI    (3.141592654)
#define NODF  (1)

// PID controller upper & lower limit [%]
#define GMA_HLIM (100.0)
#define GMA_LLIM   (0.0)

// Constants needed for system identification
typedef double vector[6];
typedef double matrix[6][6];

typedef struct _pid_params
{
   double kc; // Controller gain from Dialog Box
   double ti; // Time-constant for I action from Dialog Box
   double td; // Time-constant for D action from Dialog Box
   double ts; // Sample time [sec.] from Dialog Box
   double k_lpf; // Time constant [sec.] for LPF filter
   double k0; // k0 value for PID controller
   double k1; // k1 value for PID controller
   double k2; // k2 value for PID controller
   double k3; // k3 value for PID controller
   double lpf1; // value for LPF filter
   double lpf2; // value for LPF filter
   double lpf_1; // LPF output[k-1]
   double lpf_2; // LPF output[k-2]
   int    pid_model; // PID Controller type [0..3]
   double pp; // debug
   double pi; // debug
   double pd; // debug
   double xk_1; // x[k-1]
   double xk_2; // x[k-2]
   double ek_1; // e[k-1]
} pid_params; // struct pid_params

//------------------------------------------------------------------
// This struct contains the estimated values for the PID controller.
// These are the optimum settings for the Takahashi controller and
// are calculated from the system identification function sys_id()
// and calc_ultimate_gain_period()
//------------------------------------------------------------------
typedef struct _sys_id_params
{
   int    N;       // order N for system identification and parameter estimation
   int    stc_td;  // Time-Delay estimate for system identification
   int    stc_adf; // 1 = use adaptive directional forgetting
   double kpu;     // Ultimate gain
   double tu;      // Ultimate period
   double kc;      // Controller gain
   double ti;      // Time-constant for I action
   double td;      // Time-constant for D action
   double kr;      // Kr value for PID Takahashi controller
   double ki;      // ki value for PID Takahashi controller
   double kd;      // kd value for PID Takahashi controller
} sys_id_params; // struct sys_id_params

//--------------------
// Function Prototypes
//--------------------
void init_pid2(pid_params *p, sys_id_params *psi); // Takahashi with Self-Tuning
void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg, sys_id_params *psys_id);
void init_pid3(pid_params *p);
void init_pid4(pid_params *p);        // Type C Takahashi PID, no filtering of D-action
void pid_reg4(double xk, double *yk, double tset, pid_params *p, int vrg);

void   sys_id(double uk, double yk, int N, int use_adf);
int    calc_ultimate_gain_period(sys_id_params *p, double ts);
void   calc_pid_parameters(sys_id_params *p, double ts);

double vxv2s(vector vec1, vector vec2, int N);
void   vxv2m(vector vec1, vector vec2, matrix *m, int N);
void   vxm2v(vector vec1, matrix m1, vector *vecr, int N);
void   mxv2v(matrix m1, vector vec1, vector *vecr, int N);
void   vxs2v(vector *vec, double x, int N);
void   vav2v(vector *vec1, vector vec2, int N);
void   mxs2m(matrix *m, double x, int N);
void   mam2m(matrix *m1, matrix m2, int N);

#ifdef __cplusplus
};
#endif
#endif
