// ==========================================================================
// Filename    : EditMashScheme.cpp
// Author      : Emile
// Purpose     : Provides a form where the Mash Scheme can be modified.
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

