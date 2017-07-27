// ==========================================================================
// Filename    : Sparge_Settings.h
// Author      : Emile
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef Sparge_SettingsH
#define Sparge_SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSpargeSettings : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *EBatches;
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TEdit *EBTime;
        TLabel *Label2;
        TEdit *EBoilTime;
        TLabel *Label5;
        TLabel *Label12;
        TLabel *Label14;
        TLabel *Label17;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *Eph_time;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *Offs_Edit;
        TEdit *Offs2_Edit;
        TCheckBox *CB_VSp2;
        TLabel *Label3;
        TLabel *Label4;
        TCheckBox *CB_mash_rest;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *SP_PreBoil;
        TLabel *Label11;
        TLabel *Label13;
        TEdit *SP_Boil;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *Boil_Min_Temp;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *Boil_Det;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSpargeSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
#endif
