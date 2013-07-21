// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.8  2011/05/29 20:56:25  Emile
// - New Registry variables added: STC_N, STC_TD and STC_ADF
// - PID Settings Dialog screen extended with new parameters for self-tuning
//   controller: possibility to set the system order N, an estimate for the
//   time-delay and a boolean whether or not to use adaptive dir. forgetting.
// - PID Settings Dialog screen: parameters enabled/disabled when a
//   specific PID controller is chosen.
// - New functions time_delay() and init_time_delay() added
// - Changes made in init_pid2() function header.
// - Unit-test cases updated and extended with tests for new functions.
//
// Revision 1.7  2011/05/06 11:09:42  Emile
// - pid_reg1(), pid_reg2(), init_pid1(), init_pid2() removed.
// - pid_reg4() changed into pure Takahashi PID controller, no D-filtering anymore.
// - PID dialog updated to reflect changes.
//
// Revision 1.6  2006/11/19 10:53:55  Emile
// The power outlet (220 V) is now shared with the modulating gas burner and
// the electrical heating element. By setting the proper bits in the PID
// Settings Dialog Screen, one can select which function is allocated to the
// power outlet. If the gas burner is used, the outlet is energized when the
// pid_output exceeds the gas burner hysteresis (also in the PID settings screen).
// For this: the generate_IO_signals STD has been updated significantly.
//
// Revision 1.5  2005/11/12 22:19:38  Emile
// - PID Output (Gamma) routing added. It is now possible to send the output
//   of the PID controller to 3 devices: 1) electrical heating element,
//   2) Non-modulating (standard) gas-burner, 3) modulating gas-burner.
// - PID Dialog changed, checkboxes added.
// - New registry variables: CB_PID_OUT, DAC_A, DAC_B
// - The conversion from gamma to a value for the DA-output is done with
//   DAC_A and DAC_B. The DA-output is used to generate the PWM signal for
//   the modulating gas-burner.
// - Led visibility decreased.
// - Bug-fix: Check_HW_Devices screen appeared twice when checking I2C HW. Fixed.
// - Displaying of Vhlt, Vmlt and Ttriac on LED-displays changed (less resolution).
// - Network routines removed.
//
// Revision 1.4  2004/03/10 10:10:38  emile
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
// Revision 1.3  2003/06/29 13:11:34  emile
// - Time switch function added (PID options screen). The PID controller is
//   enabled when the predefined date and time have been met.
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
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
//---------------------------------------------------------------------------
// Since there are 3 power sources available (modulating gas burner, non-
// modulating gas burner, electrical heater) and only 1 power outlet, a
// decision has to be made, which power sources has priority over the other,
// in case they are all selected.
// This is defined with the following table:
//
// Heater | Mod. Gas | Power Outlet (220 V)
// ----------------------------------------
//   0    |    0     | None selected
//   0    |    1     | Used by modulating gas burner
//   1    |    X     | Used by electrical heater
//---------------------------------------------------------------------------

void __fastcall TPID_Settings::CB_Pid_out1Click(TObject *Sender)
{  //-----------------------------------------------------------
   // Non-modulating Gas-burner
   //
   // A non-modulating gas-burner is simply switched on and off
   // above a particular value of the PID output. This is the
   // function of the Gas Burner On/Off edit boxes.
   //-----------------------------------------------------------
   if (CB_Pid_out1->Checked)
   {
      Burner_On->Enabled  = true;
      Burner_Off->Enabled = true;
      Label10->Enabled    = true;
      Label11->Enabled    = true;
      Label12->Enabled    = true;
      Label13->Enabled    = true;
   }
   else if (!CB_Pid_out2->Checked)
   {  // Only disable gas burner on hysteresis when both the modulating
      // AND the non-modulating burners are disabled.
      Burner_On->Enabled  = false;
      Burner_Off->Enabled = false;
      Label10->Enabled    = false;
      Label11->Enabled    = false;
      Label12->Enabled    = false;
      Label13->Enabled    = false;
   } // else if
} // CB_Pid_out1Click()
//---------------------------------------------------------------------------

void __fastcall TPID_Settings::CB_Pid_out2Click(TObject *Sender)
{  //---------------------------------------------------------------
   // Modulating Gas-burner
   //
   // A Modulating Gas burner is enabled in two ways:
   // 1) send out a PWM signal to the gas-valve. This signal is
   //    generated by converting the PID output into a DC voltage.
   //    This is done by the DAC, for which the a/b coefficient
   //    DAC value edit boxes are used.
   // 2) Apply 220 V to the gas-valve to enable it.
   //---------------------------------------------------------------
   if (CB_Pid_out2->Checked)
   {
      Burner_On->Enabled  = true;
      Burner_Off->Enabled = true;
      Label10->Enabled    = true;
      Label11->Enabled    = true;
      Label12->Enabled    = true;
      Label13->Enabled    = true;
   }
   else
   {
      if (!CB_Pid_out1->Checked)
      {  // Only disable gas burner on hysteresis when both the modulating
         // AND the non-modulating burners are disabled.
         Burner_On->Enabled  = false;
         Burner_Off->Enabled = false;
         Label10->Enabled    = false;
         Label11->Enabled    = false;
         Label12->Enabled    = false;
         Label13->Enabled    = false;
      } // if
   } // else
} // CB_Pid_out2Click()
//---------------------------------------------------------------------------

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
      case 1: // Type A PID with D-filtering
        // Enable Kc, Ti, Td, lpfC. Disable stc_N, stc_TD, stc_adf
        Label1->Enabled  = true; Kc_Edit->Enabled    = true; Label6->Enabled  = true; // Kc
        Label2->Enabled  = true; Ti_Edit->Enabled    = true; Label7->Enabled  = true; // Ti
        Label3->Enabled  = true; Td_Edit->Enabled    = true; Label8->Enabled  = true; // Td
        Label4->Enabled  = true; K_LPF_Edit->Enabled = true; Label9->Enabled  = true; // Clpf

        Label18->Enabled = false; STC_N_Edit->Enabled  = false; Label19->Enabled = false; // stc_N
        Label20->Enabled = false; STC_TD_Edit->Enabled = false; Label21->Enabled = false; // stc_TD
        CB_adf->Enabled  = false; // STC_ADF
        break;
      case 2: // Takahashi ype C PID, NO D-filtering
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

