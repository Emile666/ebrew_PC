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
                                                      MainForm->tset,
                                                      MainForm->padc.ad1,
                                                      MainForm->padc.ad2,
                                                      MainForm->padc.ad3,
                                                      MainForm->Vmlt,
                                                      MainForm->PID_RB->ItemIndex,
                                                      MainForm->ms_idx,
                                                      MainForm->std_state);
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
