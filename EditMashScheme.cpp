// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the Mash Scheme can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditMashScheme.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditMashSchemeDlg *EditMashSchemeDlg;
//---------------------------------------------------------------------------
__fastcall TEditMashSchemeDlg::TEditMashSchemeDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#define IDH_EDITMASHSCHEME (0x10040)
void __fastcall TEditMashSchemeDlg::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_EDITMASHSCHEME);
}
//---------------------------------------------------------------------------

