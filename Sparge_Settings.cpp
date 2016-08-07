// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the parameters for the sparging session
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
// Revision 1.3  2003/06/01 11:53:47  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
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

#include "Sparge_Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
__fastcall TSpargeSettings::TSpargeSettings(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#define IDH_SPARGESTDSETTINGS (0x100B0)
void __fastcall TSpargeSettings::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_SPARGESTDSETTINGS);
}
//---------------------------------------------------------------------------


