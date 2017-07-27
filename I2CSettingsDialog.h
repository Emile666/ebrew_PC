// ==========================================================================
// Filename    : I2CSettingsDialog.h
// Author      : Emile
// Purpose     : Provides a form where parameters for the I2C Bus related
//               settings can be modified.
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
        TButton *Help_Button;
        TEdit *S2L_Edit;
        TLabel *S2L_L1a;
        TEdit *S2U_Edit;
        TLabel *S2U_L1a;
        TLabel *S2L_L1b;
        TLabel *S2U_L1b;
        TLabel *COM_Port_Settings_lbl;
        TEdit *COM_Port_Settings_Edit;
        TCheckBox *cb_debug_com_port;
        TRadioGroup *System_Mode;
        TLabel *Label5;
        TLabel *Label4;
        TLabel *S1L_L1a;
        TLabel *S1U_L1a;
        TLabel *S1L_L1b;
        TLabel *S1U_L1b;
        TEdit *S1L_Edit;
        TEdit *S1U_Edit;
        TLabel *S0L_L1a;
        TLabel *S0U_L1a;
        TLabel *S0L_L1b;
        TLabel *S0U_L1b;
        TEdit *S0L_Edit;
        TEdit *S0U_Edit;
        TLabel *S0_Lbl;
        TLabel *S1_Lbl;
        TLabel *S2_Lbl;
        TEdit *UDP_Settings;
        TLabel *UDP_Settings_lbl;
        TComboBox *Comm_Setting;
        TLabel *Comm_Setting_lbl;
        void __fastcall Help_ButtonClick(TObject *Sender);
        void __fastcall System_ModeExit(TObject *Sender);
        void __fastcall Comm_SettingChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TI2C_Settings(TComponent* Owner);
        void __fastcall update_i2c_gui(void);
        void __fastcall update_comm_gui(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TI2C_Settings *I2C_Settings;
//---------------------------------------------------------------------------
#endif
