//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "ViewMashProgressForm.h"
#include "Unit1.h"
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
   int i;
   char s[80];
   double cnv = MainForm->pid_pars.ts / 60.0; // convert 5 sec. ticks into minutes

   if (ViewMashProgress)
   {
      ViewMashProgress->Memo1->Lines->Clear();
      ViewMashProgress->Memo1->Lines->Add("Index\tTemp.\tTime\tTimer\tStatus");
      ViewMashProgress->Memo1->Lines->Add("------------------------------------------------------------------");
      for (i = 0; i < MainForm->ms_tot; i++)
      {
         sprintf(s,"%3d\t%6.0f\t%6.0f\t%6.0f\t",i,
                   MainForm->ms[i].temp,
                   MainForm->ms[i].time * cnv,
                   MainForm->ms[i].timer == NOT_STARTED ? 0.0 : MainForm->ms[i].timer * cnv);
         if (MainForm->ms[i].timer == NOT_STARTED)
         {
            strcat(s,"Not Started");
         } // if
         else if (MainForm->ms[i].timer < MainForm->ms[i].time)
         {
            strcat(s,"Running");
         }
         else
         {
            strcat(s,"Time-Out");
         } // else
         ViewMashProgress->Memo1->Lines->Add(s);
      } // for
   } // if
} // TViewMashProgress::UpdateTimerTimer()
//---------------------------------------------------------------------------

