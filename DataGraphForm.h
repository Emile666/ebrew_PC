// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef DataGraphFormH
#define DataGraphFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PERFGRAP.h"
#include "AnimTimer.h"
//---------------------------------------------------------------------------
class TShowDataGraphs : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label2;
        TPerformanceGraph *Tad1_Graph;
        TLabel *Label3;
        TPerformanceGraph *Tad2_Graph;
        TButton *Button1;
        TButton *Help_Button;
        TAnimTimer *GraphTimer;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall GraphTimerTimer(TObject *Sender);
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TShowDataGraphs(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowDataGraphs *ShowDataGraphs;
//---------------------------------------------------------------------------
#endif
