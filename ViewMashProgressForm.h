// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the progress of the Mash Scheme can
//               be monitored.  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.3  2002/12/30 20:21:59  emile
// - Bug 2 29-12-02 solved: start mash timers if temp >= tref instead of >.
// - Bug 3 29-12-02 solved: deadlock in std_state 4 when mashing is finished.
// - Bug 4 29-12-02 solved: rush through STD. Sparging parameters were not
//   initialised. New function Init_Sparge_Settings() added.
// - Sparge variables now added to 'View Mash Progress' screen.
// - std_struct added for more flexibility of fixing STD values.
//
// Revision 1.2  2002/12/30 13:33:45  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
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

#define BAR_LINE "---------------------------------------------------------------------------"

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
public:		// User declarations
        __fastcall TViewMashProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TViewMashProgress *ViewMashProgress;
//---------------------------------------------------------------------------
#endif
