/*==================================================================
  Function name: decode_log_file(), read_input_file(), update_tset(),
                 init_ma(), moving_average()
  File name    : $Id$
  Author       : E. vd Logt
  ------------------------------------------------------------------
  Purpose : This file contains several miscellaneous functions
  $Log$
  Revision 1.33  2016/08/07 14:26:43  Emile
  - Version works with firmware r1.29.
  - Pump 2 (HLT heat-exchanger) support added in Px command
  - V8 now also works, action bits in STD reorganised for this.
  - Tboil Ref. Temp. is now properly set (bug-fix from session 151).
  - New Registry parameter BOIL_MIN_TEMP instead of hard-coded value.

  Revision 1.32  2016/05/22 13:51:16  Emile
  Bugfixes brewing session 21-05-'16 with v3.30 PCB and HW r1.27
  - Temp.sensor error value is now '-99.99'
  - Double updates to Vboil removed in std
  - HLT and MLT thermometer objects removed

  Revision 1.31  2016/04/09 12:58:50  Emile
  - First version for new V3.30 PCB HW. Now support 4 temperatures, 4 flowsensors
    and Boil-Kettle PID-Controller. Various changes to User Interface, Registry
    parameters and scheduler/tasks.
  - Only 6 parameters left to send to HW. In line with firmware R1.23.
  - New switched/fixes added for tset_boil, gamma_boil and Tboil.

  Revision 1.30  2016/01/24 19:36:55  Emile
  - Valves and Pump now show colours: RED (on) and GREEN (off)
  - Pipes are now highlighted to show actual direction of fluid movement
  - Initial delay of tasks changed to multiples of 100 msec. scheduler tick
  - Mash-rest in new state now set to 5 min. instead of 10 min.

  Revision 1.29  2015/12/13 14:20:27  Emile
  - Size of all 3 brew-kettles now adjustable. New Reg. par. VBOIL_MAX added.
  - New 'Mash_Rest' checkbox added to 'Sparge & Mash Settings. New. Reg. par.
    CB_Mash_Rest. New state 18 'Mast Rest (10 minutes)' added to STD.
  - Pump and Valves are now all off in state 'Add Malt to MLT'.
  - Statusbar now also shows mash and sparge litres (valves indicators removed).
  - Auto-All option added to set all valves and the pump to Auto when 'A' pressed.

  Revision 1.28  2015/07/21 19:42:46  Emile
  - Setting Mash- and Sparge Volume now via maisch.sch and not in Dialog screen anymore.
  - Flow-rate indicators added (HLT->MLT and MLT->Boil) to Main-Screen.
  - Transition from 'Empty MLT' to 'Wait for Boil' now detected automatically with
    new function flow_rate_low().
  - Registry vars VMLT_EMPTY, MASH_VOL and SPARGE_VOL removed.
  - Functionality and Checkbox for 'Double initial Sparge Volume' added.
  - Registry var CB_VSP2 added.

  Revision 1.27  2015/06/09 21:00:21  Emile
  Bugfix STD: Valve V4 was ON in state 14, should be OFF. Now corrected.

  Revision 1.26  2015/06/06 14:02:33  Emile
  - User Interaction now with PopupMenu to State-label
  - PID Controller now made with a TvrPowerButton instead of a radiobutton box
  - View Mash Progress Form improved

  Revision 1.25  2015/06/05 19:18:40  Emile
  - STD optimized for new solenoid valves. User Interaction dialog added
  - Calibration & Temp. correction added for flowsensors

  Revision 1.24  2015/03/21 09:27:22  Emile
  - Vboil_simulated removed, VHLT_START added
  - task_read_vmlt_boil() with command A6 added (works with ebrew HW R1.12)
  - task_read_vhlt_vmlt() with command A5 added
  - Flow1_hlt_mlt and Flow2_mlt_boil objects added to main-screen
  - New Registry var USE_FLOWSENSORS. Switches between tasks 03/04 and 03F/04F

  Revision 1.23  2013/07/23 09:42:46  Emile
  - Fourth intermediate version: several Registry Settings added / removed.
  - Dialog Screens updated: better lay-out and matches new Registry Settings
  - Source-code improved for readability

  Revision 1.22  2013/07/21 22:32:47  Emile
  - 3rd intermediate version to support ebrew 2.0 rev.1.5 hardware
  - Changes to Measurement Dialog Screen: VHLT, VMLT, THLT, TMLT
  - Registry: several parameters removed + parameters renamed
  - Ttriac & DAC code & parameters removed

  Revision 1.21  2013/06/22 23:04:19  Emile
  - Second intermediate version: scheduler added and timer interrupt divided
    over a number of tasks.
  - Version works with Ebrew hardware, task duration needs to be optimised!

  Revision 1.20  2011/05/29 20:56:26  Emile
  - New Registry variables added: STC_N, STC_TD and STC_ADF
  - PID Settings Dialog screen extended with new parameters for self-tuning
    controller: possibility to set the system order N, an estimate for the
    time-delay and a boolean whether or not to use adaptive dir. forgetting.
  - PID Settings Dialog screen: parameters enabled/disabled when a
    specific PID controller is chosen.
  - New functions time_delay() and init_time_delay() added
  - Changes made in init_pid2() function header.
  - Unit-test cases updated and extended with tests for new functions.

  Revision 1.19  2007/08/26 22:23:20  Emile
  - Slope Limiter function added for Thlt, Tmlt, Vhlt, Vmlt and tset_hlt
  - Five Registry variables added: THLT_SLOPE, TMLT_SLOPE, VHLT_SLOPE,
    VMLT_SLOPE and TSET_HLT_SLOPE
  - Bug-fix setting MA order for HLT Volume: this was coupled to MA order of
    HLT temperature. Corrected
  - Measurements... and PID controller settings... dialog screen updated.

  Revision 1.18  2007/07/06 22:23:01  Emile
  - The real time between two lines from a log-file is now used instead of a
    fixed 5 sec. time when reading a log-file.
  - Leading-zero bug solved in Mash Progress screen
  - i2c_stop() only called with PT_CLOSE in case of exit of program
  - System Identification library functions added (but not used yet).

  Revision 1.17  2007/01/03 13:45:49  Emile
  - Bugfix: when reading a log-file, the first mash timestamp was not recognised.
  - Bugfix: Sparging timestamps were erased when a sparging parameter was updated.
  - Bugfix: sparging_start was printed to log_structp.txt instead of sparging_start[0]

  Revision 1.16  2006/11/18 23:06:37  Emile
  - View Mash/Sparging screen is improved: time-stamps are placed when a
    mashing or sparging phase has started.
  - Read_log_file is improved: time-stamps are generated, so that old log-files
    can be read and time-stamp information can be seen in the Mash/Sparging screen.
  - Datagraps are set to a step-size of 1 instead of 2 (1 div = 100 seconds now).
  - Main screen updated: Heating power is now in % and correct volumes are added.

  Revision 1.15  2006/02/19 13:14:35  Emile
  - Bug-fix reading logfile(). If the latest mash timer was not started yet,
    it was set to a high value (which was the linenumber in the logfile).
    Setting the mash-timers should be oke now.
  - Max. linenumber changed from 32767 to 65535, meaning that 91 hours in 1
    log-entry is possible now.

  Revision 1.14  2005/08/30 09:17:42  Emile
  - Bug-fix reading log-file. Only entries > 1 minute can be imported.
  - sp_idx added to log-file, instead of PID_ON.
  - Stay 10 seconds in state 5 at start of sparging for logging purposes
  - Reorganisation of routines of reading log file, added print_p_struct().

  Revision 1.13  2005/06/11 12:35:07  Emile
  - Keyboard shortcuts 'P' (Pump toggle) and '1' .. '7' (valve toggles) added.
  - Added transition from state 8 back to state 6. This prevents a transition
    change during sparging when a glitch on Vmlt happens.
  - Added Vmlt_unf (=padc.ad4) to log-file for debugging purposes.

  Revision 1.12  2004/05/13 20:50:59  emile
  - Main loop timing improved. Only 99 (of 100) cycles were executed. Loop
    timing is now reset after 100 loops (5 seconds)
  - TS parameter now only works on PID-controller time-slice. Max. is 20 sec.
  - Bug-fix in init_ma() filter when init. to a value (should be /N).
  - LPF for D-term of PID controller added. New reg. var. K_LPF
  - PID Debug label added + checkbox in PID screen. Default off (NO reg var).
  - Statusbar object added
  - Start made with network functionality. Not operational yet.

  Revision 1.11  2004/05/08 14:52:52  emile
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

  Revision 1.10  2004/05/05 15:44:16  emile
  - Main Screen picture update
  - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
  - STD update: calculation of volumes should be correct now
  - Parameter added for early start of mash timer. Registry var. TOffset2
  - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
  - Some screens updated with hints (also of labels)
  - Bug-fix: unnecessary delay after change in gamma. Is corrected now
  - Help via menu now also works

  Revision 1.9  2004/01/31 16:01:05  emile
  - Init. HW High/Low limit temp. changed to 70/50 C respectively.
  - Added code for calculation/simulation of Vhlt and Vboil
  - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
  - Registry key no longer in ebrew but in Software\\ebrew
  - First attempt to catch CVS version ID in source code

  Revision 1.8  2003/09/15 20:37:22  emile
  - LM76 constants renamed in LM92 constants
  - Pump Popupmenu added (same as already done for the valves)
  - Added support for LED3 and LED4 displays
  - 'I2C settings' renamed into 'Hardware Settings'
  - Added more variables to LED1..LED4 selection. Now 6 variables to select
  - Added SET_LED macro
  - Added Triac Temperature protection functionality

  Revision 1.7  2003/07/11 18:34:46  emile
  - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
  - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
  - Log-file header updated
  - init_adc(): all vref initialisations are now the same (/ 2560).
                Removed the / 10 division of AD4 in the main loop, this is
                now done in init_adc().
  - Multiply and division changed into <<= and >>= (in lm76_read())

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

flow_rate_low_struct frl_empty_mlt;  // Needed for detection of low flowrate
flow_rate_low_struct frl_empty_boil; // Needed for detection of low flowrate

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
#define CALC_LAST_LINE                                              \
        if (line_nr - LOG_LAST_LINE < p[log_idx - 1].bline)         \
             p[log_idx - 1].eline = p[log_idx - 1].bline;           \
        else p[log_idx - 1].eline = line_nr - LOG_LAST_LINE;        \
                                                                    \
        if (p[log_idx-1].eline - p[log_idx-1].bline < 12)           \
        {                                                           \
            /* overwrite previous entry if it less than a minute */ \
           log_idx--;                                               \
        } // if

/*--------------------------------------------------------------------*/
/* This macro checks if the variable 'std' transition from xold to x. */
/* If so, the phase_start variable is set to 'line_nr'.               */
/*--------------------------------------------------------------------*/
#define FIND_PHASE(phase_start,x,xold)                           \
            if (!phase_start && (std == x) && (std_old == xold)) \
            {                                                    \
               phase_start = line_nr;                            \
            } /* if */

double diff_seconds(char *s1, char *s2)
/*------------------------------------------------------------------
  Purpose  : This function calculates the number of seconds difference
             between the two time strings
  Variables:
        s1 : Time-string 1 (hh:mm:ss)
        s2 : Time-string 2 (hh:mm:ss)
  Returns  : the number of seconds difference
  ------------------------------------------------------------------*/
{
   char tmp[50];
   unsigned int hi1,mi1,si1;
   unsigned int hi2,mi2,si2;
   char *p;

   strcpy(tmp,s1); // copy string into temp. string
   p = strtok(tmp,":");
   if (p == NULL) return 0.0;
   else hi1 = atoi(p);
   p = strtok(NULL,":");
   if (p == NULL) return 0.0;
   else mi1 = atoi(p);
   p = strtok(NULL,"\0");
   if (p == NULL) return 0.0;
   else si1 = atoi(p);

   strcpy(tmp,s2); // copy string into temp. string
   p = strtok(tmp,":");
   if (p == NULL) return 0.0;
   else hi2 = atoi(p);
   p = strtok(NULL,":");
   if (p == NULL) return 0.0;
   else mi2 = atoi(p);
   p = strtok(NULL,"\0");
   if (p == NULL) return 0.0;
   else si2 = atoi(p);

   si1 += 60 * mi1 + 3600 * hi1; // convert to seconds
   si2 += 60 * mi2 + 3600 * hi2; // convert to seconds
   if (si1 > si2)
   {
      return (double)(si1 - si2);
   }
   else
   {
      return (double)(si2 - si1);
   } // else
} // diff_seconds

void calc_phases_start(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function finds, from the log file, the following:
             - the begin time of every log-file entry
             - the start line number of the mashing, sparging,
               boiling and chilling phases
             - the end line number of the mashing, sparging,
               boiling and chilling phases for every log-file entry.
             - the last value of ms_idx and sp_idx for every log-file entry.
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
   char         tmp[SLEN];
   char         *phlp;        /* temp. pointer */
   unsigned int line_nr = 1;  /* line number */
   int          log_idx = 0;  /* log entry index */
   int          std = 0;      /* value of ebrew_std */
   int          std_old;      /* previous value of std */
   int          i;            /* temp. variable */
   int          m_entry = 0;  /* new mash phase in a log-entry */
   int          s1_entry = 0; /* new sparging phase in a log-entry */
   int          s2_entry = 0; /* new sparging phase in a log-entry */

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
            GOTO_COLUMN(10,tmp); /* std is to be found in column 10 */
            std_old = std; // save previous value of std
            std     = atoi(phlp);
            //-----------------------------------------------
            // Find 1st entries of the various brewing phases
            //-----------------------------------------------
            // Mashing Phase
            // From 2 (Fill MLT) or 3 (Mash in Progress) -> 4 (Mash Timer Running) or 13 (Mash Preheat HLT)
            // ms_idx incremented from 13 (Mash Preheat HLT) -> 4 (Mash Timer Running)
            if ((p[log_idx].mashing_start[m_entry] == 0) &&
                (((std == S04_MASH_TIMER_RUNNING) && (std_old <= S03_WAIT_FOR_MLT_TEMP)) ||
                 ((std == S13_MASH_PREHEAT_HLT)   && (std_old <= S03_WAIT_FOR_MLT_TEMP)) ||
                 ((std == S04_MASH_TIMER_RUNNING) && (std_old == S13_MASH_PREHEAT_HLT))))
            {
               p[log_idx].mashing_start[m_entry] = line_nr;
               m_entry++;
            } /* if */
            // Sparging phase: MLT -> Boil
            if ((p[log_idx].sparging_start[s1_entry] == 0) && (std_old == S05_SPARGE_TIMER_RUNNING) &&
                ((std == S06_PUMP_FROM_MLT_TO_BOIL) || (std == S09_EMPTY_MLT)))
            {
               p[log_idx].sparging_start[s1_entry] = line_nr;
               s1_entry++;
            } /* if */
            // Sparging phase: HLT -> MLT
            if ((p[log_idx].sparging_start2[s2_entry] == 0) && (std == S07_PUMP_FROM_HLT_TO_MLT) &&
                ((std_old == S06_PUMP_FROM_MLT_TO_BOIL) || (std_old == S08_DELAY_xSEC)))
            {
               p[log_idx].sparging_start2[s2_entry] = line_nr;
               s2_entry++;
            } /* if */
            FIND_PHASE(p[log_idx].boil_start ,S11_BOILING             , S10_WAIT_FOR_BOIL);
            FIND_PHASE(p[log_idx].chill_start,S16_CHILL_PUMP_FERMENTOR, S12_BOILING_FINISHED);
         } /* if */

         if (line_nr == p[log_idx].eline)
         {
            /*------------------------------------------*/
            /* This is the end of a non-empty log entry */
            /*------------------------------------------*/
            strncpy(p[log_idx].etime,tmp,8); /* copy time string */
            p[log_idx].etime[8] = '\0';      /* terminate with '\0' */

            GOTO_COLUMN(8,tmp); /* sp_idx is to be found in column 8 */
            /* Next value to read is sp_idx */
            p[log_idx].lsp_idx = atoi(phlp);
            phlp = strtok(NULL,COLON); /* next value is ms_idx */
            p[log_idx].lms_idx = atoi(phlp);
            phlp = strtok(NULL,COLON); /* next value is std_state */
            p[log_idx].std_val = atoi(phlp);
            p[log_idx].time_period = diff_seconds(p[log_idx].etime,p[log_idx].btime);
            p[log_idx].time_period /= (p[log_idx].eline - p[log_idx].bline);
            log_idx++;    /* to next log entry */
            m_entry  = 0; /* reset mash entry */
            s1_entry = 0; /* reset sparging index */
            s2_entry = 0; /* reset sparging index */
         } /* else if */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
} /* calc_phases_start() */

void calc_sp_idx(FILE *fd, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function finds, for every log-file entry, the following:
             - the start line number where the last mash timer was started
             - the start line number of the last ebrew_std value
  Variables:
        fd : File descriptor for the log file, should be opened already
             by the calling program!
         p : Pointer to an log_struct struct
  Returns  : -
  ------------------------------------------------------------------*/
{
   char         tmp[SLEN];
   char         *phlp;              /* temp. pointer */
   unsigned int line_nr    = 1;     /* line number */
   int          log_idx    = 0;     /* log entry index */
   int          i;                  /* temp. variable */
   int          std;                /* value of ebrew_std */
   int          ffound     = FALSE; /* TRUE = start of last state found */
   int          mfound     = FALSE; /* TRUE = start of last mash timer started */

   rewind(fd); /* start again at beginning of file */
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if (line_nr > p[log_idx].bline && line_nr <= p[log_idx].eline)
         {
            GOTO_COLUMN(10,tmp); /* std_ebrew is to be found in column 10 */
            /* Next value to read is std_ebrew */
            std = atoi(phlp);

            /*----------------------------------------------------------*/
            /* Now find the 1st line where the last mash timer started. */
            /*----------------------------------------------------------*/
            if (std == S04_MASH_TIMER_RUNNING)
            {
               if (!mfound)
               {
                  p[log_idx].start_lmtmr = line_nr;
                  mfound                 = TRUE;
               } /* if */
            }
            else
            {
               mfound = FALSE;
            } /* else */

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

void print_p_struct(int log_idx, log_struct p[])
/*------------------------------------------------------------------
  Purpose  : This function prints all the information from the
             log struct p, into the file log_structp.txt
  Variables:
   log_idx : The total number of log-entries in the log_struct p[]
  Returns  : -
  ------------------------------------------------------------------*/
{
   FILE *f1; // file-descriptor
   int  i;   // loop-variable
   static int pp_1st_time = TRUE;

   if ((f1 = fopen("log_structp.txt","w")) != NULL)
   {
      if (pp_1st_time)
      {
         fprintf(f1,"ms_idx, tmr_ms_idx   , std           , start_lstd, start_lmtmr\n");
         fprintf(f1,"sp_idx, mashing_start, sparging_start, boil_start, chill_start\n");
         pp_1st_time = FALSE;
      } // if
      for (i = 0; i < log_idx; i++)
      {
         fprintf(f1,"%02d: %s, %d-%d, %s-%s\n",i,
                 p[i].brew_date,
                 p[i].bline,
                 p[i].eline,
                 p[i].btime,
                 p[i].etime);
         fprintf(f1,"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                 p[i].lms_idx,           // Last known value of ms_idx
                 p[i].tmr_ms_idx,        // Timer value of ms_idx timer
                 p[i].std_val,           // Last known value of std_state
                 p[i].start_lstd,        // The start line number of the latest ebrew_std
                 p[i].start_lmtmr,       // The start line number of the latest mash timer start
                 p[i].lsp_idx,           // Number of Sparging cycles
                 p[i].mashing_start[0],  // Start line of Mashing
                 p[i].sparging_start[0], // Start line of Sparging
                 p[i].boil_start,        // Start line of Boiling
                 p[i].chill_start);      // Start line of Chilling
         fprintf(f1,"------------------------------------------------------------------\n");
      } // for
      fclose(f1);
   } // if
} // print_p_struct()

void add_seconds(char *s, int seconds)
/*------------------------------------------------------------------
  Purpose  : This function adds a number of seconds to the time string
             given in s
  Variables:
        s  : The time-string (hh:mm:ss)
    seconds: The number of seconds to add to the time string
  Returns  : -
  ------------------------------------------------------------------*/
{
   char tmp[50];
   unsigned int hi,mi,si;
   char *p;

   strcpy(tmp,s); // copy string into temp. string
   p = strtok(tmp,":");
   if (p == NULL) return;
   else hi = atoi(p);
   p = strtok(NULL,":");
   if (p == NULL) return;
   else mi = atoi(p);
   p = strtok(NULL,"\0");
   if (p == NULL) return;
   else si = atoi(p);

   si += 60 * mi + 3600 * hi; // convert to seconds
   si += seconds;  // now add seconds

   hi  = si / 3600; // calculate hours
   si -= hi * 3600; // calculate seconds without hours
   mi  = si / 60;   // calculate minutes
   si -= mi * 60;   // calculate remaining seconds
   sprintf(s,"%02d:%02d:%02d",hi,mi,si);
} // add_seconds

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
   char         tmp[SLEN];
   char         *phlp;  /* temp. pointer */
   char         sstart[] = "Date of brewing: "; /* string to start a log-file entry */
   int          sslen    = strlen(sstart);      /* length of sstart[] */
   int          log_idx  = 0; /* log entry index */
   unsigned int line_nr  = 1; /* start at line 1 in log file */
   int          rval     = 0; /* return value */
   int          i;

   /*-----------------------------------------------------------------*/
   /* 1) Go through the log-file and determine the brew-date and the  */
   /*    begin- and end-lines of every log-entry.                     */
   /*-----------------------------------------------------------------*/
   do
   {
      if (fgets(tmp,SLEN,fd) != NULL)
      {
         if (((phlp = strstr(tmp,sstart)) != NULL) || !strncmp(tmp,sstart,sslen))
         {
            //--------------------------------------------------------
            // We found the start of a new log-entry in the file
            // Start with writing the last line of the previous entry.
            //--------------------------------------------------------
            if (log_idx > 0)
            {
              /*--------------------------------------------------------*/
              /* fill in the last valid line of the previous log entry. */
              /* Assume that the last entry may be wrong.               */
              /* Overwrite log-entry if it is less than a minute.       */
              /*--------------------------------------------------------*/
              CALC_LAST_LINE;
            } /* if */
            //--------------------------------------------------
            // Initialize log-entry and fill in bline and eline
            //--------------------------------------------------
            strncpy(p[log_idx].brew_date, phlp + sslen, 10);
            p[log_idx].brew_date[10] = '\0'; /* terminate with '\0' */
            p[log_idx].bline = line_nr + LOG_HDR_SIZE;
            /* Init. some values that we need later on */
            p[log_idx].btime[0]       = '\0';
            p[log_idx].etime[0]       = '\0';
            p[log_idx].lms_idx        = 0;
            p[log_idx].std_val        = 0;
            p[log_idx].start_lstd     = 0;
            p[log_idx].start_lmtmr    = 0;
            for (i = 0; i < MAX_MS; i++)
            {
               p[log_idx].mashing_start[i]   = 0;
            } // for i
            for (i = 0; i < MAX_SP; i++)
            {
               p[log_idx].sparging_start[i]  = 0;
               p[log_idx].sparging_start2[i] = 0;
            } // for i
            p[log_idx].boil_start     = 0;
            p[log_idx].chill_start    = 0;
            p[log_idx].tmr_ms_idx     = 0;
            p[log_idx].lsp_idx        = 0;
            log_idx++; /* increment log entry index */
         } /* if */
      } /* if */
      line_nr++; /* read next line */
   } while (!feof(fd));
   CALC_LAST_LINE;
   rval = log_idx; /* return value */

   /*------------------------------------------------------------------------*/
   /* 2) Go through the log-file again and add additional information,       */
   /*    such as the begin- and end-time, ms_idx, sp_idx and std_state.      */
   /*------------------------------------------------------------------------*/
   calc_phases_start(fd,p);

   /*-----------------------------------------------------------------*/
   /* 3) Go through the log-file AGAIN and find the start line number */
   /*    where the last mash timer was started (start_lmtmr) and the  */
   /*    start line number of the last ebrew_std value                */
   /*-----------------------------------------------------------------*/
   calc_sp_idx(fd,p);

   /*---------------------------------*/
   /* 4) Fill in the mash timer info. */
   /*---------------------------------*/
   for (i = 0; i < log_idx; i++)
   {
      if (p[i].start_lmtmr == 0)
      {
         //------------------------------------------------------------
         // The latest mash timer has not started yet, meaning that the
         // new reference temp. has not been reached yet.
         //------------------------------------------------------------
         p[i].tmr_ms_idx = NOT_STARTED;
      }
      else
      {
         if (p[i].mashing_start[p[i].lms_idx] > 0)
         {
            //-------------------------------------------------------------
            // lms_idx is the highest mash-index found in the log-entry
            // If mashing_start[lms_idx] is > 0, then the timer was running
            //                                   at the time of the crash.
            // The latest mash timer was running.
            // Estimate the time for the crash + rebooting at 4 * 5 seconds.
            //--------------------------------------------------------------
            p[i].tmr_ms_idx = (int)(p[i].time_period * (p[i].eline + 1 + 4 - p[i].start_lmtmr));
         } // if
         else
         {
            //--------------------------------------------------------------
            // If = 0, then heating to the next temp. was in progress
            //-------------------------------------------------------------
            p[i].tmr_ms_idx = NOT_STARTED;
         } // else
      } // else
   } // for
   print_p_struct(log_idx,p); // Debug: print p struct to a file
   return rval;
} /* decode_log_file() */

int read_input_file(char *inf, maisch_schedule ms[], int *count, double ts,
                    int init, int *vmash, int *vsparge)
/*------------------------------------------------------------------
  Purpose  : The purpose of this routine is to read the mash scheme
             file specified by 'inf'. Each line in inf consists of two
             real data elements, separated by a ','. The result is
             placed in the mash schedule struct ms[].
             The time of every mash phase is given in minutes.
  Variables:
       inf : A string containing the input file name
       ms[]: Array of structs holding the maisch schedule (temp & times)
     count : The total number of temp & time values read
         ts: The sample time in seconds
       init: [NO_INIT_TIMERS, INIT_TIMERS]
      vmash: The total amount of mash water in Litres
    vsparge: The total amount of sparge water in Litres
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
      ret = FALSE;
   }
   else
   {
      ret = TRUE; // init. return value
      i   = 0;    // init. index value
      fgets(tmp,SLEN,f1); /* read dummy first line */
      fgets(tmp,SLEN,f1); /* read dummy second line */
      fgets(tmp,SLEN,f1); /* read dummy third line */
      // Start with reading Amount of Mash water from file
      *vmash = 0; // result in case of file-error
      if (fgets(tmp,SLEN,f1) != NULL)
      {
         p = strtok(tmp,":"); // Read part of line up to ':'
         if (p != NULL)
         {
            p = strtok(NULL,",");
            if (p != NULL)
            {
               *vmash = atoi(p);
            } // if
         } // if
      } // if
      // Now read Amount of Sparge water from file
      *vsparge = 0; // result in case of file-error
      if (fgets(tmp,SLEN,f1) != NULL)
      {
         p = strtok(tmp,":"); // Read part of line up to ':'
         if (p != NULL)
         {
            p = strtok(NULL,",");
            if (p != NULL)
            {
               *vsparge = atoi(p);
            } // if
         } // if
      } // if
      fgets(tmp,SLEN,f1); /* read dummy blank line */
      fgets(tmp,SLEN,f1); /* read dummy comment line */
      fgets(tmp,SLEN,f1); /* read dummy comment line */
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
                  ms[i].time = (atof(p) * 60.0) / ts; /* convert to seconds */
                  (*count)++;
               } // if
            } // while
            if (init == INIT_TIMERS)
            {
               ms[i].timer         = NOT_STARTED; /* init. timer to not started */
               ms[i].time_stamp[0] = '\0';        /* init. time-stamp to empty string */
            } // if
         } // if
         i++;
      }
      while ((feof(f1) == 0) && (i < MAX_MS));
      fclose(f1); // close the input file
   } // else
   return ret;
} // read_input_file

void init_frl_struct(flow_rate_low_struct *p, int perc)
{
	p->frl_std     = 0;    // set state nr to 0
	p->frl_tmr     = 0;    // set timer to 0
	p->frl_det_lim = 0.0;  // set detection limit to 0
	p->frl_perc    = perc; // init. percentage value
} // init_frl_struct()

/*------------------------------------------------------------------
  Purpose  : This function is called every second from
             STD state 'Empty MLT' and is used to detect when the MLT
             is empty. In that case the flow-rate becomes low / zero.
  Variables:
  flow_rate: the flow-rate from MLT to Boil-kettle in L/min.
  Returns  : 1: flow-rate is low, 0: flow-rate is still high
  ------------------------------------------------------------------*/
int flow_rate_low(double flow_rate, flow_rate_low_struct *p)
{
   int retv = 0; // return-value

   switch (p->frl_std)
   {
        case 0 : // Give flow-rate time to increase because of the MA-filter
                 // Use 20 seconds which is enough to stabilize the MA-filter
				 if (++p->frl_tmr > 20)
                 {
					p->frl_det_lim = 0; // reset detection-limit
					p->frl_tmr     = 0; // reset timer
                    p->frl_std     = 1; // goto next state
                 } // if
                 break;
        case 1 : // Calculate the average flow-rate for the next 20 seconds
                 // Use perc of average flow-rate as detection-limit
                 if (++p->frl_tmr > 20)
                 {
                    p->frl_det_lim *= p->frl_perc; // Percentage of average flowrate
					p->frl_det_lim /= 2000.0;      // 20 samples, divide by 100%
                    p->frl_std      = 2;           // goto next state
                 } // if
                 else p->frl_det_lim += flow_rate; // calculate average flowrate
                 break;
        case 2: // Now check if flow-rate decreases
                if (flow_rate < p->frl_det_lim) // flow-rate < percentage of average flow-rate?
                     retv = 1;
                break;
        default: p->frl_std = 0;
                 p->frl_tmr = 0;
                 break;
   } // switch
   return retv;
} // flow_rate_low()

int update_std(volume_struct *vol, double thlt, double tmlt, double tboil,
               double *tset_hlt, double *tset_mlt, double *tset_boil,
               unsigned int *kleppen, maisch_schedule ms[],
               sparge_struct *sps, std_struct *std, int ui, int std_fx)
/*------------------------------------------------------------------
  Purpose  : This function contains the State Transition Diagram (STD)
             for the ebrew program and is called every second.
             First: the new state is calculated based upon the conditions.
             Then: the settings of the valves are calculated and returned
             in 'kleppen'. It uses various static variables (see begin of file).
  Variables:
      *vol  : Struct containing all volume variables
      thlt  : The actual temperature of the HLT
      tmlt  : The actual temperature of the MLT
      tboil : The actual temperature of the Boil-Kettle
 *tset_hlt  : The reference temperature for the HLT
 *tset_mlt  : The reference temperature for the MLT
 *tset_boil : The reference temperature for the Boil-Kettle
  *kleppen : Every bit represent a valve (1=ON, 0=OFF):
             Bits 15..8: 0 = Auto, 1 = Manual Override for valves V7-V1 + Pump
             Bits  7..1: Valves V7-V1
             Bit      0: Pump
      ms[] : Array containing the mash schedule
       sps : Struct containing all sparge variables
       std : Struct containing all STD variables
        ui : User-Interactions:
             Bit 0: 1 = PID Controller enabled
             Bit 1: 1 = User selects 'Mash added to MLT'
             Bit 2: 1 = User selects 'Boiling started'
             Bit 3: 1 = User selects 'Start chilling'
             Bit 4: 1 = User selects 'Chilling finished'
    std_fx : Fix value for ebrew_std, -1 if no fix
  Returns  : The values of ebrew_std is returned
  ------------------------------------------------------------------*/
{
   //-------------------------------------------------------------------
   //           |----------------------------> Pump 2 for HLT heat-exchanger
   //           |  |-------------------------> Main Brewing-Pump
   //           |  |  |----------------------> Future Use
   //           |  |  |  |-------------------> Out to Boil kettle
   //           |  |  |  |  |----------------> Out to Counter Flow Chiller (CFC)
   //           |  |  |  |  |  |-------------> Out Bypass Heat Exchanger
   //           |  |  |  |  |  |  |----------> Out to HLT Heat Exchanger
   //           |  |  |  |  |  |  |  |-------> In from Boil kettle
   //           |  |  |  |  |  |  |  |  |----> In from HLT
   //           |  |  |  |  |  |  |  |  |  |-> In from MLT
   //           P1 P0 V8 V7 V6 V5 V4 V3 V2 V1 Description              Hex.
   //------------------------------------------------------------------------
   // State 00: 0  0  0  0  0  0  0  0  0  0  Initialisation            0x0000
   // State 01: 1  0  0  0  0  0  0  0  0  0  Wait for HLT Temp.        0x0200
   // State 02: 1  1  0  0  0  0  1  0  1  1  Fill MLT                  0x030B
   // State 03: 1  1  0  0  0  0  1  0  0  1  Wait for MLT Temp.        0x0309
   // State 04: 1  1  0  0  0  0  1  0  0  1  Mash Timer Running        0x0309
   // State 05: 1  1  0  0  0  0  1  0  0  1  Sparge Timer Running      0x0309
   // State 06: 0  1  0  1  0  0  0  0  0  1  Pump from MLT to Boil     0x0141
   // State 07: 1  1  0  0  0  0  1  0  1  1  Pump from HLT to MLT      0x030B
   // State 08: 0  0  0  0  0  0  0  0  0  0  Delay x Sec.              0x0000
   // State 09: 0  1  0  1  0  0  0  0  0  1  Empty MLT                 0x0141
   // State 10: 0  0  0  0  0  0  0  0  0  0  Wait for Boil             0x0000
   // State 11: 0  0  0  0  0  0  0  0  0  0  Boiling                   0x0000
   // State 12: 0  0  0  0  0  0  0  0  0  0  Boiling Finished          0x0000
   // State 13: 1  0  0  0  0  0  0  0  0  0  Mash PreHeat HLT          0x0200
   // State 14: 0  0  0  0  0  0  0  0  1  1  Pump Prefill              0x0003
   // State 15: 0  0  0  0  0  0  0  0  0  0  Add Malt to MLT           0x0000
   // State 16: 0  1  0  0  1  0  0  1  0  0  Chill & Pump to Fermentor 0x0124
   // State 17: 0  0  0  0  0  0  0  0  0  0  Finished!                 0x0000
   // State 18: 0  0  0  0  0  0  0  0  0  0  Mash Rest 5 minutes       0x0000
   //------------------------------------------------------------------------
   unsigned int  klepstanden[] = {0x0000, 0x0200, 0x030B, 0x0309, 0x0309, /* 04 */
                         /* 05 */ 0x0309, 0x0141, 0x030B, 0x0000, 0x0141, /* 09 */
                         /* 10 */ 0x0000, 0x0000, 0x0000, 0x0200, 0x0003, /* 14 */
                         /* 15 */ 0x0000, 0x0124, 0x0000, 0x0000}; /* 18 */
   unsigned int  klepstand; // Help var. = klepstanden[std->ebrew_std]

   switch (std->ebrew_std)
   {
      //---------------------------------------------------------------------------
      // S00_INITIALISATION: system off, wait for PID controller to be switched on
      // - If PID is enabled, then goto S01_WAIT_FOR_HLT_TEMP
      //---------------------------------------------------------------------------
      case S00_INITIALISATION:
           std->ms_idx = 0; // init. index in mash schem
           *tset_mlt   = ms[std->ms_idx].temp;
           *tset_hlt   = *tset_mlt + 2 * sps->temp_offset;
           *tset_boil  = 0.0; // Setpoint Temp. for Boil-Kettle
           sps->pid_ctrl_boil_on = 0; // Disable PID-Controller for Boil-Kettle
           if (ui & UI_PID_ON)
           {
              std->ebrew_std = S01_WAIT_FOR_HLT_TEMP;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S01_WAIT_FOR_HLT_TEMP: Thlt < tset_HLT, continue to heat HLT
      // - If Thlt > Tset_hlt, goto S14_PUMP_PREFILL
      //---------------------------------------------------------------------------
      case S01_WAIT_FOR_HLT_TEMP:
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + 2 * sps->temp_offset;
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (thlt >= *tset_hlt) // HLT TEMP is OK
           {
              vol->Vhlt_old  = vol->Vhlt; // remember old value
              std->timer3    = 0;         // init. '1 minute' timer
              std->ebrew_std = S14_PUMP_PREFILL;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S14_PUMP_PREFILL: since the pump needs to be filled with water, prefill
      // the pump with water for a minute, then goto S02_FILL_MLT
      //---------------------------------------------------------------------------
      case S14_PUMP_PREFILL:
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + 2 * sps->temp_offset;
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (++std->timer3 >= TMR_PREFILL_PUMP) // Stay-here timer timeout?
           {
              std->ebrew_std = S02_FILL_MLT;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S02_FILL_MLT: Thlt >= tset_HLT, ready to fill MLT with water from HLT
      // - Set pump on
      // - If Vmlt > dough-in volume, goto S03_MASH_IN_PROGRESS
      //---------------------------------------------------------------------------
      case S02_FILL_MLT:
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + 2 * sps->temp_offset;
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (vol->Vmlt >= sps->mash_vol)
           {
              std->ebrew_std = S03_WAIT_FOR_MLT_TEMP;
           } // if
           break;
      //---------------------------------------------------------------------------
      //                            M A S H I N G
      //---------------------------------------------------------------------------
      // S03_MASH_IN_PROGRESS: Start of a mash phase, Tmlt < Tset_mlt, PID is active
      // - Tset_hlt = tset (from mash scheme) + double offset
      // - Start mash timer, then goto S04_MASH_TIMER_RUNNING
      //---------------------------------------------------------------------------
      case S03_WAIT_FOR_MLT_TEMP:
           // Add double offset as long as Tmlt < Tset_mlt + Offset2
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + 2 * sps->temp_offset;
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (tmlt >= ms[std->ms_idx].temp + sps->temp_offset2)
           {  // Tmlt has reached Tset_mlt + Offset2, start mash timer
              if (std->ms_idx == 0)
              {  // We need to add malt to the MLT first
                 std->ebrew_std = S15_ADD_MALT_TO_MLT;
              } // if
              else
              {  // already added malt to the MLT, start directly with timer
                 ms[std->ms_idx].timer = 0; // start the corresponding mash timer
                 std->ebrew_std        = S04_MASH_TIMER_RUNNING;
              } // else
           } // if
           break;
      //---------------------------------------------------------------------------
      // S15_ADD_MALT_TO_MLT: Before a mash-timer is started, the malt has to be
      // added to the MLT first. This is done only once (when ms_idx == 0). If this
      // is done, then goto S04_MASH_TIMER_RUNNING.
      //---------------------------------------------------------------------------
      case S15_ADD_MALT_TO_MLT:
           if (ui & UI_MALT_ADDED_TO_MLT)
           {  // malt is added to MLT, start mash timer
              ms[std->ms_idx].timer = 0; // start the corresponding mash timer
              if (std->mash_rest)
              {    // Start with mash rest for 5 min. after malt is added
                   std->mrest_tmr = 0; // init mash rest timer
                   std->ebrew_std = S18_MASH_REST_5_MIN;
              }
              else std->ebrew_std = S04_MASH_TIMER_RUNNING;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S04_MASH_TIMER_RUNNING: Tmlt has reached Tset_mlt (+Offset2)
      // - Tset_hlt = tset (from mash scheme) + single offset
      // - Increment mash timer until time-out
      // - If more mash phases are required, goto MASH_PREHEAT_HLT else goto SPARGE
      //---------------------------------------------------------------------------
      case S04_MASH_TIMER_RUNNING:
           ms[std->ms_idx].timer++; // increment mash timer
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + sps->temp_offset;  // Single offset
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (std->ms_idx < std->ms_tot - 1)
           {
              // There's next mash phase
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].time - sps->ph_timer)
              {
                 std->ebrew_std = S13_MASH_PREHEAT_HLT;
              } // if
              // else remain in this state (timer is incremented)
           }
           else
           {
              // This is the last mash phase, continue with sparging
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].time) // time-out?
              {
                 std->sp_idx    = 0;                       // init. sparging index
                 std->timer1    = sps->sp_time_ticks - 10; // timer1 -> TIME-OUT - 10 sec.
                 std->ebrew_std = S05_SPARGE_TIMER_RUNNING;       // goto SPARGING phase
              } // if
              // else remain in this state (timer is incremented)
           } // else
           break;
      //---------------------------------------------------------------------------
      // S13_MASH_PREHEAT_HLT: Increase Thlt PREHEAT_TIMER seconds before the time-
      //                       out of the mash timer. Heat HLT until the next temperature has
      //                       been reached.
      // - Tset_hlt = next tset (from mash scheme) + double offset
      // - Increment mash timer until time-out, then goto S03_MASH_IN_PROGRESS
      //---------------------------------------------------------------------------
      case S13_MASH_PREHEAT_HLT:
           ms[std->ms_idx].timer++; // increment mash timer
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = ms[std->ms_idx + 1].temp + 2 * sps->temp_offset;
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (ms[std->ms_idx].timer >= ms[std->ms_idx].time) // time-out?
           {
              std->ms_idx++; // increment index in mash scheme
              std->ebrew_std = S03_WAIT_FOR_MLT_TEMP;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S18_MASH_REST_5_MIN: If the 'Mash Rest' checkbox is enabled, remain for
      //                       5 minutes in this state, giving the malt time to
      //                       soak up all the water. If you leave the pump running
      //                       the filter-bed can be sucked dry.
      // - Tset_hlt = next tset (from mash scheme) + double offset
      // - Increment mash timer until time-out, then goto S03_MASH_IN_PROGRESS
      //---------------------------------------------------------------------------
      case S18_MASH_REST_5_MIN:
           ms[std->ms_idx].timer++; // increment mash timer
           std->mrest_tmr++;        // increment mash-rest timer
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + sps->temp_offset; // Single offset
           *tset_boil = 0.0; // Setpoint Temp. for Boil-Kettle
           if (std->ms_idx < std->ms_tot - 1)
           {
              // There's a next mash phase
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].time - sps->ph_timer)
              {  // Preheat timer has priority, since it also switches off the pump
                 std->ebrew_std = S13_MASH_PREHEAT_HLT;
              } // if
              else if (!std->mash_rest || (std->mrest_tmr >= TMR_MASH_REST_5_MIN))
              {  // after 5 min., goto normal mashing phase and switch pump on
                 std->ebrew_std = S04_MASH_TIMER_RUNNING;
              } // if
              // else remain in this state (timer is incremented)
           }
           else
           {  // Only possible when there is only one mash phase
              // This is the last mash phase, continue with sparging
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].time) // time-out?
              {
                 std->sp_idx    = 0;                        // init. sparging index
                 std->timer1    = sps->sp_time_ticks - 10;  // timer1 -> TIME-OUT - 10 sec.
                 std->ebrew_std = S05_SPARGE_TIMER_RUNNING; // goto SPARGING phase
              } // if
              // else remain in this state (timer is incremented)
           } // else
           break;
      //---------------------------------------------------------------------------
      //                            S P A R G I N G
      //---------------------------------------------------------------------------
      // S05_SPARGE_TIMER_RUNNING:
      // - Increment timer until time-out
      // - If more sparge phases are required, goto S06_PUMP_FROM_MLT_TO_BOIL
      //                                  else goto S09_EMPTY_MLT
      //---------------------------------------------------------------------------
      case S05_SPARGE_TIMER_RUNNING:
           *tset_mlt = ms[std->ms_idx].temp;
           *tset_hlt = *tset_mlt + sps->temp_offset; // Single offset
           if ((tboil > sps->boil_min_temp) || sps->pid_ctrl_boil_on)
           {  // There is sufficient wort in the Boil-Kettle
              *tset_boil = sps->sp_preboil; // PreBoil Temperature Setpoint
              sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           }
           else *tset_boil = 0.0;
           if (++std->timer1 >= sps->sp_time_ticks)
           {
              vol->Vmlt_old  = vol->Vmlt;  // save Vmlt for state 6 & 9
              vol->Vboil_old = vol->Vboil; // save Vboil for state 6 & 9
              if (std->sp_idx < sps->sp_batches)
              {
                 std->ebrew_std = S06_PUMP_FROM_MLT_TO_BOIL; // Pump to BOIL (again)
              } // if
              else
              {  // Init flowrate low struct with percentage
		 init_frl_struct(&frl_empty_mlt,vol->min_flowrate_mlt_perc);
                 std->timer1    = 0;             // reset timer1
                 std->ebrew_std = S09_EMPTY_MLT; // Finished with Sparging, empty MLT
              } // else if
           } // if
           break;
      //---------------------------------------------------------------------------
      // S06_PUMP_FROM_MLT_TO_BOIL:
      // - Pump wort from MLT to Boil kettle until Vmlt change > sparge batch size
      // - The goto S08_DELAY_xSEC
      //---------------------------------------------------------------------------
      case S06_PUMP_FROM_MLT_TO_BOIL:
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = *tset_mlt + sps->temp_offset; // Single offset
           if ((tboil > sps->boil_min_temp) || sps->pid_ctrl_boil_on)
           {  // There is sufficient wort in the Boil-Kettle
              *tset_boil = sps->sp_preboil; // PreBoil Temperature Setpoint
              sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           }
           else *tset_boil = 0.0;

           if (vol->Vmlt <= vol->Vmlt_old - (std->sp_idx == 0 ? sps->sp_vol_batch0 : sps->sp_vol_batch))
           {
              std->timer2    = 0;          // init. x sec. timer
              std->ebrew_std = S08_DELAY_xSEC;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S07_PUMP_FROM_HLT_TO_MLT:
      // - Pump hot water from HLT to MLT until Vmlt change > sparge batch size
      // - The goto S05_SPARGE_TIMER_RUNNING
      //---------------------------------------------------------------------------
      case S07_PUMP_FROM_HLT_TO_MLT:
           *tset_mlt = ms[std->ms_idx].temp;
           *tset_hlt = *tset_mlt + sps->temp_offset; // Single offset
           if ((tboil > sps->boil_min_temp) || sps->pid_ctrl_boil_on)
           {  // There is sufficient wort in the Boil-Kettle
              *tset_boil = sps->sp_preboil; // PreBoil Temperature Setpoint
              sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           }
           else *tset_boil = 0.0;

           if (vol->Vmlt >= vol->Vmlt_old + sps->sp_vol_batch)
           {
              std->sp_idx++;              // Increase #Sparging Sessions done
              std->timer1    = 0;         // init timer1
              std->ebrew_std = S05_SPARGE_TIMER_RUNNING;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S08_DELAY_xSEC: Delay of TIMEOUT_xSEC seconds
      //---------------------------------------------------------------------------
      case S08_DELAY_xSEC:
           *tset_mlt = ms[std->ms_idx].temp;
           *tset_hlt = *tset_mlt + sps->temp_offset; // Single offset
           if ((tboil > sps->boil_min_temp) || sps->pid_ctrl_boil_on)
           {  // There is sufficient wort in the Boil-Kettle
              *tset_boil = sps->sp_preboil; // PreBoil Temperature Setpoint
              sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           }
           else *tset_boil = 0.0;

           if (++std->timer2 >= TMR_DELAY_xSEC)
           {
              vol->Vhlt_old  = vol->Vhlt;          // remember old value
              vol->Vmlt_old  = vol->Vmlt;          // remember current MLT volume
              std->ebrew_std = S07_PUMP_FROM_HLT_TO_MLT;
           } // if
           else if (vol->Vmlt > vol->Vmlt_old - sps->sp_vol_batch)
           {
              std->ebrew_std = S06_PUMP_FROM_MLT_TO_BOIL; // prevent false transitions
           } // else if
           break;
      //---------------------------------------------------------------------------
      // S09_EMPTY_MLT: Sparging is finished, pump all wort to the boil kettle
      //---------------------------------------------------------------------------
      case S09_EMPTY_MLT:
           *tset_mlt  = ms[std->ms_idx].temp;
           *tset_hlt  = 0.0;          // Disable HLT PID-Controller
           *tset_boil = sps->sp_boil; // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 1; // Enable PID-Controller for Boil-Kettle
           if (flow_rate_low(vol->Flow_rate_mlt_boil,&frl_empty_mlt))
           {
              std->ebrew_std = S10_WAIT_FOR_BOIL;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S10_WAIT_FOR_BOIL: After all wort is pumped to the boil kettle, it takes
      // a while before boiling actually starts. When the user selects the
      // 'Boiling Started' option or when the boil-kettle temperature exceeds
      // the minimum temperature BOIL_DETECT, goto S11_BOILING.
      //---------------------------------------------------------------------------
      case S10_WAIT_FOR_BOIL:
           *tset_hlt  = 0.0; // disable heating element
           *tset_boil = sps->sp_boil; // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 1; // Enable PID-Controller for Boil-Kettle
           if ((ui & UI_BOILING_STARTED) || (tboil > sps->boil_detect))
           {
              std->timer5    = 0; // init. timer for boiling time
              std->ebrew_std = S11_BOILING;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S11_BOILING: Boiling has started, remain here for BOIL_TIME minutes
      //---------------------------------------------------------------------------
      case S11_BOILING:
           *tset_boil = sps->sp_boil; // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 1; // Enable PID-Controller for Boil-Kettle
           if (++std->timer5 >= sps->boil_time_ticks)
           {
              vol->Flow_cfc_out_reset_value = vol->Flow_cfc_out; // reset Flow_cfc_out
              std->ebrew_std = S12_BOILING_FINISHED;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S12_BOILING_FINISHED: Boiling has finished, remain here until the user
      // has set up everything for chilling, then goto S15_CHILL_PUMP_FERMENTOR
      // Continu with boiling in this state, so that the brewer can manually control
      // a longer boiling time, to achieve final gravity of the wort.
      //---------------------------------------------------------------------------
      case S12_BOILING_FINISHED:
           *tset_boil = sps->sp_boil; // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 1; // Enable PID-Controller for Boil-Kettle
           if (ui & UI_START_CHILLING)
           {  // Init flowrate low struct with percentage
	      init_frl_struct(&frl_empty_boil,vol->min_flowrate_boil_perc);
              std->ebrew_std = S16_CHILL_PUMP_FERMENTOR;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S16_CHILL_PUMP_FERMENTOR: The boiled wort is sent through the counterflow
      // chiller and directly into the fermentation bin.
      //---------------------------------------------------------------------------
      case S16_CHILL_PUMP_FERMENTOR:
           *tset_boil = 0.0;  // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 0; // Disable PID-Controller for Boil-Kettle
           if ((ui & UI_CHILLING_FINISHED) || (flow_rate_low(vol->Flow_rate_cfc_out,&frl_empty_boil)))
           {
              std->ebrew_std = S17_FINISHED;
           } // if
           break;
      //---------------------------------------------------------------------------
      // S17_FINISHED: All wort in pumped into the fermentation bin. The brewing
      // session is finished. This is the end-state.
      //---------------------------------------------------------------------------
      case S17_FINISHED:
           *tset_boil = 0.0;  // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 0; // Disable PID-Controller for Boil-Kettle
           // Remain in this state until Program Exit.
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
   if ((*kleppen & P1M) == 0x0000) // Pump 2 No Manual Override ?
   {
      if (klepstand & P1b) *kleppen |= P1b;
      else                 *kleppen &= ~P1b;
   }
   if ((*kleppen & P0M) == 0x0000) // Pump No Manual Override ?
   {
      if (klepstand & P0b) *kleppen |= P0b;
      else                 *kleppen &= ~P0b;
   }
   if ((*kleppen & V8M) == 0x0000) // V8 No Manual Override ?
   {
      if (klepstand & V8b) *kleppen |= V8b;
      else                 *kleppen &= ~V8b;
   }
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
   return std->ebrew_std; // return the new state of the STD
} // update_std()

void init_ma(ma *p, int N, double init_val)
/*------------------------------------------------------------------
  Purpose  : This function initialises the Moving Average (MA) struct
             that is used in the moving_average() function
  Variables:
        *p : Pointer to the ma struct (specify a new struct for every
             new filter!)
         N : The order of the Moving Average filter (1 = No filtering)
   init_val: The value to init the MA filter to
  Returns  : -
  ------------------------------------------------------------------*/
{
   int i; // temp. var.

   p->N     = N;        // order of MA filter
   p->index = 0;        // index in cyclic array
   p->sum   = init_val; // running sum
   for (i = 0; i < N; i++)
   {
      p->T[i] = init_val / N; // set cyclic array to init. value
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

void init_sample_delay(ma *p, int TD)
/*------------------------------------------------------------------
  Purpose  : This function initialises the sample delay function.
             This function uses the same struct as the moving_average()
             function uses (the MA struct).
  Variables:
        *p : Pointer to the ma struct (specify a new struct for every
             new filter!)
        TD : The number of sample-time delays (should be < MAX_MA)
  Returns  : -
  ------------------------------------------------------------------*/
{
   int i; // temp. var.

   if (TD < MAX_MA) p->N = TD;    // number of sample-time delays
   else             p->N = MAX_MA;
   p->index = 0;                  // index in cyclic array
   for (i = 0; i < p->N; i++)
   {
      p->T[i] = 0.0; // init. all stores to 0
   } // for
} // init_sample_delay()

double sample_delay(ma *p, double x)
/*------------------------------------------------------------------
  Purpose  : This function delays a signal N samples and outputs the
             value x[k-N] (N is defined in init_sample_delay()):

             Initialisation: p->N must have a value.
                             p->index should be init. to 0.0.
  Variables:
        *p : Pointer to the ma struct (specify a new struct for every
             new filter!)
         x : The actual input value x[k]
  Returns  : the delayed input value x[k-N]
  ------------------------------------------------------------------*/
{
   double xn;     // get x[k-N]
   if (p->N == 0) // no delay
   {
       xn = x; // return input value directly
   }
   else
   {
      xn = p->T[p->index]; // get x[k-N]
      p->T[p->index] = x;         // store x[k] in array
      if (++(p->index) >= p->N)   // update index in cyclic array
      {
         p->index = 0; // restore to 1st position
      } // if
   } // else
   return xn;      // return value = x[k-N]
} // sample_delay()

void slope_limiter(const double lim, const double Told, double *Tnew)
/*------------------------------------------------------------------
  Purpose  : This function limits the increase of Tnew by lim.

                              Tnew
                               |  ------------  lim
                               |/
              -----------------/------------------ -> Tin - Tout
                              /|
              -lim ----------  |

  Variables:
       lim : The limiting value
      Told : The previous value of the variable to limit
      Tnew : The output value of the variable to limit
  Returns  : none
  ------------------------------------------------------------------*/
{
   double diff = *Tnew - Told; // calculate difference

   if      (diff > lim)  *Tnew =  Told + lim;
   else if (diff < -lim) *Tnew =  Told - lim;
   // else: do nothing, Tnew is within slope limits
} // slope_limiter()

