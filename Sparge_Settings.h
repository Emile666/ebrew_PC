// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.7  2004/05/08 14:52:51  emile
// - Mash pre-heat functionality added to STD. New registry variable PREHEAT_TIME.
//   tset_hlt is set to next mash temp. if mash timer >= time - PREHEAT_TIME
// - View mash progress screen: reorganised, pre-heat timers added, timers are now
//   in seconds instead of minutes.
// - update_tset() function removed. Now incorporated in STD, states 3-5 + (new state) 13.
// - THLT_HLIMIT and THLT_LLIMIT and state 4 'Bypass Heat Exchanger' removed
// - Reorganisation of several variables (e.g. ms_idx, ms_tot) into (other) structs.
// - 'Apply' Button added to Fix parameters dialog screen.
// - 'Edit mash scheme' no longer resets the (running) mash timers
// - 'Mash progress controlled by' function removed. Registry var 'mash_control' now
//   also removed.
// - Changing init. volume of HLT did not result in an update on screen. Corrected.
//
// Revision 1.6  2004/05/05 15:44:15  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.5  2004/03/10 10:10:38  emile
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
// Revision 1.4  2003/06/01 13:40:45  emile
// - Bugfix: switch/fix for Tmlt and Thlt were in wrong time-slice. Corrected.
// - Switch/fix for std state added for easier testing
// - Vmash value added to 'Options|Sparge & STD Settings' dialog screen.
//
// Revision 1.3  2003/06/01 11:53:47  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef Sparge_SettingsH
#define Sparge_SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSpargeSettings : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *EBatches;
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TEdit *EBTime;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *EMVol;
        TLabel *Label4;
        TEdit *ESVol;
        TEdit *EBoilTime;
        TLabel *Label5;
        TLabel *Label9;
        TEdit *Evmlt_empty;
        TLabel *Label12;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label20;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *Eph_time;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *Offs_Edit;
        TEdit *Offs2_Edit;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSpargeSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
#endif
