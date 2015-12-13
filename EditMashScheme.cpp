// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the Mash Scheme can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.3  2004/03/10 10:10:38  emile
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
#include <stdio.h>
#include <Dialogs.hpp>
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

void __fastcall TEditMashSchemeDlg::Button3Click(TObject *Sender)
{
   TOpenDialog *Dlg;                 // pointer to File-Dialog
   FILE   *fd;

   Dlg = new TOpenDialog(this);
   Dlg->Filter = "ebrew mash files|*.sch";
   Dlg->Title  = "Open an ebrew mash file";
   if (Dlg->Execute() == false)
   {
      delete Dlg;
      return;
   } // if

   if ((fd = fopen(Dlg->FileName.c_str(),"r")) == NULL)
   {
      MessageBox(NULL,"Could not open mash file for reading","Init. error when trying to open file",MB_OK);
   } /* if */
   else
   {
      Memo1->Clear();
      Memo1->Lines->LoadFromFile(Dlg->FileName);
      fclose(fd);
   }
   delete Dlg;
}
//---------------------------------------------------------------------------

