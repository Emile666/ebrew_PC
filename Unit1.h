// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.54  2016/06/11 16:57:28  Emile
// - Indy UDP components removed. udp routines added.
// - Ethernet / UDP communication now works.
//
// Revision 1.53  2016/05/22 13:51:16  Emile
// Bugfixes brewing session 21-05-'16 with v3.30 PCB and HW r1.27
// - Temp.sensor error value is now '-99.99'
// - Double updates to Vboil removed in std
// - HLT and MLT thermometer objects removed
//
// Revision 1.52  2016/05/15 14:20:45  Emile
// - Logfile updated with new volumes and temperatures + date added.
//
// Revision 1.51  2016/04/17 13:00:54  Emile
// - Version after Integration Testing. Works with firmware R1.26.
// - Bug-fix PID-controller.
// - Temps (A0) and Flows (A9) now combined instead of separate tasks.
//
// Revision 1.50  2016/04/09 12:58:50  Emile
// - First version for new V3.30 PCB HW. Now support 4 temperatures, 4 flowsensors
//   and Boil-Kettle PID-Controller. Various changes to User Interface, Registry
//   parameters and scheduler/tasks.
// - Only 6 parameters left to send to HW. In line with firmware R1.23.
// - New switched/fixes added for tset_boil, gamma_boil and Tboil.
//
// Revision 1.49  2016/01/24 19:36:55  Emile
// - Valves and Pump now show colours: RED (on) and GREEN (off)
// - Pipes are now highlighted to show actual direction of fluid movement
// - Initial delay of tasks changed to multiples of 100 msec. scheduler tick
// - Mash-rest in new state now set to 5 min. instead of 10 min.
//
// Revision 1.48  2015/12/13 14:20:27  Emile
// - Size of all 3 brew-kettles now adjustable. New Reg. par. VBOIL_MAX added.
// - New 'Mash_Rest' checkbox added to 'Sparge & Mash Settings. New. Reg. par.
//   CB_Mash_Rest. New state 18 'Mast Rest (10 minutes)' added to STD.
// - Pump and Valves are now all off in state 'Add Malt to MLT'.
// - Statusbar now also shows mash and sparge litres (valves indicators removed).
// - Auto-All option added to set all valves and the pump to Auto when 'A' pressed.
//
// Revision 1.47  2015/08/07 10:59:42  Emile
// - HW revision number now also written to logfile
// - 'Check I2C Hardware' removed from menu-bar, is no longer used
// - Update to HW Settings Dialog screen: unused items are removed
// - Communication Read Timeout now set to 30 msec.
//
// Revision 1.46  2015/07/21 19:42:45  Emile
// - Setting Mash- and Sparge Volume now via maisch.sch and not in Dialog screen anymore.
// - Flow-rate indicators added (HLT->MLT and MLT->Boil) to Main-Screen.
// - Transition from 'Empty MLT' to 'Wait for Boil' now detected automatically with
//   new function flow_rate_low().
// - Registry vars VMLT_EMPTY, MASH_VOL and SPARGE_VOL removed.
// - Functionality and Checkbox for 'Double initial Sparge Volume' added.
// - Registry var CB_VSP2 added.
//
// Revision 1.45  2015/06/06 14:02:33  Emile
// - User Interaction now with PopupMenu to State-label
// - PID Controller now made with a TvrPowerButton instead of a radiobutton box
// - View Mash Progress Form improved
//
// Revision 1.44  2015/06/05 19:18:40  Emile
// - STD optimized for new solenoid valves. User Interaction dialog added
// - Calibration & Temp. correction added for flowsensors
//
// Revision 1.43  2015/03/21 09:27:22  Emile
// - Vboil_simulated removed, VHLT_START added
// - task_read_vmlt_boil() with command A6 added (works with ebrew HW R1.12)
// - task_read_vhlt_vmlt() with command A5 added
// - Flow1_hlt_mlt and Flow2_mlt_boil objects added to main-screen
// - New Registry var USE_FLOWSENSORS. Switches between tasks 03/04 and 03F/04F
//
// Revision 1.42  2014/06/01 13:59:16  Emile
// - Ethernet UDP Communication added.
// - New Registry variable UDP_IP_PORT and USB_COM_PORT renamed in COMM_CHANNEL
// - This version works with Ebrew HW R1.9
//
// Revision 1.41  2013/07/24 14:00:00  Emile
// - Version ready for Integration Testing with Ebrew HW R1.07!
// - Writing parameters to Ebrew HW now works with new task writing_pars
// - HW Revision is now displayed in statusbar if comm. is established
// - Task hw_debug added to display S1, S2 & S3 command info from Ebrew HW.
// - Key-press 'D' now initiates hw_debug task
//
// Revision 1.40  2013/07/23 09:42:46  Emile
// - Fourth intermediate version: several Registry Settings added / removed.
// - Dialog Screens updated: better lay-out and matches new Registry Settings
// - Source-code improved for readability
//
// Revision 1.39  2013/07/21 22:32:47  Emile
// - 3rd intermediate version to support ebrew 2.0 rev.1.5 hardware
// - Changes to Measurement Dialog Screen: VHLT, VMLT, THLT, TMLT
// - Registry: several parameters removed + parameters renamed
// - Ttriac & DAC code & parameters removed
//
// Revision 1.38  2013/06/22 23:04:18  Emile
// - Second intermediate version: scheduler added and timer interrupt divided
//   over a number of tasks.
// - Version works with Ebrew hardware, task duration needs to be optimised!
//
// Revision 1.37  2013/06/16 14:39:19  Emile
// Intermediate version for new Ebrew 2.0 USB hardware:
// - Hardware settings Dialog: COM Port + Settings added + LEDx removed
// - PortTalk + i2c_dll + Start_i2c_communication + Reset_I2C_Bus removed
// - New routines for COM-Port added
// - Generate_IO_Signals() now uses COM_port_write to address all hardware
// - This version works with new hardware: PUMP on/off + LEDs are working
// - HEATER led and PWM output do not work yet + TODO: add scheduler.
//
// Revision 1.36  2011/05/14 14:02:19  Emile
// - Unit test set updates, test-case 16 added
// - Self-Tuning controller N=1 and N=2 added to PID dialog screen
// - PID debug label extended with (Kc Ti Td) and sys. id. parameters
// - Max. sample-time extended to SIXTY_SECONDS (was 20 seconds)
// - Help file updated with version history
//
// Revision 1.35  2007/08/26 22:23:20  Emile
// - Slope Limiter function added for Thlt, Tmlt, Vhlt, Vmlt and tset_hlt
// - Five Registry variables added: THLT_SLOPE, TMLT_SLOPE, VHLT_SLOPE,
//   VMLT_SLOPE and TSET_HLT_SLOPE
// - Bug-fix setting MA order for HLT Volume: this was coupled to MA order of
//   HLT temperature. Corrected
// - Measurements... and PID controller settings... dialog screen updated.
//
// Revision 1.34  2007/07/07 14:25:59  Emile
// - i2c bus closed directly instead of leaving open. Every I2C routine now
//   has a i2c_start() and i2c_stop() added to it.
// - In i2c_stop() PortTalk was closed in case of error. Now done only in case
//   of a PT_CLOSE command.
// - i2c_stop() now has a wrapper asking user to exit program or to continue.
// - Mash timers were reset after an i2c reset. This is corrected
// - Check I2C hardware now done in interrupt routine instead of asynchronous.
//
// Revision 1.33  2006/11/19 10:53:55  Emile
// The power outlet (220 V) is now shared with the modulating gas burner and
// the electrical heating element. By setting the proper bits in the PID
// Settings Dialog Screen, one can select which function is allocated to the
// power outlet. If the gas burner is used, the outlet is energized when the
// pid_output exceeds the gas burner hysteresis (also in the PID settings screen).
// For this: the generate_IO_signals STD has been updated significantly.
//
// Revision 1.32  2006/11/18 23:06:37  Emile
// - View Mash/Sparging screen is improved: time-stamps are placed when a
//   mashing or sparging phase has started.
// - Read_log_file is improved: time-stamps are generated, so that old log-files
//   can be read and time-stamp information can be seen in the Mash/Sparging screen.
// - Datagraps are set to a step-size of 1 instead of 2 (1 div = 100 seconds now).
// - Main screen updated: Heating power is now in % and correct volumes are added.
//
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
#include <Graphics.hpp>
#include "VrControls.hpp"
#include "VrTank.hpp"
#include "VrThermoMeter.hpp"
#include "VrPowerMeter.hpp"
#include "VrLeds.hpp"
#include "VrButtons.hpp"
#include "VrGradient.hpp"
#include <winsock2.h>

#define TS_INIT   (20.0)
#define KC_INIT   (80.0)
#define TI_INIT  (282.0)
#define TD_INIT   (20.0)
#define LOGFILE   "ebrewlog.txt"
#define MASH_FILE "maisch.sch"
#define REGKEY    "Software\\ebrew"

// 1 minute = 1/(60*24) part of one day, see TDateTime for details
#define ONE_MINUTE (6.94444E-04)

//------------------------------
// Defines for StatusBar object
//------------------------------
#define PANEL_SYS_MODE (0)
#define PANEL_MASHS    (1)
#define PANEL_VALVE    (2)
#define PANEL_MSIDX    (3)
#define PANEL_SPIDX    (4)
#define PANEL_REVIS    (5)

//-----------------------------
// E-brew System Mode
//-----------------------------
#define GAS_MODULATING     (0)
#define GAS_NON_MODULATING (1)
#define ELECTRICAL_HEATING (2)

#define IDLE        (0)
#define EL_HTR_OFF  (1)
#define EL_HTR_ON   (2)
#define MOD_GAS_OFF (3)
#define MOD_GAS_ON  (4)

#define max(a, b)  (((a) > (b)) ? (a) : (b))

//-----------------------------------------------------------
// Defines for COM Port Communication.
// The longest task on ebrew HW is the one-wire task,
// this task lasts max. 22 msec.
// Set WAIT_READ_TIMEOUT > 22 msec.
// The max. duration for a task in this program is then:
// MAX_READ_RETRIES * (WR2RD_SLEEP_TIME + WAIT_READ_TIMEOUT).
//-----------------------------------------------------------
#define MAX_READ_RETRIES       (2)
#define NORMAL_READ_TIMEOUT   (40)
#define LONG_READ_TIMEOUT    (115)
#define MAX_BUF_WRITE        (255)
#define MAX_BUF_READ         (255)
#define COM_PORT_DEBUG_FNAME "_com_port_dbg.txt"

#define EBREW_HW_ID          "E-Brew"
#define MAX_PARS             (7)

#define SENSOR_VAL_LIM_OK    (-99.9)

typedef struct _swfx_struct
{
   bool   tset_hlt_sw;   // Switch value for tset_hlt
   double tset_hlt_fx;   // Fix value for tset_hlt
   bool   tset_boil_sw;  // Switch value for tset_boil
   double tset_boil_fx;  // Fix value for tset_boil
   bool   gamma_hlt_sw;  // Switch value for gamma-hlt
   double gamma_hlt_fx;  // Fix value for gamma-hlt
   bool   gamma_boil_sw; // Switch value for gamma-boil
   double gamma_boil_fx; // Fix value for gamma-boil

   bool   thlt_sw;      // Switch value for Thlt
   double thlt_fx;      // Fix value for Thlt [Celsius]
   bool   tmlt_sw;      // Switch value for Tmlt
   double tmlt_fx;      // Fix value for Tmlt [Celsius]
   bool   tboil_sw;     // Switch value for Tboil
   double tboil_fx;     // Fix value for Tboil [Celsius]
   bool   ttriac_sw;    // Switch value for Ttriac
   double ttriac_fx;    // Fix value for Ttriac

   bool   vhlt_sw;      // Switch value for Vhlt
   double vhlt_fx;      // Fix value for Vhlt
   bool   vmlt_sw;      // Switch value for Vmlt
   double vmlt_fx;      // Fix value for Vmlt
   bool   vboil_sw;     // Switch value for Vboil
   double vboil_fx;     // Fix value for Vboil
   bool   std_sw;       // Switch value for STD state
   int    std_fx;       // Fix value for STD state
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
        TLabel *V6;
        TLabel *V7;
        TLabel *Std_State;
        TAnimTimer *T100msec;
        TMenuItem *MenuViewMash_progress;
        TMenuItem *MenuOptionsI2CSettings;
        TMenuItem *SpargeSettings1;
        TMenuItem *Auto1;
        TMenuItem *OFF1;
        TMenuItem *ON1;
        TMenuItem *ReadLogFile1;
        TVrTank *Tank_MLT;
        TVrTank *Tank_HLT;
        TVrTank *Tank_Boil;
        TvrThermoMeter *tm_triac;
        TVrPowerMeter *Heater;
        TMenuItem *Measurements;
        TStatusBar *StatusBar;
        TMenuItem *N2;
        TLabel *PID_dbg;
        TLabel *Ttriac_lbl;
        TLabel *Flow1_hlt_mlt;
        TLabel *Flow2_mlt_boil;
        TImage *Image1;
        TPopupMenu *PopupMenu2;
        TMenuItem *MaltaddedtoMLT1;
        TMenuItem *Boilingstarted1;
        TMenuItem *StartChilling1;
        TMenuItem *ChillingFinished1;
        TVrPowerButton *PID_Ctrl;
        TLabel *Flow2_rate;
        TLabel *Flow1_rate;
        TLabel *HLT_Label;
        TLabel *MLT_Label;
        TLabel *Boil_Label;
        TLabel *Gamma_Perc;
        TVrGradient *VrGradient1;
        TVrGradient *VrGradient2;
        TVrGradient *VrGradient3;
        TVrGradient *VrGradient4;
        TVrGradient *VrGradient5;
        TVrGradient *VrGradient6;
        TVrGradient *VrGradient7;
        TVrGradient *VrGradient8;
        TVrGradient *VrGradient9;
        TVrGradient *VrGradient10;
        TVrGradient *VrGradient11;
        TVrGradient *VrGradient12;
        TVrGradient *VrGradient13;
        TVrGradient *VrGradient14;
        TVrGradient *VrGradient15;
        TVrPowerMeter *Boil;
        TLabel *Gamma_Boil;
        TLabel *Flow3_cfc;
        TLabel *Flow3_rate;
        TLabel *Flow4;
        TLabel *Flow4_rate;
        TLabel *Temp_CFC;
        TLabel *Temp_Boil;
        TLabel *Val_Tset_Boil;
        TLabel *PID_dbg2;
        TLabel *P1;
        TLabel *V8;
        void __fastcall MenuOptionsPIDSettingsClick(TObject *Sender);
        void __fastcall MenuFileExitClick(TObject *Sender);
        void __fastcall MenuEditFixParametersClick(TObject *Sender);
        void __fastcall MenuOptionsI2CSettingsClick(TObject *Sender);
        void __fastcall MenuHelpAboutClick(TObject *Sender);
        void __fastcall T100msecTimer(TObject *Sender);
        void __fastcall MenuEditMashSchemeClick(TObject *Sender);
        void __fastcall MenuViewMash_progressClick(TObject *Sender);
        void __fastcall SpargeSettings1Click(TObject *Sender);
        void __fastcall Auto1Click(TObject *Sender);
        void __fastcall OFF1Click(TObject *Sender);
        void __fastcall ON1Click(TObject *Sender);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ReadLogFile1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall MeasurementsClick(TObject *Sender);
        void __fastcall Contents1Click(TObject *Sender);
        void __fastcall HowtoUseHelp1Click(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall MaltaddedtoMLT1Click(TObject *Sender);
        void __fastcall Boilingstarted1Click(TObject *Sender);
        void __fastcall StartChilling1Click(TObject *Sender);
        void __fastcall ChillingFinished1Click(TObject *Sender);
private:	// User declarations
        void __fastcall ebrew_idle_handler(TObject *Sender, bool &Done);
        void __fastcall print_mash_scheme_to_statusbar(void);
        void __fastcall Main_Initialisation(void);
        void __fastcall Init_Sparge_Settings(void);
        void __fastcall Restore_Settings(void);
        void __fastcall exit_ebrew(void);
        void __fastcall Update_GUI(void);

public:		// User declarations
        bool   pars_changed[MAX_PARS]; // Array with list of changed parameters
        int    system_mode;        // P00: Ebrew system mode
        int    gas_non_mod_llimit; // P01: Lower-limit for switching gas-burner
        int    gas_non_mod_hlimit; // P02: Upper-limit for switching gas-burner
        int    gas_mod_pwm_llimit; // P03: Lower-limit for switching gas-burner
        int    gas_mod_pwm_hlimit; // P04: Upper-limit for switching gas-burner
        double ttriac;             // LM35 (Triac) actual temperature
        int    ttriac_llim;        // P05: Lower-limit for Triac Temp. Protection
        int    ttriac_hlim;        // P06: Upper-limit for Triac Temp. Protection
        bool   triac_too_hot;      // true = Triac is overheated

        double vhlt_max;         // Max. Volume for Vhlt
        double vmlt_max;         // Max. Volume for Vmlt
        double vboil_max;        // Boil Kettle Volume

        double thlt;             // HLT actual temperature
        double thlt_offset;      // Offset to add to Thlt measurement
        double tmlt;             // MLT actual temperature
        double tmlt_offset;      // Offset to add to Tmlt measurement
        double tboil;            // Boil-kettle actual temperature
        double tboil_offset;     // Offset to add to Tboil measurement
        double tcfc;             // CFC-output actual temperature
        double tcfc_offset;      // Offset to add to Tcfc measurement
        double tset_slope_limit; // Slope limiter for Temp. Setpoints
        double gamma_hlt;        // PID controller output for HLT
        double gamma_boil;       // PID controller output for Boil-Kettle
        double tset_hlt;         // HLT reference temperature
        double tset_mlt;         // MLT reference temperature
        double tset_boil;        // HLT reference temperature

        int    comm_channel_nr;       // Communication channel number
        char   com_port_settings[20]; // Virtual COM Port Settings
        int    udp_wait_msec_before_read; // Time-out value for UDP-reads
        char   udp_ip_port[30];       // UDP IP  & Port Number of ebrew hardware
        bool   com_port_is_open;      // true = COM-port is open for Read/Write
        int    com_port_opened;       // remember communication channel
        bool   cb_debug_com_port;     // true = file-logging for COM port communication
        bool   cb_pid_dbg;            // true = Show PID Debug label
        bool   cb_vsp2;               // true = Double the initial Sparge volume to Boil-kettle
        bool   toggle_led;            // Status of Alive LED
        bool   power_up_flag;         // true = power-up in progress
        bool   hw_debug_logging;      // true = write HW debug info to log-file
        int    flow1_err;             // Flowsensor 1 error compensation (-5% ... +5%)
        int    flow2_err;             // Flowsensor 2 error compensation (-5% ... +5%)
        int    flow3_err;             // Flowsensor 3 error compensation (-5% ... +5%)
        int    flow4_err;             // Flowsensor 4 error compensation (-5% ... +5%)
        bool   flow_temp_corr;        // true = compensate flowsensor readings for higher temperatures
        
        volume_struct   volumes;       // Struct for Volumes
        swfx_struct     swfx;          // Switch & Fix settings for tset and gamma
        pid_params      pid_pars_hlt;  // struct containing PID parameters for HLT
        pid_params      pid_pars_boil; // struct containing PID parameters for Boil-Kettle
        sys_id_params   sys_id_pars;   // struct containing system ident. parameters
        maisch_schedule ms[MAX_MS]; // struct containing maisch schedule
        sparge_struct   sp;         // Values for Sparging
        std_struct      std;        // Values for State Transition Diagram
        ma              flow1_ma;   // Moving average filter for flow-rate HLT->MLT
        ma              flow2_ma;   // Moving average filter for flow-rate MLT->Boil
        ma              flow3_ma;   // Moving average filter for flow-rate CFC-output
        ma              flow4_ma;   // Moving average filter for flowsensor 4

        unsigned int    std_out;    // position of valves
                                    // Bit 0 = Pump, Bit 1..7 = V1..V7
                                    // Bit 8..15: 0 = Auto, 1 = Manual Override
        unsigned int    time_switch;// 1: PID is controlled by a time-switch
        TDateTime       dt_time_switch;   // object holding date and time
        char            *ebrew_revision;  // contains CVS revision number
        char            srev[50];   // String for building SW+HW revision numbers
        SOCKET          sock_udp;   // Transmit & Receive socket for UDP communication
        SOCKADDR_IN     local_ip;
        SOCKADDR_IN     ebrew_hw;

        void __fastcall split_ip_port(int *port, char *ip);
        int  __fastcall udp_init(void);
        int  __fastcall udp_read(char *s, int *bytes);
        int  __fastcall udp_write(char *s);
        void __fastcall udp_close(void);
        void __fastcall comm_port_open(void);           // Init. Comm. Port
        void __fastcall comm_port_close(void);          // Close Comm. Port
        void __fastcall comm_port_write(const char *s); // Write Comm. Port
        void __fastcall comm_port_read(char *s);        // Read  Comm. Port
        void __fastcall comm_port_set_read_timeout(DWORD msec);
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
