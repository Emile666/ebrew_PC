// ==========================================================================
// Filename    : I2CSettingsDialog.cpp
// Author      : Emile
// Purpose     : Provides a form where parameters for the I2C Bus related
//               settings can be modified.
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "I2CSettingsDialog.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TI2C_Settings *I2C_Settings;
//---------------------------------------------------------------------------
__fastcall TI2C_Settings::TI2C_Settings(TComponent* Owner)
        : TForm(Owner)
{
   update_i2c_gui();
   update_comm_gui();
} // TI2C_Settings()
//---------------------------------------------------------------------------

#define IDH_HARDWARESETTINGS (0x10090)
void __fastcall TI2C_Settings::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_HARDWARESETTINGS);
}

void __fastcall TI2C_Settings::update_i2c_gui(void)
{
   switch (System_Mode->ItemIndex)
   {
        case 0: // Modulating Gas-Burner (20 kHz PWM)
                S0_Lbl->Enabled   = true;
                S0L_Edit->Enabled = true;  S0U_Edit->Enabled = true;
                S0L_L1a->Enabled  = true;  S0L_L1b->Enabled  = true;
                S0U_L1a->Enabled  = true;  S0U_L1b->Enabled  = true;
                S1_Lbl->Enabled   = false;
                S1L_Edit->Enabled = false; S1U_Edit->Enabled = false;
                S1L_L1a->Enabled  = false; S1L_L1b->Enabled  = false;
                S1U_L1a->Enabled  = false; S1U_L1b->Enabled  = false;
                S2_Lbl->Enabled   = false;
                S2L_Edit->Enabled = false; S2U_Edit->Enabled = false;
                S2L_L1a->Enabled  = false; S2L_L1b->Enabled  = false;
                S2U_L1a->Enabled  = false; S2U_L1b->Enabled  = false;
                break;
        case 1: // Non-Modulating Gas-Burner (Relay, On/Off)
                S0_Lbl->Enabled   = false;
                S0L_Edit->Enabled = false; S0U_Edit->Enabled = false;
                S0L_L1a->Enabled  = false; S0L_L1b->Enabled  = false;
                S0U_L1a->Enabled  = false; S0U_L1b->Enabled  = false;
                S1_Lbl->Enabled   = true;
                S1L_Edit->Enabled = true;  S1U_Edit->Enabled = true;
                S1L_L1a->Enabled  = true;  S1L_L1b->Enabled  = true;
                S1U_L1a->Enabled  = true;  S1U_L1b->Enabled  = true;
                S2_Lbl->Enabled   = false;
                S2L_Edit->Enabled = false; S2U_Edit->Enabled = false;
                S2L_L1a->Enabled  = false; S2L_L1b->Enabled  = false;
                S2U_L1a->Enabled  = false; S2U_L1b->Enabled  = false;
                break;
        case 2: // Electrical Heating Element (Triac, T=5 sec.)
                S0_Lbl->Enabled   = false;
                S0L_Edit->Enabled = false; S0U_Edit->Enabled = false;
                S0L_L1a->Enabled  = false; S0L_L1b->Enabled  = false;
                S0U_L1a->Enabled  = false; S0U_L1b->Enabled  = false;
                S1_Lbl->Enabled   = false;
                S1L_Edit->Enabled = false; S1U_Edit->Enabled = false;
                S1L_L1a->Enabled  = false; S1L_L1b->Enabled  = false;
                S1U_L1a->Enabled  = false; S1U_L1b->Enabled  = false;
                S2_Lbl->Enabled   = true;
                S2L_Edit->Enabled = true;  S2U_Edit->Enabled = true;
                S2L_L1a->Enabled  = true;  S2L_L1b->Enabled  = true;
                S2U_L1a->Enabled  = true;  S2U_L1b->Enabled  = true;
                break;
   } // switch
} // update_i2c_gui()

void __fastcall TI2C_Settings::update_comm_gui(void)
{
   if (Comm_Setting->ItemIndex == 0)
   {    // Ethernet UDP
        COM_Port_Settings_Edit->Enabled = false;
        COM_Port_Settings_lbl->Enabled  = false;
        UDP_Settings->Enabled = true;
        UDP_Settings_lbl->Enabled = true;
   }
   else
   {    // Virtual COM Port
        COM_Port_Settings_Edit->Enabled = true;
        COM_Port_Settings_lbl->Enabled  = true;
        UDP_Settings->Enabled = false;
        UDP_Settings_lbl->Enabled = false;
   } // else
} // TI2C_Settings::update_comm_gui()

//---------------------------------------------------------------------------
void __fastcall TI2C_Settings::System_ModeExit(TObject *Sender)
{
   update_i2c_gui();
} // System_ModeExit()
//---------------------------------------------------------------------------

void __fastcall TI2C_Settings::Comm_SettingChange(TObject *Sender)
{
   update_comm_gui();
} // Comm_SettingChange()
//---------------------------------------------------------------------------

