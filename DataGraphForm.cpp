// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.3  2002/12/30 20:21:58  emile
// - Bug 2 29-12-02 solved: start mash timers if temp >= tref instead of >.
// - Bug 3 29-12-02 solved: deadlock in std_state 4 when mashing is finished.
// - Bug 4 29-12-02 solved: rush through STD. Sparging parameters were not
//   initialised. New function Init_Sparge_Settings() added.
// - Sparge variables now added to 'View Mash Progress' screen.
// - std_struct added for more flexibility of fixing STD values.
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <dos.h>
#include "DataGraphForm.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PERFGRAP"
#pragma link "AnimTimer"
#pragma resource "*.dfm"
TShowDataGraphs *ShowDataGraphs;
//---------------------------------------------------------------------------
__fastcall TShowDataGraphs::TShowDataGraphs(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TShowDataGraphs::Button1Click(TObject *Sender)
{
   Hide(); // Hide modeless Dialog        
}
//---------------------------------------------------------------------------
void __fastcall TShowDataGraphs::GraphTimerTimer(TObject *Sender)
{
   FILE *fd;
   struct time t1;

   if ((fd = fopen(LOGFILE,"a")) != NULL)
   {
      gettime(&t1);
      fprintf(fd,"%02d:%02d:%02d, ",t1.ti_hour,t1.ti_min,t1.ti_sec);
      fprintf(fd,"%6.2f, %6.2f, %6.2f, %6.2f, %5.1f, %6.2f,%3d,%3d,%3d\n",
                                                      MainForm->gamma,
                                                      MainForm->tset_hlt,
                                                      MainForm->padc.ad1,
                                                      MainForm->padc.ad2,
                                                      MainForm->padc.ad3,
                                                      MainForm->Vmlt,
                                                      MainForm->PID_RB->ItemIndex,
                                                      MainForm->ms_idx,
                                                      MainForm->std.ebrew_std);
      fclose(fd);
   } /* if */

   if (ShowDataGraphs)
   {
      ShowDataGraphs->Gamma_Graph->DataPoint(clRed,MainForm->gamma);
      ShowDataGraphs->Gamma_Graph->Update();
      ShowDataGraphs->Tad1_Graph->DataPoint(clRed,MainForm->padc.ad1);
      ShowDataGraphs->Tad1_Graph->Update();
      ShowDataGraphs->Tad2_Graph->DataPoint(clRed,MainForm->padc.ad2);
      ShowDataGraphs->Tad2_Graph->Update();
   } // if
} // TShowDataGraphs::GraphTimerTimer()
//---------------------------------------------------------------------------
