// ==========================================================================
// Filename    : PID_Settings_Dialog.cpp
// Author      : Emile
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// ==========================================================================

//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PID_Settings_Dialog.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TPID_Settings *PID_Settings;
//----------------------------------------------------------------------------
__fastcall TPID_Settings::TPID_Settings(TComponent *Owner)
	: TForm(Owner)
{
   update_pid_gui();
} // TPID_Settings()
//----------------------------------------------------------------------------

void __fastcall TPID_Settings::RG2Click(TObject *Sender)
{
   if (RG2->ItemIndex == 1) // On
   {
      Date_Edit->Enabled  = true;
      Date_Label->Enabled = true;
      Time_Edit->Enabled  = true;
      Time_Label->Enabled = true;
   }
   else
   {
      Date_Edit->Enabled  = false;
      Date_Label->Enabled = false;
      Time_Edit->Enabled  = false;
      Time_Label->Enabled = false;
   } // else
}
//---------------------------------------------------------------------------

#define IDH_PIDSETTINGS (0x100A0)
void __fastcall TPID_Settings::Button3Click(TObject *Sender)
{
   Application->HelpContext(IDH_PIDSETTINGS);
}

void __fastcall TPID_Settings::update_pid_gui(void)
{
   switch (PID_Model->ItemIndex)
   {
      case 0: // Self Tuning Controller STC
        // Disable Kc, Ti, Td, lpfC. Enable stc_N, stc_TD, stc_adf
        Label1->Enabled = false; Kc_Edit->Enabled    = false; Label6->Enabled = false; // Kc
        Label2->Enabled = false; Ti_Edit->Enabled    = false; Label7->Enabled = false; // Ti
        Label3->Enabled = false; Td_Edit->Enabled    = false; Label8->Enabled = false; // Td
        Label4->Enabled = false; K_LPF_Edit->Enabled = false; Label9->Enabled = false; // Clpf

        Label18->Enabled = true; STC_N_Edit->Enabled  = true; Label19->Enabled = true; // stc_N
        Label20->Enabled = true; STC_TD_Edit->Enabled = true; Label21->Enabled = true; // stc_TD
        CB_adf->Enabled = true; // STC_ADF
        break;
      case 1: // Takahashi ype C PID, NO D-filtering
        // Enable Kc, Ti, Td. Disable lpfC, stc_N, stc_TD, stc_adf
        Label1->Enabled  = true; Kc_Edit->Enabled = true; Label6->Enabled  = true; // Kc
        Label2->Enabled  = true; Ti_Edit->Enabled = true; Label7->Enabled  = true; // Ti
        Label3->Enabled  = true; Td_Edit->Enabled = true; Label8->Enabled  = true; // Td

        Label4->Enabled  = false; K_LPF_Edit->Enabled  = false; Label9->Enabled  = false; // Clpf
        Label18->Enabled = false; STC_N_Edit->Enabled  = false; Label19->Enabled = false; // stc_N
        Label20->Enabled = false; STC_TD_Edit->Enabled = false; Label21->Enabled = false; // stc_TD
        CB_adf->Enabled  = false; // STC_ADF
        break;
   } // switch
} // update_pid_gui()

void __fastcall TPID_Settings::PID_ModelExit(TObject *Sender)
{
   update_pid_gui();
} // PID_ModelExit()
//---------------------------------------------------------------------------

