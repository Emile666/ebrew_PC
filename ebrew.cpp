// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.3  2002/12/30 13:33:45  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("ebrew.res");
USEFORM("Unit1.cpp", MainForm);
USEFORM("PID_Settings_Dialog.cpp", PID_Settings);
USEFORM("FixParamsDialog.cpp", Fix_Params);
USEFORM("I2CSettingsDialog.cpp", I2C_Settings);
USEUNIT("MISC.C");
USEUNIT("PID_REG.C");
USEFORM("EditMashScheme.cpp", EditMashSchemeDlg);
USEFORM("ViewMashProgressForm.cpp", ViewMashProgress);
USEFORM("DataGraphForm.cpp", ShowDataGraphs);
USEFORM("Sparge_Settings.cpp", SpargeSettings);
USEUNIT("i2c_dll\i2c_dll.cpp");
USEFORM("RestoreSettings.cpp", Restore_Program_Settings);
USEFORM("VersionAwareAbout.cpp", VersionAwareAbout);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "E-Brew!";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TPID_Settings), &PID_Settings);
                 Application->CreateForm(__classid(TFix_Params), &Fix_Params);
                 Application->CreateForm(__classid(TI2C_Settings), &I2C_Settings);
                 Application->CreateForm(__classid(TEditMashSchemeDlg), &EditMashSchemeDlg);
                 Application->CreateForm(__classid(TViewMashProgress), &ViewMashProgress);
                 Application->CreateForm(__classid(TShowDataGraphs), &ShowDataGraphs);
                 Application->CreateForm(__classid(TSpargeSettings), &SpargeSettings);
                 Application->CreateForm(__classid(TRestore_Program_Settings), &Restore_Program_Settings);
                 Application->CreateForm(__classid(TVersionAwareAbout), &VersionAwareAbout);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
