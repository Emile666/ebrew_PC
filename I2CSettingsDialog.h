// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the I2C Bus related
//               settings can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.4  2004/01/31 16:01:04  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.3  2003/09/15 20:37:21  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
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
        TLabel *Label4;
        TLabel *Label5;
        TRadioGroup *RG3;
        TRadioGroup *RG4;
        TEdit *Vis3_Edit;
        TUpDown *UpDown5;
        TUpDown *UpDown6;
        TEdit *Vis4_Edit;
        TEdit *Thlim_edit;
        TLabel *Label6;
        TEdit *Tllim_edit;
        TLabel *Label7;
        TCheckBox *Vhlt_simulated;
        TGroupBox *GroupBox1;
        TCheckBox *Vboil_simulated;
        TEdit *Vhlt_init_Edit;
        TLabel *Label8;
        TCheckBox *cb_i2c_err_msg;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *Hw_devices_Edit;
private:	// User declarations
public:		// User declarations
        __fastcall TI2C_Settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TI2C_Settings *I2C_Settings;
//---------------------------------------------------------------------------
#endif
