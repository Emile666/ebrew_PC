// ==========================================================================
// Filename    : MeasurementsDialog.h
// Author      : Emile
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// ==========================================================================
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
        TLabel *Label8;
        TEdit *Vhlt_Max_Edit;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label13;
        TEdit *Tmlt_Offset_Edit;
        TLabel *Label17;
        TEdit *Vmlt_Max_Edit;
        TCheckBox *Flow_Temp_Corr;
        TLabel *Cal_Temp;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *Flow1_Err;
        TEdit *Flow2_Err;
        TEdit *Flow3_Err;
        TEdit *Flow4_Err;
        TLabel *Label4;
        TEdit *Boil_Max_Edit;
        TLabel *Label3;
        TEdit *Thlt_Offset_Edit;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *Tcfc_Offset_Edit;
        TLabel *Label9;
        TEdit *Tboil_Offset_Edit;
        TLabel *Label12;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *Min_FR_MLT;
        TEdit *Min_FR_Boil;
        TLabel *Label16;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMeasurements(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasurements *Measurements;
//---------------------------------------------------------------------------
#endif
