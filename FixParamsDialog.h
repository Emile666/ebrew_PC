// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where various parameters can be manually
//               (fixed) to a particular value.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
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
        TButton *Button3;
        TLabel *Label1;
        TCheckBox *CB_Tad1;
        TMaskEdit *Tad1_MEdit;
        TLabel *Label2;
        TCheckBox *CB_Tad2;
        TMaskEdit *Tad2_MEdit;
        TCheckBox *CB_std;
        TMaskEdit *STD_MEdit;
        void __fastcall CB_GammaClick(TObject *Sender);
        void __fastcall CB_TsetClick(TObject *Sender);
        void __fastcall Gamma_MEditClick(TObject *Sender);
        void __fastcall Tset_MEditClick(TObject *Sender);
        void __fastcall CB_Tad1Click(TObject *Sender);
        void __fastcall Tad1_MEditClick(TObject *Sender);
        void __fastcall CB_Tad2Click(TObject *Sender);
        void __fastcall Tad2_MEditDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFix_Params(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFix_Params *Fix_Params;
//---------------------------------------------------------------------------
#endif
