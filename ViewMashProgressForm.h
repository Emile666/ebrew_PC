// ==========================================================================
// Filename    : ViewMashProgressForm.h
// Author      : Emile
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.
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

#define BAR_LINE "------------------------------------------------"

//---------------------------------------------------------------------------
class TViewMashProgress : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TButton *CloseButton;
        TButton *Help_Button;
        TAnimTimer *UpdateTimer;
        void __fastcall CloseButtonClick(TObject *Sender);
        void __fastcall UpdateTimerTimer(TObject *Sender);
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
        int  prev_ebrew_std; // previous value of ebrew_std
public:		// User declarations
        __fastcall TViewMashProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TViewMashProgress *ViewMashProgress;
//---------------------------------------------------------------------------
#endif
