// ==========================================================================
// Filename    : ebrew.cpp
// Author      : Emile
// Purpose     : Main program for BCB6 project
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

