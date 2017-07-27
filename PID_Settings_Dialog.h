// ==========================================================================
// Filename    : PID_Settings_Dialog.h
// Author      : Emile
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// ==========================================================================

//----------------------------------------------------------------------------
#ifndef PID_Settings_DialogH
#define PID_Settings_DialogH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//----------------------------------------------------------------------------

class TPID_Settings : public TForm
{
__published:
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
        TEdit *TS_edit;
        TLabel *TS_Label;
        TEdit *Kc_Edit;
        TLabel *Label1;
        TEdit *Ti_Edit;
        TLabel *Label2;
        TEdit *Td_Edit;
        TLabel *Label3;
        TMaskEdit *Date_Edit;
        TMaskEdit *Time_Edit;
        TRadioGroup *RG2;
        TLabel *Date_Label;
        TLabel *Time_Label;
        TRadioGroup *PID_Model;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label4;
        TLabel *Label9;
        TEdit *K_LPF_Edit;
        TCheckBox *CB_PID_dbg;
        TLabel *Label15;
        TLabel *Label17;
        TEdit *Tset_hlt_slope;
        TLabel *Label18;
        TLabel *Label19;
        TEdit *STC_N_Edit;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *STC_TD_Edit;
        TCheckBox *CB_adf;
        void __fastcall RG2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall PID_ModelExit(TObject *Sender);
private:
public:
	virtual __fastcall TPID_Settings(TComponent *Owner);
        void    __fastcall update_pid_gui(void);
};
//----------------------------------------------------------------------------
extern TPID_Settings *PID_Settings;
//----------------------------------------------------------------------------
#endif
