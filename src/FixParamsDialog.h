// ==========================================================================
// Filename    : FixParamsDialog.h
// Author      : Emile
// Purpose     : Provides a form where various parameters can be manually
//               (fixed) to a particular value.
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef FixParamsDialogH
#define FixParamsDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFix_Params : public TForm
{
__published:	// IDE-managed Components
        TCheckBox *CB_Gamma;
        TCheckBox *CB_Tset;
        TMaskEdit *Gamma_MEdit;
        TMaskEdit *Tset_MEdit;
        TLabel *Gamma_Label;
        TLabel *Tset_Label;
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TLabel *Label1;
        TCheckBox *CB_Tad1;
        TMaskEdit *Tad1_MEdit;
        TLabel *Label2;
        TCheckBox *CB_Tad2;
        TMaskEdit *Tad2_MEdit;
        TCheckBox *CB_std;
        TMaskEdit *STD_MEdit;
        TCheckBox *CB_vmlt;
        TMaskEdit *Vmlt_MEdit;
        TLabel *Label3;
        TButton *Apply_Button;
        TLabel *Label4;
        TCheckBox *CB_vhlt;
        TMaskEdit *Vhlt_MEdit;
        TCheckBox *CB_ttriac;
        TMaskEdit *Ttriac_MEdit;
        TLabel *Label6;
        TLabel *Label7;
        TCheckBox *CheckBox1;
        TMaskEdit *MaskEdit1;
        TLabel *Label8;
        TCheckBox *CheckBox2;
        TMaskEdit *MaskEdit2;
        TLabel *Label9;
        TCheckBox *CheckBox3;
        TMaskEdit *MaskEdit3;
        TLabel *Label10;
        TCheckBox *CheckBox4;
        TMaskEdit *MaskEdit4;
        TCheckBox *CheckBox5;
        TMaskEdit *ms_idx_MEdit;
        TCheckBox *CheckBox6;
        TMaskEdit *sp_idx_MEdit;
        void __fastcall CB_GammaClick(TObject *Sender);
        void __fastcall CB_TsetClick(TObject *Sender);
        void __fastcall Gamma_MEditClick(TObject *Sender);
        void __fastcall Tset_MEditClick(TObject *Sender);
        void __fastcall CB_Tad1Click(TObject *Sender);
        void __fastcall Tad1_MEditClick(TObject *Sender);
        void __fastcall CB_Tad2Click(TObject *Sender);
        void __fastcall Tad2_MEditClick(TObject *Sender);
        void __fastcall CB_stdClick(TObject *Sender);
        void __fastcall STD_MEditClick(TObject *Sender);
        void __fastcall CB_vmltClick(TObject *Sender);
        void __fastcall Vmlt_MEditClick(TObject *Sender);
        void __fastcall Help_ButtonClick(TObject *Sender);
        void __fastcall Apply_ButtonClick(TObject *Sender);
        void __fastcall CB_vhltClick(TObject *Sender);
        void __fastcall Vhlt_MEditClick(TObject *Sender);
        void __fastcall CB_ttriac_Click(TObject *Sender);
        void __fastcall Ttriac_MEditClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall CheckBox2Click(TObject *Sender);
        void __fastcall CheckBox3Click(TObject *Sender);
        void __fastcall CheckBox4Click(TObject *Sender);
        void __fastcall CheckBox5Click(TObject *Sender);
        void __fastcall CheckBox6Click(TObject *Sender);
        void __fastcall ms_idx_MEditClick(TObject *Sender);
        void __fastcall sp_idx_MEditClick(TObject *Sender);
        void __fastcall MaskEdit1Click(TObject *Sender);
        void __fastcall MaskEdit2Click(TObject *Sender);
        void __fastcall MaskEdit3Click(TObject *Sender);
        void __fastcall MaskEdit4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFix_Params(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFix_Params *Fix_Params;
//---------------------------------------------------------------------------
#endif
