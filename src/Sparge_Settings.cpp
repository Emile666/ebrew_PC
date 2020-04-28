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

void __fastcall TSpargeSettings::init_preheat(void)
{
   if (CB_dpht->Checked)
   {
      Label23->Enabled  = true;
      Label27->Enabled  = true;
      HLT_Bcap->Enabled = true;
      Label6->Enabled   = false;
      Label7->Enabled   = false;
      Eph_time->Enabled = false;
   }
   else
   {
      Label23->Enabled  = false;
      Label27->Enabled  = false;
      HLT_Bcap->Enabled = false;
      Label6->Enabled   = true;
      Label7->Enabled   = true;
      Eph_time->Enabled = true;
   } // else
} // TSpargeSettings::init_preheat()

//---------------------------------------------------------------------------
__fastcall TSpargeSettings::TSpargeSettings(TComponent* Owner)
        : TForm(Owner)
{
   init_preheat();
} //  TSpargeSettings::TSpargeSettings()
//---------------------------------------------------------------------------

#define IDH_SPARGESTDSETTINGS (0x100B0)
void __fastcall TSpargeSettings::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_SPARGESTDSETTINGS);
}
//---------------------------------------------------------------------------


void __fastcall TSpargeSettings::CB_dphtClick(TObject *Sender)
{
   init_preheat();
} // TSpargeSettings::CheckBox1Click()
//---------------------------------------------------------------------------

void __fastcall TSpargeSettings::CB_malt_firstClick(
      TObject *Sender)
{
   if (CB_malt_first->Checked)
   {
      CB_mash_rest->Enabled = false;
      Label4->Enabled = false;
      CB_pumps_on->Enabled = false;
      Label30->Enabled = false;
   } // if
   else
   {
      CB_mash_rest->Enabled = true;
      Label4->Enabled = true;
      CB_pumps_on->Enabled = true;
      Label30->Enabled = true;
   } // else
} // TSpargeSettings::first_malt_then_waterClick()
//---------------------------------------------------------------------------


