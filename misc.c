/*==================================================================
  Function name: read_input_file(), update_tset(), init_ma(),
                 moving_average()
  File name    : misc.c
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains miscellaneous functions
  ------------------------------------------------------------------
  ==================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pid_reg.h" /* for TS */
#include "misc.h"

int read_input_file(char *inf, maisch_schedule ms[], int *count, double ts)
/*------------------------------------------------------------------
  Purpose  : The purpose of this routine is to read the input file
             specified by 'inf'. Each line in inf consists of two
             real data elements, separated by a ','. The result is
             placed in the linked list.
  Variables:
       inf : A string containing the input file name
       ms[]: Array of structs holding the maisch schedule (temp & times)
     count : The total number of temp & time values read
         ts: The sample time in seconds
  Returns  : TRUE : No errors found
             FALSE: An error occurred
  ------------------------------------------------------------------*/
{
   FILE   *f1;        // file descriptor
   char   tmp[SLEN];  // array that holds one line of the input file
   int    ret;        // return value
   int    i;          // loop_counter
   char   *p;         // temporary pointer

   if ((f1 = fopen(inf,"r")) == NULL)
   {
      //printf("Could not open %s\n",inf);
      ret = FALSE;
   }
   else
   {
      ret = TRUE; // init. return value
      i   = 0;    // init. index value
      fgets(tmp,SLEN,f1); /* read dummy first line */
      fgets(tmp,SLEN,f1); /* read dummy second line */
      *count = 0;         /* init. count */
      do
      {
         if (fgets(tmp,SLEN,f1) != NULL)
         {
            p = strtok(tmp,",");
            if (p != NULL)
            {
               ms[i].temp = atof(p);
            } // if
            while (p != NULL)
            {
               p = strtok(NULL,",");
               if (p != NULL)
               {
                  ms[i].time = (atof(p) * 60.0) / ts; /* convert to TS ticks */
                  (*count)++;
               } // if
            } // while
            ms[i].timer = NOT_STARTED; /* init. timer to not started */
         } // if
         i++;
      }
      while ((feof(f1) == 0) && (i < MAX_MS));
      fclose(f1); // close the input file
   } // else
   return ret;
} // read_input_file

void update_tset(double *tset, double temp, double offset,
                 maisch_schedule ms[], int *ms_idx, int ms_total)
/*------------------------------------------------------------------
  Purpose  : This function checks if tset should be increased, based
             on the maisch schedule defined in ms[]. When the temp.
             is increasing towards the new set-point a double offset is
             added to tset to speed things up. When the set-point has
             been reached a single offset is added to compensate for
             heat losses between the HLT and the MLT.

            Timer    Temp > ms[].temp  Action                    Offset to tset
         ----------------------------------------------------------------------
     1)  Not Started      FALSE        None, temp. is increasing   2 * offset
     2)  Not Started      TRUE         Start Timer                 offset
     3)    Running          X          Timer++                     offset
     4)    Time-out         X          Get next temp. set-point    2 * offset
  Variables:
     *tset : The reference temperature for the PID controller
      temp : The actual temperature of the HLT
    offset : Temperature offset to add to tset (1X or 2X)
      time : the actual time in TS ticks
      ms[] : Array containing the maisch schedule
    ms_idx : index in the array [0 .. ms_total-1]
  ms_total : max. index in the array
  Returns  : No values are returned
  ------------------------------------------------------------------*/
{
   if (*ms_idx < ms_total)
   {
      *tset = ms[*ms_idx].temp + offset; // get ref. temp. from mash scheme
      if ((ms[*ms_idx].timer == NOT_STARTED) && (temp > ms[*ms_idx].temp))
      {
         /* 2) timer has not started yet and ref. temp. has been achieved */
         ms[*ms_idx].timer = 0; /* start timer */
      } // if
      else if (ms[*ms_idx].timer >= ms[*ms_idx].time)
      {
         /* 4) timer exceeds the hold time = time-out */
         if (*ms_idx < ms_total - 1)
         {
            /* NOT at last temperature setting, so get new value */
            (*ms_idx)++; /* get next maisch temperature and time */
            *tset  = ms[*ms_idx].temp; /* set new reference temperature */
            *tset += offset; /* add double offset */
         } // if
         /*-----------------------------------------------------------*/
         /* Do nothing when all temp. & time pairs have been realised */
         /* Keep *tset at the last value, do NOT increase *ms_idx     */
         /*-----------------------------------------------------------*/
      } // else if
      else if (ms[*ms_idx].timer != NOT_STARTED)
      {
         /* 3) Timer is running, increment timer */
         ms[*ms_idx].timer++;
      } // else if
      else
      {
         /* 1) Timer is running, do nothing */
         *tset += offset; /* add double offset */
      } // else
   } /* if */
} /* update_tset() */

int update_std(double vmlt, double tmlt, unsigned int *kleppen,
                 maisch_schedule ms[], int ms_idx, int ms_total,
                 sparge_struct *sps, int pid_on)
/*------------------------------------------------------------------
  Purpose  : This function contains the State Transition Diagram (STD)
             for the ebrew program.
             First: the new state is calculated based upon the conditions.
             Then: the settings of the valves are calculated and returned
             in 'kleppen'.
  Variables:
      vmlt : The volume (L) of the Mash/Lauter Tun (MLT)
      tmlt : The actual temperature of the MLT
  *kleppen : Every bit represent a valve (1=ON, 0=OFF):
             Bits 15..8: 0 = Auto, 1 = Manual Override for valves V7-V1
             Bits  7..1: Valves V7-V1
             Bit      0: Pump
      ms[] : Array containing the mash schedule
    ms_idx : index in the array [0 .. ms_total-1]
  ms_total : max. index in the array
       sps : Struct containing all sparge variables
    pid_on : 1 = PID Controller enabled (needed as a condition here)
  Returns  : The values of ebrew_std is returned
  ------------------------------------------------------------------*/
{
   static int    ebrew_std; // Current state of STD
   static int    sp_idx;    // Sparging index [0..sps->sp_batches-1]
   static int    timer1;    // Timer for state 'Sparging Rest'
   static int    timer2;    // Timer for state 'Delay_1SEC'
   static int    timer3;    // Timer for transition to state 'Empty Heat Exchanger'
   static int    timer4;    // Timer for state 'Empty Heat Exchanger'
   static int    timer5;    // Timer for boiling time
   static double vmash;     // MLT volume after mashing is completed
   unsigned int  klepstand; // Help var. = klepstanden[ebrew_std]

   unsigned int  klepstanden[] = {0x0000, 0x0024, 0x0015, 0x0013, 0x0023,
                                  0x0013, 0x0093, 0x0017, 0x0013, 0x0083,
                                  0x0010, 0x0013, 0x00c1};

   switch (ebrew_std)
   {
      case INITIALISATION:
           if (pid_on & 0x01)
           {
              ebrew_std = WAIT_FOR_HLT_TEMP;
           } // if
           break;
      case WAIT_FOR_HLT_TEMP:
           if (ms[0].timer != NOT_STARTED) // HLT TEMP is OK
           {
              ebrew_std = FILL_MLT;
           } // if
           break;
      case FILL_MLT:
           if (vmlt > sps->mash_vol)
           {
              ebrew_std = MASH_IN_PROGRESS;
           } // if
           break;
      case MASH_IN_PROGRESS:
           if ((ms_idx >= ms_total - 1) && // Mashing finished?
               (ms[ms_idx].timer >= ms[ms_idx].time))
           {
              sp_idx    = 0;    // init. sparging index
              vmash     = vmlt; // remember current MLT volume
              timer1    = sps->sp_time_ticks; // timer1 -> TIME-OUT
              ebrew_std = SPARGING_REST;
           }
           else if (tmlt > ms[ms_idx].temp + TMLT_HLIMIT)
           {
              ebrew_std = BYPASS_HEAT_EXCHANGER;
           } // else if
           break;
      case BYPASS_HEAT_EXCHANGER:
           if (tmlt < ms[ms_idx].temp + TMLT_LLIMIT)
           {
              ebrew_std = MASH_IN_PROGRESS;
           } // if
           break;
      case SPARGING_REST:
           if (++timer1 >= sps->sp_time_ticks)
           {
              if (sp_idx < sps->sp_batches - 1)
              {
                 ebrew_std = PUMP_FROM_MLT_TO_BOIL; // Pump to BOIL again
              } // if
              else
              {
                 ebrew_std = EMPTY_MLT; // Finished with Sparging, empty MLT
              } // else if
           } // if
           break;
      case PUMP_FROM_MLT_TO_BOIL:
           if (vmlt < vmash - sps->sp_vol_batch)
           {
              timer2    = 0; // init. 1 sec. timer
              ebrew_std = DELAY_1SEC;
           } // if
           break;
      case PUMP_FROM_HLT_TO_MLT:
           if (vmlt > vmash)
           {
              sp_idx++;      // Increase #Sparging Sessions done
              timer1    = 0; // init timer1
              ebrew_std = SPARGING_REST;
           } // if
           break;
      case DELAY_1SEC:
           if (++timer2 >= TIMEOUT_1SEC)
           {
              ebrew_std = PUMP_FROM_HLT_TO_MLT;
           } // if
           break;
      case EMPTY_MLT:
           if (vmlt < VMLT_EMPTY)
           {
              timer3    = 0; // init. timer for transition to 'Empty Heat Exchanger'
              timer5    = 0; // init. timer for boiling time
              ebrew_std = BOILING;
           } // if
           break;
      case BOILING:
           ++timer5;
           if ((timer3 != -1) && (++timer3 >= TIMEOUT3))
           {
              timer4    = 0;
              ebrew_std = EMPTY_HEAT_EXCHANGER;
           } // if
           else if (timer5 >= sps->boil_time_ticks)
           {
              ebrew_std = CHILL;
           } // else if
           break;
      case EMPTY_HEAT_EXCHANGER:
           ++timer5;
           if (++timer4 >= TIMEOUT4)
           {
              timer3    = -1; // disable timer3
              ebrew_std = BOILING;
           } // if
           break;
      case CHILL:
           if (pid_on & 0x02)
           {
              ebrew_std = INITIALISATION;
           } // if
           break;
      default:
           ebrew_std = INITIALISATION;
           break;
   } // switch

   //-------------------------------------------------
   // Now calculate the proper settings for the valves
   //-------------------------------------------------
   klepstand = klepstanden[ebrew_std];
   if ((*kleppen & V7M) == 0x0000) // V7 No Manual Override ?
   {
      if (klepstand & V7b) *kleppen |= V7b;
      else                 *kleppen &= ~V7b;
   }
   if ((*kleppen & V6M) == 0x0000) // V6 No Manual Override ?
   {
      if (klepstand & V6b) *kleppen |= V6b;
      else                 *kleppen &= ~V6b;
   }
   if ((*kleppen & V5M) == 0x0000) // V5 No Manual Override ?
   {
      if (klepstand & V5b) *kleppen |= V5b;
      else                 *kleppen &= ~V5b;
   }
   if ((*kleppen & V4M) == 0x0000) // V4 No Manual Override ?
   {
      if (klepstand & V4b) *kleppen |= V4b;
      else                 *kleppen &= ~V4b;
   }
   if ((*kleppen & V3M) == 0x0000) // V3 No Manual Override ?
   {
      if (klepstand & V3b) *kleppen |= V3b;
      else                 *kleppen &= ~V3b;
   }
   if ((*kleppen & V2M) == 0x0000) // V2 No Manual Override ?
   {
      if (klepstand & V2b) *kleppen |= V2b;
      else                 *kleppen &= ~V2b;
   }
   if ((*kleppen & V1M) == 0x0000) // V1 No Manual Override ?
   {
      if (klepstand & V1b) *kleppen |= V1b;
      else                 *kleppen &= ~V1b;
   }
   // No Manual Override for the pump.
   return ebrew_std; // return the new state of the STD
} // update_std()

void init_ma(ma *p, int N)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Moving Average (MA) struct
             that is used in the moving_average() function
  Variables:
        *p : Pointer to the ma struct (specify a new struct for every
             new filter!)
         N : The order of the Moving Average filter (1 = No filtering)
  Returns  : -
  ------------------------------------------------------------------*/
{
   int i; // temp. var.

   p->N     = N;   // order of MA filter
   p->index = 0;   // index in cyclic array
   p->sum   = 0.0; // running sum
   for (i = 0; i < N; i++)
   {
      p->T[i] = 0.0; // clear cyclic array
   } // for
} // init_ma()

double moving_average(ma *p, double x)
/*------------------------------------------------------------------
  Purpose  : This function calculates the Moving Average (MA) of the
             input signal x. An MA filter of order N is a low-pass
             filter with a zero at fs / N (or 2.pi/N). It's Z transform
             function is:
                                 -1         -(N-1)
             Y[z] = X[z] . (1 + z  + ... + Z      )

             Initialisation: p->N must have a value.
                             p->sum and p->index should be init. to 0.
  Variables:
        *p : Pointer to the ma struct (specify a new struct for every
             new filter!)
         x : The actual input value
  Returns  : Filter output value
  ------------------------------------------------------------------*/
{
   p->sum -= p->T[p->index];  // subtract value to overwrite from running sum
   p->T[p->index] = x / p->N; // store new value in array
   p->sum += p->T[p->index];  // update running sum with new value
   if (++(p->index) >= p->N)  // update index in cyclic array
   {
      p->index = 0; // restore to 1st position
   } // if
   return p->sum;   // return value = filter output
} // moving_average()
