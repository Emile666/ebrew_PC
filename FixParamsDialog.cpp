// ==========================================================================
// Filename    : FixParamsDialog.cpp
// Author      : Emile
// Purpose     : Provides a form where various parameters can be manually
//               (fixed) to a particular value.
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
      // HLT Setpoint Temperature
      MainForm->swfx.tset_hlt_sw = CB_Tset->Checked;
      if (MainForm->swfx.tset_hlt_sw)
      {
         MainForm->swfx.tset_hlt_fx = Tset_MEdit->Text.ToDouble();
      } // if
      // Boil-Kettle Setpoint Temperature
      MainForm->swfx.tset_boil_sw = CheckBox1->Checked;
      if (MainForm->swfx.tset_boil_sw)
      {
         MainForm->swfx.tset_boil_fx = MaskEdit1->Text.ToDouble();
      } // if
      // Set Gamma HLT (PID Output)
      MainForm->swfx.gamma_hlt_sw = CB_Gamma->Checked;
      if (MainForm->swfx.gamma_hlt_sw)
      {
         MainForm->swfx.gamma_hlt_fx = Gamma_MEdit->Text.ToDouble();
      } // if
      // Set Gamma Boil-Kettle (PID Output)
      MainForm->swfx.gamma_boil_sw = CheckBox2->Checked;
      if (MainForm->swfx.gamma_boil_sw)
      {
         MainForm->swfx.gamma_boil_fx = MaskEdit2->Text.ToDouble();
      } // if
      //--------------------------------------------------------------
      // Set HLT Temperature (Thlt)
      MainForm->swfx.thlt_sw = CB_Tad1->Checked;
      if (MainForm->swfx.thlt_sw)
      {
        MainForm->swfx.thlt_fx = Tad1_MEdit->Text.ToDouble();
      } // if
      // Set MLT Temperature (Tmlt)
      MainForm->swfx.tmlt_sw = CB_Tad2->Checked;
      if (MainForm->swfx.tmlt_sw)
      {
         MainForm->swfx.tmlt_fx = Tad2_MEdit->Text.ToDouble();
      } // if
      // Set Boil-Kettle Temperature (Tboil)
      MainForm->swfx.tboil_sw = CheckBox3->Checked;
      if (MainForm->swfx.tboil_sw)
      {
         MainForm->swfx.tboil_fx = MaskEdit3->Text.ToDouble();
      } // if
      // Set Triac Temperature (TTriac)
      MainForm->swfx.ttriac_sw = CB_ttriac->Checked;
      if (MainForm->swfx.ttriac_sw)
      {
         MainForm->swfx.ttriac_fx = Ttriac_MEdit->Text.ToDouble();
      } // if
      //--------------------------------------------------------------
      // Set HLT Volume (Vhlt)
      MainForm->swfx.vhlt_sw = CB_vhlt->Checked;
      if (MainForm->swfx.vhlt_sw)
      {
         MainForm->swfx.vhlt_fx = Vhlt_MEdit->Text.ToDouble();
      } // if
      // Set MLT Volume (Vmlt)
      MainForm->swfx.vmlt_sw = CB_vmlt->Checked;
      if (MainForm->swfx.vmlt_sw)
      {
         MainForm->swfx.vmlt_fx = Vmlt_MEdit->Text.ToDouble();
      } // if
      // Set Boil-Kettle Volume (Vboil)
      MainForm->swfx.vboil_sw = CheckBox4->Checked;
      if (MainForm->swfx.vboil_sw)
      {
         MainForm->swfx.vboil_fx = MaskEdit4->Text.ToDouble();
      } // if
      // Set STD state
      MainForm->swfx.std_sw = CB_std->Checked;
      if (MainForm->swfx.std_sw)
      {
         // Value must be between 0 and 18
         MainForm->swfx.std_fx = STD_MEdit->Text.ToInt();
         if (MainForm->swfx.std_fx < 0 ||
             MainForm->swfx.std_fx > S32_CIP_END)
         {
            MainForm->swfx.std_fx = 0; // reset to a safe value
         }
      } // if
} // TFix_Params::Apply_ButtonClick()
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox1Click(TObject *Sender)
{
   MaskEdit1->Enabled = CheckBox1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox2Click(TObject *Sender)
{
   MaskEdit2->Enabled = CheckBox2->Checked;        
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox3Click(TObject *Sender)
{
   MaskEdit3->Enabled = CheckBox3->Checked;        
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox4Click(TObject *Sender)
{
   MaskEdit4->Enabled = CheckBox4->Checked;
}
//---------------------------------------------------------------------------

