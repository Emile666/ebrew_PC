// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.3  2005/11/12 22:19:38  Emile
// - PID Output (Gamma) routing added. It is now possible to send the output
//   of the PID controller to 3 devices: 1) electrical heating element,
//   2) Non-modulating (standard) gas-burner, 3) modulating gas-burner.
// - PID Dialog changed, checkboxes added.
// - New registry variables: CB_PID_OUT, DAC_A, DAC_B
// - The conversion from gamma to a value for the DA-output is done with
//   DAC_A and DAC_B. The DA-output is used to generate the PWM signal for
//   the modulating gas-burner.
// - Led visibility decreased.
// - Bug-fix: Check_HW_Devices screen appeared twice when checking I2C HW. Fixed.
// - Displaying of Vhlt, Vmlt and Ttriac on LED-displays changed (less resolution).
// - Network routines removed.
//
// Revision 1.2  2005/10/23 12:44:38  Emile
// Several changes because of new hardware (MAX1238 instead of PCF8591):
// - Vhlt added, Vmlt and Ttriac now all adjustable to an AD-channel (the
//   PCF8591 is still supported).
// - 2 time-slices added, Vhlt, Vmlt and Ttriac are read in 3 different time-slices.
// - Ttriac also printed as label to screen, plus Switch and Fix added
// - Alive bit is now active-low, changed in exit_ebrew()
// - Registry vars removed: VREF3, VREF4, DAC, VHLT_SIMULATED
// - Registry vars added: VHLT_SRC, VHLT_A, VHLT_B, VMLT_SRC, VMLT_A, VMLT_B,
//                        TTRIAC_SRC, TTRIAC_A, TTRIAC_B and MA_VHLT
// - Debugging for ma filter removed again
// Changes to i2c_dll:
// - File reorganised into 4 layers with routines for more clarity
// - i2c_read/i2c_write: i2c_address() call added in VELLEMAN_CARD mode
// - i2c_address: i2c_start() call added in VELLEMAN_CARD mode
// - Routines added: get_analog_input() and max1238_read()
// - i2c_stop() changed into i2c_stop(enum pt_action pta) so that PortTalk
//   can be closed or remain open
// - init_adc() removed
//
// Revision 1.1  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
// ---------------------------------------------------------------------------
#ifndef MeasurementsDialogH
#define MeasurementsDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMeasurements : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        TCheckBox *Vboil_simulated;
        TLabel *Label8;
        TEdit *Vhlt_init_Edit;
        TLabel *Label3;
        TUpDown *UD_MA_VMLT;
        TEdit *NMA_edit;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7a;
        TEdit *Ttriac_a;
        TLabel *Label9;
        TLabel *Label10;
        TUpDown *UD_MA_HLT;
        TEdit *MA_HLT_Edit;
        TLabel *Label11;
        TUpDown *UD_MA_MLT;
        TEdit *MA_MLT_Edit;
        TLabel *Label12;
        TEdit *Thlt_Offset;
        TLabel *Label13;
        TEdit *Tmlt_Offset;
        TComboBox *Vhlt_src;
        TLabel *Label14;
        TLabel *Label15;
        TComboBox *Vmlt_src;
        TComboBox *Ttriac_src;
        TLabel *Label16;
        TLabel *Label7b;
        TEdit *Ttriac_b;
        TLabel *Label17;
        TEdit *Vmlt_a;
        TLabel *Label18;
        TEdit *Vmlt_b;
        TLabel *Label19;
        TEdit *Vhlt_a;
        TLabel *Label20;
        TEdit *Vhlt_b;
        TLabel *Label21;
        TUpDown *UD_MA_VHLT;
        TEdit *MA_Vhlt;
        TLabel *Label7;
        TLabel *Label22;
        TEdit *Thlt_Slope;
        TEdit *Tmlt_Slope;
        TLabel *Label23;
        TEdit *Vhlt_Slope;
        TLabel *Label24;
        TEdit *Vmlt_Slope;
        void __fastcall Help_ButtonClick(TObject *Sender);
        void __fastcall Vhlt_srcChange(TObject *Sender);
        void __fastcall Vmlt_srcChange(TObject *Sender);
        void __fastcall Ttriac_srcChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMeasurements(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasurements *Measurements;
//---------------------------------------------------------------------------
#endif
