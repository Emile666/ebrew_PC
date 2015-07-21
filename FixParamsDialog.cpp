// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where various parameters can be manually
//               (fixed) to a particular value.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.8  2015/06/05 19:18:39  Emile
// - STD optimized for new solenoid valves. User Interaction dialog added
// - Calibration & Temp. correction added for flowsensors
//
// Revision 1.7  2005/10/23 12:44:38  Emile
// Several changes because of new hardware (MAX1238 instead of PCF8591):
// - Vhlt added, Vmlt and Ttriac now all adjustable to an AD-channel (the
//   PCF8591 is still supported).
// - 2 time-slices added, Vhlt, Vmlt and Ttriac are read in 3 different time-slices.
// - Ttriac also printed as label to screen, plus Switch and Fix added
// - Alive bit is now active-low, changed in exit_ebrew()
// - Registry vars removed: VREF3, VREF4, DAC, VHLT_SIMULATED
// - Registry vars added: VHLT_SRC, VHLT_A, VHLT_B, VMLT_SRC, VMLT_A, VMLT_B,
//                        TTRIAC_SRC, TTRIAC_A, TTRIAC_B and MA_VHLT
// - Debugging for ma filter removed again
// Changes to i2c_dll:
// - File reorganised into 4 layers with routines for more clarity
// - i2c_read/i2c_write: i2c_address() call added in VELLEMAN_CARD mode
// - i2c_address: i2c_start() call added in VELLEMAN_CARD mode
// - Routines added: get_analog_input() and max1238_read()
// - i2c_stop() changed into i2c_stop(enum pt_action pta) so that PortTalk
//   can be closed or remain open
// - init_adc() removed
//
// Revision 1.6  2004/05/08 14:52:50  emile
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
// Revision 1.5  2004/03/10 10:10:38  emile
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
// Revision 1.4  2003/06/01 19:36:34  emile
// - Switch/Fix added for Vmlt
//
// Revision 1.3  2003/06/01 14:08:06  emile
// - Same as previous log entry: CVS and BCB got confused => saved old files.
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

#include "Unit1.h"
#include "FixParamsDialog.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFix_Params *Fix_Params;
//---------------------------------------------------------------------------
__fastcall TFix_Params::TFix_Params(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CB_GammaClick(TObject *Sender)
{
   Gamma_MEdit->Enabled = CB_Gamma->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CB_TsetClick(TObject *Sender)
{
   Tset_MEdit->Enabled = CB_Tset->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Gamma_MEditClick(TObject *Sender)
{
   Gamma_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Tset_MEditClick(TObject *Sender)
{
   Tset_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_Tad1Click(TObject *Sender)
{
   Tad1_MEdit->Enabled = CB_Tad1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Tad1_MEditClick(TObject *Sender)
{
   Tad1_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_Tad2Click(TObject *Sender)
{
   Tad2_MEdit->Enabled = CB_Tad2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Tad2_MEditDblClick(TObject *Sender)
{
   Tad2_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_stdClick(TObject *Sender)
{
   STD_MEdit->Enabled = CB_std->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_ttriacClick(TObject *Sender)
{
   Ttriac_MEdit->Enabled = CB_ttriac->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Ttriac_MEditClick(TObject *Sender)
{
   Ttriac_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::STD_MEditClick(TObject *Sender)
{
   STD_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_vmltClick(TObject *Sender)
{
   Vmlt_MEdit->Enabled = CB_vmlt->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Vmlt_MEditClick(TObject *Sender)
{
   Vmlt_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_vhltClick(TObject *Sender)
{
   Vhlt_MEdit->Enabled = CB_vhlt->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Vhlt_MEditClick(TObject *Sender)
{
   Vhlt_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

#define IDH_FIXPARAMETERS (0x010050)
void __fastcall TFix_Params::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_FIXPARAMETERS);
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Apply_ButtonClick(TObject *Sender)
{
      // Get Ref. Temp (Tset_hlt)
      MainForm->swfx.tset_hlt_sw = CB_Tset->Checked;
      if (MainForm->swfx.tset_hlt_sw)
      {
         MainForm->swfx.tset_hlt_fx = Tset_MEdit->Text.ToDouble();
      } // if
      // Get PID Output (Gamma)
      MainForm->swfx.gamma_sw = CB_Gamma->Checked;
      if (MainForm->swfx.gamma_sw)
      {
         MainForm->swfx.gamma_fx = Gamma_MEdit->Text.ToDouble();
      } // if
      // Get Thlt
      MainForm->swfx.thlt_sw = CB_Tad1->Checked;
      if (MainForm->swfx.thlt_sw)
      {
        MainForm->swfx.thlt_fx = Tad1_MEdit->Text.ToDouble();
      } // if
      // Get Tmlt
      MainForm->swfx.tmlt_sw = CB_Tad2->Checked;
      if (MainForm->swfx.tmlt_sw)
      {
         MainForm->swfx.tmlt_fx = Tad2_MEdit->Text.ToDouble();
      } // if
      // Get STD state
      MainForm->swfx.std_sw = CB_std->Checked;
      if (MainForm->swfx.std_sw)
      {
         // Value must be between 0 and 13
         MainForm->swfx.std_fx = STD_MEdit->Text.ToInt();
         if (MainForm->swfx.std_fx < 0 ||
             MainForm->swfx.std_fx > S17_FINISHED)
         {
            MainForm->swfx.std_fx = 0; // reset to a safe value
         }
      } // if
      // Get Ttriac value
      MainForm->swfx.ttriac_sw = CB_ttriac->Checked;
      if (MainForm->swfx.ttriac_sw)
      {
         MainForm->swfx.ttriac_fx = Ttriac_MEdit->Text.ToDouble();
      } // if
      // Get Vhlt value
      MainForm->swfx.vhlt_sw = CB_vhlt->Checked;
      if (MainForm->swfx.vhlt_sw)
      {
         MainForm->swfx.vhlt_fx = Vhlt_MEdit->Text.ToDouble();
      } // if
      // Get Vmlt value
      MainForm->swfx.vmlt_sw = CB_vmlt->Checked;
      if (MainForm->swfx.vmlt_sw)
      {
         MainForm->swfx.vmlt_fx = Vmlt_MEdit->Text.ToDouble();
      } // if
} // TFix_Params::Apply_ButtonClick()
//---------------------------------------------------------------------------



