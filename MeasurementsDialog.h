// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// --------------------------------------------------------------------------
// $Log$
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
        TCheckBox *Vhlt_simulated;
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
        TLabel *Label7;
        TEdit *DAC_edit;
        TUpDown *UpDown3;
        TLabel *Vref4_Label;
        TEdit *Vref4_edit;
        TLabel *Vref3_Label;
        TEdit *Vref3_edit;
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
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMeasurements(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasurements *Measurements;
//---------------------------------------------------------------------------
#endif
