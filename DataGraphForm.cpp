// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.4  2003/06/01 11:53:47  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
//
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
                                                      MainForm->tset_mlt,
                                                      MainForm->tset_hlt,
                                                      MainForm->padc.ad1, // HLT
                                                      MainForm->padc.ad2, // MLT
                                                      MainForm->padc.ad3, // TTriac
                                                      MainForm->Vmlt,
                                                      MainForm->PID_RB->ItemIndex,
                                                      MainForm->ms_idx,
                                                      MainForm->std.ebrew_std);
      fclose(fd);
   } /* if */

   if (ShowDataGraphs)
   {
      ShowDataGraphs->Tad1_Graph->DataPoint(clRed,MainForm->padc.ad1);
      ShowDataGraphs->Tad1_Graph->Update();
      ShowDataGraphs->Tad2_Graph->DataPoint(clRed,MainForm->padc.ad2);
      ShowDataGraphs->Tad2_Graph->Update();
   } // if
} // TShowDataGraphs::GraphTimerTimer()
//---------------------------------------------------------------------------
