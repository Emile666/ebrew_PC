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
#ifndef MeasurementsDialogH
#define MeasurementsDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMeasurements : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TCheckBox *Vboil_simulated;
        TLabel *Label8;
        TEdit *Vhlt_init_Edit;
        TLabel *Label3;
        TUpDown *UpDown4;
        TEdit *NMA_edit;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Vref3_Label;
        TEdit *Ttriac_a;
        TLabel *Label9;
        TLabel *Label10;
        TUpDown *UD_MA_HLT;
        TEdit *MA_HLT_Edit;
        TLabel *Label11;
        TUpDown *UD_MA_MLT;
        TEdit *MA_MLT_Edit;
        TLabel *Label12;
        TEdit *Thlt_Offset;
        TLabel *Label13;
        TEdit *Tmlt_Offset;
        TComboBox *Vhlt_src;
        TLabel *Label14;
        TLabel *Label15;
        TComboBox *Vmlt_src;
        TComboBox *Ttriac_src;
        TLabel *Label16;
        TLabel *Label7;
        TEdit *Ttriac_b;
        TLabel *Label17;
        TEdit *Vmlt_a;
        TLabel *Label18;
        TEdit *Vmlt_b;
        TLabel *Label19;
        TEdit *Vhlt_a;
        TLabel *Label20;
        TEdit *Vhlt_b;
        TLabel *Label21;
        TUpDown *UpDown1;
        TEdit *MA_Vhlt;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMeasurements(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasurements *Measurements;
//---------------------------------------------------------------------------
#endif
