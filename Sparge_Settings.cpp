// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Sparge_Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
__fastcall TSpargeSettings::TSpargeSettings(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
