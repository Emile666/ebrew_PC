// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.  
// --------------------------------------------------------------------------
// $Log$
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

#define PRINT_TIMER_STATUS(timer,timeout)         \
         if (timer == NOT_STARTED)                \
         {                                        \
            strcat(s,"Not Started");              \
         } /* if */                               \
         else if (timer < timeout)                \
         {                                        \
            strcat(s,"Running");                  \
         }                                        \
         else                                     \
         {                                        \
            strcat(s,"Time-Out");                 \
         } /* else */                             \
         ViewMashProgress->Memo1->Lines->Add(s)

void __fastcall TViewMashProgress::UpdateTimerTimer(TObject *Sender)
{
   int    i;
   char   s[120];
   double x;

   if (ViewMashProgress)
   {
      ViewMashProgress->Memo1->Lines->Clear();
      ViewMashProgress->Memo1->Lines->Add("Mash  Temp. Time PH-Time Timer   Timer");
      ViewMashProgress->Memo1->Lines->Add("Index [°C]  [sec] [sec]  [sec]   Status");
      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      //------------------------------------------------------------------
      // Sample time of ms[] update is 1 second.
      // Time of ms[i].time was converted to seconds in read_input_file().
      //------------------------------------------------------------------
      for (i = 0; i < MainForm->std.ms_tot; i++)
      {
         x = MainForm->ms[i].time - MainForm->sp.ph_timer;
         if (x < 0.0)
         {
            x = 0.0;
         } // if
         sprintf(s,"%3d %5.0f %6.0f %6.0f %6.0f ",i,
                   MainForm->ms[i].temp,
                   MainForm->ms[i].time,
                   x,
                   MainForm->ms[i].timer == NOT_STARTED ? 0.0 : MainForm->ms[i].timer);
         PRINT_TIMER_STATUS(MainForm->ms[i].timer, MainForm->ms[i].time);
      } // for

      ViewMashProgress->Memo1->Lines->Add(BAR_LINE);
      sprintf(s,"ebrew_std = %d, ms_idx = %d, sp_idx = %d",MainForm->std.ebrew_std,
                                                           MainForm->std.ms_idx,
                                                           MainForm->std.sp_idx);
      ViewMashProgress->Memo1->Lines->Add(s);
      sprintf(s,"Timer1 (state 05->06) = %d/%d sec.",MainForm->std.timer1,MainForm->sp.sp_time_ticks);
      ViewMashProgress->Memo1->Lines->Add(s);
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

