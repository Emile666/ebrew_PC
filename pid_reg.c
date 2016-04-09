/*==================================================================
  Function name: init_pid2(), pid_reg2()
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
  Revision 1.11  2013/07/23 09:42:46  Emile
  - Fourth intermediate version: several Registry Settings added / removed.
  - Dialog Screens updated: better lay-out and matches new Registry Settings
  - Source-code improved for readability

  Revision 1.10  2011/05/29 20:56:26  Emile
  - New Registry variables added: STC_N, STC_TD and STC_ADF
  - PID Settings Dialog screen extended with new parameters for self-tuning
    controller: possibility to set the system order N, an estimate for the
    time-delay and a boolean whether or not to use adaptive dir. forgetting.
  - PID Settings Dialog screen: parameters enabled/disabled when a
    specific PID controller is chosen.
  - New functions time_delay() and init_time_delay() added
  - Changes made in init_pid2() function header.
  - Unit-test cases updated and extended with tests for new functions.

  Revision 1.9  2011/05/14 14:02:19  Emile
  - Unit test set updates, test-case 16 added
  - Self-Tuning controller N=1 and N=2 added to PID dialog screen
  - PID debug label extended with (Kc Ti Td) and sys. id. parameters
  - Max. sample-time extended to SIXTY_SECONDS (was 20 seconds)
  - Help file updated with version history

  Revision 1.8  2011/05/06 11:09:42  Emile
  - pid_reg1(), pid_reg2(), init_pid1(), init_pid2() removed.
  - pid_reg4() changed into pure Takahashi PID controller, no D-filtering anymore.
  - PID dialog updated to reflect changes.

  Revision 1.7  2007/07/06 22:23:02  Emile
  - The real time between two lines from a log-file is now used instead of a
    fixed 5 sec. time when reading a log-file.
  - Leading-zero bug solved in Mash Progress screen
  - i2c_stop() only called with PT_CLOSE in case of exit of program
  - System Identification library functions added (but not used yet).

  Revision 1.6  2004/05/13 20:51:00  emile
  - Main loop timing improved. Only 99 (of 100) cycles were executed. Loop
    timing is now reset after 100 loops (5 seconds)
  - TS parameter now only works on PID-controller time-slice. Max. is 20 sec.
  - Bug-fix in init_ma() filter when init. to a value (should be /N).
  - LPF for D-term of PID controller added. New reg. var. K_LPF
  - PID Debug label added + checkbox in PID screen. Default off (NO reg var).
  - Statusbar object added
  - Start made with network functionality. Not operational yet.

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
#include "misc.h"
#include <math.h>

static double ek_1;  // e[k-1]  = SP[k-1] - PV[k-1] = Tset_hlt[k-1] - Thlt[k-1]
//static double ek_2;  // e[k-2]  = SP[k-2] - PV[k-2] = Tset_hlt[k-2] - Thlt[k-2]
static double xk_1;  // PV[k-1] = Thlt[k-1]
static double xk_2;  // PV[k-2] = Thlt[k-1]
//static double yk_1;  // y[k-1]  = Gamma[k-1]
//static double yk_2;  // y[k-2]  = Gamma[k-1]
static double lpf_1; // lpf[k-1] = LPF output[k-1]
static double lpf_2; // lpf[k-2] = LPF output[k-2]

// The following variables are needed for the system identification
vector theta;    // parameter estimate vector {a1,a2,a3,b1,b2,b3}
vector d;        // regression vector {-y[k-1] -y[k-2] -y[k-3] u[k-1] u[k-2] u[k-3]}
matrix C;        // covariance matrix
double rho;      // rho parameter
double phi;      // directional forgetting factor
double lambda;   // parameter lambda
double eta;      // parameter eta (v)
ma     yk_delay; // delayed values of pid controller

void init_pid2(pid_params *p, sys_id_params *psi)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Self-Tuning PID controller.

             The PID controller is a type C Takahashi PID controller.
             It is used together with on-line system identification
             using the recursive least squares algorithm.
  Variables: p   : pointer to struct containing all PID parameters
             psi : Pointer to struct containing system identification parameters
                   Used to find the order N and the Time-Delay TD
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   int i,j; // loop counters

   init_pid3(p); // identical to init_pid3()
   init_sample_delay(&yk_delay, psi->stc_td); // init. struct for delayed values of y[k]
   // init. parameter estimate vector
   theta[0] = 0.1; // a1 = 0.1
   theta[1] = (psi->N == 1) ? 0.1 : 0.2; // N == 1 => b1 = 0.1 else a2 = 0.2
   theta[2] = (psi->N == 2) ? 0.1 : 0.3; // N == 2 => b1 = 0.1 else a3 = 0.3
   theta[3] = (psi->N == 2) ? 0.2 : 0.1; // N == 2 => b2 = 0.2 else b1 = 0.1
   theta[4] = 0.2;
   theta[5] = 0.3;

   // init. auxiliary parameters
   rho    = 0.99;     // parameter rho
   phi    = 1.0;      // directional forgetting factor
   lambda = 0.001;    // parameter lambda
   eta    = 0.000001; // parameter eta (v)

   for (i = 0; i < 6; i++)
   {
      d[i] = 0.0; // init. regression vector
      for (j = 0; j < 6; j++)
      {
         if (i == j)
              C[i][j] = 1e+9;
         else C[i][j] = 0.0;
      } // for j
   } // for i
} // init_pid2()

void pid_reg2(double xk, double *yk, double tset, pid_params *p, int vrg,
              sys_id_params *psi)
/*------------------------------------------------------------------
  Purpose  : This function implements a Self-Tuning PID controller.
             It contains the following:
             - sys_id(): calculate the a1, b1, a2, b2, ... parameters
                         for the transfer function of the process
             - calc_ultimate_gain_period(): calculate the ultimate gain
                         and period based on the a1, b1, a2, b2, ... parameters
             - calc_pid_parameters(): calculate the PID parameters based on
                         the ultimate gain and period
             - calculate the Takahashi PID controller. Choose between
                         the on-line parameters or the default parameters
             The PID control algorithm is identical to pid_reg4(), which
             is a Takahashi Type C with no filtering of the D-action.
             This function should be called once every TS seconds.
  Variables:
        xk : The input variable x[k] (= measured temperature)
       *yk : The output variable y[k] (= gamma value for power electronics)
      tset : The setpoint value for the temperature
        *p : Pointer to struct containing PID parameters
        vrg: Release signal: 1 = Start control, 0 = disable PID controller
      *psi : Pointer to struct containing system identification parameters
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   double yk_TD; // value of y[k-TD]

   if (vrg)
   {
      yk_TD = sample_delay(&yk_delay, *yk);    // get value of y[k-TD]
      sys_id(yk_TD, xk, psi->N, psi->stc_adf); // calc. parameters for transfer function
      calc_ultimate_gain_period(psi, p->ts);   // calc. Kpu and Tu
      calc_pid_parameters(psi, p->ts);         // calc. PID parameters from Kpu and Tu

      //--------------------------------------------------------------------------------
      // Takahashi Type C PID controller (NO filtering of D-action):
      //
      //                                    Kc.Ts        Kc.Td
      // y[k] = y[k-1] + Kc.(x[k-1]-x[k]) + -----.e[k] + -----.(2.x[k-1]-x[k]-x[k-2])
      //                                      Ti           Ts
      //
      // Use internal parameters Kc, Ki, Kd calc. from calc_pid_parameters()
      //--------------------------------------------------------------------------------
      p->pp = psi->kc * (xk_1 - xk);              // Kc.(x[k-1]-x[k])
      p->pi = psi->ki * (tset - xk);              // (Kc.Ts/Ti).e[k]
      p->pd = psi->kd * (2.0 * xk_1 - xk - xk_2); // (Kc.Td/Ts).(2.x[k-1]-x[k]-x[k-2])
      *yk  += p->pp + p->pi + p->pd;          // add y[k-1] + P, I & D actions to y[k]
   }
   else { *yk = p->pp = p->pi = p->pd = 0.0; }

   xk_2  = xk_1;
   xk_1  = xk;    // PV[k-1] = PV[k]

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

void init_pid3(pid_params *p)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Type A PID controller.
             It also utilizes filtering of the D-action.
  Variables: p: pointer to struct containing all PID parameters

                   Kc.Ts
             k0 =  -----   (for I-term)
                    Ti

                       Td
             k1 = Kc . --  (for D-term)
                       Ts

             The parameters for lowpass-filtering of the D-action
             are also initialised here:
             lpf[k] = lpf1 * lpf[k-1] + lpf2 * lpf[k-2]
             
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   p->ts_ticks = (int)((p->ts * 1000.0) / T_50MSEC);
   if (p->ts_ticks > SIXTY_SECONDS)
   {
      p->ts_ticks = SIXTY_SECONDS;
   } // if
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
  Purpose  : This function implements the type A PID controller.
             It also utilizes filtering of the D-action.
             All terms are dependent on the error signal e[k].
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
      //-----------------------------------------------------------------------------------
      // Type A PID controller with filtering of D-action
      //
      //                                    Kc.Ts        Kc.Td
      // y[k] = y[k-1] + Kc.(e[k]-e[k-1]) + -----.e[k] + -----.(lpf[k]-2.lpf[k-1]+lpf[k-2])
      //                                      Ti           Ts
      //
      // lpf[k] is the lowpass filtered version of e[k]
      //
      //-----------------------------------------------------------------------------------
      p->pp = p->kc * (ek - ek_1);                 // Kc.(e[k]-e[k-1])
      p->pi = p->k0 * ek;                          // (Kc.Ts/Ti).e[k]
      p->pd = p->k1 * (lpf - 2.0 * lpf_1 + lpf_2); // (Kc.Td/Ts).(lpf[k]-2.lpf[k-1]+lpf[k-2])
      *yk  += p->pp + p->pi + p->pd;               // add y[k-1] + P, I & D actions to y[k]
   }
   else { *yk = p->pp = p->pi = p->pd = 0.0; }

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
  Purpose  : This function initialises the Takahashi Type C PID
             controller.
  Variables: p: pointer to struct containing all PID parameters
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   init_pid3(p); // identical to init_pid3()
} // init_pid4()

void pid_reg4(double xk, double *yk, double tset, pid_params *p, int vrg)
/*------------------------------------------------------------------
  Purpose  : This function implements the Takahashi Type C PID 
             controller: the P and D term are no longer dependent
             on the set-point, only on PV (which is Thlt).
             The D term is NOT low-pass filtered.
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
   if (vrg)
   {
      //--------------------------------------------------------------------------------
      // Takahashi Type C PID controller (NO filtering of D-action):
      //
      //                                    Kc.Ts        Kc.Td
      // y[k] = y[k-1] + Kc.(x[k-1]-x[k]) + -----.e[k] + -----.(2.x[k-1]-x[k]-x[k-2])
      //                                      Ti           Ts
      //
      //--------------------------------------------------------------------------------
      p->pp = p->kc * (xk_1 - xk);              // Kc.(x[k-1]-x[k])
      p->pi = p->k0 * (tset - xk);              // (Kc.Ts/Ti).e[k]
      p->pd = p->k1 * (2.0 * xk_1 - xk - xk_2); // (Kc.Td/Ts).(2.x[k-1]-x[k]-x[k-2])
      *yk  += p->pp + p->pi + p->pd;            // add y[k-1] + P, I & D actions to y[k]
   } // if
   else { *yk = p->pp = p->pi = p->pd = 0.0; }

   xk_2  = xk_1;  // x[k-2] = x[k-1]
   xk_1  = xk;    // x[k-1] = x[k]

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

void sys_id(double uk, double yk, int N, int use_adf)
/*------------------------------------------------------------------
  Purpose  : This function performs system identification for a
             second or third order process.
             The transfer function of the model is the following:

                        Y(z^-1)       b1*z^-1 + b1*z^-2 + b2*z^-3
             Gs(z^-1) = ------- = ---------------------------------
                        U(z^-1)     1 + p1*z^-1 + p2*z^-2 + p3*z^-3

             This function is supplied with the input values u[k] and
             the output values y[k] and calculates the values for
             b1, b2, a1 and a2.

             It uses the Recursive Least Square (RLS) method,
             combined with adaptive directional forgetting.
  Variables:
        uk : The process input (= PID output) = u[k]
        yk : The process output y[k] (= measured temperature)
         N : order of process [1 | 2 | 3]
   use_adf : 1 = use adaptive directional forgetting
  Returns  : No values are returned, but the global vector theta,
             containing the parameter estimates, is updated with the
             new values.
  ------------------------------------------------------------------*/
{
   double ep;       // prediction error
   double ksi;      // auxiliary scalar ksi
   double eps;      // auxiliary parameter epsilon
   double te;       // parameter theta
   vector tmp;      // temporary vector
   vector tmp1;     // temporary vector
   double xtmp;     // temporary scalar
   matrix mtmp;     // temporary matrix
   int    nn = 2*N; // order 2N

   // cyclic data substitution in regression vector (1)
   if (N == 1)
   {
      d[1] = uk;   //  u[k-1]
   }
   else if (N == 2)
   {
      d[3] = d[2]; //  u[k-2]
      d[2] = uk;   //  u[k-1]
   }
   else // N == 3
   {
      d[5] = d[4]; //  u[k-3]
      d[4] = d[3]; //  u[k-2]
      d[3] = uk;   //  u[k-1]
   } // else

   // Update of parameter estimates
   ep = yk - vxv2s(theta,d,nn);    // Matlab: ep  = y - theta' * d (eq.29)
   vxm2v(d,C,&tmp,nn);             // Matlab: tmp = d' * c
   ksi = vxv2s(tmp,d,nn);          // Matlab: ksi = d' * c * d (eq.28)
   mxv2v(C,d,&tmp,nn);             // Matlab: tmp = c*d
   vxs2v(&tmp,ep / (1 + ksi),nn);  // Matlab: tmp = (c*d/(1+ks))*ep
   vav2v(&theta,tmp,nn);           // Matlab: theta = theta + tmp (eq.27)

   // Update of identification variables
   if (ksi > 0)
   {
      eps = phi - (1.0 - phi) / ksi;  // eq.31
      eps = 1.0 / eps;                // inv(eps)
      mxv2v(C,d,&tmp,nn);             // Matlab: c*d (eq.30)
      vxm2v(d,C,&tmp1,nn);            // Matlab: d'*c
      vxv2m(tmp,tmp1,&mtmp,nn);       // Matlab: c*d*d'*c
      mxs2m(&mtmp,-1.0/(eps+ksi),nn); // Matlab: -c*d*d'*c/(inv(eps)+ksi)
      mam2m(&C,mtmp,nn);              // Matlab: c = c-c*d*d'*c/(inv(eps)+ksi)
   } // if

   if (!use_adf) phi = 1.0; // no adaptive directional forgetting
   else
   {
      lambda = phi * (lambda + ep * ep / (1.0 + ksi)); // eq.36
      eta    = phi * (eta + 1.0);                      // eq.35
      te     = ep * ep / lambda;                       // eq.34
      xtmp   = (eta + 1.0) * te / (1.0 + ksi + te) - 1.0;
      xtmp  *= ksi / (1.0 + ksi);

      // the next 2 lines contains the formule found in sid.m
      //xtmp  += 1.0 + ksi;
      //phi    = 1.0 + (1.0 + rho) * log(xtmp);          // eq.33 (log = ln function)
      //phi    = 1.0 / phi;                              // eq.33
      // the next line contains the original formula in the book
      xtmp  += 1.0 + (1.0 + rho) * log(1.0 + ksi);
      phi    = 1.0 / xtmp;
   } // else

   // cyclic data substitution in regression vector (2)
   if (N == 1)
   {
      d[0] = -yk;  // -y[k-1]
   }
   else if (N == 2)
   {
      d[1] = d[0]; // -y[k-2]
      d[0] = -yk;  // -y[k-1]
   }
   else // N == 3
   {
      d[2] = d[1]; // -y[k-3]
      d[1] = d[0]; // -y[k-2]
      d[0] = -yk;  // -y[k-1]
   } // else
} // sys_id()

int calc_ultimate_gain_period(sys_id_params *p, double ts)
/*------------------------------------------------------------------
  Purpose  : This function computes the ultimate gain and the
             corresponding ultimate period for a discrete model of
             order N

                        Y(z^-1)       b1*z^-1 + b1*z^-2 + b2*z^-3
             Gs(z^-1) = ------- = ---------------------------------
                        U(z^-1)     1 + a1*z^-1 + a2*z^-2 + a3*z^-3

             Precondition: the vector theta (containing the parameter
                           estimates) MUST have a value!
  Variables:
         p : pointer to struct with sys_id parameters
        ts : sample period
  Returns  : 1= could not compute ultimate gain (defaults are returned)
             0= ultimate gain and period are calculated successfully
  ------------------------------------------------------------------*/
{
   double a1,a2,a3,b1,b2,b3; // estimated parameters
   double k_c[2]; // gain for complex roots
   double k_r;    // gain for root -1
   double alfa[2];
   double f  = 1.0; // var. used for calc. of poles
   double f1 = 1.0; // var. used for calc. of poles
   double f2 = 1.0; // var. used for calc. of poles
   double r0,r1,r2,D; // temp. variables
   int    err = 0;    // return value, assume no error
   int    i;          // loop counter

   switch (p->N)
   {
      case 1: // first order process
           a1 = theta[0];
           b1 = theta[1];
           if (b1 != 0.0)
           {
              p->kpu = (1.0 - a1) / b1;
           }
           else
           {
              err    = 1; // error
              p->kpu = 1.0;
           } // else
           p->tu  = 2.0 * ts;
           break;

      case 2: // second order process
           a1 = theta[0]; a2 = theta[1];
           b1 = theta[2]; b2 = theta[3];
           k_c[0] = 0.0; // gain for complex roots
           if (b2 != 0.0)
           {
              k_c[0]  = (1.0 - a2) / b2;
              alfa[0] = (a1 + k_c[0] * b1) / -2.0;
              if ((alfa[0] < -1.0) || (alfa[0] >= 1.0))
              {
                 k_c[0] = 0.0; // can't compute gain for complex roots
              } // if
           } // if
           k_r = 0.0; // gain for real roots
           if (b1 != b2)
           {
              k_r = (1.0 - a1 + a2) / (b1 - b2);
              // check that second root is stable
              f = a1 + k_r * b1 - 1.0;
              if ((f < -1.0) || (f > 1.0))
              {
                 k_r = 0.0; // can't compute gain for root -1
              } // if
           } // if
           // Use 2 complex roots if possible
           if ((k_c[0] > 0.0) || ((k_c[0] < 0) && (k_r <= 0.0)))
           {
              p->kpu = k_c[0];
              p->tu  = ts * 2.0 * PI / acos(alfa[0]);
           }
           else if (k_r != 0.0)
           {
              p->kpu = k_r;
              p->tu  = ts * 2.0;
           }
           else
           {
              err    = 1; // error!
              p->kpu = 1.0;
              p->tu  = 2.0 * ts;
           } // else
           break;

      case 3: // third order process
           a1 = theta[0]; a2 = theta[1]; a3 = theta[2];
           b1 = theta[3]; b2 = theta[4]; b3 = theta[5];
           k_c[0] = k_c[1] = 0.0; // gain for complex roots
           r2     = b3 * (b1 - b3);
           r1     = b3 * (a1 - 2.0 * a3) + a3 * b1 - b2;
           r0     = a3 * (a1 - a3) - a2 + 1.0;
           if (r2 != 0.0)
           {
              D = r1 * r1 - 4.0 * r2 * r0;
              if (D >= 0.0)
              {
                 D      = sqrt(D);
                 k_c[0] = (-r1 + D) / (2.0 * r2);
                 k_c[1] = (-r1 - D) / (2.0 * r2);
              } // if
           } // if
           else if (r1 != 0.0)
           {
              k_c[1] = -r0 / r1;
           } // else if
           for (i = 0; i < 2; i++)
           {  // check 3rd root
              f = a3 + k_c[i] * b3;
              if ((f < -1.0) || (f >= 1.0))
              {
                 k_c[i] = 0.0;
              }
              else
              {  // check real parts of ultimate roots
                 alfa[i] = (a3 - a1 + k_c[i] * (b3 - b1)) / 2.0;
                 if ((alfa[i] < -1.0) || (alfa[i] >= 1.0))
                 {
                    k_c[i] = 0.0; // can't compute gain for complex roots
                 } // if
              } // else
           } // for i

           k_r = 0.0; // gain for real roots
           if (b1 - b2 + b3 != 0.0)
           {
              k_r = (1.0 - a1 + a2 - a3) / (b1 - b2 + b3);
              r1  = a1 + k_r * b1 - 1.0; // = f1
              r0  = a3 + k_r * b3;       // = f2
              D   = r1*r1 - 4.0 * r0;
              if (D >= 0)
              {
                 // check that the other 2 roots are stable
                 D  = sqrt(D);
                 f1 = (-r1 + D) / 2.0;
                 f2 = (-r1 - D) / 2.0;
              } // if
              if ((f1 < -1.0) || (f1 >= 1.0) || (f2 < -1.0) || (f2 >= 1.0))
              {
                 k_r = 0.0;
              } // if
           } // if
           // Use 2 complex roots if possible
           if ((k_c[0] > 0.0) || ((k_c[0] < 0.0) && (k_c[1] <= 0.0) && (k_r <= 0.0)))
           {
              p->kpu = k_c[0];
              p->tu  = ts * 2.0 * PI  / acos(alfa[0]);
           }
           else if ((k_c[1] > 0.0) || ((k_c[1] < 0.0) && (k_r <= 0.0)))
           {
              p->kpu = k_c[1];
              p->tu  = ts * 2.0 * PI  / acos(alfa[1]);
           }
           else if (k_r != 0.0)
           {
              p->kpu = k_r;
              p->tu  = ts * 2.0;
           }
           else
           {
              err    = 1; // error!
              p->kpu = 1.0;
              p->tu  = ts * 2.0;
           } // else
           break;
   } // switch
   return err;
} // calc_ultimate_gain_period()

void calc_pid_parameters(sys_id_params *p, double ts)
/*------------------------------------------------------------------
  Purpose  : This function computes both the internal parameters for
             the Takahashi PID controller AND the Kc, Ti and Td
             parameters.
             The calculation of these parameters are calculated from
             the ultimate gain and the corresponding ultimate period.
             These ultimate parameters are the result of the
             system identification algorithm. This algorithm is executed
             by calling sys_id() and calc_ultimate_gain_period().

             Internal parameters          External parameters
             ---------------------        ------------------------
             Kr = 0.6*Kpu - Ki/2          Kc = 0.6*Kpu*(Tu-Ts)/Tu
             Ki = 1.2*Kpu*Ts/Tu           Ti = (Tu-Ts)/2
             Kd = 3*Kpu*Tu/(40*Ts)        Td = Tu^2 / (8*(Tu-Ts))

  Variables:
       kpu : ultimate gain
        tu : ultimate period
         p : pointer to the sys_id_params structs, where the calculated
             parameters of this function are stored
  Returns  : -
  ------------------------------------------------------------------*/
{
   // internal parameters
   p->ki = 1.2 * p->kpu * ts / p->tu;
   p->kr = 0.6 * p->kpu - 0.5 * p->ki;
   p->kd = 3.0 * p->kpu * p->tu / (40.0 * ts);

   // external parameters
   p->kc = 0.6 * p->kpu * (p->tu - ts) / p->tu;
   p->ti = 0.5 * (p->tu - ts);
   p->td = p->tu  * p->tu / (8.0 * (p->tu - ts));
} // calc_pid_parameters()

double vxv2s(vector vec1, vector vec2, int N)
/*------------------------------------------------------------------
  Purpose  : Vector x Vector To Scalar
	     This function calculates the inner product of two
             vectors and returns the result as a scalar.
  Variables:
      vec1 : The first vector
      vec2 : The second vector
         N : order of process [1 | 2 | 3]
  Returns  : the inner product of the two vectors
  ------------------------------------------------------------------*/
{
   int i;               // loop counter
   double result = 0.0; // the result

   for (i = 0; i < N; i++)
   {
      result += vec1[i] * vec2[i];
   } // for i
   return result;
} // vxv2s()

void vxv2m(vector vec1, vector vec2, matrix *m, int N)
/*------------------------------------------------------------------
  Purpose  : Vector x Vector To Matrix
	     This function calculates the product of two
             vectors and returns the result as a matrix.
  Variables:
      vec1 : The first vector
      vec2 : The second vector
         m : The matrix with the result
         N : order of process [1 | 2 | 3]
  Returns  : -
  ------------------------------------------------------------------*/
{
   int i,j;               // loop counter

   for (i = 0; i < N; i++)
   {
      for (j = 0; j < N; j++)
      {
	     (*m)[i][j] = vec1[i] * vec2[j];
      } // for j
   } // for i
} // vxv2m()

 void vxm2v(vector vec1, matrix m1, vector *vecr, int N)
/*------------------------------------------------------------------
  Purpose  : Vector Transposed x Matrix To Vector
	     This function calculates the product of a vector with
             a matrix. The result is a vector.
  Variables:
      vec1 : The vector
         m : The matrix
      vecr : The result as a vector
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i,j; // loop counters

   for (i = 0; i < N; i++)
   {
      (*vecr)[i] = 0.0;
      for (j = 0; j < N; j++)
      {
         (*vecr)[i] += vec1[j] * m1[j][i];
      } // for j
   } // for i
} // vxm2v()

void mxv2v(matrix m1, vector vec1, vector *vecr, int N)
/*------------------------------------------------------------------
  Purpose  : Matrix x Vector To Vector
	     This function calculates the product of a matrix with
             a vector. The result is a vector.
  Variables:
         m : The matrix
      vec1 : The vector
      vecr : The result as a vector
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i,j; // loop counters

   for (i = 0; i < N; i++)
   {
      (*vecr)[i] = 0.0;
      for (j = 0; j < N; j++)
      {
         (*vecr)[i] += vec1[j] * m1[i][j];
      } // for j
   } // for i
} // mxv2v()

void vxs2v(vector *vec, double x, int N)
/*------------------------------------------------------------------
  Purpose  : Vector x Scalar To Vector
	     This function multiplies every vectorcomponent with the
             scalar x. The result is returned in vec.
  Variables:
       vec : The vector
         x : The scalar to multiply with (a double)
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i; // loop counter

   for (i = 0; i < N; i++)
   {
      (*vec)[i] *= x;
   } // for i
} // vxs2v()

void mxs2m(matrix *m, double x, int N)
/*------------------------------------------------------------------
  Purpose  : Matrix x Scalar To Matrix
	     This function multiplies every matrixcomponent with the
             scalar x. The result is returned in m.
  Variables:
         m : The matrix
         x : The scalar to multiply with (a double)
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i,j; // loop counters

   for (i = 0; i < N; i++)
   {
      for (j = 0; j < N; j++)
      {
         (*m)[i][j] *= x;
      } // for j
   } // for i
} // mxs2m()

void mam2m(matrix *m1, matrix m2, int N)
/*------------------------------------------------------------------
  Purpose  : Matrix Add Matrix To Matrix
             This function adds two matrices together.
             The result is returned in m1.
  Variables:
        m1 : The 1st matrix. Is also used to return the result
        m2 : The 2nd matrix
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i,j; // loop counters

   for (i = 0; i < N; i++)
   {
      for (j = 0; j < N; j++)
      {
         (*m1)[i][j] += m2[i][j];
      } // for j
   } // for i
} // mam2m()

void vav2v(vector *vec1, vector vec2, int N)
/*------------------------------------------------------------------
  Purpose  : Vector Add Vector To Vector
		     This function adds two vectors together.
             The result is returned in vec1.
  Variables:
      vec1 : The 1st vector. Is also used to return the result
      vec2 : The 2nd vector
         N : order of process [1 | 2 | 3]
  Returns  : none
  ------------------------------------------------------------------*/
{
   int i; // loop counter

   for (i = 0; i < N; i++)
   {
      (*vec1)[i] += vec2[i];
   } // for i
} // vav2v()




