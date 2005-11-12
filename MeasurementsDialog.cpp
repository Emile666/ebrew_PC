// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.1  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
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

void __fastcall TMeasurements::Vhlt_srcChange(TObject *Sender)
{
   if (Vhlt_src->ItemIndex == 0)
   {  // Simulate Vhlt, so disable a- and b-coefficients, enable Start Volume
      Label19->Enabled        = false;
      Label20->Enabled        = false;
      Vhlt_a->Enabled         = false;
      Vhlt_b->Enabled         = false;
      Label8->Enabled         = true;
      Vhlt_init_Edit->Enabled = true;
   }
   else
   {  // Vhlt measurement, disable Start volume
      Label19->Enabled        = true;
      Label20->Enabled        = true;
      Vhlt_a->Enabled         = true;
      Vhlt_b->Enabled         = true;
      Label8->Enabled         = false;
      Vhlt_init_Edit->Enabled = false;
   } // else
} // Vhlt_srcChange()
//---------------------------------------------------------------------------

void __fastcall TMeasurements::Vmlt_srcChange(TObject *Sender)
{
   if (Vmlt_src->ItemIndex == 0)
   {  // No Vmlt measurement, so disable a- and b-coefficients
      Label17->Enabled        = false;
      Label18->Enabled        = false;
      Vmlt_a->Enabled         = false;
      Vmlt_b->Enabled         = false;
   }
   else
   {  // Vmlt measurement
      Label17->Enabled        = true;
      Label18->Enabled        = true;
      Vmlt_a->Enabled         = true;
      Vmlt_b->Enabled         = true;
   } // else
} // Vhlt_srcChange()
//---------------------------------------------------------------------------

void __fastcall TMeasurements::Ttriac_srcChange(TObject *Sender)
{
   if (Ttriac_src->ItemIndex == 0)
   {  // No Ttriac measurement, so disable a- and b-coefficients
      Label7a->Enabled        = false;
      Label7b->Enabled        = false;
      Ttriac_a->Enabled       = false;
      Ttriac_b->Enabled       = false;
   }
   else
   {  // Ttriac measurement
      Label7a->Enabled        = true;
      Label7b->Enabled        = true;
      Ttriac_a->Enabled       = true;
      Ttriac_b->Enabled       = true;
   } // else
} // Ttriac_srcChange()
//---------------------------------------------------------------------------

