// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the Mash Scheme can be modified.
// --------------------------------------------------------------------------
// $Log$
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
        TButton *Button3;
private:	// User declarations
public:		// User declarations
        __fastcall TEditMashSchemeDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditMashSchemeDlg *EditMashSchemeDlg;
//---------------------------------------------------------------------------
#endif
