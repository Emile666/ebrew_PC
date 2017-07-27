// ==========================================================================
// Filename    : Sparge_Settings.cpp
// Author      : Emile
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
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


