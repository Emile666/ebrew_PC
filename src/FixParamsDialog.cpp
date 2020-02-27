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
// HLT variables
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CB_TsetClick(TObject *Sender)
{
   Tset_MEdit->Enabled = CB_Tset->Checked;
   if (Tset_MEdit->Enabled)
   {
       Tset_MEdit->Text = AnsiString(MainForm->tset_hlt);
       if (Visible) Tset_MEdit->SetFocus();
   }
   else Tset_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::Tset_MEditClick(TObject *Sender)
{
   Tset_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_GammaClick(TObject *Sender)
{
   Gamma_MEdit->Enabled = CB_Gamma->Checked;
   if (Gamma_MEdit->Enabled)
   {
       Gamma_MEdit->Text = AnsiString(MainForm->gamma_hlt);
       if (Visible) Gamma_MEdit->SetFocus();
   }
   else Gamma_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::Gamma_MEditClick(TObject *Sender)
{
   Gamma_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_Tad1Click(TObject *Sender)
{
   Tad1_MEdit->Enabled = CB_Tad1->Checked;
   if (Tad1_MEdit->Enabled)
   {
       Tad1_MEdit->Text = AnsiString(MainForm->thlt);
       if (Visible) Tad1_MEdit->SetFocus();
   }
   else Tad1_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::Tad1_MEditClick(TObject *Sender)
{
   Tad1_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_vhltClick(TObject *Sender)
{
   Vhlt_MEdit->Enabled = CB_vhlt->Checked;
   if (Vhlt_MEdit->Enabled)
   {
       Vhlt_MEdit->Text = AnsiString(MainForm->volumes.Vhlt);
       if (Visible) Vhlt_MEdit->SetFocus();
   }
   else Vhlt_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Vhlt_MEditClick(TObject *Sender)
{
   Vhlt_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// MLT variables
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CB_Tad2Click(TObject *Sender)
{
   Tad2_MEdit->Enabled = CB_Tad2->Checked;
   if (Tad2_MEdit->Enabled)
   {
       Tad2_MEdit->Text = AnsiString(MainForm->tmlt);
       if (Visible) Tad2_MEdit->SetFocus();
   }
   else Tad2_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::Tad2_MEditClick(TObject *Sender)
{
   Tad2_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_vmltClick(TObject *Sender)
{
   Vmlt_MEdit->Enabled = CB_vmlt->Checked;
   if (Vmlt_MEdit->Enabled)
   {
       Vmlt_MEdit->Text = AnsiString(MainForm->volumes.Vmlt);
       if (Visible) Vmlt_MEdit->SetFocus();
   }
   else Vmlt_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::Vmlt_MEditClick(TObject *Sender)
{
   Vmlt_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Boil-kettle variables
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CheckBox1Click(TObject *Sender)
{  // Boil-kettle setpoint temperature
   MaskEdit1->Enabled = CheckBox1->Checked;
   if (MaskEdit1->Enabled)
   {
       MaskEdit1->Text = AnsiString(MainForm->tset_boil);
       if (Visible) MaskEdit1->SetFocus();
   }
   else MaskEdit1->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::MaskEdit1Click(TObject *Sender)
{
   MaskEdit1->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox2Click(TObject *Sender)
{  // Gamma Boil-kettle
   MaskEdit2->Enabled = CheckBox2->Checked;
   if (MaskEdit2->Enabled)
   {
       MaskEdit2->Text = AnsiString(MainForm->gamma_boil);
       if (Visible) MaskEdit2->SetFocus();
   }
   else MaskEdit2->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::MaskEdit2Click(TObject *Sender)
{
   MaskEdit2->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox3Click(TObject *Sender)
{  // Boil-kettle Temperature
   MaskEdit3->Enabled = CheckBox3->Checked;
   if (MaskEdit3->Enabled)
   {
       MaskEdit3->Text = AnsiString(MainForm->tboil);
       if (Visible) MaskEdit3->SetFocus();
   }
   else MaskEdit3->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::MaskEdit3Click(TObject *Sender)
{
   MaskEdit3->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox4Click(TObject *Sender)
{  // Boil-kettle Volume
   MaskEdit4->Enabled = CheckBox4->Checked;
   if (MaskEdit4->Enabled)
   {
       MaskEdit4->Text = AnsiString(MainForm->volumes.Vboil);
       if (Visible) MaskEdit4->SetFocus();
   }
   else MaskEdit4->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::MaskEdit4Click(TObject *Sender)
{
   MaskEdit4->SelectAll();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Other variables
//---------------------------------------------------------------------------
void __fastcall TFix_Params::CB_stdClick(TObject *Sender)
{
   STD_MEdit->Enabled = CB_std->Checked;
   if (STD_MEdit->Enabled)
   {
       STD_MEdit->Text = AnsiString(MainForm->std.ebrew_std);
       if (Visible) STD_MEdit->SetFocus();
   }
   else STD_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::STD_MEditClick(TObject *Sender)
{
   STD_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CB_ttriac_Click(TObject *Sender)
{
   Ttriac_MEdit->Enabled = CB_ttriac->Checked;
   if (Ttriac_MEdit->Enabled)
   {
       Ttriac_MEdit->Text = AnsiString(MainForm->ttriac);
       if (Visible) Ttriac_MEdit->SetFocus();
   }
   else Ttriac_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::Ttriac_MEditClick(TObject *Sender)
{
   Ttriac_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox5Click(TObject *Sender)
{
    ms_idx_MEdit->Enabled = CheckBox5->Checked;
    if (ms_idx_MEdit->Enabled)
    {
        ms_idx_MEdit->Text = AnsiString(MainForm->std.ms_idx);
        if (Visible) ms_idx_MEdit->SetFocus();
    }
    else ms_idx_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::ms_idx_MEditClick(TObject *Sender)
{
    ms_idx_MEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFix_Params::CheckBox6Click(TObject *Sender)
{
    sp_idx_MEdit->Enabled = CheckBox6->Checked;
    if (sp_idx_MEdit->Enabled)
    {
        sp_idx_MEdit->Text = AnsiString(MainForm->std.sp_idx);
        if (Visible) sp_idx_MEdit->SetFocus();
    }
    else sp_idx_MEdit->Text = AnsiString("");
}
//---------------------------------------------------------------------------
void __fastcall TFix_Params::sp_idx_MEditClick(TObject *Sender)
{
    sp_idx_MEdit->SelectAll();
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
      int tmp;

      //--------------------------------------------------------------
      // HLT Setpoint Temperature
      MainForm->swfx.tset_hlt_sw = CB_Tset->Checked;
      if (MainForm->swfx.tset_hlt_sw)
      {
         MainForm->swfx.tset_hlt_fx = Tset_MEdit->Text.ToDouble();
      } // if
      // Set Gamma HLT (PID Output)
      MainForm->swfx.gamma_hlt_sw = CB_Gamma->Checked;
      if (MainForm->swfx.gamma_hlt_sw)
      {
         MainForm->swfx.gamma_hlt_fx = Gamma_MEdit->Text.ToDouble();
      } // if
      // Set HLT Temperature (Thlt)
      MainForm->swfx.thlt_sw = CB_Tad1->Checked;
      if (MainForm->swfx.thlt_sw)
      {
        MainForm->swfx.thlt_fx = Tad1_MEdit->Text.ToDouble();
      } // if
      // Set HLT Volume (Vhlt)
      MainForm->swfx.vhlt_sw = CB_vhlt->Checked;
      if (MainForm->swfx.vhlt_sw)
      {
         MainForm->swfx.vhlt_fx = Vhlt_MEdit->Text.ToDouble();
      } // if

      //--------------------------------------------------------------
      // Set MLT Temperature (Tmlt)
      MainForm->swfx.tmlt_sw = CB_Tad2->Checked;
      if (MainForm->swfx.tmlt_sw)
      {
         MainForm->swfx.tmlt_fx = Tad2_MEdit->Text.ToDouble();
      } // if
      // Set MLT Volume (Vmlt)
      MainForm->swfx.vmlt_sw = CB_vmlt->Checked;
      if (MainForm->swfx.vmlt_sw)
      {
         MainForm->swfx.vmlt_fx = Vmlt_MEdit->Text.ToDouble();
      } // if

      //--------------------------------------------------------------
      // Boil-Kettle Setpoint Temperature
      MainForm->swfx.tset_boil_sw = CheckBox1->Checked;
      if (MainForm->swfx.tset_boil_sw)
      {
         MainForm->swfx.tset_boil_fx = MaskEdit1->Text.ToDouble();
      } // if
      // Set Gamma Boil-Kettle (PID Output)
      MainForm->swfx.gamma_boil_sw = CheckBox2->Checked;
      if (MainForm->swfx.gamma_boil_sw)
      {
         MainForm->swfx.gamma_boil_fx = MaskEdit2->Text.ToDouble();
      } // if
      // Set Boil-Kettle Temperature (Tboil)
      MainForm->swfx.tboil_sw = CheckBox3->Checked;
      if (MainForm->swfx.tboil_sw)
      {
         MainForm->swfx.tboil_fx = MaskEdit3->Text.ToDouble();
      } // if
      // Set Boil-Kettle Volume (Vboil)
      MainForm->swfx.vboil_sw = CheckBox4->Checked;
      if (MainForm->swfx.vboil_sw)
      {
         MainForm->swfx.vboil_fx = MaskEdit4->Text.ToDouble();
      } // if

      //--------------------------------------------------------------
      // Set STD state
      if (CB_std->Checked)
      {  // Set ebrew_std only once, no switch/fix
         // Value must be between 0 and S32_CIP_END
         tmp = STD_MEdit->Text.ToInt(); // get value from MaskEdit component
         if ((tmp >= S00_INITIALISATION) && (tmp <= S32_CIP_END))
         {
            MainForm->std.ebrew_std = tmp; // set ebrew_std
         }
         // else: no update of ebrew_std
      } // if
      // Set Triac Temperature (TTriac)
      MainForm->swfx.ttriac_sw = CB_ttriac->Checked;
      if (MainForm->swfx.ttriac_sw)
      {
         MainForm->swfx.ttriac_fx = Ttriac_MEdit->Text.ToDouble();
      } // if
      // Set mash-index
      if (CheckBox5->Checked)
      {  // Set ms_idx only once, no switch/fix
         tmp = ms_idx_MEdit->Text.ToInt(); // get value from MaskEdit component
         if ((tmp >= 0) && (tmp < MainForm->std.ms_tot))
         {
            MainForm->std.ms_idx = tmp; // set ms_idx
         }
         // else: no update of ms_idx
      } // if
      // Set sparge-index
      if (CheckBox6->Checked)
      {  // Set sp_idx only once, no switch/fix
         tmp = sp_idx_MEdit->Text.ToInt(); // get value from MaskEdit component
         if ((tmp >= 0) && (tmp < MainForm->sp.sp_batches))
         {
            MainForm->std.sp_idx = tmp; // set sp_idx
         }
         // else: no update of sp_idx
      } // if
} // TFix_Params::Apply_ButtonClick()
//---------------------------------------------------------------------------

