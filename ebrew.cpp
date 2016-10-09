// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.8  2013/06/22 23:04:19  Emile
// - Second intermediate version: scheduler added and timer interrupt divided
//   over a number of tasks.
// - Version works with Ebrew hardware, task duration needs to be optimised!
//
// Revision 1.7  2013/06/16 14:39:19  Emile
// Intermediate version for new Ebrew 2.0 USB hardware:
// - Hardware settings Dialog: COM Port + Settings added + LEDx removed
// - PortTalk + i2c_dll + Start_i2c_communication + Reset_I2C_Bus removed
// - New routines for COM-Port added
// - Generate_IO_Signals() now uses COM_port_write to address all hardware
// - This version works with new hardware: PUMP on/off + LEDs are working
// - HEATER led and PWM output do not work yet + TODO: add scheduler.
//
// Revision 1.6  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
// Revision 1.5  2004/03/10 10:10:39  emile
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
// Revision 1.4  2003/12/21 21:16:59  emile
// - Old About screen removed, replaced by Version Aware About box. This
//   new About screen shows the version number (and a nice picture!).
//
// Revision 1.3  2002/12/30 13:33:45  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("Unit1.cpp", MainForm);
USEFORM("PID_Settings_Dialog.cpp", PID_Settings);
USEFORM("FixParamsDialog.cpp", Fix_Params);
USEFORM("I2CSettingsDialog.cpp", I2C_Settings);
USEFORM("EditMashScheme.cpp", EditMashSchemeDlg);
USEFORM("ViewMashProgressForm.cpp", ViewMashProgress);
USEFORM("Sparge_Settings.cpp", SpargeSettings);
USEFORM("RestoreSettings.cpp", Restore_Program_Settings);
USEFORM("VersionAwareAbout.cpp", VersionAwareAbout);
USEFORM("MeasurementsDialog.cpp", Measurements);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "E-Brew!";
                 Application->HelpFile = "EBREW.HLP";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TPID_Settings), &PID_Settings);
                 Application->CreateForm(__classid(TFix_Params), &Fix_Params);
                 Application->CreateForm(__classid(TI2C_Settings), &I2C_Settings);
                 Application->CreateForm(__classid(TEditMashSchemeDlg), &EditMashSchemeDlg);
                 Application->CreateForm(__classid(TViewMashProgress), &ViewMashProgress);
                 Application->CreateForm(__classid(TSpargeSettings), &SpargeSettings);
                 Application->CreateForm(__classid(TRestore_Program_Settings), &Restore_Program_Settings);
                 Application->CreateForm(__classid(TVersionAwareAbout), &VersionAwareAbout);
                 Application->CreateForm(__classid(TMeasurements), &Measurements);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
