/*==================================================================
  Function name: init_pid1(), pid_reg1(), init_pid2(), pid_reg2()
                 init_pid3(), pid_reg3(), init_pid4(), pid_reg4()
  Author       : E. vd Logt
  File name    : $Id$
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
  $Log$
  Revision 1.5  2004/05/10 20:54:30  emile
  - Bug-fix: log-file header: '\n' was removed, is corrected now
  - Hints added to PID dialog screen
  - Now four PID controllers to choose from. Old ebrew version is still listed,
    but should not be used anymore. Simulation showed stability problems.
    Preferably use the type C controller.

  Revision 1.4  2004/04/26 13:30:22  emile
  Bug-fix: init_pid3() did not calculate correctly when Ti = 0. Corrected.

  Revision 1.3  2004/04/25 14:02:17  emile
  - Added a 'type C' PID controller, function pid_reg3(). Possible to select
    this from the PID settings dialog screen. Left the old one in for
    compatibility. New registry variable PID_Model.
  - Gamma added to log-file, so that the PID controller can be tuned.

  Revision 1.2  2002/12/30 13:33:45  emile
  - Headers with CVS tags added to every source file
  - Restore Settings function is added
  - "ebrew" registry key now in a define REGKEY

  ==================================================================
*/
#include "pid_reg.h"

static double ek_1;  // e[k-1]  = SP[k-1] - PV[k-1] = Tset_hlt[k-1] - Thlt[k-1]
static double ek_2;  // e[k-2]  = SP[k-2] - PV[k-2] = Tset_hlt[k-2] - Thlt[k-2]
static double xk_1;  // PV[k-1] = Thlt[k-1]
static double xk_2;  // PV[k-2] = Thlt[k-1]
static double yk_1;  // y[k-1]  = Gamma[k-1]
static double yk_2;  // y[k-2]  = Gamma[k-1]
static double lpf_1; // lpf[k-1] = LPF output[k-1]
static double lpf_2; // lpf[k-2] = LPF output[k-2]

void init_pid1(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the PID controller, based on
             implementation method 2 (Bilinear Transformation with Taylor series)
             Do not use this version!
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
   if (p->ts_ticks > TWENTY_SECONDS)
   {
      p->ts_ticks = TWENTY_SECONDS;
   }
   if (p->ti == 0.0)
   {
      p->k0 = p->kc * (+1.0 + (2.0 * p->td / p->ts));
      p->k1 = p->kc * (-1.0 - (6.0 * p->td / p->ts));
   }
   else
   {
      p->k0 = p->kc * (+1.0 + (p->ts / (2.0 * p->ti)) + (2.0 * p->td / p->ts));
      p->k1 = p->kc * (-1.0 + (p->ts / (2.0 * p->ti)) - (6.0 * p->td / p->ts));
   } // else
   p->k2 = p->kc * 8.0 * p->td / p->ts;
} // init_pid1()

void pid_reg1(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the PID controller, conform
             implementation method 2 ("Bilinear Transformation with Taylor Series").
             Do not use this version!
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
      *yk += (p->k0 * ek);   // y[k] = y[k-1] + k0 * e[k]
      *yk += (p->k1 * ek_1); //               + k1 * e[k-1]
      *yk += (p->k2 * ek_2); //               + k2 * e[k-2]
   }
   else *yk = 0.0;

   ek_2  = ek_1; // e[k-2] = e[k-1]
   ek_1  = ek;   // e[k-1] = e[k]

   // limit y[k] to GMA_HLIM and GMA_LLIM
   if (*yk > GMA_HLIM)
   {
      *yk = GMA_HLIM;
   }
   else if (*yk < GMA_LLIM)
   {
      *yk = GMA_LLIM;
   } // else
} // pid_reg1()

void init_pid2(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the PID controller, based on
             the new Type A PID controller. Update of pid_reg1().
  Variables: p: pointer to struct containing all PID parameters
                           Ts        2.Td
             k0 = Kc.(1 + ---- + ------------)
                          2.Ti   Ts + 2.k_lpf

                                  Ts.(alfa + 1)       4.Td
             k1 = Kc.(alfa - 1 + -------------- - ------------)
                                       2.Ti       Ts + 2.k_lpf

                              alfa.Ts       2.Td
             k2 = Kc.(-alfa + ------- + ------------)
                                2.Ti    Ts + 2.k_lpf

                         Ts - 2.k_lpf
             with alfa = ------------
                         Ts + 2.k_lpf

  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   double alfa = (p->ts - 2.0 * p->k_lpf) / (p->ts + 2.0 * p->k_lpf); // help variable

   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ts_ticks > TWENTY_SECONDS)
   {
      p->ts_ticks = TWENTY_SECONDS;
   }
   if (p->ti == 0.0)
   {
      p->k0 = p->kc * (+1.0 + (2.0 * p->td / (p->ts + 2.0 * p->k_lpf)));
      p->k1 = p->kc * (alfa - 1.0 - (4.0 * p->td / (p->ts + 2.0 * p->k_lpf)));
      p->k2 = p->kc * (-alfa + (2.0 * p->td / (p->ts + 2.0 * p->k_lpf)));
   }
   else
   {
      p->k0 = p->kc * (+1.0 + (p->ts / (2.0 * p->ti))
                            + (2.0 * p->td / (p->ts + 2.0 * p->k_lpf))
                      );
      p->k1 = p->kc * (alfa - 1.0 +
                       (p->ts * (alfa + 1.0) / (2.0 * p->ti)) -
                       4.0 * p->td / (p->ts + 2.0 * p->k_lpf)
                      );
      p->k2 = p->kc * (-alfa + (alfa * p->ts / (2.0 * p->ti))
                                 + (2.0 * p->td / (p->ts + 2.0 * p->k_lpf))
                      );
   } // else
   //--------------------------------------------------
   // y[k] = (1- alfa)*y[k-1] + alfa*y[k-2] + ....
   //--------------------------------------------------
   p->lpf1 = 1.0 - alfa;
   p->lpf2 = alfa;
} // init_pid2()

void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the updated PID controller.
             It is an update of pid_reg1(), derived with Bilinear
             Transformation. It is a Type A controller.
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
      *yk  = p->lpf1 * yk_1 + p->lpf2 * yk_2; // y[k] = (1-alfa)*y[k-1] + alfa*y[k-2]
      *yk += p->k0 * ek;                      //  ... + k0 * e[k]
      *yk += p->k1 * ek_1;                    //  ... + k1 * e[k-1]
      *yk += p->k2 * ek_2;                    //  ... + k2 * e[k-2]
   }
   else *yk = 0.0;

   ek_2 = ek_1; // e[k-2] = e[k-1]
   ek_1 = ek;   // e[k-1] = e[k]

   // limit y[k] to GMA_HLIM and GMA_LLIM
   if (*yk > GMA_HLIM)
   {
      *yk = GMA_HLIM;
   }
   else if (*yk < GMA_LLIM)
   {
      *yk = GMA_LLIM;
   } // else

   yk_2 = yk_1; // y[k-2] = y[k-1]
   yk_1 = *yk;  // y[k-1] = y[k]
} // pid_reg2()

void init_pid3(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Allen Bradley Type A PID
             controller.
  Variables: p: pointer to struct containing all PID parameters

                   Kc.Ts
             k0 =  -----   (for I-term)
                    Ti

                       Td
             k1 = Kc . --  (for D-term)
                       Ts

             The LPF parameters are also initialised here:
             lpf[k] = lpf1 * lpf[k-1] + lpf2 * lpf[k-2]
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ts_ticks > TWENTY_SECONDS)
   {
      p->ts_ticks = TWENTY_SECONDS;
   }
   if (p->ti == 0.0)
   {
      p->k0 = 0.0;
   }
   else
   {
      p->k0 = p->kc * p->ts / p->ti;
   } // else
   p->k1   = p->kc * p->td / p->ts;
   p->lpf1 = (2.0 * p->k_lpf - p->ts) / (2.0 * p->k_lpf + p->ts);
   p->lpf2 = p->ts / (2.0 * p->k_lpf + p->ts);
} // init_pid3()

void pid_reg3(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the type Allen Bradley Type A PID
             controller. All terms are dependent on the error signal e[k].
             The D term is also low-pass filtered.
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
   double ek;  // e[k]
   double lpf; //LPF output

   ek = tset - xk;  // calculate e[k] = SP[k] - PV[k]
   //--------------------------------------
   // Calculate Lowpass Filter for D-term
   //--------------------------------------
   lpf = p->lpf1 * lpf_1 + p->lpf2 * (ek + ek_1);

   if (vrg)
   {
      //-----------------------------------------------------------
      // Calculate PID controller:
      // y[k] = y[k-1] + Kc*(e[k] - e[k-1] +
      //                     Ts*e[k]/Ti +
      //                     Td/Ts*(lpf[k] - 2*lpf[k-1]+lpf[k-2]))
      //-----------------------------------------------------------
      p->pp = p->kc * (ek - ek_1);  // y[k] = y[k-1] + Kc*(e[k] - e[k-1])
      p->pi = p->k0 * ek;           //      + Kc*Ts/Ti * e[k]
      p->pd = p->k1 * (lpf - 2.0 * lpf_1 + lpf_2);
      *yk += p->pp + p->pi + p->pd;
   }
   else *yk = 0.0;

   ek_1  = ek;    // e[k-1] = e[k]
   lpf_2 = lpf_1; // update stores for LPF
   lpf_1 = lpf;

   // limit y[k] to GMA_HLIM and GMA_LLIM
   if (*yk > GMA_HLIM)
   {
      *yk = GMA_HLIM;
   }
   else if (*yk < GMA_LLIM)
   {
      *yk = GMA_LLIM;
   } // else
} // pid_reg3()

void init_pid4(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Allen Bradley Type C PID
             controller.
  Variables: p: pointer to struct containing all PID parameters
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   init_pid3(p); // identical to init_pid3()
} // init_pid4()

void pid_reg4(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Allen Bradley Type C PID
             controller, the P and D term are no longer dependent on
             the set-point, only on PV (which is Thlt).
             The D term is also low-pass filtered.
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
   double ek;  // e[k]
   double lpf; //LPF output

   ek = tset - xk;  // calculate e[k] = SP[k] - PV[k]
   //---------------------------------------------------------------
   // Calculate Lowpass Filter for D-term: use x[k] instead of e[k]!
   //---------------------------------------------------------------
   lpf = p->lpf1 * lpf_1 + p->lpf2 * (xk + xk_1);

   if (vrg)
   {
      //-----------------------------------------------------------
      // Calculate PID controller:
      // y[k] = y[k-1] - Kc*(PV[k] - PV[k-1] +
      //                     Ts*e[k]/Ti -
      //                     Td/Ts*(lpf[k] - 2*lpf[k-1]+lpf[k-2]))
      //-----------------------------------------------------------
      p->pp = -p->kc * (xk - xk_1);  // y[k] = y[k-1] - Kc*(PVk - PVk-1)
      p->pi = p->k0 * ek;            //      + Kc*Ts/Ti * e[k]
      p->pd = -p->k1 * (lpf - 2.0 * lpf_1 + lpf_2);
      *yk += p->pp + p->pi + p->pd;
   }
   else { *yk = p->pp = p->pi = p->pd = 0.0; }

   xk_1  = xk;    // PV[k-1] = PV[k]
   lpf_2 = lpf_1; // update stores for LPF
   lpf_1 = lpf;

   // limit y[k] to GMA_HLIM and GMA_LLIM
   if (*yk > GMA_HLIM)
   {
      *yk = GMA_HLIM;
   }
   else if (*yk < GMA_LLIM)
   {
      *yk = GMA_LLIM;
   } // else
} // pid_reg4()
