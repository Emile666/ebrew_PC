// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.  
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef ViewMashProgressFormH
#define ViewMashProgressFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AnimTimer.h"
//---------------------------------------------------------------------------
class TViewMashProgress : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TButton *CloseButton;
        TButton *Button3;
        TAnimTimer *UpdateTimer;
        void __fastcall CloseButtonClick(TObject *Sender);
        void __fastcall UpdateTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TViewMashProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TViewMashProgress *ViewMashProgress;
//---------------------------------------------------------------------------
#endif
