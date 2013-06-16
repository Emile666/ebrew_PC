// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the I2C Bus related
//               settings can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.9  2004/05/05 15:44:15  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.8  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
// Revision 1.7  2004/03/10 10:10:38  emile
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
// Revision 1.6  2004/02/22 12:56:44  emile
// - SCL clock frequency now adjustable. Following changes are made:
//   - New Registry value: FSCL_PRESCALER
//   - Hardware Setting screen now contains a Combobox with frequencies
//
// Revision 1.5  2004/02/21 23:11:20  emile
// - Changed behaviour after I2C Bus reset to a more silent one. Resulted in:
//   - Addition of checkbox "Give message on successful reset after I2C error"
//     in Hardware Settings. New registry variable "CB_I2C_ERR_MSG".
//   - Print Hardware status dialog screen only if hardware configuration has
//     changed. Added "I2C Devices present" textbox in Hardware Settings.
//     New registry variable "KNOWN_HW_DEVICES"
//   - Restore_Settings only after power-down/power-up (added 'power_up_flag').
// - Exit ebrew if I2C reset was unsuccessful
// - TTRIAC_LLIM default value set to 60 instead of 50
//
// Revision 1.4  2004/01/31 16:01:04  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.3  2003/09/15 20:37:21  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef I2CSettingsDialogH
#define I2CSettingsDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TI2C_Settings : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TLabel *COM_Port_Nr_lbl;
        TEdit *COM_Port_Edit;
        TUpDown *UpDown1;
        TEdit *Thlim_edit;
        TLabel *Label6;
        TEdit *Tllim_edit;
        TLabel *Label7;
        TCheckBox *cb_i2c_err_msg;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *Hw_devices_Edit;
        TComboBox *fscl_combo;
        TLabel *Label11;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *COM_Port_Settings_lbl;
        TEdit *COM_Port_Settings_Edit;
        TCheckBox *cb_debug_com_port;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TI2C_Settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TI2C_Settings *I2C_Settings;
//---------------------------------------------------------------------------
#endif
