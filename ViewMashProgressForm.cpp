// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.12  2015/06/05 19:18:40  Emile
// - STD optimized for new solenoid valves. User Interaction dialog added
// - Calibration & Temp. correction added for flowsensors
//
// Revision 1.11  2013/07/23 09:42:46  Emile
// - Fourth intermediate version: several Registry Settings added / removed.
// - Dialog Screens updated: better lay-out and matches new Registry Settings
// - Source-code improved for readability
//
// Revision 1.10  2007/07/06 22:23:01  Emile
// - The real time between two lines from a log-file is now used instead of a
//   fixed 5 sec. time when reading a log-file.
// - Leading-zero bug solved in Mash Progress screen
// - i2c_stop() only called with PT_CLOSE in case of exit of program
// - System Identification library functions added (but not used yet).
//
// Revision 1.9  2007/02/24 19:14:37  Emile
// Additional space added in Mash Progress screen.
//
// Revision 1.8  2006/11/18 23:06:37  Emile
// - View Mash/Sparging screen is improved: time-stamps are placed when a
//   mashing or sparging phase has started.
// - Read_log_file is improved: time-stamps are generated, so that old log-files
//   can be read and time-stamp information can be seen in the Mash/Sparging screen.
// - Datagraps are set to a step-size of 1 instead of 2 (1 div = 100 seconds now).
// - Main screen updated: Heating power is now in % and correct volumes are added.
//
// Revision 1.7  2004/05/08 14:52:52  emile
// - Mash pre-heat functionality added to STD. New registry variable PREHEAT_TIME.
//   tset_hlt is set to next mash temp. if mash timer >= time - PREHEAT_TIME
// - View mash progress screen: reorganised, pre-heat timers added, timers are now
//   in seconds instead of minutes.
// - update_tset() function removed. Now incorporated in STD, states 3-5 + (new state) 13.
// - THLT_HLIMIT and THLT_LLIMIT and state 4 'Bypass Heat Exchanger' removed
// - Reorganisation of several variables (e.g. ms_idx, ms_tot) into (other) structs.
// - 'Apply' Button added to Fix parameters dialog screen.
// - 'Edit mash scheme' no longer resets the (running) mash timers
// - 'Mash progress controlled by' function removed. Registry var 'mash_control' now
//   also removed.
// - Changing init. volume of HLT did not result in an update on screen. Corrected.
//
// Revision 1.6  2004/05/05 15:44:15  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.5  2004/03/10 10:10:39  emile
// - Reduced complexity of several routines:
//   - T50msecTimer split, new routine Generate_IO_Signals added
//   - PopupMenu1Popup now uses (new) macro SET_POPUPMENU
//   - Reset_I2C_Bus now included in SET_LED macro
// - Every I2C write action now in a separate time-slice to avoid
//   I2C bus errors if fscl is low
// - This is the first version where the help file function is enabled
//   - All help buttons and F1 function key are operational
//   - Help file sources: ebrew.rtf and ebrew.hpj are added to CVS
// - ad1, ad2 and ad3 variables -> thlt, tmlt and ttriac (new variables)
//
// Revision 1.4  2003/06/01 11:53:48  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
//
// Revision 1.3  2002/12/30 20:21:59  emile
// - Bug 2 29-12-02 solved: start mash timers if temp >= tref instead of >.
// - Bug 3 29-12-02 solved: deadlock in std_state 4 when mashing is finished.
// - Bug 4 29-12-02 solved: rush through STD. Sparging parameters were not
//   initialised. New function Init_Sparge_Settings() added.
// - Sparge variables now added to 'View Mash Progress' screen.
// - std_struct added for more flexibility of fixing STD values.
//
// Revision 1.2  2002/12/30 13:33:45  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "ViewMashProgressForm.h"
#include "Unit1.h"
#include "misc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AnimTimer"
#pragma resource "*.dfm"
TViewMashProgress *ViewMashProgress;
//---------------------------------------------------------------------------
__fastcall TViewMashProgress::TViewMashProgress(TComponent* Owner)
        : TForm(Owner)
{
   LongTimeFormat = "hh:mm:ss"; // global var. used by TimeToStr() function
}
//---------------------------------------------------------------------------

void __fastcall TViewMashProgress::CloseButtonClick(TObject *Sender)
{
   Hide(); // Hide modeless Dialog
}
//---------------------------------------------------------------------------

void __fastcall TViewMashProgress::UpdateTimerTimer(TObject *Sender)
{
   int    i,tmp;
   char   s[120];
   double x;
   maisch_schedule *p; // pointer naar maisch_schedule structure

   if (ViewMashProgress)
   {
      ViewMashProgress->Memo1->Lines->Clear();
      ViewMashProgress->Memo1->Lines->Add("Mash Temp Time  PreHt Timer Timer        Time-");
      ViewMashProgress->Memo1->Lines->Add("idx  [°C] [sec] [sec] [sec] Status       Stamp");
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      //------------------------------------------------------------------
      // Sample time of ms[] update is 1 second.
      // Time of ms[i].time was converted to seconds in read_input_file().
      //------------------------------------------------------------------
      for (i = 0; i < MainForm->std.ms_tot; i++)
      {
         p = &MainForm->ms[i]; // hulp pointer
         x = p->time - MainForm->sp.ph_timer;
         if (x < 0.0)
         {
            x = 0.0;
         } // if
         //---------------------------------------------------
         // Now update time_stamp if a mash-timer has started
         //---------------------------------------------------
         if ((p->timer != NOT_STARTED) && (strlen(p->time_stamp) == 0))
         {
            strcpy(p->time_stamp, TimeToStr(Time()).c_str());

         } // if
         sprintf(s,"%2d %5.0f %5.0f %5.0f %5.0f  ",i, p->temp, p->time, x,
                   p->timer == NOT_STARTED ? 0.0 : p->timer);
         if (p->timer == NOT_STARTED)
         {
            strcat(s,"Not Started ");
         } /* if */
         else if (p->timer < p->time)
         {
            strcat(s,"Running     ");
         }
         else
         {
            strcat(s,"Time-Out    ");
         } /* else */
         strcat(s,p->time_stamp);
         ViewMashProgress->Memo1->Lines->Add(s);
      } // for
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      sprintf(s,"Mash Index (ms_idx) = %d",MainForm->std.ms_idx);
      ViewMashProgress->Memo1->Lines->Add(s);

      //-----------------------------------
      // Now print the Sparging Information
      //-----------------------------------
      ViewMashProgress->Memo1->Lines->Add(" ");
      ViewMashProgress->Memo1->Lines->Add("index  HLT to MLT   MLT to Boil  Sparging");
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);

      if ((MainForm->std.ebrew_std == S06_PUMP_FROM_MLT_TO_BOIL) &&
          (prev_ebrew_std == S05_SPARGE_TIMER_RUNNING))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.mlt2boil[MainForm->std.sp_idx],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S07_PUMP_FROM_HLT_TO_MLT) &&
          ((prev_ebrew_std == S08_DELAY_xSEC) || (prev_ebrew_std == S06_PUMP_FROM_MLT_TO_BOIL)))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.hlt2mlt[1+MainForm->std.sp_idx],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S11_BOILING) && (prev_ebrew_std == S10_WAIT_FOR_BOIL))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.boil[0],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S12_BOILING_FINISHED) && (prev_ebrew_std == S11_BOILING))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.boil[1],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S16_CHILL_PUMP_FERMENTOR) && (prev_ebrew_std == S12_BOILING_FINISHED))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.chill[0],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S17_FINISHED) && (prev_ebrew_std == S16_CHILL_PUMP_FERMENTOR))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.chill[1],TimeToStr(Time()).c_str());
      } // if
      prev_ebrew_std = MainForm->std.ebrew_std; // update previous value

      x = MainForm->sp.sp_vol_batch;
      sprintf(s,"  0     - - - -  %12s     - -  , %4.1f L",MainForm->sp.mlt2boil[0],x);
      ViewMashProgress->Memo1->Lines->Add(s);
      for (i = 1; i <= MainForm->sp.sp_batches; i++)
      {
         if (i == MainForm->sp.sp_batches)
              sprintf(s,"%3d %12s %12s    %4.1f L, Empty"  ,i,MainForm->sp.hlt2mlt[i],MainForm->sp.mlt2boil[i],x);
         else sprintf(s,"%3d %12s %12s    %4.1f L, %4.1f L",i,MainForm->sp.hlt2mlt[i],MainForm->sp.mlt2boil[i],x,x);
         ViewMashProgress->Memo1->Lines->Add(s);
      } // for i
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      sprintf(s,"Sparge Index (sp_idx) = %d",MainForm->std.sp_idx);
      ViewMashProgress->Memo1->Lines->Add(s);
      ViewMashProgress->Memo1->Lines->Add(" ");

      sprintf(s,"Timer1 (state 'Sparge Timer Running'): %d/%d min.",MainForm->std.timer1/60,MainForm->sp.sp_time);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer2 (state 'Delay')               : %02d/%02d sec.",MainForm->std.timer2,TMR_DELAY_xSEC);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer3 (state 'Pump Pre-Fill')       : %02d/%02d sec.",MainForm->std.timer3,TMR_PREFILL_PUMP);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer5 (state 'Now Boiling')         : %d/%d min.",MainForm->std.timer5/60,MainForm->sp.boil_time);
      ViewMashProgress->Memo1->Lines->Add(s);
      s[0] = '\0';
      tmp  = MainForm->std.timer1 + MainForm->std.timer5;
      tmp  = (tmp % 60) / 5;
      for (i = 0; i < tmp; i++) strcat(s,".");
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Boiling  started at: %s",MainForm->sp.boil[0]);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Chilling started at: %s",MainForm->sp.chill[0]);
      ViewMashProgress->Memo1->Lines->Add(s);
   } // if
} // TViewMashProgress::UpdateTimerTimer()
//---------------------------------------------------------------------------

#define IDH_VIEWMASHSPARGEPROGRESS (0x10080)
void __fastcall TViewMashProgress::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_VIEWMASHSPARGEPROGRESS);
}
//---------------------------------------------------------------------------

