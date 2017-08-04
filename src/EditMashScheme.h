// ==========================================================================
// Filename    : EditMashScheme.h
// Author      : Emile
// Purpose     : Provides a form where the Mash Scheme can be modified.
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef EditMashSchemeH
#define EditMashSchemeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEditMashSchemeDlg : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TButton *Button3;
        void __fastcall Help_ButtonClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TEditMashSchemeDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditMashSchemeDlg *EditMashSchemeDlg;
//---------------------------------------------------------------------------
#endif
