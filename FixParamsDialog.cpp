//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

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



