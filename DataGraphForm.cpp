// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.12  2005/08/28 22:17:30  Emile
// - DataGrapfForm: TTimer replaced again for TAnimTimer
// - Debug-code added for MA filter of Vmlt
// - 'H' key now toggles heater between 0% and 100%
// - Text Temp. Sensor 1 & 2 replaced by Temp. Sensor HLT & MLT
// - Printing of vmlt_unf (padc.ad4) removed again from log-file
//
// Revision 1.11  2005/06/11 12:35:07  Emile
// - Keyboard shortcuts 'P' (Pump toggle) and '1' .. '7' (valve toggles) added.
// - Added transition from state 8 back to state 6. This prevents a transition
//   change during sparging when a glitch on Vmlt happens.
// - Added Vmlt_unf (=padc.ad4) to log-file for debugging purposes.
//
// Revision 1.10  2004/05/08 14:52:50  emile
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
// Revision 1.9  2004/04/25 14:02:17  emile
// - Added a 'type C' PID controller, function pid_reg3(). Possible to select
//   this from the PID settings dialog screen. Left the old one in for
//   compatibility. New registry variable PID_Model.
// - Gamma added to log-file, so that the PID controller can be tuned.
//
// Revision 1.8  2004/03/10 10:10:37  emile
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
// Revision 1.7  2004/01/31 16:01:04  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.6  2003/09/15 20:37:21  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.5  2003/07/11 18:34:45  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm92_read())
//
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
   FILE *fd2;
   struct time t1;
   int  i;
    
   if ((fd = fopen(LOGFILE,"a")) != NULL)
   {
      gettime(&t1);
      fprintf(fd,"%02d:%02d:%02d, ",t1.ti_hour,t1.ti_min,t1.ti_sec);
      fprintf(fd,"%6.2f, %6.2f, %6.2f, %6.2f, %5.1f, %6.2f,%3d,%3d,%3d, %5.1f\n",
                                                      MainForm->tset_mlt,
                                                      MainForm->tset_hlt,
                                                      MainForm->thlt,
                                                      MainForm->tmlt,
                                                      MainForm->ttriac,
                                                      MainForm->volumes.Vmlt,
                                                      MainForm->std.sp_idx,
                                                      MainForm->std.ms_idx,
                                                      MainForm->std.ebrew_std,
                                                      MainForm->gamma);
      // Debugging of MLT Volume
      if ((MainForm->volumes.Vmlt < 0.9 * MainForm->vmlt_old) &&
          (MainForm->volumes.Vmlt > 10.0))
      {
         if ((fd2 = fopen("ma_mlt.log","a")) != NULL)
         {
            fprintf(fd2,"%02d:%02d:%02d, ",t1.ti_hour,t1.ti_min,t1.ti_sec);
            fprintf(fd2,"%6.2f %6.2f %3d %3d\n",MainForm->padc.ad4,
                                                MainForm->str_vmlt.sum,
                                                MainForm->str_vmlt.N,
                                                MainForm->str_vmlt.index);
            for (i = 0; i < MainForm->str_vmlt.N; i++)
            {
               fprintf(fd2,"%6.2f",MainForm->str_vmlt.T[i]);
            } // for
            fprintf(fd2,"\n\n");
            fclose(fd2);
         } // if
      } // if
      // Debugging of MLT Volume

      fclose(fd);
   } /* if */

   if (ShowDataGraphs)
   {
      ShowDataGraphs->Tad1_Graph->DataPoint(clRed,MainForm->thlt);
      ShowDataGraphs->Tad1_Graph->Update();
      ShowDataGraphs->Tad2_Graph->DataPoint(clRed,MainForm->tmlt);
      ShowDataGraphs->Tad2_Graph->Update();
   } // if
} // TShowDataGraphs::GraphTimerTimer()
//---------------------------------------------------------------------------
#define IDH_VIEWDATA (0x10070)
void __fastcall TShowDataGraphs::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_VIEWDATA);
}
//---------------------------------------------------------------------------

