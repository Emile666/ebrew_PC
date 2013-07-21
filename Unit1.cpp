// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.61  2013/06/22 23:04:18  Emile
// - Second intermediate version: scheduler added and timer interrupt divided
//   over a number of tasks.
// - Version works with Ebrew hardware, task duration needs to be optimised!
//
// Revision 1.60  2013/06/16 14:39:19  Emile
// Intermediate version for new Ebrew 2.0 USB hardware:
// - Hardware settings Dialog: COM Port + Settings added + LEDx removed
// - PortTalk + i2c_dll + Start_i2c_communication + Reset_I2C_Bus removed
// - New routines for COM-Port added
// - Generate_IO_Signals() now uses COM_port_write to address all hardware
// - This version works with new hardware: PUMP on/off + LEDs are working
// - HEATER led and PWM output do not work yet + TODO: add scheduler.
//
// Revision 1.59  2011/05/29 20:56:26  Emile
// - New Registry variables added: STC_N, STC_TD and STC_ADF
// - PID Settings Dialog screen extended with new parameters for self-tuning
//   controller: possibility to set the system order N, an estimate for the
//   time-delay and a boolean whether or not to use adaptive dir. forgetting.
// - PID Settings Dialog screen: parameters enabled/disabled when a
//   specific PID controller is chosen.
// - New functions time_delay() and init_time_delay() added
// - Changes made in init_pid2() function header.
// - Unit-test cases updated and extended with tests for new functions.
//
// Revision 1.58  2011/05/14 14:02:18  Emile
// - Unit test set updates, test-case 16 added
// - Self-Tuning controller N=1 and N=2 added to PID dialog screen
// - PID debug label extended with (Kc Ti Td) and sys. id. parameters
// - Max. sample-time extended to SIXTY_SECONDS (was 20 seconds)
// - Help file updated with version history
//
// Revision 1.57  2011/05/06 11:09:42  Emile
// - pid_reg1(), pid_reg2(), init_pid1(), init_pid2() removed.
// - pid_reg4() changed into pure Takahashi PID controller, no D-filtering anymore.
// - PID dialog updated to reflect changes.
//
// Revision 1.56  2010/05/16 18:33:00  Emile
// - Bug-fix: offset to Thlt and Tmlt was added every second. Now corrected.
//
// Revision 1.55  2007/08/26 22:23:20  Emile
// - Slope Limiter function added for Thlt, Tmlt, Vhlt, Vmlt and tset_hlt
// - Five Registry variables added: THLT_SLOPE, TMLT_SLOPE, VHLT_SLOPE,
//   VMLT_SLOPE and TSET_HLT_SLOPE
// - Bug-fix setting MA order for HLT Volume: this was coupled to MA order of
//   HLT temperature. Corrected
// - Measurements... and PID controller settings... dialog screen updated.
//
// Revision 1.54  2007/07/07 14:25:59  Emile
// - i2c bus closed directly instead of leaving open. Every I2C routine now
//   has a i2c_start() and i2c_stop() added to it.
// - In i2c_stop() PortTalk was closed in case of error. Now done only in case
//   of a PT_CLOSE command.
// - i2c_stop() now has a wrapper asking user to exit program or to continue.
// - Mash timers were reset after an i2c reset. This is corrected
// - Check I2C hardware now done in interrupt routine instead of asynchronous.
//
// Revision 1.53  2007/07/06 22:23:01  Emile
// - The real time between two lines from a log-file is now used instead of a
//   fixed 5 sec. time when reading a log-file.
// - Leading-zero bug solved in Mash Progress screen
// - i2c_stop() only called with PT_CLOSE in case of exit of program
// - System Identification library functions added (but not used yet).
//
// Revision 1.52  2007/01/03 13:45:49  Emile
// - Bugfix: when reading a log-file, the first mash timestamp was not recognised.
// - Bugfix: Sparging timestamps were erased when a sparging parameter was updated.
// - Bugfix: sparging_start was printed to log_structp.txt instead of sparging_start[0]
//
// Revision 1.51  2006/11/19 10:53:55  Emile
// The power outlet (220 V) is now shared with the modulating gas burner and
// the electrical heating element. By setting the proper bits in the PID
// Settings Dialog Screen, one can select which function is allocated to the
// power outlet. If the gas burner is used, the outlet is energized when the
// pid_output exceeds the gas burner hysteresis (also in the PID settings screen).
// For this: the generate_IO_signals STD has been updated significantly.
//
// Revision 1.50  2006/11/18 23:06:37  Emile
// - View Mash/Sparging screen is improved: time-stamps are placed when a
//   mashing or sparging phase has started.
// - Read_log_file is improved: time-stamps are generated, so that old log-files
//   can be read and time-stamp information can be seen in the Mash/Sparging screen.
// - Datagraps are set to a step-size of 1 instead of 2 (1 div = 100 seconds now).
// - Main screen updated: Heating power is now in % and correct volumes are added.
//
// Revision 1.49  2006/06/04 12:28:23  Emile
// - Bug-fix: Vhlt simulated did nog get a correct value at power-up. Corrected
// - Project Make file corrected
// - Build with runtime packages now in .exe file
//
// Revision 1.48  2006/02/19 13:14:35  Emile
// - Bug-fix reading logfile(). If the latest mash timer was not started yet,
//   it was set to a high value (which was the linenumber in the logfile).
//   Setting the mash-timers should be oke now.
// - Max. linenumber changed from 32767 to 65535, meaning that 91 hours in 1
//   log-entry is possible now.
//
// Revision 1.47  2005/11/12 22:19:38  Emile
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
// Revision 1.46  2005/10/23 20:00:30  Emile
// - Bug-fix writing to log-file
// - Default values of Registry variables updated when created initially.
//
// Revision 1.45  2005/10/23 17:50:26  Emile
// - Writing to log-file updated with Vhlt
//
// Revision 1.44  2005/10/23 12:44:38  Emile
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
// Revision 1.43  2005/08/30 09:17:42  Emile
// - Bug-fix reading log-file. Only entries > 1 minute can be imported.
// - sp_idx added to log-file, instead of PID_ON.
// - Stay 10 seconds in state 5 at start of sparging for logging purposes
// - Reorganisation of routines of reading log file, added print_p_struct().
//
// Revision 1.42  2005/08/28 22:17:30  Emile
// - DataGrapfForm: TTimer replaced again for TAnimTimer
// - Debug-code added for MA filter of Vmlt
// - 'H' key now toggles heater between 0% and 100%
// - Text Temp. Sensor 1 & 2 replaced by Temp. Sensor HLT & MLT
// - Printing of vmlt_unf (padc.ad4) removed again from log-file
//
// Revision 1.41  2005/06/11 12:35:07  Emile
// - Keyboard shortcuts 'P' (Pump toggle) and '1' .. '7' (valve toggles) added.
// - Added transition from state 8 back to state 6. This prevents a transition
//   change during sparging when a glitch on Vmlt happens.
// - Added Vmlt_unf (=padc.ad4) to log-file for debugging purposes.
//
// Revision 1.40  2005/04/11 12:11:03  Emile
// - Added safety feature: gas burner is disabled if time_switch is set.
//
// Revision 1.39  2005/04/11 10:57:39  Emile
// - Bug-fix: exit_ebrew() did not shut down gas-burner (and alive LED). Corrected.
//
// Revision 1.38  2005/04/11 10:50:27  Emile
// Bug-fix: Gas burner hysteresis did not work. Is corrected.
//
// Revision 1.37  2005/04/11 10:35:12  Emile
// - exit_ebrew(): delete pointers only if still active
// - Only PUMP and HEATER bits inverted, alive and burner bits not
//
// Revision 1.36  2005/03/26 13:53:21  Emile
// - During State "Mash Preheat" pump is set to ON (again)
// - Added a burner_on option (bit 4 on LSB_IO). For this two new registry
//   variables are introduced (BURNER_HHYST and BURNER_LHYST)
// - Various screens a little enlarged (no scrollbars visible anymore)
//
// Revision 1.35  2004/05/13 20:50:59  emile
// - Main loop timing improved. Only 99 (of 100) cycles were executed. Loop
//   timing is now reset after 100 loops (5 seconds)
// - TS parameter now only works on PID-controller time-slice. Max. is 20 sec.
// - Bug-fix in init_ma() filter when init. to a value (should be /N).
// - LPF for D-term of PID controller added. New reg. var. K_LPF
// - PID Debug label added + checkbox in PID screen. Default off (NO reg var).
// - Statusbar object added
// - Start made with network functionality. Not operational yet.
//
// Revision 1.34  2004/05/10 20:54:29  emile
// - Bug-fix: log-file header: '\n' was removed, is corrected now
// - Hints added to PID dialog screen
// - Now four PID controllers to choose from. Old ebrew version is still listed,
//   but should not be used anymore. Simulation showed stability problems.
//   Preferably use the type C controller.
//
// Revision 1.33  2004/05/08 14:52:51  emile
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
// Revision 1.32  2004/05/05 15:44:15  emile
// - Main Screen picture update
// - Init_ma() now initialises with a value instead of 0. Avoids reset of signal.
// - STD update: calculation of volumes should be correct now
// - Parameter added for early start of mash timer. Registry var. TOffset2
// - Registry variables Kc, Ti, Td, TOffset and TS are now floats instead of integers.
// - Some screens updated with hints (also of labels)
// - Bug-fix: unnecessary delay after change in gamma. Is corrected now
// - Help via menu now also works
//
// Revision 1.31  2004/04/26 13:30:22  emile
// Bug-fix: init_pid3() did not calculate correctly when Ti = 0. Corrected.
//
// Revision 1.30  2004/04/25 14:02:17  emile
// - Added a 'type C' PID controller, function pid_reg3(). Possible to select
//   this from the PID settings dialog screen. Left the old one in for
//   compatibility. New registry variable PID_Model.
// - Gamma added to log-file, so that the PID controller can be tuned.
//
// Revision 1.29  2004/04/19 21:55:49  emile
// - Added calibration offsets and MA-filters for Thlt and Tmlt:
//   - New Registry variables MA_THLT, MA_TMLT, THLT_OFFSET and TMLT_OFFSET.
//   - New Measurement Dialog screen
//   - Several parameters moved from HW Settings Dialog Screen
// - Added new Registry variable MA_VMLT: MA filter order of Vmlt is now
//   also stored in Registry
// - Help-file is updated
//
// Revision 1.28  2004/03/10 10:10:38  emile
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
// Revision 1.27  2004/02/25 18:51:05  emile
// - Separate Start_I2C_Communication routine created
// - 'T50msec->Enabled = False' removed. This caused lots of problems. Once
//   enabled, always leave it like that (possible bug in TAnimTimer).
// - Menu option 'View|Check I2C HW devices' added
// - CVS revision is now also printed to the log-file
//
// Revision 1.26  2004/02/25 10:07:38  emile
// - fscl_values[] now moved to i2c_dll.cpp
// - bug-fix in exit routine: no i2c_stop error message if hw_status == 0
// - disable t50msec interrupt routine first before calling i2c_stop()
//
// Revision 1.25  2004/02/22 12:56:44  emile
// - SCL clock frequency now adjustable. Following changes are made:
//   - New Registry value: FSCL_PRESCALER
//   - Hardware Setting screen now contains a Combobox with frequencies
//
// Revision 1.24  2004/02/21 23:11:20  emile
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
// Revision 1.23  2004/02/15 14:48:53  emile
// - HLT and MLT Thermometer objects on screen: max. value is 90 degrees.
// - Error handling improved:
//   - added error detection for WriteIOByte, read_adc and set_led routines.
//   - Error message now also includes text of I2C error
//
// Revision 1.22  2004/02/01 14:47:13  emile
// - Rebuild with new i2c_dll version. The SCL clock frequency is now reset
//   to 10 kHz again (the 90 kHz caused frequent lock-ups of the I2C bus)
// - All GUI related updates are removed from the timer routine and are
//   stored in a separate Idle routine (ebrew_idle_handler). This routine
//   runs in the background, which has a great impact on the CPU load.
// - The timer routine (T50msec2Timer) now only contains the I2C IO and no
//   longer any screen object updates.
//
// Revision 1.21  2004/01/31 21:28:24  emile
// - cvs revision number now added to About Screen
// - Hints added to objects on MainForm
// - vol_hlt, vol_mlt and vol_boil display objects added
// - rename of various display objects for consistency
//
// Revision 1.20  2004/01/31 16:01:05  emile
// - Init. HW High/Low limit temp. changed to 70/50 C respectively.
// - Added code for calculation/simulation of Vhlt and Vboil
// - Hardware dialog updated: 3 new controls added for Vhlt and Vboil simulation
// - Registry key no longer in ebrew but in Software\\ebrew
// - First attempt to catch CVS version ID in source code
//
// Revision 1.19  2004/01/25 22:00:50  emile
// - Major update of main form. Added thermometer and tank controls from the
//   TMS Instrumentation Workshop (TIW) package.
//
// Revision 1.18  2004/01/11 21:49:29  emile
// - Error corrected: an I2C bus error triggered the Reset_i2C_bus function,
//   Main_Initialisation was called and ms_idx was set to 0. This got logged
//   in the log-file. When reading ms_idx from the log-file, it read 0!
// - During read_log_file: Last mash timer was set to running when in sparging.
//   Now set all mash timers to time-out when mashing has finished.
//
// Revision 1.17  2003/12/21 21:16:59  emile
// - Old About screen removed, replaced by Version Aware About box. This
//   new About screen shows the version number (and a nice picture!).
//
// Revision 1.16  2003/12/21 13:39:40  emile
// - Writing to LSB_IO: all bits are inverted, because hardware is modified to
//   enable sinking instead of sourcing.
//
// Revision 1.15  2003/09/24 21:01:18  emile
// - lm92_read() function: no second read if first read already returns with an error.
// - Reset_I2C_Bus() function added. Called when lm92_read() returns with an error.
// - LED Display update: time-slice now every second (was every TS seconds)
// - hw_status is set to 0 if i2c_start() returns with an error
//
// Revision 1.14  2003/09/15 20:37:21  emile
// - LM76 constants renamed in LM92 constants
// - Pump Popupmenu added (same as already done for the valves)
// - Added support for LED3 and LED4 displays
// - 'I2C settings' renamed into 'Hardware Settings'
// - Added more variables to LED1..LED4 selection. Now 6 variables to select
// - Added SET_LED macro
// - Added Triac Temperature protection functionality
//
// Revision 1.13  2003/08/03 13:27:32  emile
// - FileOpen Dialog added to Restore Settings function, so that other log files
//   besides the default name can be read.
// - Menu option 'Read log file...' added.
//
// Revision 1.12  2003/07/11 18:34:45  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm92_read())
//
// Revision 1.11  2003/06/29 20:47:43  emile
// - Changes in Main_Initialisation(). Single exit-point, all code is evaluated,
//   even if i2c_init() or i2c_start() fail. Done for easier debugging.
// - Time-switch debugged, now works properly.
//
// Revision 1.10  2003/06/29 13:11:35  emile
// - Time switch function added (PID options screen). The PID controller is
//   enabled when the predefined date and time have been met.
//
// Revision 1.9  2003/06/01 19:36:34  emile
// - Switch/Fix added for Vmlt
//
// Revision 1.8  2003/06/01 14:08:06  emile
// - Same as previous log entry: CVS and BCB got confused => saved old files.
//
// Revision 1.7  2003/06/01 13:37:42  emile
// - Bugfix: switch/fix for Tmlt and Thlt were in wrong time-slice. Corrected.
// - Switch/fix for std state added for easier testing
// - Vmash value added to 'Options|Sparge & STD Settings' dialog screen.
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
// Revision 1.5  2003/01/05 14:31:21  emile
// - Bug-fix: Pressing the top-right Close button of EBrew now activates the
//   MenuFileExitClick method. This was not the case yet, which resulted in
//   a termination of EBrew without the I2C bus properly closed.
//
// Revision 1.4  2003/01/04 22:35:49  emile
// - Restore Settings function now restores all relevant variables (not just
//   the mashing variables). Several separate functions are created to
//   support this.
//
// Revision 1.3  2002/12/30 20:21:58  emile
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
#include <vcl.h>
#pragma hdrstop

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Unit1.h"
#include "FixParamsDialog.h"
#include "PID_Settings_Dialog.h"
#include "I2CSettingsDialog.h"
#include "MeasurementsDialog.h"
#include "EditMashScheme.h"
#include "ViewMashProgressForm.h"
#include "Sparge_Settings.h"
#include "RestoreSettings.h"
#include "misc.h"
#include "VersionAwareAbout.h"
#include <Dialogs.hpp>
#include "scheduler.h"

extern vector theta; // defined in pid_reg.h
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AnimTimer"
#pragma link "VrControls"
#pragma link "VrTank"
#pragma link "VrThermoMeter"
#pragma link "VrPowerMeter"
#pragma link "VrLeds"
#pragma resource "*.dfm"

TMainForm *MainForm;

// GLOBAL VARIABLES for COM Port Communication
HANDLE       hComm = NULL;
COMMTIMEOUTS ctmoNew = {0}, ctmoOld;
FILE         *fdbg_com; // COM-port debug file-descriptor

/*-----------------------------------------------------------------------------
  Purpose    : TASK 01: Read THLT (HLT Temperature) from Ebrew hardware
  Period-Time: 1 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_read_thlt(void)
{
    char   s[MAX_BUF_READ];

    MainForm->COM_port_write("A3\n"); // A3 = THLT
    MainForm->COM_port_read(s);       // Read HLT temp. from LM92 device
    if (!strncmp(s,"Thlt=",5)) MainForm->Val_Thlt->Font->Color = clLime;
    else                       MainForm->Val_Thlt->Font->Color = clRed;
    MainForm->thlt = atof(&s[5]);
    if (MainForm->swfx.thlt_sw)
    {  // Switch & Fix
       MainForm->thlt = (double)(MainForm->swfx.thlt_fx);
    } // if
} // task_read_thlt()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 02: Read TMLT (MLT Temperature) from Ebrew hardware
  Period-Time: 1 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_read_tmlt(void)
{
    char   s[MAX_BUF_READ];

    MainForm->COM_port_write("A4\n"); // A4 = TMLT
    MainForm->COM_port_read(s);       // Read MLT temp. from LM92 device
    if (!strncmp(s,"Tmlt=",5)) MainForm->Val_Tmlt->Font->Color = clLime;
    else                       MainForm->Val_Tmlt->Font->Color = clRed;
    MainForm->tmlt = atof(&s[5]);
    if (MainForm->swfx.tmlt_sw)
    {  // Switch & Fix
       MainForm->tmlt = (double)(MainForm->swfx.tmlt_fx);
    } // if
} // task_read_tmlt()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 03: Read VHLT (HLT Volume) from Ebrew hardware
  Period-Time: 1 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_read_vhlt(void)
{
    char   s[MAX_BUF_READ];

    MainForm->COM_port_write("A1\n"); // A1 = VHLT
    MainForm->COM_port_read(s);       // Read HLT Volume from pressure sensor
    if (!strncmp(s,"Vhlt=",5)) MainForm->Vol_HLT->Font->Color = clLime;
    else                       MainForm->Vol_HLT->Font->Color = clRed;
    MainForm->volumes.Vhlt  = atof(&s[5]);
    if (MainForm->swfx.vhlt_sw)
    {  // Switch & Fix
       MainForm->volumes.Vhlt = MainForm->swfx.vhlt_fx;
    } // if
} // task_read_vhlt()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 04: Read VMLT (MLT Volume) from Ebrew hardware
  Period-Time: 1 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_read_vmlt(void)
{
    char   s[MAX_BUF_READ];

    MainForm->COM_port_write("A2\n"); // A2 = VMLT
    MainForm->COM_port_read(s);       // Read MLT Volume from pressure sensor
    if (!strncmp(s,"Vmlt=",5)) MainForm->Vol_MLT->Font->Color = clLime;
    else                       MainForm->Vol_MLT->Font->Color = clRed;
    MainForm->volumes.Vmlt  = atof(&s[5]);
    if (MainForm->swfx.vmlt_sw)
    {  // Switch & Fix
       MainForm->volumes.Vmlt = MainForm->swfx.vmlt_fx;
    } // if
} // task_read_vmlt()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 05: Read LM35 temperature from Ebrew hardware. Typically
               this is the temperature inside the Ebrew hardware and is coupled
               to the Triac Temp. The check for overtemp. is also done here.
  Period-Time: 1 second
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void task_read_lm35(void)
{
    char   s[MAX_BUF_READ];

    if (MainForm->ttriac_src != NONE) // get real value from Ebrew hardware
    {
        MainForm->COM_port_write("A0\n"); // A0 = LM35
        MainForm->COM_port_read(s);       // Read LM35 Volume from Ebrew hardware
        if (!strncmp(s,"Lm35=",5)) MainForm->Ttriac_lbl->Font->Color = clLime;
        else                       MainForm->Ttriac_lbl->Font->Color = clRed;
        MainForm->ttriac  = atof(&s[5]);
    } // if
    if (MainForm->swfx.ttriac_sw)
    {  // Switch & Fix
       MainForm->ttriac = MainForm->swfx.ttriac_fx;
    } // if
    //---------------------------------------------------
    // Triac Temperature Protection: hysteresis function
    //---------------------------------------------------
    if (MainForm->triac_too_hot)
    { // Reset if temp. < lower-limit
      MainForm->triac_too_hot = (MainForm->ttriac >= MainForm->ttriac_llim);
    } // if
    else
    { // set if temp. >= upper-limit
      MainForm->triac_too_hot = (MainForm->ttriac >= MainForm->ttriac_hlim);
    } // else
} // task_read_lm35()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 06: Run PID Controller and generate Gamma value [0%..100%]
  Period-Time: Controlled by Parameter TS [sec.]
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_pid_ctrl(void)
{
    char      s[MAX_BUF_READ];
    TDateTime td_now; // holds current date and time

    // Only useful if PID controller is disabled AND time_switch is enabled
    if ((MainForm->PID_RB->ItemIndex != 1) && (MainForm->time_switch == 1))
    {
       td_now = Now(); // Get Current Date and Time
       if ((td_now >= MainForm->dt_time_switch) &&
           (td_now <  MainForm->dt_time_switch + ONE_MINUTE))
       {
          MainForm->PID_RB->ItemIndex = 1; // Enable PID Controller
       } // if
    } // if

    // PID_RB->ItemIndex = 1 => PID Controller On
    switch (MainForm->pid_pars.pid_model)
    {
         case 0 : pid_reg2(MainForm->thlt             , &MainForm->gamma,
                           MainForm->tset_hlt         , &MainForm->pid_pars,
                           MainForm->PID_RB->ItemIndex, &MainForm->sys_id_pars);
                  break; // Self-Tuning Takahashi Type C
         case 1 : pid_reg3(MainForm->thlt     , &MainForm->gamma,
                           MainForm->tset_hlt , &MainForm->pid_pars,
                           MainForm->PID_RB->ItemIndex);
                  break; // Type A with filtering of D-action
         case 2 : pid_reg4(MainForm->thlt     , &MainForm->gamma,
                           MainForm->tset_hlt , &MainForm->pid_pars,
                           MainForm->PID_RB->ItemIndex);
                  break; // Takahashi Type C, NO filtering of D-action
         default: pid_reg4(MainForm->thlt     , &MainForm->gamma,
                           MainForm->tset_hlt , &MainForm->pid_pars,
                           MainForm->PID_RB->ItemIndex);
                  break; // default to Type C, NO filtering of D-action
    } // switch
    if (MainForm->swfx.gamma_sw)
    {
       MainForm->gamma = MainForm->swfx.gamma_fx; // fix gamma
    } // if

    //--------------------------------------------------------------------
    // Now write PID-output (Gamma) as a PWM signal to the Ebrew hardware.
    // This is relevant only when the Modulating Gas-Burner is selected.
    //--------------------------------------------------------------------
    sprintf(s,"W%0d\n", MainForm->gamma); // PID-Output Gamma [0%..100%]
    MainForm->COM_port_write(s); // output to Ebrew hardware
    MainForm->COM_port_read(s);  // read response from Ebrew hardware
} // task_pid_ctrl()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 07: Run Main State Transition Diagram (STD) that controls
                        the entire application / Brewing States
  Period-Time: 1 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_update_std(void)
{
    char   s[MAX_BUF_READ];
    int    std_tmp = -1;
    double old_tset_hlt;  // previous value of tset_hlt

    if (MainForm->swfx.std_sw)
    {
       std_tmp = MainForm->swfx.std_fx;
    }
    old_tset_hlt = MainForm->tset_hlt; // Previous value of HLT temp. reference
    update_std(&MainForm->volumes ,  MainForm->tmlt    ,  MainForm->thlt,
               &MainForm->tset_mlt, &MainForm->tset_hlt, &MainForm->std_out,
                MainForm->ms      , &MainForm->sp      , &MainForm->std,
                MainForm->PID_RB->ItemIndex, std_tmp);
    if (MainForm->swfx.tset_hlt_sw)
    {
       MainForm->tset_hlt = MainForm->swfx.tset_hlt_fx; // fix tset_hlt
    } // if
    slope_limiter(MainForm->tset_hlt_slope, old_tset_hlt, &MainForm->tset_hlt);
    //-----------------------------------------------------------------
    // Now output all valve bits to Ebrew hardware (NOT implemented yet).
    // NOTE: The pump bit is sent using the P0/P1 command
    //-----------------------------------------------------------------
    //sprintf(s,"V%02x\n",std_out & 0x00FE); // Output valves except Pump (bit 0)
    //MainForm->COM_port_write(s); // output to Ebrew hardware
} // task_update_std()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 08: ALIVE Led toggle
  Period-Time: 0.5 second
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_alive_led(void)
{
    char   s[MAX_BUF_READ];

   //------------------------------------------------------------------
   // Toggle alive toggle bit to see if this routine is still alive
   //------------------------------------------------------------------
   if (MainForm->toggle_led)
        strcpy(s,"L1\n");
   else strcpy(s,"L0\n");
   MainForm->toggle_led = !(MainForm->toggle_led);

   //--------------------------------------------
   // Send Pump On/Off signal to ebrew hardware.
   //--------------------------------------------
   if (MainForm->std_out & P0b)
   {    // New PUMP bit should be 1
        strcat(s,"P1\n");
   } // if
   else
   {    // New PUMP bit should be 0
        strcat(s,"P0\n");
   } // else
   MainForm->COM_port_write(s); // Send command to ebrew hardware
   MainForm->COM_port_read(s);
} // task_alive_led()

/*-----------------------------------------------------------------------------
  Purpose    : TASK 09: Write all relevant data to a log-file
  Period-Time: 5 seconds
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_log_file(void)
{
   FILE   *fd;
   struct time t1;
   char   s[MAX_BUF_READ];

   if ((fd = fopen(LOGFILE,"a")) != NULL)
   {
      gettime(&t1);
      fprintf(fd,"%02d:%02d:%02d,",t1.ti_hour,t1.ti_min,t1.ti_sec);
      fprintf(fd,"%6.2f,%6.2f,%6.2f,%6.2f,%5.1f,%6.1f,%2d,%2d,%3d, %5.1f,%6.1f\n",
                 MainForm->tset_mlt,
                 MainForm->tset_hlt,
                 MainForm->thlt,
                 MainForm->tmlt,
                 MainForm->ttriac,
                 MainForm->volumes.Vmlt,
                 MainForm->std.sp_idx,
                 MainForm->std.ms_idx,
                 MainForm->std.ebrew_std,
                 MainForm->gamma,
                 MainForm->volumes.Vhlt);
      fclose(fd);
   } // if
   list_all_tasks();
} // task_log_file()

/*-----------------------------------------------------------------------------
  Purpose    : Open Virtual COM Port (Virtual: COM Port emulated via USB)
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::COM_port_open(void)
{
   DCB  dcbCommPort;
   char s[50];

   // OPEN THE COM PORT.
   sprintf(s,"COM%d",usb_com_port_nr); // USB COM Port Number
   hComm = CreateFile(s,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

   // IF THE PORT CANNOT BE OPENED, BAIL OUT.
   if(hComm == INVALID_HANDLE_VALUE)
   {
       strcat(s," could not be opened. Please check COM Port Settings.");
       MessageBox(NULL,s,"ERROR Opening Virtual USB COM Port",MB_OK);
       com_port_is_open = false;
   } // if
   else
   {  // Set the Communication Timeouts
     GetCommTimeouts(hComm,&ctmoOld);
     ctmoNew.ReadTotalTimeoutConstant    = 100;
     ctmoNew.ReadTotalTimeoutMultiplier  =   0;
     ctmoNew.WriteTotalTimeoutMultiplier =   0;
     ctmoNew.WriteTotalTimeoutConstant   =   0;
     SetCommTimeouts(hComm, &ctmoNew);

     // Set Baud-rate, Parity, wordsize and stop-bits.
     // There are other ways of doing these setting, but this is the easiest way.
     // If you want to add code for other baud-rates, remember that the argument
     // for BuildCommDCB must be a pointer to a string. Also note that
     // BuildCommDCB() defaults to NO Handshaking.
     dcbCommPort.DCBlength = sizeof(DCB);
     GetCommState(hComm, &dcbCommPort);
     BuildCommDCB(com_port_settings, &dcbCommPort); // "19200,N,8,1"
     SetCommState(hComm, &dcbCommPort);
     if ((fdbg_com = fopen(COM_PORT_DEBUG_FNAME,"a")) == NULL)
     {  // Open COM-port debugging file
        MessageBox(NULL,"Could not open COM-port debug log-file","Error during Initialisation",MB_OK);
     } /* if */
     else fprintf(fdbg_com,"File opened\n");
     com_port_is_open = true;
   } // if
} // COM_port_open()

/*-----------------------------------------------------------------------------
  Purpose    : Close Virtual COM Port (Virtual: COM Port emulated via USB)
               that was opened by COM_port_open().
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::COM_port_close(void)
{
   PurgeComm(hComm, PURGE_RXABORT);
   SetCommTimeouts(hComm, &ctmoOld);
   CloseHandle(hComm); // close Virtual USB COM-port
   fprintf(fdbg_com,"File closed\n");
   fclose(fdbg_com);   // close COM-port debugging file
   com_port_is_open = false; // set flag to 'not open'
} // COM_port_close()

/*-----------------------------------------------------------------------------
  Purpose    : Read a string from the Virtual COM Port opened by COM_port_open()
  Variables  : s: contains the null-terminated string that is read
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::COM_port_read(char *s)
{
   DWORD i, dwBytesRead;

   if (com_port_is_open)
   {
     ReadFile(hComm, s, MAX_BUF_READ-1, &dwBytesRead, NULL);
     if(dwBytesRead)
     {
        s[dwBytesRead] = 0; // Null-Terminate the string
     } // if
     if (cb_debug_com_port)
     {
          for (i = 0; i < dwBytesRead; i++)
          {
             if ((s[i] == '\n') || (s[i] == '\r')) s[i] = '_';
          }
          fprintf(fdbg_com,"r[%s]\n",s);
     } // if
   } // if
} // COM_port_read()

/*-----------------------------------------------------------------------------
  Purpose    : Write a string to the Virtual COM Port opened by COM_port_open()
  Variables  : s: contains the null-terminated string to write to the COM port.
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::COM_port_write(const char *s)
{
   char send_buffer[MAX_BUF_WRITE]; // contains string to send to RS232 port
   int  bytes_to_send = 0;          // Number of bytes to send
   int  i, bytes_sent = 0;          // Number of bytes sent to COM port

   if (com_port_is_open)
   {
     strcpy(send_buffer,s);    // copy command to send into send_buffer
     bytes_to_send = strlen(send_buffer);
     bytes_sent    = 0;
     while (bytes_sent < bytes_to_send)
     {
        if (!TransmitCommChar(hComm, send_buffer[bytes_sent++]))
        {
           MessageBox(NULL,"TransmitCommChar() Error","COM_port_write()",MB_OK);
        } // if
     } // while()
     if (cb_debug_com_port)
     {
          for (i = 0; i < bytes_to_send; i++)
          {
             if (send_buffer[i] == '\n') send_buffer[i] = '_';
          }
          fprintf(fdbg_com,"w[%s]",send_buffer);
     } // if
   } // if
} // COM_port_write()

/*------------------------------------------------------------------
  Purpose  : This is the main constructor for the program.
  Returns  : None
  ------------------------------------------------------------------*/
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
   char s[50];
   ebrew_revision   = "$Revision$";
   //ShowDataGraphs   = new TShowDataGraphs(this);   // create modeless Dialog
   ViewMashProgress = new TViewMashProgress(this); // create modeless Dialog
   TRegistry *Reg   = new TRegistry();
   power_up_flag    = true;  // indicate that program power-up is active
   com_port_is_open = false; // indicate that COM-port is closed at power-up

   try
    {
       if (!Reg->KeyExists(REGKEY))
       {
          // No entry in Registry, create all keys
          Reg->CreateKey(REGKEY); // Create key if it does not exist yet
          Reg->OpenKey(REGKEY,FALSE);
          Reg->WriteInteger("USB_COM_PORT",3);   // Virtual USB COM port number
          Reg->WriteString("COM_PORT_SETTINGS","19200,N,8,1"); // COM port settings
          Reg->WriteInteger("KNOWN_HW_DEVICES",known_hw_devices);
          fscl_prescaler = 10;
          Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler); // set fscl to 20 kHz
          // PID Settings Dialog
          Reg->WriteFloat("TS",TS_INIT);         // Set Default sample time
          Reg->WriteFloat("Kc",KC_INIT);         // Controller gain
          Reg->WriteFloat("Ti",TI_INIT);         // Ti constant
          Reg->WriteFloat("Td",TD_INIT);         // Td constant
          Reg->WriteFloat("K_LPF",0);            // LPF filter time-constant
          Reg->WriteFloat("TSET_HLT_SLOPE",1.0); // Slope Limit for Tset_HLT
          Reg->WriteFloat("TOffset",1.0);        // HLT - MLT heat loss
          Reg->WriteFloat("TOffset2",-0.5);      // offset for early start of mash timers
          Reg->WriteInteger("PID_Model",2);      // Takahashi PID Controller
          sys_id_pars.N = 1; // order N for system identification
          Reg->WriteInteger("STC_N",sys_id_pars.N);   // [1,2,3]
          sys_id_pars.stc_td  = 0; // Time-Delay estimate for system identification
          Reg->WriteInteger("STC_TD",sys_id_pars.stc_td);
          sys_id_pars.stc_adf = 0; // true = use Adaptive directional forgetting
          Reg->WriteBool("STC_ADF",(sys_id_pars.stc_adf > 0));
          Reg->WriteInteger("BURNER_HHYST",35);  // Gas Burner Hysteresis High Limit
          Reg->WriteInteger("BURNER_LHYST",30);  // Gas Burner Hysteresis Low Limit
          // Send PID-Output to the various heaters / burners
          cb_pid_out = 0; // The PID-output is not connected to anything
          Reg->WriteInteger("CB_PID_OUT",cb_pid_out);

          ttriac_hlim = 70; // Upper limit for triac temp.
          Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
          ttriac_llim = 60; // Lower limit for triac temp.
          Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);
          cb_i2c_err_msg          = true;
          Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);
          cb_debug_com_port       = false;
          Reg->WriteBool("CB_DEBUG_COM_PORT",cb_debug_com_port);

          known_hw_devices = DIG_IO_LSB_OK | MAX1238_OK | LM92_1_OK | LM92_2_OK;

          // Init values for mash scheme variables
          Reg->WriteInteger("ms_idx",MAX_MS);   // init. index in mash scheme
          // Init values for Sparge Settings
          Reg->WriteInteger("SP_BATCHES",5);    // #Sparge Batches
          Reg->WriteInteger("SP_TIME",20);      // Time between sparge batches
          Reg->WriteInteger("MASH_VOL",30);     // Total Mash Volume (L)
          Reg->WriteInteger("SP_VOL",50);       // Total Sparge Volume (L)
          Reg->WriteInteger("BOIL_TIME",120);    // Total Boil Time (min.)
          // Init values for STD
          Reg->WriteInteger("PREHEAT_TIME",0);// PREHEAT_TIME [sec]
          Reg->WriteFloat("VMLT_EMPTY", 3.0); // Vmlt_EMPTY [L]
          Reg->WriteInteger("TO_XSEC",1);     // TIMEOUT_xSEC [sec]
          Reg->WriteInteger("TO3",300);       // TIMEOUT3 [sec]
          Reg->WriteInteger("TO4",20);        // TIMEOUT4 [sec
          // Measurements
          Reg->WriteFloat("THLT_OFFSET",0.0);  // Offset for Thlt
          Reg->WriteFloat("THLT_SLOPE",2.0);   // Slope limit for Thlt °C/sec.
          Reg->WriteFloat("TMLT_OFFSET",0.0);  // Offset for Tmlt
          Reg->WriteFloat("TMLT_SLOPE",2.0);   // Slope limit for Tmlt °C/sec.
          Reg->WriteFloat("VHLT_OFFSET",0.0);  // Offset for Vhlt
          Reg->WriteFloat("VHLT_MAX",140.1);   // Max. HLT volume
          Reg->WriteFloat("VHLT_SLOPE",1.0);   // Slope limit for Vhlt L/sec.
          Reg->WriteFloat("VMLT_OFFSET",0.0);  // Offset for Vmlt
          Reg->WriteFloat("VMLT_MAX",110.1);   // Max. MLT volume
          Reg->WriteFloat("VMLT_SLOPE",1.0);   // Slope limit for Vmlt L/sec.
          volumes.Vboil_simulated = true;
          Reg->WriteBool("VBOIL_SIMULATED",volumes.Vboil_simulated);
          cb_pid_dbg       = false; // no PID debug to screen
          PID_dbg->Visible = false;
       } // if
       Reg->CloseKey();
       delete Reg;
    } // try
    catch (ERegistryException &E)
    {
       ShowMessage(E.Message);
       delete Reg;
       return;
    } // catch
    //-------------------------------------
    // Set rev. number in Tstatusbar panel
    //-------------------------------------
    strcpy(s,"ebrew revision ");
    strncat(s,&ebrew_revision[11],4); // extract the CVS revision number
    s[19] = '\0';
    StatusBar->Panels->Items[PANEL_REVIS]->Text = AnsiString(s);

    Main_Initialisation(); // Init all I2C Hardware, ISR and PID controller
    //----------------------------------------
    // Init. volumes. Should be done only once
    //----------------------------------------
    if (volumes.Vboil_simulated)
    {  // Vboil is not measured but calculated from Vmlt
       volumes.Vboil  = VBOIL_START;
    }
    power_up_flag = false; // power-up is finished
} // TMainForm::TMainForm()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Main_Initialisation(void)
/*------------------------------------------------------------------
  Purpose  : This function Initialises all I2C Hardware and checks if
             it is present. It also initialises the Interrupt Service
             Routine (ISR) and the PID controller.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   FILE *fd;   // Log File Descriptor
   int  i;     // temp. variable
   char s[255]; // Temp. string

   //----------------------------------------
   // Initialise all variables from Registry
   //----------------------------------------
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         known_hw_devices = Reg->ReadInteger("KNOWN_HW_DEVICES");
         pid_pars.ts = Reg->ReadFloat("TS");  // Read TS from registry
         pid_pars.kc = Reg->ReadFloat("Kc");  // Read Kc from registry
         pid_pars.ti = Reg->ReadFloat("Ti");  // Read Ti from registry
         pid_pars.td = Reg->ReadFloat("Td");  // Read Td from registry
         pid_pars.k_lpf      = Reg->ReadFloat("K_LPF");
         tset_hlt_slope      = Reg->ReadFloat("TSET_HLT_SLOPE");
         pid_pars.pid_model  = Reg->ReadInteger("PID_Model"); // [0..3]
         sys_id_pars.N       = Reg->ReadInteger("STC_N");     // [1,2,3]
         sys_id_pars.stc_td  = Reg->ReadInteger("STC_TD");    // [0..100]
         sys_id_pars.stc_adf = Reg->ReadBool("STC_ADF");      // true = use ADF
         pid_pars.burner_hyst_h = Reg->ReadInteger("BURNER_HHYST");
         pid_pars.burner_hyst_l = Reg->ReadInteger("BURNER_LHYST");
         // Send PID-Output to the various heaters / burners
         cb_pid_out = Reg->ReadInteger("CB_PID_OUT");

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM"); // Read high limit
         tm_triac->SetPoint->Value = ttriac_hlim;       // update object on screen
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM"); // Read low limit

         volumes.Vboil_simulated = Reg->ReadBool("VBOIL_SIMULATED");
         cb_i2c_err_msg          = Reg->ReadBool("CB_I2C_ERR_MSG");    // display message
         cb_debug_com_port       = Reg->ReadBool("CB_DEBUG_COM_PORT"); // display message
         usb_com_port_nr         = Reg->ReadInteger("USB_COM_PORT");   // Virtual USB COM port number
         strcpy(com_port_settings,Reg->ReadString("COM_PORT_SETTINGS").c_str()); // COM port settings

         thlt_offset = Reg->ReadFloat("THLT_OFFSET");         // offset calibration
         thlt_slope  = Reg->ReadFloat("THLT_SLOPE");          // Slope limiter for Thlt
         tmlt_offset = Reg->ReadFloat("TMLT_OFFSET");         // offset calibration
         tmlt_slope  = Reg->ReadFloat("TMLT_SLOPE");          // Slope limiter for Tmlt

         vhlt_max    = Reg->ReadFloat("VHLT_MAX");    // Read max. HLT volume
         vhlt_offset = Reg->ReadFloat("VHLT_OFFSET"); // Read Vmlt Offset
         vhlt_slope  = Reg->ReadFloat("VHLT_SLOPE");  // Slope limiter for Vhlt
         vmlt_max    = Reg->ReadFloat("VMLT_MAX");    // Read max. MLT volume
         vmlt_offset = Reg->ReadFloat("VMLT_OFFSET"); // Read Vmlt Offset
         vmlt_slope  = Reg->ReadFloat("VMLT_SLOPE");  // Slope limiter for Vmlt

         Reg->SaveKey(REGKEY,"ebrew_reg");
         Reg->CloseKey();      // Close the Registry
         switch (pid_pars.pid_model)
         {
            case 0 : // Self-Tuning Takahashi, N = 1 .. 3
                     init_pid2(&pid_pars,&sys_id_pars);
                     break;
            case 1 : init_pid3(&pid_pars);  // Type A with D-filtering controller
                     break;
            case 2 : init_pid4(&pid_pars);  // Takahashi Type C controller
                     break;
            default: pid_pars.pid_model = 2; // Takahashi Type C controller
                     init_pid4(&pid_pars);
                     break;
         } // switch
         // Do NOT delete Reg yet, since we need it further on
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch

   //----------------------------------------------------------------------
   // Start Communication with ebrew Hardware and print a list of all
   // devices found if it does not match known devices.
   //----------------------------------------------------------------------
   COM_port_open(); // Open USB Virtual COM port

   //--------------------------------------------------------------------------
   // Read Mash Scheme for maisch.sch file
   // Mash scheme is used in the STD, sample time of STD is 1 second
   // Init mash. timers only when doing a power-up, not after an I2C reset
   //--------------------------------------------------------------------------
   if (!read_input_file(MASH_FILE,ms,&(std.ms_tot),1.0,power_up_flag ? INIT_TIMERS : NO_INIT_TIMERS))
   {
       MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
   } /* if */
   print_mash_scheme_to_statusbar();
   Init_Sparge_Settings(); // Initialise the Sparge Settings struct (STD needs it)
   if (power_up_flag)
   {  // only reset time-stamps when doing a power-up, not after an I2C reset
      for (i = 0; i < sp.sp_batches; i++)
      {
         sp.mlt2boil[i][0] = '\0'; // empty time-stamp strings
         sp.hlt2mlt[i][0]  = '\0';
      } // for i
   } // if

   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         i = Reg->ReadInteger("ms_idx");
         if ((i < MAX_MS) && (power_up_flag == true))
         {
            //--------------------------------------------------------------
            // ebrew program was terminated abnormally. Open a dialog box,
            // present entries from the log file and ask to restore settings
            // of a particular log-file entry. If yes, restore settings.
            // Only restore after power-down, no restore after successful
            // of I2C bus (after I2C bus error occurred).
            // NB: Make sure that Init_Sparge_Settings is called prior to
            //     calling Restore_Settings()!
            //--------------------------------------------------------------
            Restore_Settings();
         } // if i
         Reg->WriteInteger("ms_idx",std.ms_idx); // update registry setting
         Reg->CloseKey();                        // Close the Registry
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch

   //-------------
   // Init. timers
   //-------------
   tmr.isrstate  = IDLE;              // disable heaters
   tmr.htimer    = tmr.ltimer   = 0;  // init. low timer
   tmr.alive     = tmr.alive_tmr = 0; // init. alive timers
   tmr.pid_tmr   = 1; // init. timer that controls PID controller timing

   //--------------------------------------------------------------------------
   // Init. the position of all valves & the Pump to OFF (closed).
   // No Manual Override of valves yet.
   //--------------------------------------------------------------------------
   std_out = 0x0000;
   lsb_io  = 0x00; // disable ALIVE-led and PUMP

   // Init logfile
   if ((fd = fopen(LOGFILE,"a")) == NULL)
   {
      MessageBox(NULL,"Could not open log-file","Error during Initialisation",MB_OK);
   } /* if */
   else
   {
      date d1;
      getdate(&d1);
      fprintf(fd,"\nDate of brewing: %02d-%02d-%4d\n",d1.da_day,d1.da_mon,d1.da_year);
      fprintf(fd,"Kc = %6.2f, Ti = %6.2f, Td = %6.2f, K_lpf = %6.2f, Ts = %5.2f, ",
                 pid_pars.kc, pid_pars.ti, pid_pars.td, pid_pars.k_lpf, pid_pars.ts);
      fprintf(fd,"PID_Model =%2d\n",pid_pars.pid_model);
      strncpy(s,&ebrew_revision[11],4); // extract the CVS revision number
      s[4] = '\0';
      fprintf(fd,"ebrew CVS Rev. %s\n\n",s);
      fprintf(fd,"ms_tot =%2d, fscl_prescaler =%2d\n", std.ms_tot, fscl_prescaler);
      fprintf(fd,"Temp Offset = %4.1f, Temp Offset2 = %4.1f\n",sp.temp_offset,sp.temp_offset2);
      fprintf(fd,"Vhlt_max=%7.4f, Vhlt_offset=%7.4f, Vmlt_max=%7.4f, Vmlt_offset=%7.4f\n\n",
                 vhlt_max, vhlt_offset, vmlt_max, vmlt_offset);
      fprintf(fd," Time   TsetMLT TsetHLT  Thlt   Tmlt  TTriac  Vmlt sp ms STD  Gamma  Vhlt\n");
      fprintf(fd,"[h:m:s]    [°C]   [°C]   [°C]   [°C]   [°C]   [L]  id id       [%]    [L]\n");
      fprintf(fd,"-------------------------------------------------------------------------\n");
      fclose(fd);
   } // else

   //-----------------------------------------
   // Now add all the tasks for the scheduler
   //-----------------------------------------
   add_task(task_read_thlt , "read_thlt"  ,   0, 1000);
   add_task(task_read_tmlt , "read_tmlt"  , 100, 1000);
   add_task(task_read_vhlt , "read_vhlt"  , 200, 1000);
   add_task(task_read_vmlt , "read_vmlt"  , 300, 1000);
   add_task(task_read_lm35 , "read_lm35"  , 400, 1000);
   add_task(task_pid_ctrl  , "pid_control", 500, (uint16_t)(pid_pars.ts * 1000));
   add_task(task_update_std, "update_std" , 600, 1000);
   add_task(task_alive_led , "alive_pump" , 650,  500);
   add_task(task_log_file  , "wr_log_file", 800, 5000);

   COM_port_read(s); // Read power-up notice from Ebrew hardware
   ctmoNew.ReadTotalTimeoutConstant = 20; // Now change Read time-out to 20 msec.
   SetCommTimeouts(hComm, &ctmoNew);

   //----------------------------------
   // We came all the way! Start Timers
   //----------------------------------
   if (T50msec)          T50msec->Enabled                       = true; // start Interrupt Timer
   if (ViewMashProgress) ViewMashProgress->UpdateTimer->Enabled = true; // Start Mash Progress Update timer
   PID_RB->Enabled = true; // Enable PID Controller Radio-buttons
   time_switch     = 0;    // Time switch disabled at power-up
   delete Reg; // Delete Registry object to prevent memory leak
} // Main_Initialisation()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This is the main Timer function which is called every
             50 milliseconds. This can be seen as the main control
             loop of the program. It utilises time-slices to divide
             computations over time.
             After 60 seconds (1200 calls) the main control loop
             starts again
  Returns  : None
  ------------------------------------------------------------------*/
void __fastcall TMainForm::T50msecTimer(TObject *Sender)
{
   scheduler_isr(); // Scheduler for all tasks
} // TMainForm::T50msecTimer()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Restore_Settings(void)
/*------------------------------------------------------------------
  Purpose  : This function reads all relevant info from the log-file,
             opens a Dialog screen where the user can select the
             appropriate log-file entry.
             The data from this log-file entry is then used to
             restore the ebrew program to that situation.
  Returns  : None
  ------------------------------------------------------------------*/
{
   FILE       *fd;       // Log File Descriptor
   log_struct p1[100];   // Array of structs with log info to fill
   char       s[80];     // temp. string
   int        cnt_i = 1; // #items in Combobox
   int        j;         // temp. variable
   int        k;         // temp. variable
   int        x;         // temp. variable

   TRestore_Program_Settings *prps;  // pointer to Form
   TOpenDialog *Dlg;                 // pointer to File-Dialog

   Dlg = new TOpenDialog(this);
   Dlg->Filter = "ebrew log files|ebrew*.txt";
   Dlg->Title  = "Open an ebrew log file";
   if (Dlg->Execute() == false)
   {
      delete Dlg;
      return; // exit Restore_Settings function
   } // if

   if ((fd = fopen(Dlg->FileName.c_str(),"r")) == NULL)
   {
      MessageBox(NULL,"Could not open log file for reading","Init. error when trying to restore data",MB_OK);
   } /* if */
   else
   {
      //--------------------------------------------------------
      // Read all information from the log-file and try to
      // reconstruct the previous situation as good as possible
      //--------------------------------------------------------
      j = decode_log_file(fd,p1); // read log file with data just before crash
      fclose(fd);                 // close log-file
      prps = new TRestore_Program_Settings(this);
      prps->ComboBox1->Items->Add("Reset to default values (ms_idx=0, sp_idx=0, ebrew_std=0)");
      for (k = 0; k < j; k++)
      {
         if ((p1[k].lms_idx > 0) || (p1[k].std_val > 0))
         {
            cnt_i++; // increment #items in Combobox
            sprintf(s,"%02d %s %s-%s ms_idx=%2d sp_idx=%2d std=%2d",k,
                                                         p1[k].brew_date,
                                                         p1[k].btime,
                                                         p1[k].etime,
                                                         p1[k].lms_idx,
                                                         p1[k].lsp_idx,
                                                         p1[k].std_val);
            prps->ComboBox1->Items->Add(s);
         } // if
      } // for k
      prps->ComboBox1->DropDownCount = cnt_i;
      prps->ComboBox1->ItemIndex     = 0;
      if ((prps->ShowModal() == 0x1) && (prps->ComboBox1->ItemIndex > 0)) // mrOK
      {
         //-----------------------------------------------------------------
         // Initialise the various values with the values from the log-file
         //-----------------------------------------------------------------
         strncpy(s,prps->ComboBox1->Items->Strings[prps->ComboBox1->ItemIndex].c_str(),2);
         s[2]             = '\0';    // terminate string
         k                = atoi(s); // convert to integer
         //---------------------------
         // Restore Mashing parameters
         //---------------------------
         std.ms_idx       = p1[k].lms_idx;
         for (j = 0; j < MAX_MS; j++)
         {
            if (p1[k].mashing_start[j] > p1[k].bline)
            {
               ms[j].timer      = ms[j].time; // set previous timers to time-out
               strcpy(ms[j].time_stamp, p1[k].btime);
               add_seconds(ms[j].time_stamp, (int)(p1[k].time_period*(p1[k].mashing_start[j] - p1[k].bline)));
            } // if
            else
            {  // mash_time not started yet
               ms[j].timer         = NOT_STARTED;
               ms[j].time_stamp[0] = '\0';
            } // else
         } // for j
         if (!p1[k].sparging_start[0])
         {  //-------------------------------------------------------------------
            // mash was in progress, Two options: (computed by decode_log_file())
            // 1) tmr_ms_idx = NOT_STARTED: Ref. temp. was not reached yet
            // 2) tmr_ms_idx has an actual timer value
            //-------------------------------------------------------------------
            ms[std.ms_idx].timer = p1[k].tmr_ms_idx;
         } // if

         //----------------------------
         // Restore Sparging parameters
         //----------------------------
         std.ebrew_std = p1[k].std_val; // Current state
         std.sp_idx    = p1[k].lsp_idx; // Sparging sessions done
         sp.sp_batches = max(sp.sp_batches,std.sp_idx);
         
         // Restore MLT -> Boil en HLT -> MLT time-stamps
         for (j = 0; j <= MAX_SP-1; j++)
         {
            x = p1[k].sparging_start[j];
            if (x > 0)
            {
               strcpy(MainForm->sp.mlt2boil[j],p1[k].btime);
               add_seconds(MainForm->sp.mlt2boil[j], (int)(p1[k].time_period * (x - p1[k].bline)));
            } // if
            else MainForm->sp.mlt2boil[j][0] = '\0';
            x = p1[k].sparging_start2[j];
            if (x > 0)
            {
               strcpy(MainForm->sp.hlt2mlt[j+1],p1[k].btime);
               add_seconds(MainForm->sp.hlt2mlt[j+1], (int)(p1[k].time_period * (x - p1[k].bline)));
            } // if
            else MainForm->sp.hlt2mlt[j+1][0] = '\0';
         } // for j
         // Restore other timing parameters
         x             = p1[k].eline + 1 - p1[k].start_lstd;
         x            *= p1[k].time_period; // Log-file -> 5 sec.: STD called -> 1 sec.
         switch (std.ebrew_std) // init. timers for states that have timers
         {
            case S05_SPARGING_REST:        if (std.sp_idx > 0)
                                           {
                                              std.timer1 = x;
                                           }
                                           else
                                           {  // if crash occurs in state 5 when sp_idx==0
                                              std.timer1 = sp.sp_time_ticks;
                                           }
                                           break;
            case S08_DELAY_xSEC:           std.timer2 = 1;
                                           break;
            case S10_BOILING:              std.timer3 = x;
                                           std.timer4 = 0;
                                           std.timer5 = x;
                                           break;
            case S11_EMPTY_HEAT_EXCHANGER: std.timer4 = x;
                                           std.timer5 = x + sp.to3;
                                           break;
            default:                       break;
         } // switch
      } // if mrOK
      delete prps;  // prevent memory leaks
   } // else
   delete Dlg; // prevent memory leaks
} // TMainForm::Restore_Settings()
//---------------------------------------------------------------------------

void __fastcall TMainForm::print_mash_scheme_to_statusbar(void)
/*------------------------------------------------------------------
  Purpose  : This function prints the contents of the mash scheme
             to the correct panel of the StatusBar object.
  Returns  : None
  ------------------------------------------------------------------*/
{
   char s[255] = "";
   char s1[50] = "";
   int  i;

   for (i = 0; i < std.ms_tot ; i++)
   {
      sprintf(s1,"%1.0f°(%1.0f)",ms[i].temp,ms[i].time/60);
      if (i == std.ms_tot - 1)
      {
         strcat(s,s1);
      }
      else
      {
         strcat(s1,", ");
         strcat(s,s1);
      } // else
   } // for
   StatusBar->Panels->Items[PANEL_MASHS]->Text = AnsiString(s);
} // TMainForm::print_mash_scheme_to_statusbar()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuOptionsPIDSettingsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | PID Settings'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   TPID_Settings *ptmp;
   char tmp[255]; // temp. array

   ptmp = new TPID_Settings(this);

   // Get PID Controller setting from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         // Read parameters from registry
         ptmp->TS_edit->Text        = AnsiString(Reg->ReadFloat("TS"));
         ptmp->Kc_Edit->Text        = AnsiString(Reg->ReadFloat("Kc"));
         ptmp->Ti_Edit->Text        = AnsiString(Reg->ReadFloat("Ti"));
         ptmp->Td_Edit->Text        = AnsiString(Reg->ReadFloat("Td"));
         ptmp->K_LPF_Edit->Text     = AnsiString(Reg->ReadFloat("K_LPF"));
         ptmp->STC_N_Edit->Text     = AnsiString(Reg->ReadInteger("STC_N"));
         ptmp->STC_TD_Edit->Text    = AnsiString(Reg->ReadInteger("STC_TD"));
         ptmp->CB_adf->Checked      = Reg->ReadBool("STC_ADF");
         ptmp->Tset_hlt_slope->Text = AnsiString(Reg->ReadFloat("TSET_HLT_SLOPE"));
         ptmp->PID_Model->ItemIndex = Reg->ReadInteger("PID_Model");
         ptmp->Burner_On->Text      = AnsiString(Reg->ReadInteger("BURNER_HHYST"));
         ptmp->Burner_Off->Text     = AnsiString(Reg->ReadInteger("BURNER_LHYST"));
         ptmp->RG2->ItemIndex       = time_switch; // Value of time-switch [off, on]
         ptmp->CB_PID_dbg->Checked  = cb_pid_dbg;   // PID debug info
         // Send PID-Output to the various heaters / burners
         cb_pid_out = Reg->ReadInteger("CB_PID_OUT");
         ptmp->CB_Pid_out0->Checked = (cb_pid_out & PID_OUT_ELECTRIC);
         ptmp->CB_Pid_out1->Checked = (cb_pid_out & PID_OUT_GAS_NON_MOD);
         ptmp->CB_Pid_out2->Checked = (cb_pid_out & PID_OUT_GAS_MODULATE);
         ptmp->CB_Pid_out1Click(this); // Enable/disable labels/Edit fields in form
         ptmp->CB_Pid_out2Click(this); // Enable/disable labels/Edit fields in form

         if (time_switch)
         {
            ptmp->Date_Edit->Text = DateToStr(dt_time_switch);
            AnsiString s = TimeToStr(dt_time_switch);
            s.SetLength(s.Length()-3); // remove seconds
            ptmp->Time_Edit->Text = s;
         } // if

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            pid_pars.ts = ptmp->TS_edit->Text.ToDouble();
            Reg->WriteFloat("TS",pid_pars.ts);
            pid_pars.kc = ptmp->Kc_Edit->Text.ToDouble();
            Reg->WriteFloat("Kc",pid_pars.kc);
            pid_pars.ti = ptmp->Ti_Edit->Text.ToDouble();
            Reg->WriteFloat("Ti",pid_pars.ti);
            pid_pars.td = ptmp->Td_Edit->Text.ToDouble();
            Reg->WriteFloat("Td",pid_pars.td);
            pid_pars.k_lpf = ptmp->K_LPF_Edit->Text.ToDouble();
            Reg->WriteFloat("K_LPF",pid_pars.k_lpf);
            sys_id_pars.N  = ptmp->STC_N_Edit->Text.ToInt();
            if ((sys_id_pars.N < 1) || (sys_id_pars.N > 3)) sys_id_pars.N = 1;
            Reg->WriteInteger("STC_N",sys_id_pars.N);
            sys_id_pars.stc_td = ptmp->STC_TD_Edit->Text.ToInt();
            if ((sys_id_pars.stc_td < 0) || (sys_id_pars.stc_td > MAX_MA)) sys_id_pars.stc_td = 0;
            Reg->WriteInteger("STC_TD",sys_id_pars.stc_td);
            if (ptmp->CB_adf->Checked)
                 sys_id_pars.stc_adf = 1; // use adaptive directional forgetting
            else sys_id_pars.stc_adf = 0; // NO ADF
            Reg->WriteBool("STC_ADF",(sys_id_pars.stc_adf > 0));
            tset_hlt_slope = ptmp->Tset_hlt_slope->Text.ToDouble();
            Reg->WriteFloat("TSET_HLT_SLOPE",tset_hlt_slope);
            pid_pars.pid_model = ptmp->PID_Model->ItemIndex;
            pid_pars.burner_hyst_h = ptmp->Burner_On->Text.ToInt();
            Reg->WriteInteger("BURNER_HHYST",pid_pars.burner_hyst_h);
            pid_pars.burner_hyst_l = ptmp->Burner_Off->Text.ToInt();
            Reg->WriteInteger("BURNER_LHYST",pid_pars.burner_hyst_l);
            switch (pid_pars.pid_model)
            {
               case 0 : // Self-Tuning Takahashi, N = 1 .. 3
                        init_pid2(&pid_pars,&sys_id_pars);
                        break;
               case 1 : init_pid3(&pid_pars);   // Type A with D-filtering controller
                        break;
               case 2 : init_pid4(&pid_pars);   // Takahashi Type C controller
                        break;
               default: pid_pars.pid_model = 2; // Takahashi Type C controller
                        init_pid4(&pid_pars);
                        break;
            } // switch
            Reg->WriteInteger("PID_Model",pid_pars.pid_model);
            cb_pid_dbg  = ptmp->CB_PID_dbg->Checked; // PID debug info
            PID_dbg->Visible = cb_pid_dbg;

            cb_pid_out = 0x00; // value in Registry
            if (ptmp->CB_Pid_out0->Checked)
            {  // Electrical Heating Element
               cb_pid_out |= PID_OUT_ELECTRIC;
            } // if
            if (ptmp->CB_Pid_out1->Checked)
            {  // Non-modulating Gas-Burner
               cb_pid_out |= PID_OUT_GAS_NON_MOD;
            } // if
            if (ptmp->CB_Pid_out2->Checked)
            {  // Modulating Gas-Burner
               cb_pid_out |= PID_OUT_GAS_MODULATE;
            } // if
            Reg->WriteInteger("CB_PID_OUT",cb_pid_out);

            time_switch = ptmp->RG2->ItemIndex; // 0 = off, 1 = on
            if (time_switch)
            {
               strcpy(tmp,ptmp->Date_Edit->Text.c_str());
               strcat(tmp," ");
               strcat(tmp,ptmp->Time_Edit->Text.c_str());
               dt_time_switch = StrToDateTime(tmp);
            } // if
         } // if
         delete ptmp;
         ptmp = 0; // NULL the pointer
         Reg->CloseKey(); // Close the Registry
         delete Reg;
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
      delete Reg;
      return;
   } // catch
} // TMainForm::MenuOptionsPIDSettingsClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuOptionsI2CSettingsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Hardware Settings'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   int x1;  // temp. variable representing new USB COM Port number
   char *endp; // temp. pointer for strtol() function
   char s[80]; // temp. array
   int  init_needed = false; // temp. flag, TRUE = Main_Initialisation to be called

   TRegistry     *Reg = new TRegistry();
   TI2C_Settings *ptmp;
   AnsiString    S;

   ptmp = new TI2C_Settings(this);

   // Get I2C Bus Settings from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);

         usb_com_port_nr = Reg->ReadInteger("USB_COM_PORT"); // Read virtual COM port nr.
         ptmp->UpDown1->Position = usb_com_port_nr;

         S = Reg->ReadString("COM_PORT_SETTINGS");
         ptmp->COM_Port_Settings_Edit->Text = S;

         known_hw_devices            = Reg->ReadInteger("KNOWN_HW_DEVICES");
         ptmp->Hw_devices_Edit->Text = IntToHex(known_hw_devices,3);

         fscl_prescaler              = Reg->ReadInteger("FSCL_PRESCALER");
         ptmp->fscl_combo->ItemIndex = fscl_prescaler;

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM");
         ptmp->Thlim_edit->Text  = AnsiString(ttriac_hlim);
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM");
         ptmp->Tllim_edit->Text  = AnsiString(ttriac_llim);
         cb_i2c_err_msg = Reg->ReadBool("CB_I2C_ERR_MSG");
         ptmp->cb_i2c_err_msg->Checked = cb_i2c_err_msg;
         cb_debug_com_port = Reg->ReadBool("CB_DEBUG_COM_PORT");
         ptmp->cb_debug_com_port->Checked = cb_debug_com_port;

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            x1 = ptmp->COM_Port_Edit->Text.ToInt();  // Retrieve COM Port Nr.
            if (x1 != usb_com_port_nr)
            {
               init_needed     = true;
               usb_com_port_nr = x1;
               Reg->WriteInteger("USB_COM_PORT",x1); // Save new COM port nr.
            } // if

            strcpy(com_port_settings, ptmp->COM_Port_Settings_Edit->Text.c_str());
            if (strcmp(com_port_settings,S.c_str())) // COM Port Settings
            {
               init_needed = true;
               Reg->WriteString("COM_PORT_SETTINGS",AnsiString(com_port_settings));
            } // if

            strcpy(s,ptmp->Hw_devices_Edit->Text.c_str()); // retrieve hex value
            known_hw_devices = (int)(strtol(s,&endp,16));  // convert to integer
            Reg->WriteInteger("KNOWN_HW_DEVICES",known_hw_devices);

            if (fscl_prescaler != ptmp->fscl_combo->ItemIndex)
            {
               fscl_prescaler = ptmp->fscl_combo->ItemIndex;
               Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler);
            } // if

            ttriac_hlim = ptmp->Thlim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
            tm_triac->SetPoint->Value = ttriac_hlim;
            ttriac_llim = ptmp->Tllim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);

            cb_i2c_err_msg = ptmp->cb_i2c_err_msg->Checked;
            Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);

            cb_debug_com_port = ptmp->cb_debug_com_port->Checked;
            Reg->WriteBool("CB_DEBUG_COM_PORT",cb_debug_com_port);

            if (init_needed)
            {  //--------------------------------------------------------
               // COM-port settings were changed. Close COM-port if
               // still open and re-open with new settings.
               //--------------------------------------------------------
               if (com_port_is_open)
               {
                  COM_port_close();
               } // if
               COM_port_open(); // Open COM-port with new settings
            } // if
         } // if
         Reg->CloseKey(); // Close the Registry
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch
   // Clean up
   delete Reg;
   delete ptmp;
   ptmp = 0; // NULL the pointer
} // TMainForm::MenuOptionsI2CSettingsClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuFileExitClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'File | Exit'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   exit_ebrew();
} // TMainForm::MenuFileExitClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::exit_ebrew(void)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'File | Exit'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();

   if (T50msec) T50msec->Enabled = false; // Disable Interrupt Timer
   if (ViewMashProgress)
   {
      ViewMashProgress->UpdateTimer->Enabled = false; // Stop Mash Progress Update timer
   }
   Sleep(51);                // Make sure that Timer is disabled
   if (ViewMashProgress)
   {
      delete ViewMashProgress;  // close modeless dialog
      ViewMashProgress = 0;     // null the pointer
   }
   if (com_port_is_open)
   {    // Disable Gas-Burner PWM, Heater and Alive LEDs
        COM_port_write("l0\nw0\np0\n");
        COM_port_close();
   } // if

   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         Reg->WriteInteger("ms_idx",MAX_MS); // terminated normally
      } // if
      Reg->CloseKey(); // Close the Registry
      delete Reg;
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
      delete Reg;
      return;
   } // catch
   Application->Terminate(); // exit program
} //  TMainForm::exit_ebrew()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuEditFixParametersClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Edit | Fix Parameters'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TFix_Params *ptmp;

   ptmp = new TFix_Params(this);

   // 1. Get Switch/Fix settings from struct
   // 2. Initialise Dialog values and execute Dialog
   // 3. Write values to struct
   //-------------------------------------------------------------------------
   ptmp->CB_Tset->Checked = swfx.tset_hlt_sw; // Set Checkbox for Tset_hlt
   if (swfx.tset_hlt_sw)
   {
      ptmp->Tset_MEdit->Text = AnsiString(swfx.tset_hlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_Gamma->Checked = swfx.gamma_sw; // Set Checkbox for Gamma
   if (swfx.gamma_sw)
   {
      ptmp->Gamma_MEdit->Text = AnsiString(swfx.gamma_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_Tad1->Checked = swfx.thlt_sw; // Set Checkbox for Thlt
   if (swfx.thlt_sw)
   {
      ptmp->Tad1_MEdit->Text = AnsiString(swfx.thlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_Tad2->Checked = swfx.tmlt_sw; // Set Checkbox for Tmlt
   if (swfx.tmlt_sw)
   {
      ptmp->Tad2_MEdit->Text = AnsiString(swfx.tmlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_std->Checked = swfx.std_sw; // Set Checkbox for ebrew_std
   if (swfx.std_sw)
   {
      ptmp->STD_MEdit->Text = AnsiString(swfx.std_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_ttriac->Checked = swfx.ttriac_sw; // Set Checkbox for Ttriac
   if (swfx.ttriac_sw)
   {
      ptmp->Ttriac_MEdit->Text = AnsiString(swfx.ttriac_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_vhlt->Checked = swfx.vhlt_sw; // Set Checkbox for Vhlt
   if (swfx.vhlt_sw)
   {
      ptmp->Vhlt_MEdit->Text = AnsiString(swfx.vhlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_vmlt->Checked = swfx.vmlt_sw; // Set Checkbox for Vmlt
   if (swfx.vmlt_sw)
   {
      ptmp->Vmlt_MEdit->Text = AnsiString(swfx.vmlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   if (ptmp->ShowModal() == 0x1) // mrOK
   {
      ptmp->Apply_ButtonClick(this);
   } // if
   delete ptmp;
   ptmp = 0; // NULL the pointer
} //  TMainForm::FixParameters1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuHelpAboutClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Help | About...'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   Application->CreateForm(__classid(TVersionAwareAbout), &VersionAwareAbout);
   VersionAwareAbout->ShowModal();
   delete VersionAwareAbout;
} // TMainForm::About1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuEditMashSchemeClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Edit | Mash Scheme'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TEditMashSchemeDlg *ptmp;

   ptmp = new TEditMashSchemeDlg(this);
   ptmp->Memo1->Lines->LoadFromFile(MASH_FILE);
   if (ptmp->ShowModal() == 0x1) // mrOK
   {
      ptmp->Memo1->Lines->SaveToFile(MASH_FILE); // Write to File
      //-----------------------------------------------------
      // Read Mash Scheme again, but do not init. mash timers
      //-----------------------------------------------------
      if (!read_input_file(MASH_FILE,ms,&(std.ms_tot),1.0,NO_INIT_TIMERS))
      {
          MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
      } /* if */
      print_mash_scheme_to_statusbar();
   } // if
   delete ptmp;
   ptmp = 0; // NULL the pointer
} // TMainForm::MashScheme1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewMash_progressClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'View | Mash Progress'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   ViewMashProgress->Show(); // Show modeless Dialog
} // TMainForm::MenuViewMash_progressClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Init_Sparge_Settings(void)
/*------------------------------------------------------------------
  Purpose  : This function Initialises the sparge struct with the
             values from the Registry. Call this method during power-up
             and after a change in Sparge Settings parameters.
  Returns  : None
  ------------------------------------------------------------------*/
{
  TRegistry *Reg = new TRegistry();
  int i;

  // Get Sparge  & STD Settings from the Registry
  try
  {
     if (Reg->KeyExists(REGKEY))
     {
        Reg->OpenKey(REGKEY,FALSE);
        // Mash Settings
        sp.temp_offset  = Reg->ReadFloat("TOffset");
        sp.temp_offset2 = Reg->ReadFloat("TOffset2");
        sp.ph_timer     = Reg->ReadInteger("PREHEAT_TIME");
        // Sparge Settings
        sp.sp_batches   = Reg->ReadInteger("SP_BATCHES"); // Number of Sparge Batches
        sp.sp_time      = Reg->ReadInteger("SP_TIME");    // Time between two Sparge Batches
        sp.mash_vol     = Reg->ReadInteger("MASH_VOL");   // Total Mash Volume (L)
        sp.sp_vol       = Reg->ReadInteger("SP_VOL");     // Total Sparge Volume (L)
        sp.boil_time    = Reg->ReadInteger("BOIL_TIME");  // Total Boil Time (min.)
        sp.sp_vol_batch = ((double)(sp.sp_vol)) / sp.sp_batches;
        //---------------------------------------------------------------
        // Now calculate the internal values for the timers. Since the
        // State Transition Diagram is called every second, every 'tick'
        // is therefore a second.
        // SP_TIME [min.], BOIL_TIME [min.]
        //---------------------------------------------------------------
        sp.sp_time_ticks   = sp.sp_time * 60;
        sp.boil_time_ticks = sp.boil_time * 60;
        // STD Settings
        sp.vmlt_empty   = Reg->ReadFloat("VMLT_EMPTY");
        sp.to_xsec      = Reg->ReadInteger("TO_XSEC");
        sp.to3          = Reg->ReadInteger("TO3");
        sp.to4          = Reg->ReadInteger("TO4");
        Reg->CloseKey(); // Close the Registry
        delete Reg;
     } // if
  } // try
  catch (ERegistryException &E)
  {
     ShowMessage(E.Message);
     delete Reg;
     return;
  } // catch
} // TMainForm::Init_Sparge_Settings()
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpargeSettings1Click(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Sparge Settings'.
             Is assumes the Init_Sparge_Settings() has been called
             during Initialisation and that the struct sp is f
  Returns  : None
  ------------------------------------------------------------------*/
{
  TSpargeSettings *ptmp;
  ptmp = new TSpargeSettings(this);
  TRegistry *Reg = new TRegistry();

  // Get Sparge Settings from the Registry
  try
  {
     if (Reg->KeyExists(REGKEY))
     {
        Reg->OpenKey(REGKEY,FALSE);
        /*---------------*/
        /* Mash Settings */
        /*---------------*/
        ptmp->Offs_Edit->Text  = AnsiString(sp.temp_offset);
        ptmp->Offs2_Edit->Text = AnsiString(sp.temp_offset2);
        ptmp->Eph_time->Text   = AnsiString(sp.ph_timer);  // PREHEAT_TIME [sec]
        /*-----------------*/
        /* Sparge Settings */
        /*-----------------*/
        ptmp->EBatches->Text  = AnsiString(sp.sp_batches); // Number of Sparge Batches
        ptmp->EBTime->Text    = AnsiString(sp.sp_time);    // Time between two Sparge Batches
        ptmp->EMVol->Text     = AnsiString(sp.mash_vol);   // Total Mash Volume (L)
        ptmp->ESVol->Text     = AnsiString(sp.sp_vol);     // Total Sparge Volume (L)
        ptmp->EBoilTime->Text = AnsiString(sp.boil_time);  // Total Boil Time (min.)
        /*--------------*/
        /* STD Settings */
        /*--------------*/
        ptmp->Evmlt_empty->Text = AnsiString(sp.vmlt_empty);  // Vmlt_EMPTY [L]
        ptmp->Eto_xsec->Text    = AnsiString(sp.to_xsec);     // TIMEOUT_xSEC [sec]
        ptmp->Eto3->Text        = AnsiString(sp.to3);         // TIMEOUT3 [sec]
        ptmp->Eto4->Text        = AnsiString(sp.to4);         // TIMEOUT4 [sec]

        if (ptmp->ShowModal() == 0x1) // mrOK
        {
           Reg->WriteFloat("TOffset"       ,ptmp->Offs_Edit->Text.ToDouble());
           Reg->WriteFloat("TOffset2"      ,ptmp->Offs2_Edit->Text.ToDouble());
           Reg->WriteInteger("PREHEAT_TIME",ptmp->Eph_time->Text.ToInt());    // PREHEAT_TIME [sec]
           Reg->WriteInteger("SP_BATCHES",ptmp->EBatches->Text.ToInt());  // Number of Sparge Batches
           Reg->WriteInteger("SP_TIME",   ptmp->EBTime->Text.ToInt());    // Time (min.) between two Sparge Batches
           Reg->WriteInteger("MASH_VOL",  ptmp->EMVol->Text.ToInt());     // Total Mash Volume (L)
           Reg->WriteInteger("SP_VOL",    ptmp->ESVol->Text.ToInt());     // Total Sparge Volume (L)
           Reg->WriteInteger("BOIL_TIME", ptmp->EBoilTime->Text.ToInt()); // Total Boil Time (min.)

           Reg->WriteFloat("VMLT_EMPTY",    ptmp->Evmlt_empty->Text.ToDouble()); // Vmlt_EMPTY [L]
           Reg->WriteInteger("TO_XSEC",     ptmp->Eto_xsec->Text.ToInt());    // TIMEOUT_xSEC [sec]
           Reg->WriteInteger("TO3",         ptmp->Eto3->Text.ToInt());        // TIMEOUT3 [sec]
           Reg->WriteInteger("TO4",         ptmp->Eto4->Text.ToInt());        // TIMEOUT4 [sec]
           Init_Sparge_Settings(); // Init. struct with sparge settings with new values
        } // if
        delete ptmp;
        ptmp = 0; // NULL the pointer
        Reg->CloseKey(); // Close the Registry
        delete Reg;
     } // if
  } // try
  catch (ERegistryException &E)
  {
     ShowMessage(E.Message);
     delete Reg;
     return;
  } // catch
} // TMainForm::SpargeSettings1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Auto1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out &= ~(P0M | P0b); // Pump = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V1") == 0)
   {
      std_out &= ~(V1M | V1b); // V1 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V2") == 0)
   {
      std_out &= ~(V2M | V2b); // V2 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V3") == 0)
   {
      std_out &= ~(V3M | V3b); // V3 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V4") == 0)
   {
      std_out &= ~(V4M | V4b); // V4 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V5") == 0)
   {
      std_out &= ~(V5M | V5b); // V5 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V6") == 0)
   {
      std_out &= ~(V6M | V6b); // V6 = Auto mode + OFF
   }
   else std_out &= ~(V7M | V7b); // V7 = Auto mode + OFF
} // TMainForm::Auto1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::OFF1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out |=  P0M; // Pump = Manual Override mode
      std_out &= ~P0b; // Pump = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V1") == 0)
   {
      std_out |=  V1M; // V1 = Manual Override mode
      std_out &= ~V1b; // V1 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V2") == 0)
   {
      std_out |=  V2M; // V2 = Manual Override mode
      std_out &= ~V2b; // V2 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V3") == 0)
   {
      std_out |=  V3M; // V3 = Manual Override mode
      std_out &= ~V3b; // V3 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V4") == 0)
   {
      std_out |=  V4M; // V4 = Manual Override mode
      std_out &= ~V4b; // V4 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V5") == 0)
   {
      std_out |=  V5M; // V5 = Manual Override mode
      std_out &= ~V5b; // V5 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V6") == 0)
   {
      std_out |=  V6M; // V6 = Manual Override mode
      std_out &= ~V6b; // V6 = OFF
   }
   else
   {
      std_out |=  V7M; // V7 = Manual Override mode
      std_out &= ~V7b; // V7 = OFF
   }
} //  TMainForm::OFF1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::ON1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out |= (P0M | P0b); // Pump = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V1") == 0)
   {
      std_out |= (V1M | V1b); // V1 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V2") == 0)
   {
      std_out |= (V2M | V2b); // V2 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V3") == 0)
   {
      std_out |= (V3M | V3b); // V3 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V4") == 0)
   {
      std_out |= (V4M | V4b); // V4 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V5") == 0)
   {
      std_out |= (V5M | V5b); // V5 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V6") == 0)
   {
      std_out |= (V6M | V6b); // V6 = Manual Override mode + ON
   }
   else std_out |= (V7M | V7b); // V7 = Manual Override mode + ON
} // TMainForm::ON1Click()
//---------------------------------------------------------------------------

#define SET_POPUPMENU(Vx,VxM,Vxb)                                                     \
  if (PopupMenu1->PopupComponent->Name.AnsiCompare(Vx) == 0)                          \
  {                                                                                   \
     if (std_out & VxM) /* Manual Override Mode */                                    \
     {                                                                                \
        if (std_out & Vxb) PopupMenu1->Items->Items[2]->Checked = true; /* On (M) */  \
        else               PopupMenu1->Items->Items[1]->Checked = true; /* Off (M) */ \
     }                                                                                \
     else PopupMenu1->Items->Items[0]->Checked = true; /* Auto */                     \
  }

void __fastcall TMainForm::PopupMenu1Popup(TObject *Sender)
{
        SET_POPUPMENU("P0",P0M,P0b)
   else SET_POPUPMENU("V1",V1M,V1b)
   else SET_POPUPMENU("V2",V2M,V2b)
   else SET_POPUPMENU("V3",V3M,V3b)
   else SET_POPUPMENU("V4",V4M,V4b)
   else SET_POPUPMENU("V5",V5M,V5b)
   else SET_POPUPMENU("V6",V6M,V6b)
   else SET_POPUPMENU("V7",V7M,V7b)
}  // TMainForm::PopupMenu1Popup()
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
   exit_ebrew();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ReadLogFile1Click(TObject *Sender)
{
   TRegistry *Reg   = new TRegistry();
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         Restore_Settings();                     // Read Log File & Restore Settings
         Reg->WriteInteger("ms_idx",std.ms_idx); // Update registry setting
         tset_hlt = ms[std.ms_idx].temp;         // Set tset value for HLT
         Reg->CloseKey();                        // Close the Registry
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch
   delete Reg; // Delete Registry object to prevent memory leak
} // TMainForm::ReadLogFile1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Update_GUI(void)
{
   char tmp_str[120];  // temp string for calculations
   char tmp_str2[80];  // temp string for calculations
   int  p;             // temp. variable
   int  i;             // temp. variable

   sprintf(tmp_str,"%4.2f",thlt);   // Display Thlt value on screen
   Val_Thlt->Caption    = tmp_str;
   tm_hlt->Value->Value = thlt;     // update HLT thermometer object

   sprintf(tmp_str,"%4.2f",tmlt);   // Display Tmlt value on screen
   Val_Tmlt->Caption    = tmp_str;
   tm_mlt->Value->Value = tmlt;     // update MLT thermometer object

   tm_triac->Value->Value = ttriac; // Update thermometer object
   sprintf(tmp_str,"%2.0f",ttriac+0.5);
   Ttriac_lbl->Caption    = tmp_str;

   //---------------------------------------------------
   // Triac Temperature Protection: hysteresis function
   //---------------------------------------------------
   if (triac_too_hot)
   {
      tm_triac->ColorAfter  = clMaroon;
      tm_triac->ColorBefore = clRed;
   }
   else
   {
      tm_triac->ColorAfter  = clLime;
      tm_triac->ColorBefore = clGreen;
   }

   sprintf(tmp_str,"%4.1f",volumes.Vmlt); // Display MA filter output on screen
   Vol_MLT->Caption   = tmp_str;          // AD4 = Pressure Transducer
   Tank_MLT->Position = volumes.Vmlt;

   // Heater object shows %
   Heater->Value = gamma; // Update object on screen

   sprintf(tmp_str,"%4.1f",tset_mlt);
   Val_Tset_mlt->Caption   = tmp_str;
   tm_mlt->SetPoint->Value = tset_mlt; // Update MLT thermometer object

   sprintf(tmp_str,"%4.1f",tset_hlt);
   Val_Tset_hlt->Caption = tmp_str;
   tm_hlt->SetPoint->Value = tset_hlt; // update HLT thermometer object

   switch (std.ebrew_std)
   {
      case  0: Std_State->Caption = "00. Initialisation"       ; break;
      case  1: Std_State->Caption = "01. Wait for HLT Temp."   ; break;
      case  2: Std_State->Caption = "02. Fill MLT"             ; break;
      case  3: Std_State->Caption = "03. Mash in Progress"     ; break;
      case  4: Std_State->Caption = "04. Mash Timer Running"   ; break;
      case  5: Std_State->Caption = "05. Sparging Rest"        ; break;
      case  6: Std_State->Caption = "06. Pump from MLT to Boil"; break;
      case  7: Std_State->Caption = "07. Pump from HLT to MLT" ; break;
      case  8: Std_State->Caption = "08. Delay"                ; break;
      case  9: Std_State->Caption = "09. Empty MLT"            ; break;
      case 10: Std_State->Caption = "10. Boiling"              ; break;
      case 11: Std_State->Caption = "11. Empty Heat Exchanger" ; break;
      case 12: Std_State->Caption = "12. Chill"                ; break;
      case 13: Std_State->Caption = "13. Mash PreHeat HLT"     ; break;
      default: break;
   } // switch
   //----------------------------------------------------------------------
   // Now update tank objects (volumes). MLT is already done (time-slice 1)
   //----------------------------------------------------------------------
   Tank_HLT->Position  = volumes.Vhlt;
   sprintf(tmp_str,"%4.1f",volumes.Vhlt);
   Vol_HLT->Caption = tmp_str;

   Tank_Boil->Position = volumes.Vboil;
   sprintf(tmp_str,"%4.1f",volumes.Vboil);
   Vol_Boil->Caption = tmp_str;

   //--------------------------------------------------------------------------
   // Update the Captions for all valves (they might have
   // changed as a result of the call to update_std().
   // The change in Manual Override settings is taken care of
   // in the PopupMemu. Therefore only update the valve captions
   // in Auto Mode in case of a bit change.
   //--------------------------------------------------------------------------
   switch (std_out & (P0M | P0b))
   {
      case P0M | P0b: P0->Caption = P01MTXT; break;
      case P0M      : P0->Caption = P00MTXT; break;
      case P0b      : P0->Caption = P01ATXT; break;
      default       : P0->Caption = P00ATXT; break;
   } // switch
   switch (std_out & (V1M | V1b))
   {
      case V1M | V1b: V1->Caption = V11MTXT; break;
      case V1M      : V1->Caption = V10MTXT; break;
      case V1b      : V1->Caption = V11ATXT; break;
      default       : V1->Caption = V10ATXT; break;
   } // switch
   switch (std_out & (V2M | V2b))
   {
      case V2M | V2b: V2->Caption = V21MTXT; break;
      case V2M      : V2->Caption = V20MTXT; break;
      case V2b      : V2->Caption = V21ATXT; break;
      default       : V2->Caption = V20ATXT; break;
   } // switch
   switch (std_out & (V3M | V3b))
   {
      case V3M | V3b: V3->Caption = V31MTXT; break;
      case V3M      : V3->Caption = V30MTXT; break;
      case V3b      : V3->Caption = V31ATXT; break;
      default       : V3->Caption = V30ATXT; break;
   } // switch
   switch (std_out & (V4M | V4b))
   {
      case V4M | V4b: V4->Caption = V41MTXT; break;
      case V4M      : V4->Caption = V40MTXT; break;
      case V4b      : V4->Caption = V41ATXT; break;
      default       : V4->Caption = V40ATXT; break;
   } // switch
   switch (std_out & (V5M | V5b))
   {
      case V5M | V5b: V5->Caption = V51MTXT; break;
      case V5M      : V5->Caption = V50MTXT; break;
      case V5b      : V5->Caption = V51ATXT; break;
      default       : V5->Caption = V50ATXT; break;
   } // switch
   switch (std_out & (V6M | V6b))
   {
      case V6M | V6b: V6->Caption = V61MTXT; break;
      case V6M      : V6->Caption = V60MTXT; break;
      case V6b      : V6->Caption = V61ATXT; break;
      default       : V6->Caption = V60ATXT; break;
   } // switch
   switch (std_out & (V7M | V7b))
   {
      case V7M | V7b: V7->Caption = V71MTXT; break;
      case V7M      : V7->Caption = V70MTXT; break;
      case V7b      : V7->Caption = V71ATXT; break;
      default       : V7->Caption = V70ATXT; break;
   } // switch

   //-------------------------------------------
   // Debug PID Controller
   //-------------------------------------------
   if (cb_pid_dbg)
   {
      if (pid_pars.pid_model == 0)
      {  // pid_model = 0: Self-Tuning controllers
         sprintf(tmp_str,"%6.2f %6.2f %6.2f %6.2f %6.2f (%6.2f %6.2f %6.2f)",
                         pid_pars.pp, pid_pars.pi, pid_pars.pd,
                         pid_pars.pp + pid_pars.pi + pid_pars.pd, gamma,
                         sys_id_pars.kc, sys_id_pars.ti, sys_id_pars.td);
         switch (sys_id_pars.N)
         {
            case 1: sprintf(tmp_str2,"(%6.4f %6.4f)",theta[0],theta[1]);
                    break;
            case 2: sprintf(tmp_str2,"(%6.4f %6.4f %6.4f %6.4f)",
                                     theta[0],theta[1],theta[2],theta[3]);
                    break;
            case 3: sprintf(tmp_str2,"(%6.4f %6.4f %6.4f %6.4f %6.4f %6.4f)",
                                     theta[0],theta[1],theta[2],theta[3],theta[4],theta[5]);
                    break;
         } // switch
         strcat(tmp_str,tmp_str2); // add 2 strings together
      } // if
      else
      {  // pid_model = 1 or 2, PID controllers with fixed parameters
         sprintf(tmp_str,"%6.2f %6.2f %6.2f %6.2f %6.2f",
                         pid_pars.pp, pid_pars.pi, pid_pars.pd,
                         pid_pars.pp + pid_pars.pi + pid_pars.pd, gamma);
      } // else
      PID_dbg->Caption = tmp_str;
   } // if

   //-------------------------------------------
   // Update the various panels of the Statusbar
   //-------------------------------------------
   i = 0; // number of burners
   if (cb_pid_out & PID_OUT_ELECTRIC)
   {
      sprintf(tmp_str,"Electric");
      i++;
   }
   else sprintf(tmp_str,"");
   if (cb_pid_out & PID_OUT_GAS_NON_MOD)
   {
      if (++i > 1)
      {
         strcat(tmp_str," + ");
      }
      strcat(tmp_str,"Non-Mod.");
   } // if
   if (cb_pid_out & PID_OUT_GAS_MODULATE)
   {
      if (++i > 1)
      {
         strcat(tmp_str," + ");
      }
      strcat(tmp_str,"Modulating");
   } // if
   StatusBar->Panels->Items[PANEL_TCPIP]->Text = AnsiString(tmp_str);

   sprintf(tmp_str,"ms_idx = %d",std.ms_idx);
   StatusBar->Panels->Items[PANEL_MSIDX]->Text = AnsiString(tmp_str);
   sprintf(tmp_str,"sp_idx = %d",std.sp_idx);
   StatusBar->Panels->Items[PANEL_SPIDX]->Text = AnsiString(tmp_str);
   strcpy(tmp_str,"V7 [");
   p = 128;
   for (i = 7; i > 0; i--)
   {  // cycle through the 7 valve bits V7..V1
      std_out & p ? strcat(tmp_str,"1") : strcat(tmp_str,"0");
      p = p / 2; // decrease power of 2
   } // for
   strcat(tmp_str,"] V1");
   StatusBar->Panels->Items[PANEL_VALVE]->Text = AnsiString(tmp_str);
} // Update_GUI()

void __fastcall TMainForm::ebrew_idle_handler(TObject *Sender, bool &Done)
{
   dispatch_tasks(); // Call all tasks that are ready to run
   Update_GUI();
} // ebrew_idle_handler()
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
   Application->OnIdle = ebrew_idle_handler;
} // TMainForm::FormCreate()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuView_I2C_HW_DevicesClick(TObject *Sender)
{
   //---------------------------------------------------------------------------
   // Restart I2C bus communication and print all devices found.
   // This is done in the main timer interrupt to avoid I2C timing problems.
   //---------------------------------------------------------------------------
   i2c_hw_scan_req = true;
} // MenuView_I2C_HW_DevicesClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MeasurementsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Measurements'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   TMeasurements *ptmp;

   ptmp = new TMeasurements(this);

   // Get Measurements Settings from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         //------------------
         // HLT Temperature
         //------------------
         ptmp->Thlt_Offset_Edit->Text   = Reg->ReadFloat("THLT_OFFSET");
         ptmp->Thlt_Slope_Edit->Text    = Reg->ReadFloat("THLT_SLOPE");
         //------------------
         // MLT Temperature
         //------------------
         ptmp->Tmlt_Offset_Edit->Text   = Reg->ReadFloat("TMLT_OFFSET");
         ptmp->Tmlt_Slope_Edit->Text    = Reg->ReadFloat("TMLT_SLOPE");
         //------------------
         // HLT Volume
         //------------------
         ptmp->Vhlt_Max_Edit->Text      = Reg->ReadFloat("VHLT_MAX");
         ptmp->Vhlt_Offset_Edit->Text   = Reg->ReadFloat("VHLT_OFFSET");
         ptmp->Vhlt_Slope_Edit->Text    = Reg->ReadFloat("VHLT_SLOPE");
         //------------------
         // MLT Volume
         //------------------
         ptmp->Vmlt_Max_Edit->Text      = Reg->ReadFloat("VMLT_MAX");
         ptmp->Vmlt_Offset_Edit->Text   = Reg->ReadFloat("VMLT_OFFSET");
         ptmp->Vmlt_Slope_Edit->Text    = Reg->ReadFloat("VMLT_SLOPE");
         //-------------------
         // Boil Kettle Volume
         //-------------------
         volumes.Vboil_simulated        = Reg->ReadBool("VBOIL_SIMULATED");
         ptmp->Vboil_simulated->Checked = volumes.Vboil_simulated;

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            //------------------
            // HLT Temperature
            //------------------
            thlt_offset = ptmp->Thlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("THLT_OFFSET",thlt_offset);
            thlt_slope = ptmp->Thlt_Slope_Edit->Text.ToDouble();
            Reg->WriteFloat("THLT_SLOPE",thlt_slope);
            //------------------
            // MLT Temperature
            //------------------
            tmlt_offset = ptmp->Tmlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("TMLT_OFFSET",tmlt_offset);
            tmlt_slope = ptmp->Tmlt_Slope_Edit->Text.ToDouble();
            Reg->WriteFloat("TMLT_SLOPE",tmlt_slope);
            //------------------
            // HLT Volume
            //------------------
            vhlt_max = ptmp->Vhlt_Max_Edit->Text.ToDouble();
            Reg->WriteFloat("VHLT_MAX",vhlt_max);
            vhlt_offset = ptmp->Vhlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("VHLT_OFFSET",vhlt_offset);
            vhlt_slope = ptmp->Vhlt_Slope_Edit->Text.ToDouble();
            Reg->WriteFloat("VHLT_SLOPE",vhlt_slope);
            //------------------
            // MLT Volume
            //------------------
            vmlt_max = ptmp->Vmlt_Max_Edit->Text.ToDouble();
            Reg->WriteFloat("VMLT_MAX",vmlt_max);
            vmlt_offset = ptmp->Vmlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("VMLT_OFFSET",vmlt_offset);
            vmlt_slope = ptmp->Vmlt_Slope_Edit->Text.ToDouble();
            Reg->WriteFloat("VMLT_SLOPE",vmlt_slope);
            //-------------------
            // Boil Kettle Volume
            //-------------------
            volumes.Vboil_simulated = ptmp->Vboil_simulated->Checked;
            Reg->WriteBool("VBOIL_SIMULATED",volumes.Vboil_simulated);
          } // if
         Reg->CloseKey(); // Close the Registry
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch
   // Clean up
   delete Reg;
   delete ptmp;
   ptmp = 0; // NULL the pointer
} // TMainForm::MeasurementsClick()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Contents1Click(TObject *Sender)
{
   Application->HelpCommand(HELP_FINDER,0);
} // TMainForm::Contents1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::HowtoUseHelp1Click(TObject *Sender)
{
   Application->HelpCommand(HELP_HELPONHELP,0);
} // TMainForm::HowtoUseHelp1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyPress(TObject *Sender, char &Key)
{
   if (UpCase(Key) == 'P')
   {
      std_out |= P0M; // Set Pump Manual bit
      std_out ^= P0b; // Toggle Pump On/Off
   } // if
   else if (UpCase(Key) == 'H')
   {
      swfx.gamma_sw = true; // Set switch for gamma
      if (swfx.gamma_fx > 99.9)
      {
         swfx.gamma_fx = 0.0; // fix gamma to 0 %
      } // if
      else
      {
         swfx.gamma_fx = 100.0; // fix gamma to 100 %
      } // else
   } // else
   else if ((Key >= '1') && (Key <= '7'))
   {
      // This code only works if V7 is the MSB and V1 is the LSB!! (see misc.h)
      std_out |= (V1M << (Key - '1')); // set corresponding V1M..V7M bit
      std_out ^= (V1b << (Key - '1')); // set corresponding V1b..V7b bit
   }
} // FormKeyPress()
//---------------------------------------------------------------------------




