/*==================================================================
  File name    : $Id$
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains the defines for the PID controller.
  ------------------------------------------------------------------
  $Log$
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

#define T_50MSEC  (50)   // Period time of TTimer in msec.

#define GMA_HLIM (100.0) // PID controller upper limit [%]
#define GMA_LLIM   (0.0) // PID controller lower limit [%]

typedef struct _pid_params
{
   double kc; // Controller gain from Dialog Box
   double ti; // Time-constant for I action from Dialog Box
   double td; // Time-constant for D action from Dialog Box
   double ts; // Sample time [sec.] from Dialog Box
   double k0; // k0 value for PID controller
   double k1; // k1 value for PID controller
   double k2; // k2 value for PID controller
   double k3; // k3 value for PID controller
   int    ts_ticks; // ticks for timer
   int    pid_model;    // 0 = Type A (pid_reg2), 1 = Type C (pid_reg3)
} pid_params; // struct pid_params

//--------------------
// Function Prototypes
//--------------------
void init_pid2(pid_params *p);
void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg);
void init_pid3(pid_params *p);
void pid_reg3(double xk, double *yk, double tset, pid_params *p, int vrg);

#ifdef __cplusplus
};
#endif
#endif
