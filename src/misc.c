/*==================================================================
  File name : misc.c
  Author    : Emile
  Purpose   : This file contains several miscellaneous functions
  ==================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pid_reg.h" /* for TS */
#include "misc.h"

flow_rate_low_struct frl_empty_mlt;  // Needed for detection of low flowrate
flow_rate_low_struct frl_empty_boil; // Needed for detection of low flowrate
flow_rate_low_struct frl_empty_cip;  // Needed for detection of low flowrate

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
	p->frl_std         = 0;    // set state nr to 0
	p->frl_tmr         = 0;    // set timer to 0
        p->frl_min_det_lim = 1.5;  // Flow-sensor should at least give 1.5 L/min.
	p->frl_det_lim     = 0.0;  // set detection limit to 0
	p->frl_perc        = perc; // init. percentage value
} // init_frl_struct()

/*------------------------------------------------------------------
  Purpose  : This function is called every second from
             STD state 'Empty MLT' and is used to detect when the MLT
             is empty. In that case the flow-rate becomes low / zero.
             It is also used in STD state 'Chill & Pump to fermentation Bin'.
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
                 // Use 30 seconds which is enough to stabilize the MA-filter
		 if (++p->frl_tmr > 30)
                 {
		     p->frl_det_lim = 0.0; // reset detection-limit
                     p->frl_tmr     = 0;   // reset timer
                     p->frl_std     = 1;   // goto next state
                 } // if
                 break;
        case 1 : // Calculate the average flow-rate for the next 30 seconds
                 // Use perc of average flow-rate as detection-limit
                 if (++p->frl_tmr > 30)
                 {
                    p->frl_det_lim /= 30.0; // calculate average flow-rate
                    p->frl_std      = 2;    // goto next state
                 } // if
                 else p->frl_det_lim += flow_rate; // calculate average flowrate
                 break;
        case 2:  // Now check if average flow-rate is above a minimum. If not, repeat calculations
                 if (p->frl_det_lim > p->frl_min_det_lim)
                 {    // average flow-rate > minimum flow-rate?
                      p->frl_det_lim *= p->frl_perc; // Percentage of average flowrate
                      p->frl_det_lim /= 100.0;       // Divide by 100%
                      p->frl_std = 3;
                 } // if
                 else
                 {
                      p->frl_tmr = 0; // reset timer
                      p->frl_std = 0; // repeat measurements
                 } // else
                 break;
        case 3:  // Now check if flow-rate decreases
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
   //           |  |  |  |  |  |-------------> Future Use
   //           |  |  |  |  |  |  |----------> Out to HLT Heat Exchanger
   //           |  |  |  |  |  |  |  |-------> In from Boil kettle
   //           |  |  |  |  |  |  |  |  |----> In from HLT
   //           |  |  |  |  |  |  |  |  |  |-> In from MLT
   //           P1 P0 V8 V7 V6 V5 V4 V3 V2 V1 Description                Hex.
   //-------------------------------------------------------------------------
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
   // State 19: 1  1  0  0  0  0  1  0  0  1  Ready to add malt to MLT  0x0309
   // State 20: 0  0  0  0  0  0  0  0  0  0  CIP: Initalisation        0x0000   
   // State 21: 0  1  0  1  1  0  1  1  0  0  CIP: Heat Up              0x016C   
   // State 22: 0  1  0  1  1  0  1  1  0  0  CIP: Circulate 5 Minutes  0x016C   
   // State 23: 0  0  0  0  0  0  0  0  0  0  CIP: Rest 5 Minutes       0x0000   
   // State 24: 0  1  0  0  1  0  1  1  0  0  CIP: Drain Boil-Kettle 1  0x012C
   // State 25: 0  1  0  0  1  0  0  1  0  0  CIP: Drain Boil-Kettle 2  0x0124
   // State 26: 0  0  0  0  0  0  0  0  0  0  CIP: Fill HLT             0x0000
   // State 27: 0  1  0  1  0  0  0  0  1  0  CIP: Clean Output V7      0x0142
   // State 28: 0  1  0  0  1  0  0  0  1  0  CIP: Clean Output V6      0x0122
   // State 29: 0  1  0  0  0  0  1  0  1  0  CIP: Clean Output V4      0x010A
   // State 30: 0  0  0  0  0  0  0  1  1  0  CIP: Clean Input V3       0x0006
   // State 31: 0  0  0  0  0  0  0  0  1  1  CIP: Clean Input V1       0x0003
   // State 32: 0  0  0  0  0  0  0  0  0  0  CIP: End                  0x0000
//----------------------------------------------------------------------------
   unsigned int  klepstanden[] = {0x0000, 0x0200, 0x030B, 0x0309, 0x0309, /* 04 */
                         /* 05 */ 0x0309, 0x0141, 0x030B, 0x0000, 0x0141, /* 09 */
                         /* 10 */ 0x0000, 0x0000, 0x0000, 0x0200, 0x0003, /* 14 */
                         /* 15 */ 0x0000, 0x0124, 0x0000, 0x0000, 0x0309, /* 19 */
                         /* 20 */ 0x0000, 0x016C, 0x016C, 0x0000, 0x012C, /* 24 */
                         /* 25 */ 0x0124, 0x0000, 0x0142, 0x0122, 0x010A, /* 29 */
                         /* 30 */ 0x0006, 0x0003, 0x0000}; /* 32 */

   unsigned int  klepstand; // Help var. = klepstanden[std->ebrew_std]

   switch (std->ebrew_std)
   {
      //---------------------------------------------------------------------------
      // S00_INITIALISATION: system off, wait for PID controller to be switched on
      // - If PID is enabled, then goto S01_WAIT_FOR_HLT_TEMP
      //---------------------------------------------------------------------------
      case S00_INITIALISATION:
           std->ms_idx = 0; // init. index in mash schem
           *tset_mlt   = ms[std->ms_idx].temp;          // get temp. from mash-scheme
           *tset_hlt   = *tset_mlt + sps->temp_offset0; // compensate for dough-in losses
           *tset_boil  = 0.0;                           // Setpoint Temp. for Boil-Kettle
           sps->pid_ctrl_boil_on = 0; // Disable PID-Controller for Boil-Kettle
           if (ui & UI_PID_ON)
           {  // start with normal brewing states
              std->ebrew_std = S01_WAIT_FOR_HLT_TEMP;
           } // if
           else if (ui & UI_CIP_INIT)
           {  // Clean-in-Place program
              std->ebrew_std = S20_CIP_INIT;
           } // else if
           break;
      //---------------------------------------------------------------------------
      // S01_WAIT_FOR_HLT_TEMP: Thlt < tset_HLT, continue to heat HLT
      // - If Thlt > Tset_hlt, goto S14_PUMP_PREFILL
      //---------------------------------------------------------------------------
      case S01_WAIT_FOR_HLT_TEMP:
           *tset_mlt  = ms[std->ms_idx].temp;          // get temp. from mash-scheme
           *tset_hlt  = *tset_mlt + sps->temp_offset0; // compensate for dough-in losses
           *tset_boil = 0.0;                           // Setpoint Temp. for Boil-Kettle
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
           *tset_mlt  = ms[std->ms_idx].temp;          // get temp. from mash-scheme
           *tset_hlt  = *tset_mlt + sps->temp_offset0; // compensate for dough-in losses
           *tset_boil = 0.0;                           // Setpoint Temp. for Boil-Kettle
           if (++std->timer3 >= TMR_PREFILL_PUMP)      // Stay-here timer timeout?
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
           *tset_mlt  = ms[std->ms_idx].temp;          // get temp. from mash-scheme
           *tset_hlt  = *tset_mlt + sps->temp_offset0; // compensate for dough-in losses
           *tset_boil = 0.0;                           // Setpoint Temp. for Boil-Kettle
           if (vol->Vmlt >= sps->mash_vol)
           {
              std->ebrew_std = S03_WAIT_FOR_MLT_TEMP;
           } // if
           break;
      //---------------------------------------------------------------------------
      //                            M A S H I N G
      //---------------------------------------------------------------------------
      // S03_MASH_IN_PROGRESS: Start of a mash phase, Tmlt < Tset_mlt, PID is active
      // - Tset_hlt = tset (from mash scheme) + double offset or dough-in offset
      // - Start mash timer, then goto S04_MASH_TIMER_RUNNING
      //---------------------------------------------------------------------------
      case S03_WAIT_FOR_MLT_TEMP:
           // Add double offset as long as Tmlt < Tset_mlt + Offset2
           *tset_mlt  = ms[std->ms_idx].temp;             // get temp. from mash-scheme
           // *tset_hlt is NOT set here, but in previous state (FILL_MLT or MASH_PREHEAT_HLT)
           *tset_boil = 0.0;                              // Setpoint Temp. for Boil-Kettle
           if (tmlt >= ms[std->ms_idx].temp + sps->temp_offset2)
           {  // Tmlt has reached Tset_mlt + Offset2, start mash timer
              if (std->ms_idx == 0)
              {  // We need to add malt to the MLT first
                 // Depending on the state of the checkbox 'Leave pumps running',
                 // we go directly to state 15 or only after the user selects the
                 // 'Start adding malt to the MLT' menu-option.
                 if (sps->leave_pumps_on)
                      std->ebrew_std = S19_RDY_TO_ADD_MALT; // leave pumps running for now
                 else std->ebrew_std = S15_ADD_MALT_TO_MLT; // switch pumps off directly
              } // if
              else
              {  // already added malt to the MLT, start directly with timer
                 ms[std->ms_idx].timer = 0; // start the corresponding mash timer
                 std->ebrew_std        = S04_MASH_TIMER_RUNNING;
              } // else
           } // if
           break;
      //---------------------------------------------------------------------------
      // S19_RDY_TO_ADD_MALT: Tmlt >= Tset_mlt, PID is active
      // - Keeps pumps running, so that water is circulating through the MLT
      // - If user presses 'OK' in dialog screen, then goto S15_ADD_MALT_TO_MLT
      //---------------------------------------------------------------------------
      case S19_RDY_TO_ADD_MALT:
           if (ui & UI_ADDING_MALT_TO_MLT)
              std->ebrew_std = S15_ADD_MALT_TO_MLT;
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
              //if (ms[std->ms_idx].timer >= ms[std->ms_idx].time - sps->ph_timer)
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].preht)
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
                 std->sp_idx    = 0;                        // init. sparging index
                 std->timer1    = sps->sp_time_ticks - 10;  // timer1 -> TIME-OUT - 10 sec.
                 std->ebrew_std = S05_SPARGE_TIMER_RUNNING; // goto SPARGING phase
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
              //if (ms[std->ms_idx].timer >= ms[std->ms_idx].time - sps->ph_timer)
              if (ms[std->ms_idx].timer >= ms[std->ms_idx].preht)
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
      //                            B O I L I N G   &    C H I L L I N G
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
              std->timer5    = 0; // assure a min. stay time, so transition is detected
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
           if ((++std->timer5 >= 10) && ((ui & UI_BOILING_STARTED) || (tboil > sps->boil_detect)))
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
              std->brest_tmr = 0; // init boil-rest timer
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
           *tset_boil = 0.0;          // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 0; // Disable PID-Controller for Boil-Kettle
           if ((!std->boil_rest || (++std->brest_tmr > TMR_BOIL_REST_5_MIN)) && (ui & UI_START_CHILLING))
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
      //---------------------------------------------------------------------------
      //               C L E A N I N G    I N    P L A C E   ( C I P )
      //---------------------------------------------------------------------------
      // S20_CIP_INIT: Start of Cleaning-In-Place Program.
      // Wait until Boil-kettle has been filled with 1% NaOH solution.
      //---------------------------------------------------------------------------
      case S20_CIP_INIT:
           *tset_boil            = 0.0; // Boil Temperature Setpoint
           sps->pid_ctrl_boil_on = 0;   // Disable PID-Controller for Boil-Kettle
           std->cip_circ         = 0;   // Init. CIP circulation counter
           if (ui & UI_CIP_BOIL_FILLED) // User indicated that Boil-Kettle is filled
                std->ebrew_std = S21_CIP_HEAT_UP;
           else if (!(ui & UI_CIP_INIT))
                std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S21_CIP_HEAT_UP: Heat Boil-kettle up to predefined temperature.
      //---------------------------------------------------------------------------
      case S21_CIP_HEAT_UP:
           *tset_boil            = CIP_TEMP_SETPOINT; // Boil-Kettle Temperature Setpoint
           sps->pid_ctrl_boil_on = 2;           // Enable Feed-Forward for Boil-Kettle
           if (tboil > CIP_TEMP_SETPOINT - 5.0) // Almost at setpoint temperature
           {
              std->cip_tmr1  = 0;        // Init. CIP timer
              std->ebrew_std = S22_CIP_CIRC_5_MIN;  
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S22_CIP_CIRC_5_MIN: Circulate NaOH solution through brewing system pipes
      //---------------------------------------------------------------------------
      case S22_CIP_CIRC_5_MIN:
           *tset_boil            = CIP_TEMP_SETPOINT; // Boil-Kettle Temperature Setpoint
           sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           if (++std->cip_tmr1 >= TMR_CIP_CIRC_TIME)
           {
              std->cip_tmr1  = 0;        // Reset CIP timer  
              if (++std->cip_circ > 1)   // Count number of CIP circulations
              {    // Init flowrate low struct with percentage
	           init_frl_struct(&frl_empty_cip,vol->min_flowrate_boil_perc);
                   std->ebrew_std = S24_CIP_DRAIN_BOIL1;
	      }	// if
              else std->ebrew_std = S23_CIP_REST_5_MIN;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S23_CIP_REST_5_MIN: Rest period, all valves and pump off
      //---------------------------------------------------------------------------
      case S23_CIP_REST_5_MIN:
           *tset_boil            = CIP_TEMP_SETPOINT; // Boil-Kettle Temperature Setpoint
           sps->pid_ctrl_boil_on = 1;    // Enable PID-Controller for Boil-Kettle
           if (++std->cip_tmr1 >= TMR_CIP_REST_TIME)
           {
              std->cip_tmr1  = 0;        // Reset CIP timer
              std->ebrew_std = S22_CIP_CIRC_5_MIN;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S24_CIP_DRAIN_BOIL1: Empty Boil-Kettle, remove NaOH solution. Do this by
      //                      placing the CFC-output and MLT-return hoses in the drain.
      //---------------------------------------------------------------------------
      case S24_CIP_DRAIN_BOIL1:
           *tset_boil            = 0.0; // Boil-Kettle Temperature Setpoint
           sps->pid_ctrl_boil_on = 0;   // Disable PID-Controller for Boil-Kettle
           if (ui | UI_CIP_HOSES_IN_DRAIN)
              std->ebrew_std = S25_CIP_DRAIN_BOIL2;
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S25_CIP_DRAIN_BOIL2: Empty Boil-Kettle, remove NaOH solution. Check when
      //                      CFC-output flowrate is low, then boil-kettle is empty.
      //---------------------------------------------------------------------------
      case S25_CIP_DRAIN_BOIL2:
           *tset_boil            = 0.0; // Boil-Kettle Temperature Setpoint
           sps->pid_ctrl_boil_on = 0;   // Disable PID-Controller for Boil-Kettle
           if (flow_rate_low(vol->Flow_rate_cfc_out,&frl_empty_cip))
              std->ebrew_std = S26_CIP_FILL_HLT;
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S26_CIP_FILL_HLT: Fill HLT with sufficient fresh water
      //---------------------------------------------------------------------------
      case S26_CIP_FILL_HLT:
           if (ui & UI_CIP_HLT_FILLED)
	   {  // User indicated that HLT has been filled with fresh water
              std->cip_tmr1  = 0;
              std->ebrew_std = S27_CIP_CLEAN_OUTPUT_V7;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S27_CIP_CLEAN_OUTPUT_V7: Clean output V7 of brewing system with fresh water
      //---------------------------------------------------------------------------
      case S27_CIP_CLEAN_OUTPUT_V7:
           if (++std->cip_tmr1 >= TMR_CIP_CLEAN_OUTPUTS)
           {
              std->cip_tmr1  = 0; // Reset CIP timer
              std->ebrew_std = S28_CIP_CLEAN_OUTPUT_V6;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S28_CIP_CLEAN_OUTPUT_V6: Clean output V6 of brewing system with fresh water
      //---------------------------------------------------------------------------
      case S28_CIP_CLEAN_OUTPUT_V6:
           if (++std->cip_tmr1 >= TMR_CIP_CLEAN_OUTPUTS)
           {
              std->cip_tmr1  = 0; // Reset CIP timer
              std->ebrew_std = S29_CIP_CLEAN_OUTPUT_V4;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S29_CIP_CLEAN_OUTPUT_V4: Clean output V4 of brewing system with fresh water
      //---------------------------------------------------------------------------
      case S29_CIP_CLEAN_OUTPUT_V4:
           if (++std->cip_tmr1 >= TMR_CIP_CLEAN_OUTPUTS)
           {
              std->cip_tmr1  = 0; // Reset CIP timer
              std->ebrew_std = S30_CIP_CLEAN_INPUT_V3;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S30_CIP_CLEAN_INPUT_V3: Clean input V3 of brewing system with fresh water.
      //                         This is done by gravity-feed, not with a pump.
      //---------------------------------------------------------------------------
      case S30_CIP_CLEAN_INPUT_V3:
           if (++std->cip_tmr1 >= TMR_CIP_CLEAN_INPUTS)
           {
              std->cip_tmr1  = 0; // Reset CIP timer
              std->ebrew_std = S31_CIP_CLEAN_INPUT_V1;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      //---------------------------------------------------------------------------
      // S31_CIP_CLEAN_INPUT_V1: Clean input V1 of brewing system with fresh water.
      //                         This is done by gravity-feed, not with a pump.
      //---------------------------------------------------------------------------
      case S31_CIP_CLEAN_INPUT_V1:
           if (++std->cip_tmr1 >= TMR_CIP_CLEAN_INPUTS)
           {
              std->ebrew_std = S32_CIP_END;
           } // if
           else if (!(ui & UI_CIP_INIT))
              std->ebrew_std = S00_INITIALISATION;
           break;
      case S32_CIP_END:
           // End of CIP-Program. UI (Main-Program) sets state to S00_Initialisation
           break;
      //---------------------------------------------------------------------------
      // Default: should never get here
      //---------------------------------------------------------------------------
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

