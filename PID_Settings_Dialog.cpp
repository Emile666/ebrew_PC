// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.4  2004/03/10 10:10:38  emile
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
// Revision 1.3  2003/06/29 13:11:34  emile
// - Time switch function added (PID options screen). The PID controller is
//   enabled when the predefined date and time have been met.
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PID_Settings_Dialog.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TPID_Settings *PID_Settings;
//----------------------------------------------------------------------------
__fastcall TPID_Settings::TPID_Settings(TComponent *Owner)
	: TForm(Owner)
{
}
//----------------------------------------------------------------------------

void __fastcall TPID_Settings::RG2Click(TObject *Sender)
{
   if (RG2->ItemIndex == 1) // On
   {
      Date_Edit->Enabled  = true;
      Date_Label->Enabled = true;
      Time_Edit->Enabled  = true;
      Time_Label->Enabled = true;
   }
   else
   {
      Date_Edit->Enabled  = false;
      Date_Label->Enabled = false;
      Time_Edit->Enabled  = false;
      Time_Label->Enabled = false;
   } // else
}
//---------------------------------------------------------------------------

#define IDH_PIDSETTINGS (0x100A0)
void __fastcall TPID_Settings::Button3Click(TObject *Sender)
{
   Application->HelpContext(IDH_PIDSETTINGS);
}
//---------------------------------------------------------------------------

void __fastcall TPID_Settings::CB_Pid_out1Click(TObject *Sender)
{
   // Non-modulating Gas-burner
   if (CB_Pid_out1->Checked)
   {
      Burner_On->Enabled  = true;
      Burner_Off->Enabled = true;
      Label10->Enabled    = true;
      Label11->Enabled    = true;
      Label12->Enabled    = true;
      Label13->Enabled    = true;
   }
   else
   {
      Burner_On->Enabled  = false;
      Burner_Off->Enabled = false;
      Label10->Enabled    = false;
      Label11->Enabled    = false;
      Label12->Enabled    = false;
      Label13->Enabled    = false;
   } // else
} // CB_Pid_out1Click()
//---------------------------------------------------------------------------

void __fastcall TPID_Settings::CB_Pid_out2Click(TObject *Sender)
{
   // modulating Gas-burner
   if (CB_Pid_out2->Checked)
   {
      DAC_A_Edit->Enabled = true;
      DAC_B_Edit->Enabled = true;
      Label14->Enabled    = true;
      Label16->Enabled    = true;
   }
   else
   {
      DAC_A_Edit->Enabled = false;
      DAC_B_Edit->Enabled = false;
      Label14->Enabled    = false;
      Label16->Enabled    = false;
   } // else
} // CB_Pid_out2Click()
//---------------------------------------------------------------------------

