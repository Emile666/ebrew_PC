// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for controlling the
//               various measurements (volume, temperature) can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.9  2016/04/09 12:58:50  Emile
// - First version for new V3.30 PCB HW. Now support 4 temperatures, 4 flowsensors
//   and Boil-Kettle PID-Controller. Various changes to User Interface, Registry
//   parameters and scheduler/tasks.
// - Only 6 parameters left to send to HW. In line with firmware R1.23.
// - New switched/fixes added for tset_boil, gamma_boil and Tboil.
//
// Revision 1.8  2015/12/13 14:20:26  Emile
// - Size of all 3 brew-kettles now adjustable. New Reg. par. VBOIL_MAX added.
// - New 'Mash_Rest' checkbox added to 'Sparge & Mash Settings. New. Reg. par.
//   CB_Mash_Rest. New state 18 'Mast Rest (10 minutes)' added to STD.
// - Pump and Valves are now all off in state 'Add Malt to MLT'.
// - Statusbar now also shows mash and sparge litres (valves indicators removed).
// - Auto-All option added to set all valves and the pump to Auto when 'A' pressed.
//
// Revision 1.7  2015/06/05 19:18:40  Emile
// - STD optimized for new solenoid valves. User Interaction dialog added
// - Calibration & Temp. correction added for flowsensors
//
// Revision 1.6  2015/03/21 09:27:21  Emile
// - Vboil_simulated removed, VHLT_START added
// - task_read_vmlt_boil() with command A6 added (works with ebrew HW R1.12)
// - task_read_vhlt_vmlt() with command A5 added
// - Flow1_hlt_mlt and Flow2_mlt_boil objects added to main-screen
// - New Registry var USE_FLOWSENSORS. Switches between tasks 03/04 and 03F/04F
//
// Revision 1.5  2013/07/21 22:32:47  Emile
// - 3rd intermediate version to support ebrew 2.0 rev.1.5 hardware
// - Changes to Measurement Dialog Screen: VHLT, VMLT, THLT, TMLT
// - Registry: several parameters removed + parameters renamed
// - Ttriac & DAC code & parameters removed
//
// Revision 1.4  2007/08/26 22:23:20  Emile
// - Slope Limiter function added for Thlt, Tmlt, Vhlt, Vmlt and tset_hlt
// - Five Registry variables added: THLT_SLOPE, TMLT_SLOPE, VHLT_SLOPE,
//   VMLT_SLOPE and TSET_HLT_SLOPE
// - Bug-fix setting MA order for HLT Volume: this was coupled to MA order of
//   HLT temperature. Corrected
// - Measurements... and PID controller settings... dialog screen updated.
//
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
        TLabel *Label8;
        TEdit *Vhlt_Max_Edit;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label13;
        TEdit *Tmlt_Offset_Edit;
        TLabel *Label17;
        TEdit *Vmlt_Max_Edit;
        TCheckBox *Flow_Temp_Corr;
        TLabel *Cal_Temp;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *Flow1_Err;
        TEdit *Flow2_Err;
        TEdit *Flow3_Err;
        TEdit *Flow4_Err;
        TLabel *Label4;
        TEdit *Boil_Max_Edit;
        TLabel *Label3;
        TEdit *Thlt_Offset_Edit;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *Tcfc_Offset_Edit;
        TLabel *Label9;
        TEdit *Tboil_Offset_Edit;
        TLabel *Label12;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *Min_FR_MLT;
        TEdit *Min_FR_Boil;
        TLabel *Label16;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMeasurements(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasurements *Measurements;
//---------------------------------------------------------------------------
#endif
