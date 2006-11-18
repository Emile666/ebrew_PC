// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.31  2005/11/12 22:19:38  Emile
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
// Revision 1.30  2005/10/23 12:44:39  Emile
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
// Revision 1.29  2005/08/28 22:17:30  Emile
// - DataGrapfForm: TTimer replaced again for TAnimTimer
// - Debug-code added for MA filter of Vmlt
// - 'H' key now toggles heater between 0% and 100%
// - Text Temp. Sensor 1 & 2 replaced by Temp. Sensor HLT & MLT
// - Printing of vmlt_unf (padc.ad4) removed again from log-file
//
// Revision 1.28  2005/06/11 12:35:07  Emile
// - Keyboard shortcuts 'P' (Pump toggle) and '1' .. '7' (valve toggles) added.
// - Added transition from state 8 back to state 6. This prevents a transition
//   change during sparging when a glitch on Vmlt happens.
// - Added Vmlt_unf (=padc.ad4) to log-file for debugging purposes.
//
// Revision 1.27  2005/04/11 10:50:27  Emile
// Bug-fix: Gas burner hysteresis did not work. Is corrected.
//
// Revision 1.26  2004/05/13 20:50:59  emile
// - Main loop timing improved. Only 99 (of 100) cycles were executed. Loop
//   timing is now reset after 100 loops (5 seconds)
// - TS parameter now only works on PID-controller time-slice. Max. is 20 sec.
// - Bug-fix in init_ma() filter when init. to a value (should be /N).
// - LPF for D-term of PID controller added. New reg. var. K_LPF
// - PID Debug label added + checkbox in PID screen. Default off (NO reg var).
// - Statusbar object added
// - Start made with network functionality. Not operational yet.
//
// Revision 1.25  2004/05/08 14:52:51  emile
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
// Revision 1.24  2004/05/05 15:44:15  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.23  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
// Revision 1.22  2004/03/10 10:10:39  emile
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
// Revision 1.21  2004/02/25 18:51:05  emile
// - Separate Start_I2C_Communication routine created
// - 'T50msec->Enabled = False' removed. This caused lots of problems. Once
//   enabled, always leave it like that (possible bug in TAnimTimer).
// - Menu option 'View|Check I2C HW devices' added
// - CVS revision is now also printed to the log-file
//
// Revision 1.20  2004/02/21 23:11:20  emile
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
// Revision 1.19  2004/02/15 14:48:54  emile
// - HLT and MLT Thermometer objects on screen: max. value is 90 degrees.
// - Error handling improved:
//   - added error detection for WriteIOByte, read_adc and set_led routines.
//   - Error message now also includes text of I2C error
//
// Revision 1.18  2004/02/01 14:47:14  emile
// - Rebuild with new i2c_dll version. The SCL clock frequency is now reset
//   to 10 kHz again (the 90 kHz caused frequent lock-ups of the I2C bus)
// - All GUI related updates are removed from the timer routine and are
//   stored in a separate Idle routine (ebrew_idle_handler). This routine
//   runs in the background, which has a great impact on the CPU load.
// - The timer routine (T50msec2Timer) now only contains the I2C IO and no
//   longer any screen object updates.
//
// Revision 1.17  2004/01/31 21:28:24  emile
// - cvs revision number now added to About Screen
// - Hints added to objects on MainForm
// - vol_hlt, vol_mlt and vol_boil display objects added
// - rename of various display objects for consistency
//
// Revision 1.16  2004/01/31 16:01:05  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.15  2004/01/25 22:00:51  emile
// - Major update of main form. Added thermometer and tank controls from the
//   TMS Instrumentation Workshop (TIW) package.
//
// Revision 1.14  2003/09/24 21:01:18  emile
// - lm92_read() function: no second read if first read already returns with an error.
// - Reset_I2C_Bus() function added. Called when lm92_read() returns with an error.
// - LED Display update: time-slice now every second (was every TS seconds)
// - hw_status is set to 0 if i2c_start() returns with an error
//
// Revision 1.13  2003/09/15 20:37:22  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.12  2003/08/03 13:27:33  emile
// - FileOpen Dialog added to Restore Settings function, so that other log files
//   besides the default name can be read.
// - Menu option 'Read log file...' added.
//
// Revision 1.11  2003/07/11 18:34:46  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm92_read())
//
// Revision 1.10  2003/06/29 20:47:43  emile
// - Changes in Main_Initialisation(). Single exit-point, all code is evaluated,
//   even if i2c_init() or i2c_start() fail. Done for easier debugging.
// - Time-switch debugged, now works properly.
//
// Revision 1.9  2003/06/29 13:11:35  emile
// - Time switch function added (PID options screen). The PID controller is
//   enabled when the predefined date and time have been met.
//
// Revision 1.8  2003/06/01 19:36:34  emile
// - Switch/Fix added for Vmlt
//
// Revision 1.7  2003/06/01 14:08:06  emile
// - Same as previous log entry: CVS and BCB got confused => saved old files.
//
// Revision 1.6  2003/06/01 11:53:48  emile
// - tset has been renamed in tset_hlt for more clearance
// - STD: state 1 -> 2 has been changed. This was 'ms[0].timer != NOT_STARTED'.
//        This is now 'thlt >= tset_hlt', because timer0 only starts with water
//        in the MLT => this caused a dead-lock.
// - 6 defines have been made adjustable via 'Options|Sparge & STD Settings'.
//   These defines were TMLT_HLIMIT, TMLT_LLIMIT, TIMEOUT_1SEC, VMLT_EMPTY,
//   TIMEOUT3 and TIMEOUT4.
//
// Revision 1.5  2003/01/05 14:31:22  emile
// - Bug-fix: Pressing the top-right Close button of EBrew now activates the
//   MenuFileExitClick method. This was not the case yet, which resulted in
//   a termination of EBrew without the I2C bus properly closed.
//
// Revision 1.4  2003/01/04 22:35:50  emile
// - Restore Settings function now restores all relevant variables (not just
//   the mashing variables). Several separate functions are created to
//   support this.
//
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
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Registry.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ComCtrls.hpp>

#include "misc.h"
#include "pid_reg.h"
#include "AnimTimer.h"
#include "i2c_dll.h"
#include <Graphics.hpp>
#include "VrControls.hpp"
#include "VrTank.hpp"
#include "VrThermoMeter.hpp"
#include "VrPowerMeter.hpp"
#include <ScktComp.hpp>
#include "VrLeds.hpp"

#define TS_INIT    (5.0)
#define KC_INIT   (69.0)
#define TI_INIT  (520.0)
#define TD_INIT  (130.0)
#define LOGFILE "ebrewlog.txt"
#define MASH_FILE "maisch.sch"
#define REGKEY    "Software\\ebrew"
#define PHEATER (3000)

// 1 minute = 1/(60*24) part of one day, see TDateTime for details
#define ONE_MINUTE (6.94444E-04)

// Macro, used in TMainForm::Main_Initialisation()
#define PR_HW_STAT(x)   hw_status & x ? strcpy(s1,YES_TXT) : strcpy(s1,NOT_TXT)

//---------------------------------------------------------------
// For the LED Displays: 0=Thlt  , 1=Tmlt, 2=Tset_hlt, 3=Tset_mlt
//                       4=Ttriac, 5=Vmlt, 6=Vhlt    , 7=gamma
// Macro, used in TMainForm::T50msec2Timer()
//---------------------------------------------------------------
#define SET_LED(LEDX_OK,w_disp,ledx,ledx_vis,ledx_base)                        \
 if (hw_status & (LEDX_OK))                                                    \
 {                                                                             \
   switch (ledx)                                                               \
   {                                                                           \
     case 0: err  = set_led((int)(100.0 * thlt),2,(w_disp),(ledx_vis));        \
             break;                                                            \
     case 1: err  = set_led((int)(100.0 * tmlt),2,(w_disp),(ledx_vis));        \
             break;                                                            \
     case 2: err  = set_led((int)(100.0 * tset_hlt),2,(w_disp),(ledx_vis));    \
             break;                                                            \
     case 3: err  = set_led((int)(100.0 * tset_mlt),2,(w_disp),(ledx_vis));    \
             break;                                                            \
     case 4: err  = set_led((int)(10.0 * ttriac),3,(w_disp),(ledx_vis));       \
             break;                                                            \
     case 5: err  = set_led((int)(10.0 * volumes.Vmlt),3,(w_disp),(ledx_vis)); \
             break;                                                            \
     case 6: err  = set_led((int)(10.0 * volumes.Vhlt),3,(w_disp),(ledx_vis)); \
             break;                                                            \
     case 7: err  = set_led((int)(10.0 * gamma),3,(w_disp),(ledx_vis));        \
             break;                                                            \
    default: break;                                                            \
   }                                                                           \
   if (err) Reset_I2C_Bus(ledx_base,err);                                      \
 }

//------------------------------
// Defines for StatusBar object
//------------------------------
#define PANEL_TCPIP (0)
#define PANEL_MASHS (1)
#define PANEL_MSIDX (2)
#define PANEL_SPIDX (3)
#define PANEL_VALVE (4)
#define PANEL_REVIS (5)

//----------------------------------------------------------------------
// Defines for cb_pid_out. This is an indicator where the output of the
// PID-Controller [%] is sent to. There are three possibilities:
// ELECTRIC    : the output is sent to an electric heating element
// GAS_NON_MOD : the output is sent to a non-modulating gas-burner
// GAS_MODULATE: the output is sent to a modulating gas-burner
//----------------------------------------------------------------------
#define PID_OUT_ELECTRIC     (0x01)
#define PID_OUT_GAS_NON_MOD  (0x02)
#define PID_OUT_GAS_MODULATE (0x04)

//------------------------------------------------------------------------------
// The text I2C_STOP_ERR_TXT is printed whenever i2c_stop() was not successful
//------------------------------------------------------------------------------
#define I2C_STOP_ERR_TXT "i2c_stop() not successful: Cycle power Off -> On, then press OK button"

typedef struct _swfx_struct
{
   bool   tset_hlt_sw;  // Switch value for tset_hlt
   double tset_hlt_fx;  // Fix value for tset_hlt
   bool   gamma_sw;     // Switch value for gamma
   double gamma_fx;     // Fix value for gamma
   bool   thlt_sw;      // Switch value for Thlt
   double thlt_fx;      // Fix value for Thlt [Celsius]
   bool   tmlt_sw;      // Switch value for Tmlt
   double tmlt_fx;      // Fix value for Tmlt [Celsius]
   bool   std_sw;       // Switch value for STD state
   int    std_fx;       // Fix value for STD state
   bool   ttriac_sw;    // Switch value for Ttriac
   double ttriac_fx;    // Fix value for Ttriac
   bool   vmlt_sw;      // Switch value for Vmlt
   double vmlt_fx;      // Fix value for Vmlt
   bool   vhlt_sw;      // Switch value for Vhlt
   double vhlt_fx;      // Fix value for Vhlt
} swfx_struct;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TPopupMenu *PopupMenu1;
        TMenuItem *Edit1;
        TMenuItem *View1;
        TMenuItem *MenuOptions;
        TMenuItem *Help1;
        TMenuItem *MenuHelpAbout;
        TMenuItem *HowtoUseHelp1;
        TMenuItem *Contents1;
        TMenuItem *MenuOptionsPIDSettings;
        TMenuItem *MenuView_I2C_HW_Devices;
        TMenuItem *MenuViewData_graphs;
        TMenuItem *MenuEditMashScheme;
        TMenuItem *File2;
        TMenuItem *MenuFileExit;
        TMenuItem *MenuEditFixParameters;
        TLabel *Val_Thlt;
        TLabel *Val_Tset_hlt;
        TLabel *Val_Tmlt;
        TLabel *Val_Tset_mlt;
        TLabel *Vol_HLT;
        TLabel *Vol_Boil;
        TLabel *Vol_MLT;
        TLabel *P0;
        TLabel *V1;
        TLabel *V2;
        TLabel *V3;
        TLabel *V4;
        TLabel *V5;
        TLabel *V6;
        TLabel *V7;
        TLabel *Std_State;
        TRadioGroup *PID_RB;
        TAnimTimer *T50msec;
        TMenuItem *MenuViewMash_progress;
        TMenuItem *MenuOptionsI2CSettings;
        TMenuItem *SpargeSettings1;
        TMenuItem *Auto1;
        TMenuItem *OFF1;
        TMenuItem *ON1;
        TMenuItem *ReadLogFile1;
        TvrThermoMeter *tm_mlt;
        TVrTank *Tank_MLT;
        TvrThermoMeter *tm_hlt;
        TVrTank *Tank_HLT;
        TVrTank *Tank_Boil;
        TvrThermoMeter *tm_triac;
        TVrPowerMeter *Heater;
        TMenuItem *Measurements;
        TImage *Image1;
        TStatusBar *StatusBar;
        TMenuItem *N2;
        TLabel *PID_dbg;
        TLabel *Ttriac_lbl;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        void __fastcall MenuOptionsPIDSettingsClick(TObject *Sender);
        void __fastcall MenuFileExitClick(TObject *Sender);
        void __fastcall MenuEditFixParametersClick(TObject *Sender);
        void __fastcall MenuOptionsI2CSettingsClick(TObject *Sender);
        void __fastcall MenuHelpAboutClick(TObject *Sender);
        void __fastcall T50msec2Timer(TObject *Sender);
        void __fastcall MenuEditMashSchemeClick(TObject *Sender);
        void __fastcall MenuViewMash_progressClick(TObject *Sender);
        void __fastcall MenuViewData_graphsClick(TObject *Sender);
        void __fastcall SpargeSettings1Click(TObject *Sender);
        void __fastcall Auto1Click(TObject *Sender);
        void __fastcall OFF1Click(TObject *Sender);
        void __fastcall ON1Click(TObject *Sender);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ReadLogFile1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall MenuView_I2C_HW_DevicesClick(TObject *Sender);
        void __fastcall MeasurementsClick(TObject *Sender);
        void __fastcall Contents1Click(TObject *Sender);
        void __fastcall HowtoUseHelp1Click(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
        void __fastcall ebrew_idle_handler(TObject *Sender, bool &Done);
        void __fastcall Start_I2C_Communication(int known_status);
        void __fastcall print_mash_scheme_to_statusbar(void);
        void __fastcall Main_Initialisation(void);
        void __fastcall Init_Sparge_Settings(void);
        void __fastcall Restore_Settings(void);
        void __fastcall exit_ebrew(void);
        void __fastcall Reset_I2C_Bus(int i2c_bus_id, int err);
        void __fastcall Generate_IO_Signals(void);

        timer_vars      tmr;        // struct with timer variables
        ma              str_thlt;   // Struct for MA5 filter for HLT temperature
        ma              str_tmlt;   // Struct for MA5 filter for MLT temperature
        int             led1;       // Which variable to display?
        int             led2;       //
        int             led3;       //
        int             led4;       //
        int             led1_vis;   // 1..7: LED1 Visibility
        int             led2_vis;   // 1..7: LED2 Visibility
        int             led3_vis;   // 1..7: LED3 Visibility
        int             led4_vis;   // 1..7: LED4 Visibility
        int             ttriac_hlim;    // High limit for Triac Temp. Protection
        int             ttriac_llim;    // Low  limit for Triac Temp. Protection
        bool            triac_too_hot;  // true = Triac is overheated
        bool            cb_i2c_err_msg; // true = give error message on successful I2C reset
        bool            power_up_flag;  // true = power-up
        int             known_hw_devices; // list of known I2C hardware devices
        int             fscl_prescaler;   // index into PCF8584 prescaler values, see i2c_dll.cpp
        double          thlt_offset;      // calibration offset to add to Thlt measurement
        double          tmlt_offset;      // calibration offset to add to Tmlt measurement
        bool            cb_pid_dbg;       // true = Show PID Debug label
        byte            cb_pid_out;       // [ELECTRIC, GAS_NON_MOD, GAS_MODULATE]
        double          dac_a;            // a-coefficient for y=a.x+b DAC calc.
        double          dac_b;            // b-coefficient for y=a.x+b DAC calc.
        enum i2c_adc    vhlt_src;         // Vhlt source AD channel
        double          vhlt_a;           // a-coefficient for y=a.x+b
        double          vhlt_b;           // b-coefficient for y=a.x+b
        enum i2c_adc    vmlt_src;         // Vmlt source AD channel
        double          vmlt_a;           // a-coefficient for y=a.x+b
        double          vmlt_b;           // b-coefficient for y=a.x+b
        enum i2c_adc    ttriac_src;       // Ttriac source AD channel
        double          ttriac_a;         // a-coefficient for y=a.x+b
        double          ttriac_b;         // b-coefficient for y=a.x+b
public:		// User declarations
        ma              str_vhlt;   // Struct for MA5 filter for HLT volume
        ma              str_vmlt;   // Struct for MA5 filter for MLT volume
        swfx_struct     swfx;       // Switch & Fix settings for tset and gamma
        double          gamma;      // PID controller output
        double          tset_hlt;   // HLT reference temperature
        double          tset_mlt;   // MLT reference temperature
        double          thlt;       // HLT actual temperature
        double          tmlt;       // MLT actual temperature
        double          ttriac;     // Triac electronics actual temperature
        pid_params      pid_pars;   // struct containing PID parameters
        int             hw_status;  // I2C HW status, see i2c_dll.h for bit settings
        maisch_schedule ms[MAX_MS]; // struct containing maisch schedule
        sparge_struct   sp;         // Values for Sparging
        std_struct      std;        // Values for State Transition Diagram
        unsigned int    std_out;    // position of valves
                                    // Bit 0 = Pump, Bit 1..7 = V1..V7
                                    // Bit 8..15: 0 = Auto, 1 = Manual Override
        unsigned int    time_switch;// 1: PID is controlled by a time-switch
        TDateTime       dt_time_switch;  // object holding date and time
        volume_struct   volumes;         // Struct for Volumes
        bool            burner_on;       // true = gas burner is on
        char            *ebrew_revision; // contains CVS revision number
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
