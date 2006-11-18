// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.  
// --------------------------------------------------------------------------
// $Log$
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
}
//---------------------------------------------------------------------------

void __fastcall TViewMashProgress::CloseButtonClick(TObject *Sender)
{
   Hide(); // Hide modeless Dialog
}
//---------------------------------------------------------------------------

void __fastcall TViewMashProgress::UpdateTimerTimer(TObject *Sender)
{
   int    i;
   char   s[120];
   double x;
   char   time_stamp[50];
   maisch_schedule *p; // pointer naar maisch_schedule structure
   int    ltr;

   if (ViewMashProgress)
   {
      ViewMashProgress->Memo1->Lines->Clear();
      ViewMashProgress->Memo1->Lines->Add("Mash Temp Time  PreHt Timer Timer       Time-");
      ViewMashProgress->Memo1->Lines->Add("idx  [°C] [sec] [sec] [sec] Status      Stamp");
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
         sprintf(s,"%2d %5.0f %5.0f %5.0f %5.0f ",i, p->temp, p->time, x,
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
      sprintf(s,"mash index (ms_idx) = %d",MainForm->std.ms_idx);
      ViewMashProgress->Memo1->Lines->Add(s);

      //-----------------------------------
      // Now print the Sparging Information
      //-----------------------------------
      ViewMashProgress->Memo1->Lines->Add(" ");
      ViewMashProgress->Memo1->Lines->Add("index  HLT to MLT   MLT to Boil  Sparging");
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);

      ltr = MainForm->sp.sp_vol_batch;
      if ((MainForm->std.ebrew_std == S06_PUMP_FROM_MLT_TO_BOIL) &&
          (prev_ebrew_std == S05_SPARGING_REST))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.mlt2boil[MainForm->std.sp_idx],TimeToStr(Time()).c_str());
      } // if
      if ((MainForm->std.ebrew_std == S07_PUMP_FROM_HLT_TO_MLT) &&
          ((prev_ebrew_std == S08_DELAY_xSEC) || (prev_ebrew_std == S06_PUMP_FROM_MLT_TO_BOIL)))
      {  // New transition detected, copy time-stamp into array of strings
         strcpy(MainForm->sp.hlt2mlt[1+MainForm->std.sp_idx],TimeToStr(Time()).c_str());
      } // if
      prev_ebrew_std = MainForm->std.ebrew_std; // update previous value

      sprintf(s,"  0     - - - -  %12s    - - , %2d L",MainForm->sp.mlt2boil[0],ltr);
      ViewMashProgress->Memo1->Lines->Add(s);
      for (i = 1; i <= MainForm->sp.sp_batches; i++)
      {
         if (i == MainForm->sp.sp_batches)
              sprintf(s,"%3d %12s %12s    %2d L, Empty",i,MainForm->sp.hlt2mlt[i],MainForm->sp.mlt2boil[i],ltr);
         else sprintf(s,"%3d %12s %12s    %2d L, %2d L",i,MainForm->sp.hlt2mlt[i],MainForm->sp.mlt2boil[i],ltr,ltr);
         ViewMashProgress->Memo1->Lines->Add(s);
      } // for i
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      sprintf(s,"sp_idx = %d, Timer = %d/%d sec.",MainForm->std.sp_idx,
                                                  MainForm->std.timer1,
                                                  MainForm->sp.sp_time_ticks);
      ViewMashProgress->Memo1->Lines->Add(s);
      ViewMashProgress->Memo1->Lines->Add(" ");

      sprintf(s,"Timer2 (state 08->07) = %d/%d sec.",MainForm->std.timer2,MainForm->sp.to_xsec);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer3 (state 10->11) = %d/%d sec.",MainForm->std.timer3,MainForm->sp.to3);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer4 (state 11->10) = %d/%d sec.",MainForm->std.timer4,MainForm->sp.to4);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer5 (state 10->12) = %d/%d sec.",MainForm->std.timer5,MainForm->sp.boil_time_ticks);
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

