/*==================================================================
  Function name: init_pid2(), pid_reg2()
  File name    : pid_reg.c
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains the main body of the PID controller.
            For design details, please read the Word document
            "PID Controller Calculus".

            In the GUI, the following parameters can be changed:
            Kc: The controller gain
            Ti: Time-constant for the Integral Gain
            Td: Time-constant for the Derivative Gain
            Ts: The sample period [seconds]
  ------------------------------------------------------------------
  Change history: Please refer to pid_reg.h
  ==================================================================
*/
#include "pid_reg.h"

static double ek1; // e[k-1]
static double ek2; // e[k-2]

void init_pid2(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the PID controller, based on
             implementation method 2 (Bilinear Transformation with Taylor series)
  Variables: p: pointer to struct containing all PID parameters
                           Ts      Td
             k0 = Kc.(1 + ---- + 2.--)
                          2.Ti     Ts

                            Ts    6.Td
             k1 = Kc.(-1 + ---- - ----)
                           2.Ti    Ts

                  8.Kc.Td
             k2 = --------
                    Ts
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ti == 0.0)
   {
      p->k0 = p->kc * (1.0 + ((2.0 * p->td) / p->ts));
      p->k1 = p->kc * (-1.0 - ((6.0 * p->td) / p->ts));
   }
   else
   {
      p->k0 = p->kc * (1.0 + (p->ts / (2.0 * p->ti)) + ((2.0 * p->td) / p->ts));
      p->k1 = p->kc * (-1.0 + (p->ts / (2.0 * p->ti)) - ((6.0 * p->td) / p->ts));
   } // else
   p->k2 = (p->kc * 8.0 * p->td) / p->ts;
} // init_pid2()

void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the PID controller, conform
             implementation method 2 ("Bilinear Transformation with Taylor Series").
             This function should be called once every TS seconds.
  Variables:
        xk : The input variable x[k] (= measured temperature)
       *yk : The output variable y[k] (= gamma value for power electronics)
      tset : The setpoint value for the temperature
        *p : Pointer to struct containing PID parameters
        vrg: Release signal: 1 = Start control, 0 = disable PID controller
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   double ek; // e[k]

   ek = tset - xk;  // calculate e[k]
   if (vrg)
   {
      *yk += (p->k0 * ek);  // y[k] = y[k-1] + k0 * e[k]
      *yk += (p->k1 * ek1); //               + k1 * e[k-1]
      *yk += (p->k2 * ek2); //               + k2 * e[k-2]
   }
   else *yk = 0.0;

   ek2  = ek1;        // e[k-2] = e[k-1]
   ek1  = ek;         // e[k-1] = e[k]

   // limit y[k] to GMA_HLIM and GMA_LLIM
   if (*yk > GMA_HLIM)
   {
      *yk = GMA_HLIM;
   }
   else if (*yk < GMA_LLIM)
   {
      *yk = GMA_LLIM;
   } // else
} // pid_reg2()
