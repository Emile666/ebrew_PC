// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : 
// --------------------------------------------------------------------------
// $Log$
// Revision 1.6  2005/08/28 22:17:30  Emile
// - DataGrapfForm: TTimer replaced again for TAnimTimer
// - Debug-code added for MA filter of Vmlt
// - 'H' key now toggles heater between 0% and 100%
// - Text Temp. Sensor 1 & 2 replaced by Temp. Sensor HLT & MLT
// - Printing of vmlt_unf (padc.ad4) removed again from log-file
//
// Revision 1.5  2004/04/25 14:02:17  emile
// - Added a 'type C' PID controller, function pid_reg3(). Possible to select
//   this from the PID settings dialog screen. Left the old one in for
//   compatibility. New registry variable PID_Model.
// - Gamma added to log-file, so that the PID controller can be tuned.
//
// Revision 1.4  2004/03/26 10:22:55  emile
// - Several files added to CVS repository, where not included yet
// - Help file extended with many help items. First real help file version.
// - TAnimTimer replaced by standard TTimer in TDataGraph form
//
// Revision 1.3  2004/03/10 10:10:37  emile
// - Reduced complexity of several routines:
//   - T50msecTimer split, new routine Generate_IO_Signals added
//   - PopupMenu1Popup now uses (new) macro SET_POPUPMENU
//   - Reset_I2C_Bus now included in SET_LED macro
// - Every I2C write action now in a separate time-slice to avoid
//   I2C bus errors if fscl is low
// - This is the first version where the help file function is enabled
//   - All help buttons and F1 function key are operational
//   - Help file sources: ebrew.rtf and ebrew.hpj are added to CVS
// - ad1, ad2 and ad3 variables -> thlt, tmlt and ttriac (new variables)
//
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
#include <ExtCtrls.hpp>
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
        TTimer *GraphTimer;
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
