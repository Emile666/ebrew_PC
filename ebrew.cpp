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
USEFORM("About_Form.cpp", About);
USEFORM("EditMashScheme.cpp", EditMashSchemeDlg);
USEFORM("ViewMashProgressForm.cpp", ViewMashProgress);
USEFORM("DataGraphForm.cpp", ShowDataGraphs);
USEFORM("Sparge_Settings.cpp", SpargeSettings);
USEUNIT("i2c_dll\i2c_dll.cpp");
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
                 Application->CreateForm(__classid(TAbout), &About);
                 Application->CreateForm(__classid(TEditMashSchemeDlg), &EditMashSchemeDlg);
                 Application->CreateForm(__classid(TViewMashProgress), &ViewMashProgress);
                 Application->CreateForm(__classid(TShowDataGraphs), &ShowDataGraphs);
                 Application->CreateForm(__classid(TSpargeSettings), &SpargeSettings);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
