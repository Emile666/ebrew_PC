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
}
//---------------------------------------------------------------------------


