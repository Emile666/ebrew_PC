/*==================================================================
  File name    : $Id$
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains the defines for the PID controller.
  ------------------------------------------------------------------
  $Log$
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
   int    ts_ticks; // ticks for timer
   double temp_offset; // Offset to add to Tset to compensate for HLT-MLT losses
   int    mash_control; // 0 = Tad1, 1 = Tad2
} pid_params; // struct pid_params

//--------------------
// Function Prototypes
//--------------------
void init_pid2(pid_params *p);
void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg);

#ifdef __cplusplus
};
#endif
#endif
