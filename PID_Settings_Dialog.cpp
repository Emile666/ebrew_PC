// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
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

