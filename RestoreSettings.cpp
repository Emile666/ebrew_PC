// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the contents of the log-file are
//               displayed (in a Combobox), so that the proper log-entry to
//               restore from, can be selected.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RestoreSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRestore_Program_Settings *Restore_Program_Settings;
//---------------------------------------------------------------------------
__fastcall TRestore_Program_Settings::TRestore_Program_Settings(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
#define IDH_READLOGFILE (0x10020)
void __fastcall TRestore_Program_Settings::Help_ButtonClick(
      TObject *Sender)
{
   Application->HelpContext(IDH_READLOGFILE);
}
//---------------------------------------------------------------------------

