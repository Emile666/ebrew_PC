/*==================================================================
  Function name: decode_log_file(), read_input_file(), update_tset(),
                 init_ma(), moving_average()
  File name    : $Id$
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains several miscellaneous functions
  $Log$
  Revision 1.6  2003/06/01 13:40:45  emile
  - Bugfix: switch/fix for Tmlt and Thlt were in wrong time-slice. Corrected.
  - Switch/fix for std state added for easier testing
  - Vmash value added to 'Options|Sparge & STD Settings' dialog screen.

  Revision 1.5  2003/06/01 11:53:48  emile
  - tset has been renamed in tset_hlt for more clearance
  - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
         This is now 'thlt >= tset_hlt', because timer0 only starts with water
         in the MLT => this caused a dead-lock.
  - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
    These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
    TIMEOUT3 and TIMEOUT4.

  Revision 1.4  2003/01/04 22:35:50  emile
  - Restore Settings function now restores all relevant variables (not just
    the mashing variables). Several separate functions are created to
    support this.

  Revision 1.3  2002/12/30 20:21:59  emile
  - Bug 2 29-12-02 solved: start mash timers if temp >= tref instead of >.
  - Bug 3 29-12-02 solved: deadlock in std_state 4 when mashing is finished.
  - Bug 4 29-12-02 solved: rush through STD. Sparging parameters were not
    initialised. New function Init_Sparge_Settings() added.
  - Sparge variables now added to 'View Mash Progress' screen.
  - std_struct added for more flexibility of fixing STD values.

  Revision 1.2  2002/12/30 13:33:45  emile
  - Headers with CVS tags added to every source file
  - Restore Settings function is added
  - "ebrew" registry key now in a define REGKEY

  ==================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pid_reg.h" /* for TS */
#include "misc.h"

/*----------------------------------------------------*/
/* This macro is used by decode_log_file() to advance */
/* to the x-th column in the string s. The value can  */
/* then be read by a atoi(phlp) / atof(phlp).         */
/* phlp & i to be defined by the function.            */
/*----------------------------------------------------*/
#define GOTO_COLUMN(x,stmp)                                         \
            phlp = strtok(stmp,COLON); /* Find first ',' on line */ \
            for (i = 1; i < x; i++)                                 \
            {                                                       \
               phlp = strtok(NULL,COLON); /* Skip other columns */  \
            } /* for */

/*------------------------------------------------------*/
/* This macro calculates the value for the last line of */
/* the log entry. It makes sure that the last line is   */
/* greater than or equal to the first line.             */
/*------------------------------------------------------*/
#define CALC_LAST_LINE                                           \
            if (line_nr - LOG_LAST_LINE < p[log_idx - 1].bline)  \
                 p[log_idx - 1].eline = p[log_idx - 1].bline;    \
            else p[log_idx - 1].eline = line_nr - LOG_LAST_LINE

/*------------------------------------------------------*/
/* This macro checks if the variable 'std' is > x.      */
/* If so, the phase_start variable is set to 'line_nr'. */
/*------------------------------------------------------*/
#define FIND_PHASE(phase_start,x)          \
            if (!phase_start && (std > x)) \
            {                              \
               phase_start = line_nr;      \
            } /* if */

void calc_phases_start(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function finds, from the log file, the following:
             - the begin time of every log-file entry
             - the start line number of the mashing, sparging,
               boiling and chilling phases
             - the end line number of the mashing, sparging,
               boiling and chilling phases for every log-file entry.
             - the last value of ms_idx for every log-file entry.
             - the last value of ebrew_std for every log-file entry
             Pre-condition: - p[].bline, p[].eline must be set.
                            - p[].mashing_start .. p[].chill_start must be set.
    Variables:
        fd : File descriptor for the log file, should be opened already
             by the calling program!
         p : Pointer to an log_struct struct
  Returns  : -
  ------------------------------------------------------------------*/
{
   char tmp[SLEN];
   char *phlp;             /* temp. pointer */
   int  line_nr = 1;       /* line number */
   int  log_idx = 0;       /* log entry index */
   int  std;               /* value of ebrew_std */
   int  i;                 /* temp. variable */
   double dtmp;            /* Value of Vmlt just before transition ebrew_std 3->5 */
   static double dtmp_old; /* Previous value of dtmp */

   rewind(fd); /* start again at beginning of file */
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if (p[log_idx].bline == line_nr)
         {
            /*-------------------------------------------*/
            /* This is the start of the actual log entry */
            /*-------------------------------------------*/
            strncpy(p[log_idx].btime,tmp,8); /* copy time string */
            p[log_idx].btime[8] = '\0';      /* terminate with '\0' */
         } /* if */

         if (line_nr > p[log_idx].bline && line_nr < p[log_idx].eline)
         {
            /*-----------------------------------------------*/
            /* Find the start moments of the various phases: */
            /* Mashing, Sparging, Boiling and Chilling       */
            /*-----------------------------------------------*/
            GOTO_COLUMN(7,tmp); /* Vmlt is to be found in column 10 */
            /* Next value to read is Vmlt */
            dtmp = atof(phlp);
            phlp = strtok(NULL,COLON); /* skip PID_ON */
            phlp = strtok(NULL,COLON); /* skip ms_idx */
            phlp = strtok(NULL,COLON); /* ebrew_std */
            std  = atoi(phlp);
            if (std > p[log_idx].max_std)
            {
               p[log_idx].max_std = std;
            } /* if */
            FIND_PHASE(p[log_idx].mashing_start,1);  /* Find 1st entry of "2. FILL MLT" */
            if (!p[log_idx].sparging_start && std > 4)
            {
               p[log_idx].sparging_start = line_nr;
               p[log_idx].vmash          = dtmp_old; /* last Vmlt before transition */
            } /* if */
            dtmp_old = dtmp; /* update previous value */
            FIND_PHASE(p[log_idx].boil_start,9);     /* Find 1st entry of "10. BOIL" */
            FIND_PHASE(p[log_idx].chill_start,11);   /* Find 1st entry of "12. CHILL" */
         } /* if */

         if (p[log_idx].eline == line_nr)
         {
            /*-----------------------------------------*/
            /* This is the end of the actual log entry */
            /*-----------------------------------------*/
            strncpy(p[log_idx].etime,tmp,8); /* copy time string */
            p[log_idx].etime[8] = '\0';      /* terminate with '\0' */

            GOTO_COLUMN(9,tmp); /* ms_idx is to be found in column 9 */
            /* Next value to read is ms_idx */
            p[log_idx].lms_idx = atoi(phlp);
            phlp = strtok(NULL,COLON); /* next value is std_state */
            p[log_idx].std_val = atoi(phlp);
            log_idx++; /* to next log entry */
         } /* else if */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
} /* calc_phases_start() */

void calc_mash_timer_info(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function finds, from the log file, for every
             log-file entry, the following:
             - tmr_ms_idx: the status of the last mash timer
             Pre-condition: - p[].bline, p[].eline must be set.
                            - p[].lms_idx must be set
    Variables:
        fd : File descriptor for the log file, should be opened already
             by the calling program!
         p : Pointer to an log_struct struct
  Returns  : -
  ------------------------------------------------------------------*/
{
   char tmp[SLEN];
   char *phlp;           /* temp. pointer */
   int  line_nr = 1;     /* line number */
   int  log_idx = 0;     /* log entry index */
   int  found   = FALSE; /* temp. boolean var. */
   int  i;               /* temp. variable */
   
   rewind(fd); /* start again at beginning of file */
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if ((line_nr >= p[log_idx].bline) && (line_nr < p[log_idx].eline) &&
             (p[log_idx].lms_idx > 0) && !found)
         {
            /*--------------------------------------------*/
            /* Mash in progress: now calculate timer info */
            /*--------------------------------------------*/
            GOTO_COLUMN(9,tmp); /* ms_idx is to be found in column 9 */
            /*----------------------------------*/
            /* Next value to read is ms_idx     */
            /* Check if ms_idx == latest ms_idx */
            /* If so, calculate #clock-ticks    */
            /*----------------------------------*/
            if (atoi(phlp) == p[log_idx].lms_idx)
            {
               if (p[log_idx].sparging_start)
               {
                  /* Mashing was completed & followed by Sparging */
                  p[log_idx].tmr_ms_idx = p[log_idx].sparging_start - line_nr;
               }
               else
               {
                  /* Still in Mashing mode, not completed yet */
                  p[log_idx].tmr_ms_idx = p[log_idx].eline + 1 - line_nr;
               } /* else */
               found = TRUE;
            } /* if */
         } /* if */
         else if (line_nr == p[log_idx].eline)
         {
            log_idx++; /* next log entry number */
            found = FALSE;
         } /* else */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
} /* calc_mash_timer_info() */

void calc_sp_idx(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function finds, for every log-file entry, the following:
             - the number of sparging sessions done
             - the start line number of the last ebrew_std value
  Variables:
        fd : File descriptor for the log file, should be opened already
             by the calling program!
         p : Pointer to an log_struct struct
  Returns  : -
  ------------------------------------------------------------------*/
{
   char tmp[SLEN];
   char *phlp;              /* temp. pointer */
   int  line_nr    = 1;     /* line number */
   int  log_idx    = 0;     /* log entry index */
   int  i;                  /* temp. variable */
   int  prev_state = 0;     /* previous value of std */
   int  std;                /* value of ebrew_std */
   int  ffound     = FALSE; /* TRUE = start of last state found */
   
   rewind(fd); /* start again at beginning of file */
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if (line_nr >= p[log_idx].bline && line_nr <= p[log_idx].eline)
         {
            GOTO_COLUMN(10,tmp); /* std_ebrew is to be found in column 10 */
            /* Next value to read is std_ebrew */
            std = atoi(phlp);

            /*-------------------------------------------*/
            /* Find the number of sparging sessions done */
            /*-------------------------------------------*/
            if (line_nr >= p[log_idx].sparging_start && line_nr < p[log_idx].boil_start)
            {
               /*--------------------------------------------*/
               /* We are now in the correct sparging section */
               /*--------------------------------------------*/
               if (std == 5 && (prev_state >= 6 && prev_state <= 8))
               {
                  p[log_idx].sp_idx++;
               } /* if */
               prev_state = std; /* update previous value of std */
            } /* if */
   
            /*-----------------------------------------------------*/
            /* Now find the 1st line where the last state started. */
            /*-----------------------------------------------------*/
            if (std == p[log_idx].std_val)
            {
               if (!ffound)
               {
                  p[log_idx].start_lstd = line_nr;
                  ffound                = TRUE;
               } /* if */
            }
            else
            {
               ffound = FALSE;
            } /* else */

            if (line_nr == p[log_idx].eline)
            {
               log_idx++; /* advance to next log file entry */
               ffound = FALSE;
            } /* if */
         } /* if */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
} /* calc_sp_idx() */

int decode_log_file(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function reads all the information from the
             log file, of which f is the file descriptor.
  Variables:
        fd : File descriptor for the log file, should be opened already
             by the calling program!
         p : Pointer to an array of log_struct structs
  Returns  : The number of log-file entries found in the log file
  ------------------------------------------------------------------*/
{
   char tmp[SLEN];
   char *phlp;  /* temp. pointer */
   char sstart[] = "Date of brewing: "; /* string to start a log-file entry */
   int  sslen    = strlen(sstart);      /* length of sstart[] */
   int  log_idx  = 0; /* log entry index */
   int  line_nr  = 1; /* start at line 1 in log file */
   int  rval     = 0; /* return value */

   /*-----------------------------------------------------------------*/
   /* 1) Go through the log-file and determine the brew-date and the  */
   /*    and the begin- and end-lines of every log-entry.             */
   /*-----------------------------------------------------------------*/
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if (((phlp = strstr(tmp,sstart)) != NULL) || !strncmp(tmp,sstart,sslen))
         {
            /*---------------------------------------------------*/
            /* We found the start of a new log-entry in the file */
            /*---------------------------------------------------*/
            strncpy(p[log_idx].brew_date, phlp + sslen, 10);
            p[log_idx].brew_date[10] = '\0'; /* terminate with '\0' */

            p[log_idx].bline = line_nr + LOG_HDR_SIZE;
            /* Init. some values that we need later on */
            p[log_idx].mashing_start  = 0;
            p[log_idx].sparging_start = 0;
            p[log_idx].boil_start     = 0;
            p[log_idx].chill_start    = 0;
            p[log_idx].tmr_ms_idx     = 0;
            p[log_idx].sp_idx         = 0;
            p[log_idx].max_std        = 0;
            p[log_idx].vmash          = 0.0;

            if (log_idx > 0)
            {
              /*--------------------------------------------------------*/
              /* fill in the last valid line of the previous log entry. */
              /* Assume that the last entry may be wrong.               */
              /*--------------------------------------------------------*/
              CALC_LAST_LINE;
            } /* if */
            log_idx++; /* increment log entry index */
         } /* if */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
   CALC_LAST_LINE;
   rval = log_idx; /* return value */
   /*------------------------------------------------------------------------*/
   /* 2) Go through the log-file again and add additional information,       */
   /*    such as the begin- and end-time, ms_idx and std_state.              */
   /*------------------------------------------------------------------------*/
   calc_phases_start(fd,p);

   /*---------------------------------------------------------------*/
   /* 3) Go through the log-file AGAIN and fill in mash timer info. */
   /*---------------------------------------------------------------*/
   calc_mash_timer_info(fd,p);

   /*---------------------------------------------------------------*/
   /* 4) Go through the log-file AGAIN and find sp_idx,             */
   /*    this is the number of sparging cycles already done.        */
   /*---------------------------------------------------------------*/
   calc_sp_idx(fd,p); /* find sp_idx if relevant */

   return rval;
} /* decode_log_file() */

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

double update_tset(double *tset, double temp, double offset,
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
      temp : The actual temperature of the HLT or MLT
    offset : Temperature offset to add to tset (1X or 2X)
      time : the actual time in TS ticks
      ms[] : Array containing the maisch schedule
    ms_idx : index in the array [0 .. ms_total-1]
  ms_total : max. index in the array
  Returns  : MLT reference temp. (= value from mash scheme)
  ------------------------------------------------------------------*/
{
   if (*ms_idx < ms_total)
   {
      *tset = ms[*ms_idx].temp + offset; // get ref. temp. from mash scheme
      if ((ms[*ms_idx].timer == NOT_STARTED) && (temp >= ms[*ms_idx].temp))
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
   return ms[*ms_idx].temp; /* return ref. temp. for MLT */
} /* update_tset() */

int update_std(double vmlt, double tmlt, double thlt, double tset_hlt,
               unsigned int *kleppen, maisch_schedule ms[], int ms_idx, int ms_total,
               sparge_struct *sps, std_struct *std, int pid_on, int std_fx)
/*------------------------------------------------------------------
  Purpose  : This function contains the State Transition Diagram (STD)
             for the ebrew program.
             First: the new state is calculated based upon the conditions.
             Then: the settings of the valves are calculated and returned
             in 'kleppen'.
             It uses various static variables (see begin of file).
  Variables:
      vmlt : The volume (L) of the Mash/Lauter Tun (MLT)
      tmlt : The actual temperature of the MLT
      thlt : The actual temperature of the HLT
  tset_hlt : The reference temperature for the HLT
  *kleppen : Every bit represent a valve (1=ON, 0=OFF):
             Bits 15..8: 0 = Auto, 1 = Manual Override for valves V7-V1
             Bits  7..1: Valves V7-V1
             Bit      0: Pump
      ms[] : Array containing the mash schedule
    ms_idx : index in the array [0 .. ms_total-1]
  ms_total : max. index in the array
       sps : Struct containing all sparge variables
       std : Struct containing all STD variables
    pid_on : 1 = PID Controller enabled (needed as a condition here)
    std_fx : Fix value for ebrew_std, -1 if no fix
  Returns  : The values of ebrew_std is returned
  ------------------------------------------------------------------*/
{
   unsigned int  klepstand; // Help var. = klepstanden[std->ebrew_std]
   unsigned int  klepstanden[] = {0x0000, 0x0024, 0x0015, 0x0013, 0x0023,
                                  0x0013, 0x0093, 0x0017, 0x0013, 0x0083,
                                  0x0010, 0x0013, 0x00c1};
   switch (std->ebrew_std)
   {
      case S00_INITIALISATION:
           if (pid_on & 0x01)
           {
              std->ebrew_std = S01_WAIT_FOR_HLT_TEMP;
           } // if
           break;
      case S01_WAIT_FOR_HLT_TEMP:
           if (thlt >= tset_hlt) // HLT TEMP is OK
           {
              std->ebrew_std = S02_FILL_MLT;
           } // if
           break;
      case S02_FILL_MLT:
           if (vmlt > sps->mash_vol)
           {
              std->ebrew_std = S03_MASH_IN_PROGRESS;
           } // if
           break;
      case S03_MASH_IN_PROGRESS:
           if ((ms_idx >= ms_total - 1) && // Mashing finished?
               (ms[ms_idx].timer >= ms[ms_idx].time))
           {
              std->sp_idx    = 0;    // init. sparging index
              std->vmash     = vmlt; // remember current MLT volume
              std->timer1    = sps->sp_time_ticks; // timer1 -> TIME-OUT
              std->ebrew_std = S05_SPARGING_REST;
           }
           else if (tmlt > ms[ms_idx].temp + sps->tmlt_hlimit)
           {
              std->ebrew_std = S04_BYPASS_HEAT_EXCHANGER;
           } // else if
           break;
      case S04_BYPASS_HEAT_EXCHANGER:
           if ((tmlt < ms[ms_idx].temp + sps->tmlt_llimit) ||
               ((ms_idx >= ms_total - 1) && // Mashing finished?
                (ms[ms_idx].timer >= ms[ms_idx].time)))
           {
              std->ebrew_std = S03_MASH_IN_PROGRESS;
           } // if
           break;
      case S05_SPARGING_REST:
           if (++std->timer1 >= sps->sp_time_ticks)
           {
              if (std->sp_idx < sps->sp_batches)
              {
                 std->ebrew_std = S06_PUMP_FROM_MLT_TO_BOIL; // Pump to BOIL again
              } // if
              else
              {
                 std->ebrew_std = S09_EMPTY_MLT; // Finished with Sparging, empty MLT
              } // else if
           } // if
           break;
      case S06_PUMP_FROM_MLT_TO_BOIL:
           if (vmlt < std->vmash - sps->sp_vol_batch)
           {
              std->timer2    = 0; // init. 1 sec. timer
              std->ebrew_std = S08_DELAY_xSEC;
           } // if
           break;
      case S07_PUMP_FROM_HLT_TO_MLT:
           if (vmlt > std->vmash)
           {
              std->sp_idx++;      // Increase #Sparging Sessions done
              std->timer1    = 0; // init timer1
              std->ebrew_std = S05_SPARGING_REST;
           } // if
           break;
      case S08_DELAY_xSEC:
           if (++std->timer2 >= sps->to_xsec)
           {
              std->ebrew_std = S07_PUMP_FROM_HLT_TO_MLT;
           } // if
           break;
      case S09_EMPTY_MLT:
           if (vmlt < sps->vmlt_empty)
           {
              std->timer3    = 0; // init. timer for transition to 'Empty Heat Exchanger'
              std->timer5    = 0; // init. timer for boiling time
              std->ebrew_std = S10_BOILING;
           } // if
           break;
      case S10_BOILING:
           ++std->timer5;
           if ((std->timer3 != NOT_STARTED) && (++std->timer3 >= sps->to3))
           {
              std->timer4    = 0;
              std->ebrew_std = S11_EMPTY_HEAT_EXCHANGER;
           } // if
           else if (std->timer5 >= sps->boil_time_ticks)
           {
              std->ebrew_std = S12_CHILL;
           } // else if
           break;
      case S11_EMPTY_HEAT_EXCHANGER:
           ++std->timer5;
           if (++std->timer4 >= sps->to4)
           {
              std->timer3    = NOT_STARTED; // disable timer3
              std->ebrew_std = S10_BOILING;
           } // if
           break;
      case S12_CHILL:
           if (pid_on & 0x02)
           {
              std->ebrew_std = S00_INITIALISATION;
           } // if
           break;
      default:
           std->ebrew_std = S00_INITIALISATION;
           break;
   } // switch

   if (std_fx != -1)
   {
      std->ebrew_std = std_fx;
   } // if

   //-------------------------------------------------
   // Now calculate the proper settings for the valves
   //-------------------------------------------------
   klepstand = klepstanden[std->ebrew_std];
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
   return std->ebrew_std; // return the new state of the STD
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
