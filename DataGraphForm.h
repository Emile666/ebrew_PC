// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
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
        TPerformanceGraph *Gamma_Graph;
        TLabel *Label1;
        TLabel *Label2;
        TPerformanceGraph *Tad1_Graph;
        TLabel *Label3;
        TPerformanceGraph *Tad2_Graph;
        TButton *Button1;
        TButton *Button3;
        TAnimTimer *GraphTimer;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall GraphTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TShowDataGraphs(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowDataGraphs *ShowDataGraphs;
//---------------------------------------------------------------------------
#endif
