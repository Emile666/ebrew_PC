// ==========================================================================
// Filename    : ViewMashProgressForm.cpp
// Author      : Emile
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.
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

      sprintf(s,"  0     - - - -  %12s     - -  , %4.1f L",MainForm->sp.mlt2boil[0],MainForm->sp.sp_vol_batch0);
      ViewMashProgress->Memo1->Lines->Add(s);
      x = MainForm->sp.sp_vol_batch;
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
      sprintf(s,"Timer4 (state 'Mash Rest 5 Min.')    : %02d/300 sec.",MainForm->std.mrest_tmr);
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

