// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// --------------------------------------------------------------------------
// $Log$
// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MeasurementsDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMeasurements *Measurements;
//---------------------------------------------------------------------------
__fastcall TMeasurements::TMeasurements(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#define IDH_MEASUREMENTS (0x100D0)
void __fastcall TMeasurements::Help_ButtonClick(TObject *Sender)
{
   Application->HelpContext(IDH_MEASUREMENTS);
}
//---------------------------------------------------------------------------

