// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the I2C Bus related
//               settings can be modified.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef I2CSettingsDialogH
#define I2CSettingsDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TI2C_Settings : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TRadioGroup *RG1;
        TRadioGroup *RG2;
        TLabel *Vref1_Label;
        TEdit *Vref1_edit;
        TLabel *Vref2_label;
        TEdit *Vref2_edit;
        TEdit *HW_Base_Edit;
        TLabel *HW_base_lbl;
        TLabel *hex_lbl;
        TEdit *Vis1_Edit;
        TUpDown *UpDown1;
        TLabel *Vis1_Lbl;
        TUpDown *UpDown2;
        TEdit *Vis2_Edit;
        TLabel *Label1;
        TLabel *Vref3_Label;
        TLabel *Vref4_Label;
        TEdit *Vref3_edit;
        TEdit *Vref4_edit;
        TLabel *Label2;
        TEdit *DAC_edit;
        TUpDown *UpDown3;
        TLabel *Label3;
        TUpDown *UpDown4;
        TEdit *NMA_edit;
private:	// User declarations
public:		// User declarations
        __fastcall TI2C_Settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TI2C_Settings *I2C_Settings;
//---------------------------------------------------------------------------
#endif
