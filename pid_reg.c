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

static double ek_1; // e[k-1]  = SP[k-1] - PV[k-1] = Tset_hlt[k-1] - Thlt[k-1]
static double ek_2; // e[k-2]  = SP[k-2] - PV[k-2] = Tset_hlt[k-2] - Thlt[k-2]
static double xk_1; // PV[k-1] = Thlt[k-1]
static double xk_2; // PV[k-2] = Thlt[k-1]
static double yk_1; // y[k-1]  = Gamma[k-1]
static double yk_2; // y[k-2]  = Gamma[k-1]

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
                           Ts      Td
             k0 = Kc.(1 + ---- + 2.--)
                          2.Ti     Ts

                      Ts   4.Td
             k1 = Kc.(-- - ----)
                      Ti    Ts

                            Ts    2.Td
             k2 = Kc.(-1 + ---- + ----)
                           2.Ti    Ts

  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ti == 0.0)
   {
      p->k0 = p->kc * (+1.0 + (2.0 * p->td / p->ts));
      p->k1 = p->kc * (-4.0 * p->td / p->ts);
      p->k2 = p->kc * (-1.0 + (2.0 * p->td / p->ts));
   }
   else
   {
      p->k0 = p->kc * (+1.0 + (p->ts / (2.0 * p->ti)) + (2.0 * p->td / p->ts));
      p->k1 = p->kc * ((p->ts / p->ti) -4.0 * p->td / p->ts);
      p->k2 = p->kc * (-1.0 + (p->ts / (2.0 * p->ti)) + (2.0 * p->td / p->ts));
   } // else
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
      *yk  = yk_2 + (p->k0 * ek); // y[k] = y[k-2] + k0 * e[k]
      *yk += (p->k1 * ek_1);      //               + k1 * e[k-1]
      *yk += (p->k2 * ek_2);      //               + k2 * e[k-2]
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

                          Ts   Td                       2.Td
             k0 = Kc.(1 + -- + --)        k1 = -Kc.(1 + ----)
                          Ti   Ts                        Ts

                       Td
             k2 = Kc . --
                       Ts

  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ti == 0.0)
   {
      p->k0 = p->kc * (1.0 + (p->td / p->ts));
   }
   else
   {
      p->k0 = p->kc * (1.0 + (p->ts / p->ti) + (p->td / p->ts));
   } // else
   p->k1 = p->kc * (-1.0 - 2.0 * p->td / p->ts);
   p->k2 = p->kc * p->td / p->ts;
} // init_pid3()

void pid_reg3(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the type Allen Bradley Type A PID
             controller.
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

   ek = tset - xk;  // calculate e[k] = SP[k] - PV[k]
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
} // pid_reg3()

void init_pid4(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Allen Bradley Type C PID
             controller.
  Variables: p: pointer to struct containing all PID parameters

                  Kc.Ts                      Td
             k0 = -----            k1 = -Kc.(-- + 1)
                    Ti                       Ts

                      2.Td                Kc.Td
             k2 = Kc.(---- + 1)    k3 = - -----
                       Ts                   Ts

  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ti == 0.0)
   {
      p->k0 = 0.0;
   }
   else
   {
      p->k0 = p->kc * p->ts / p->ti;
   } // else

   p->k1 = p->kc * (-1.0 - (p->td / p->ts));
   p->k2 = p->kc * (+1.0 + (2.0 * p->td / p->ts));
   p->k3 = p->kc *  -1.0 * p->td / p->ts;
} // init_pid4()

void pid_reg4(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Allen Bradley Type C PID
             controller.
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

   ek = tset - xk;  // calculate e[k] = SP[k] - PV[k]
   if (vrg)
   {
      *yk += (p->k0 * ek);   // y[k] = y[k-1] + k0 * e[k]
      *yk += (p->k1 * xk);   //               + k1 * PV[k]
      *yk += (p->k2 * xk_1); //               + k2 * PV[k-1]
      *yk += (p->k3 * xk_2); //               + k2 * PV[k-2]
   }
   else *yk = 0.0;

   xk_2  = xk_1; // PV[k-2] = PV[k-1]
   xk_1  = xk;   // PV[k-1] = PV[k]

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
