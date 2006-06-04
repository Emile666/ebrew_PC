// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
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

//---------------------------------------------------------------------------
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
#include "DataGraphForm.h"
#include "Sparge_Settings.h"
#include "RestoreSettings.h"
#include "misc.h"
#include "VersionAwareAbout.h"
#include <Dialogs.hpp>

#include "i2c_dll.h"
extern byte fscl_values[]; // defined in i2c_dll.cpp
extern byte base_adc[];    // defined in i2c_dll.cpp

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
      j = decode_log_file(fd,p1); // read log file with data just before chrash
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
         for (j = 0; j < std.ms_idx; j++)
         {
            ms[j].timer = ms[j].time; // set previous timers to time-out
         } // for j
         if (p1[k].sparging_start)
         {  // set to time-out if mashing is finished
            ms[std.ms_idx].timer = ms[std.ms_idx].time;
         }
         else
         {
            //-------------------------------------------------------------------
            // mash was in progress, Two options: (computed by decode_log_file())
            // 1) tmr_ms_idx = NOT_STARTED: Ref. temp. was not reached yet
            // 2) tmr_ms_idx has an actual timer value
            //-------------------------------------------------------------------
            ms[std.ms_idx].timer = p1[k].tmr_ms_idx;
         } // else

         //----------------------------
         // Restore Sparging parameters
         //----------------------------
         std.ebrew_std = p1[k].std_val; // Current state
         std.sp_idx    = p1[k].lsp_idx; // Sparging sessions done
         x             = p1[k].eline + 1 - p1[k].start_lstd;
         x            *= 5;             // Log-file -> 5 sec.: STD called -> 1 sec.
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

void __fastcall TMainForm::Start_I2C_Communication(int known_status)
/*------------------------------------------------------------------
  Purpose  : This function starts I2C communication, reports any
             errors found and perform a check on which I2C hardware
             devices are present.
  Variables: known_status: if the hardware status does not match this
                           variable, a messagebox with the actual
                           hardware status is printed on the screen.
  Returns  : None
             The global variable 'hw_status' is given a value
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   char s[80];          // temp. string
   char s1[20];         // temp. string
   char st[1024];       // string for MessageBox
   int  x1;             // Hardware base address from Registry

   //--------------------------------------------
   // Get variables for i2c_init() from Registry
   //--------------------------------------------
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         x1 = Reg->ReadInteger("I2C_Base");    // Read HW IO address as an int.
         fscl_prescaler = Reg->ReadInteger("FSCL_PRESCALER");
         if ((fscl_prescaler < 0) || (fscl_prescaler > 12))
         {
            fscl_prescaler = 5; // set fscl to 11.72 kHz
            Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler);
         } // if
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch
   Reg->CloseKey(); // Close the Registry
   delete Reg;      // Delete Registry object to prevent memory leak

   hw_status = i2c_init(x1, LPT_CARD|WIN_XP, fscl_values[fscl_prescaler]);
   if (hw_status != I2C_NOERR)
   {
     sprintf(s,"Error in i2c_init(0x%x)",x1);
     MessageBox(NULL,I2C_ARGS_MSG,s,MB_OK);
   } // if
   else
   {
     hw_status = i2c_start(); // Start I2C Communication
     switch (hw_status)
     {
        case I2C_BB  : MessageBox(NULL,I2C_BB_MSG,"Error in i2c_start()",MB_OK);
                       hw_status = 0; // set to 'No Devices present'
                       break;
        case I2C_BERR: MessageBox(NULL,I2C_BERR_MSG,"Error in i2c_start()",MB_OK);
                       hw_status = 0; // set to 'No Devices present'
                       break;
        default      : //-------------------------------------------------
                       // No error, check the individual Hardware Devices.
                       //-------------------------------------------------
                       check_i2c_hw(&hw_status); // check all hardware
                       // Print information using a MessageBox
                       PR_HW_STAT(LCD_OK);       // LCD Display
                       sprintf(st,LCD_TXT,s1);
                       PR_HW_STAT(DIG_IO_LSB_OK); // IO Port LSB
                       sprintf(s,DIG_IO_LSB_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(DIG_IO_MSB_OK); // IO Port MSB
                       sprintf(s,DIG_IO_MSB_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(MAX1238_OK);    // MAX1238 ADC 12-bit 12-ch.
                       sprintf(s,MAX1238_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LED1_OK);       // LED1
                       sprintf(s,LED1_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LED2_OK);       // LED2
                       sprintf(s,LED2_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LED3_OK);       // LED3
                       sprintf(s,LED3_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LED4_OK);       // LED4
                       sprintf(s,LED4_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(ADDA_OK);       // PCF8591 AD-DA Converter
                       sprintf(s,ADDA_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LM92_1_OK);     // LM92 Temp. Sensor
                       sprintf(s,LM92_1_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(LM92_2_OK);     // LM92 Temp. Sensor
                       sprintf(s,LM92_2_TXT,s1);
                       strcat(st,s);
                       PR_HW_STAT(FM24C08_OK);    // FM24C08 EEPROM
                       sprintf(s,FM24C08_TXT,s1);
                       strcat(st,s);

                       if (hw_status != known_status)
                       {  // Print only if HW device configuration has changed
                          MessageBox(NULL,st,"Results of I2C Hardware Check",MB_OK);
                       }
                       break; /* NO_ERR */
     } // switch
   } // else
} // TMainForm::Start_I2C_Communication()
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

void __fastcall TMainForm::Main_Initialisation(void)
/*------------------------------------------------------------------
  Purpose  : This function Initialises all I2C Hardware and checks if
             it is present. It also initialises the Interrupt Service
             Routine (ISR) and the PID controller.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   FILE *fd;            // Log File Descriptor
   int  i;              // temp. variable
   char s[40];          // Temp. string

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
         pid_pars.k_lpf     = Reg->ReadFloat("K_LPF");
         pid_pars.pid_model = Reg->ReadInteger("PID_Model"); // [0..3]
         pid_pars.burner_hyst_h = Reg->ReadInteger("BURNER_HHYST");
         pid_pars.burner_hyst_l = Reg->ReadInteger("BURNER_LHYST");
         // Send PID-Output to the various heaters / burners
         cb_pid_out = Reg->ReadInteger("CB_PID_OUT");
         dac_a = Reg->ReadFloat("DAC_A"); // a-coefficient for y=a.x+b DAC calc.
         dac_b = Reg->ReadFloat("DAC_B"); // b-coefficient for y=a.x+b DAC calc.

         led1 = Reg->ReadInteger("LED1"); // Read led1 from registry
         led2 = Reg->ReadInteger("LED2"); // Read led2 from registry
         led3 = Reg->ReadInteger("LED3"); // Read led3 from registry
         led4 = Reg->ReadInteger("LED4"); // Read led4 from registry

         led1_vis = Reg->ReadInteger("LED1_VIS"); // Read led1 Visibility
         led2_vis = Reg->ReadInteger("LED2_VIS"); // Read led2 Visibility
         led3_vis = Reg->ReadInteger("LED3_VIS"); // Read led3 Visibility
         led4_vis = Reg->ReadInteger("LED4_VIS"); // Read led4 Visibility

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM"); // Read high limit
         tm_triac->SetPoint->Value = ttriac_hlim;       // update object on screen
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM"); // Read low limit

         volumes.Vhlt_start      = Reg->ReadInteger("VHLT_START"); // Read initial volume
         volumes.Vboil_simulated = Reg->ReadBool("VBOIL_SIMULATED");
         cb_i2c_err_msg          = Reg->ReadBool("CB_I2C_ERR_MSG"); // display message

         init_ma(&str_thlt,Reg->ReadInteger("MA_THLT"),thlt); // MA filter for Thlt
         thlt_offset = Reg->ReadFloat("THLT_OFFSET");         // offset calibration
         init_ma(&str_tmlt,Reg->ReadInteger("MA_TMLT"),tmlt); // MA filter for Tmlt
         tmlt_offset = Reg->ReadFloat("TMLT_OFFSET");         // offset calibration
         init_ma(&str_vmlt,Reg->ReadInteger("MA_VMLT"),volumes.Vmlt); // MA filter for Vmlt
         //--------------------------------------------------------------------
         // The enum i2c_adc starts at NONE (0), which is also the start value
         // of the first entry of the combo-box.
         //--------------------------------------------------------------------
         vhlt_src   = (enum i2c_adc)Reg->ReadInteger("VHLT_SRC");    // source AD channel
         volumes.Vhlt_simulated = vhlt_src;           // needed for STD
         vhlt_a     = Reg->ReadFloat("VHLT_A");       // a-coefficient for y=a.x+b
         vhlt_b     = Reg->ReadFloat("VHLT_B");       // b-coefficient for y=a.x+b
         if (vhlt_src == 0)
         {
            // Vhlt is simulated, init. MA filter for Vhlt
            init_ma(&str_vhlt,Reg->ReadInteger("MA_VHLT"),volumes.Vhlt_start);
         }
         else
         {  // Vhlt is measured from pressure transducer, init. MA filter for Vhlt
            init_ma(&str_vhlt,Reg->ReadInteger("MA_VHLT"),volumes.Vhlt);
         } // else
         vmlt_src   = (enum i2c_adc)Reg->ReadInteger("VMLT_SRC");   // source AD channel
         vmlt_a     = Reg->ReadFloat("VMLT_A");       // a-coefficient for y=a.x+b
         vmlt_b     = Reg->ReadFloat("VMLT_B");       // b-coefficient for y=a.x+b

         ttriac_src = (enum i2c_adc)Reg->ReadInteger("TTRIAC_SRC"); // source AD channel
         ttriac_a   = Reg->ReadFloat("TTRIAC_A");     // a-coefficient for y=a.x+b
         ttriac_b   = Reg->ReadFloat("TTRIAC_B");     // b-coefficient for y=a.x+b

         Reg->SaveKey(REGKEY,"ebrew_reg");
         Reg->CloseKey();      // Close the Registry
         switch (pid_pars.pid_model)
         {
            case 0 : init_pid1(&pid_pars); break; // First   ebrew PID controller
            case 1 : init_pid2(&pid_pars); break; // Updated ebrew PID controller
            case 2 : init_pid3(&pid_pars); break; // Allen Bradley Type A controller
            case 3 : init_pid4(&pid_pars); break; // Allen Bradley Type C controller
            default: pid_pars.pid_model = 3;
                     init_pid4(&pid_pars); break; // Allen Bradley Type C controller
         } // switch
         // Do NOT delete Reg yet, since we need it further on
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch

   //----------------------------------------------------------------------
   // Start I2C Communication and print list of I2C devices found if it
   // does not match known devices.
   //----------------------------------------------------------------------
   Start_I2C_Communication(known_hw_devices);

   //-------------------------------------
   // Read Mash Scheme for maisch.sch file
   // Mash scheme is used in the STD
   // Sample time of STD is 1 second
   //-------------------------------------
   if (!read_input_file(MASH_FILE,ms,&(std.ms_tot),1.0,INIT_TIMERS))
   {
       MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
   } /* if */
   print_mash_scheme_to_statusbar();
   Init_Sparge_Settings(); // Initialise the Sparge Settings struct (STD needs it)

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
         //tset_hlt = ms[std.ms_idx].temp;         // Set tset value for HLT
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
   tmr.isrstate  = 1;                 // disable heater
   tmr.htimer    = tmr.ltimer   = 0;  // init. low timer
   tmr.time_high = tmr.time_low = 0;  // init. time bit = 1
   tmr.alive     = tmr.alive_tmr = 0; // init. alive timers
   tmr.pid_tmr   = 1; // init. timer that controls PID controller timing

   //--------------------------------------------------------------------------
   // Init. the position of all valves & the Pump to OFF (closed).
   // No Manual Override of valves yet.
   //--------------------------------------------------------------------------
   std_out = 0x0000;

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
      fprintf(fd,"ma_thlt=%d, ma_tmlt=%d, ma_vhlt=%d, ma_vmlt=%d; ",
                 str_thlt.N, str_tmlt.N, str_vhlt.N, str_vmlt.N);
      strncpy(s,&ebrew_revision[11],4); // extract the CVS revision number
      s[4] = '\0';
      fprintf(fd,"ebrew CVS Rev. %s\n",s);
      fprintf(fd,"hw_status = 0x%02X, ms_tot =%2d, fscl_prescaler =%2d\n",hw_status,
                                                                          std.ms_tot,
                                                                          fscl_prescaler);
      fprintf(fd,"Temp Offset = %4.1f, Temp Offset2 = %4.1f\n",sp.temp_offset,sp.temp_offset2);
      fprintf(fd,"Vhlt_a=%7.4f, Vhlt_b=%7.4f, Vmlt_a=%7.4f, Vmlt_b=%7.4f\n\n",vhlt_a,vhlt_b,vmlt_a,vmlt_b);
      fprintf(fd," Time   TsetMLT TsetHLT  Thlt   Tmlt  TTriac  Vmlt sp ms STD  Gamma  Vhlt\n");
      fprintf(fd,"[h:m:s]    [°C]   [°C]   [°C]   [°C]   [°C]   [L]  id id       [%]    [L]\n");
      fprintf(fd,"-------------------------------------------------------------------------\n");
      fclose(fd);
   } // else

   //----------------------------------
   // We came all the way! Start Timers
   //----------------------------------
   T50msec->Enabled                       = true; // start Interrupt Timer
   ShowDataGraphs->GraphTimer->Enabled    = true; // Start Graph Update timer
   ViewMashProgress->UpdateTimer->Enabled = true; // Start Mash Progress Update timer
   PID_RB->Enabled                        = true; // Enable PID Controller Radio-buttons
   time_switch                            = 0;    // Time switch disabled at power-up
   delete Reg; // Delete Registry object to prevent memory leak
} // Main_Initialisation()
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
/*------------------------------------------------------------------
  Purpose  : This is the main constructor for the program.
  Returns  : None
  ------------------------------------------------------------------*/
{
   char s[50];
   ebrew_revision   = "$Revision$";
   ShowDataGraphs   = new TShowDataGraphs(this);   // create modeless Dialog
   ViewMashProgress = new TViewMashProgress(this); // create modeless Dialog
   TRegistry *Reg   = new TRegistry();
   power_up_flag    = true; // indicate that program power-up is active

   try
    {
       if (!Reg->KeyExists(REGKEY))
       {
          // No entry in Registry, create all keys
          Reg->CreateKey(REGKEY); // Create key if it does not exist yet
          Reg->OpenKey(REGKEY,FALSE);
          Reg->WriteInteger("I2C_Base",0x378);   // I2C HW Base Address
          Reg->WriteInteger("KNOWN_HW_DEVICES",known_hw_devices);
          fscl_prescaler = 5;
          Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler); // set fscl to 11.72 kHz
          // PID Settings Dialog
          Reg->WriteFloat("TS",TS_INIT);       // Set Default sample time
          Reg->WriteFloat("Kc",KC_INIT);       // Controller gain
          Reg->WriteFloat("Ti",TI_INIT);       // Ti constant
          Reg->WriteFloat("Td",TD_INIT);       // Td constant
          Reg->WriteFloat("K_LPF",0);          // LPF filter time-constant
          Reg->WriteFloat("TOffset",1.0);      // HLT - MLT heat loss
          Reg->WriteFloat("TOffset2",-0.5);    // offset for early start of mash timers
          Reg->WriteInteger("PID_Model",0);    // Type A PID Controller
          Reg->WriteInteger("BURNER_HHYST",35); // Gas Burner Hysteresis High Limit
          Reg->WriteInteger("BURNER_LHYST",30); // Gas Burner Hysteresis Low Limit
          Reg->WriteString("SERVER_NAME","PC-EMILE"); // Server to connect to
          // Send PID-Output to the various heaters / burners
          cb_pid_out = 0; // The PID-output is not connected to anything
          Reg->WriteInteger("CB_PID_OUT",cb_pid_out);
          dac_a = -2.04; dac_b = 232.05;  // 5% -> just on, 95% -> full on
          Reg->WriteFloat("DAC_A",dac_a); // default value for DAC calculations
          Reg->WriteFloat("DAC_B",dac_b);

          //---------------------------------------------------------------
          // For the LED Displays: 0=Thlt  , 1=Tmlt, 2=Tset_hlt, 3=Tset_mlt
          //                       4=Ttriac, 5=Vmlt, 6=Vhlt    , 7=gamma
          // Macro, used in TMainForm::T50msec2Timer()
          //---------------------------------------------------------------
          led1 = 0; Reg->WriteInteger("LED1",led1);   // Thlt
          led2 = 1; Reg->WriteInteger("LED2",led2);   // Tmlt
          led3 = 5; Reg->WriteInteger("LED3",led3);   // Tset_hlt
          led4 = 6; Reg->WriteInteger("LED4",led4);   // Tset_mlt

          led1_vis = 1; // 3 mA visibility
          Reg->WriteInteger("LED1_VIS",led1_vis); // LED1 Visibility
          led2_vis = 1; // 3 mA visibility
          Reg->WriteInteger("LED2_VIS",led2_vis); // LED2 Visibility
          led3_vis = 2; // 6 mA visibility
          Reg->WriteInteger("LED3_VIS",led3_vis); // LED3 Visibility
          led4_vis = 2; // 6 mA visibility
          Reg->WriteInteger("LED4_VIS",led4_vis); // LED4 Visibility

          ttriac_hlim = 70; // Upper limit for triac temp.
          Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
          ttriac_llim = 60; // Lower limit for triac temp.
          Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);
          cb_i2c_err_msg          = true;
          Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);
          known_hw_devices = DIG_IO_LSB_OK | LED1_OK    | LED2_OK   | LED3_OK |
                             LED4_OK       | MAX1238_OK | LM92_1_OK | LM92_2_OK;

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
          Reg->WriteInteger("MA_THLT",5);      // Order MA filter Thlt
          Reg->WriteFloat("THLT_OFFSET",0.0);  // Offset for Thlt
          Reg->WriteInteger("MA_TMLT",5);      // Order MA filter Tmlt
          Reg->WriteFloat("TMLT_OFFSET",0.0);  // Offset for Tmlt
          Reg->WriteInteger("MA_VMLT",5);      // Order MA filter Vmlt
          Reg->WriteInteger("MA_VHLT",5);      // Order MA filter Vhlt
          volumes.Vhlt_start = 90;             // Starting volume of HLT
          Reg->WriteInteger("VHLT_START",volumes.Vhlt_start);
          Reg->WriteInteger("VHLT_SRC",6);     // Vhlt = AIN1_MAX1238
          Reg->WriteFloat("VHLT_A",0.025);     // a-coefficient for y=a.x+b
          Reg->WriteFloat("VHLT_B",0.0);       // b-coefficient for y=a.x+b
          Reg->WriteInteger("VMLT_SRC",7);     // Vmlt = AIN2_MAX1238
          Reg->WriteFloat("VMLT_A",0.025);     // a-coefficient for y=a.x+b
          Reg->WriteFloat("VMLT_B",0.0);       // b-coefficient for y=a.x+b
          Reg->WriteInteger("TTRIAC_SRC",5);   // Ttriac = AIN0_MAX1238
          Reg->WriteFloat("TTRIAC_A",0.1);     // a-coefficient for y=a.x+b
          Reg->WriteFloat("TTRIAC_B",0.0);     // b-coefficient for y=a.x+b
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
    volumes.Vhlt_simulated = vhlt_src; // needed for STD
    if (vhlt_src == 0)
    {  // Vhlt is not measured but calculated from Vmlt
       volumes.Vhlt  = volumes.Vhlt_start;
    }
    if (volumes.Vboil_simulated)
    {  // Vboil is not measured but calculated from Vmlt
       volumes.Vboil  = VBOIL_START;
    }
    power_up_flag = false; // power-up is finished
} // TMainForm::TMainForm()
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
         dac_a = Reg->ReadFloat("DAC_A");
         ptmp->DAC_A_Edit->Text     = AnsiString(dac_a);
         dac_b = Reg->ReadFloat("DAC_B");
         ptmp->DAC_B_Edit->Text     = AnsiString(dac_b);

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
            pid_pars.pid_model = ptmp->PID_Model->ItemIndex;
            pid_pars.burner_hyst_h = ptmp->Burner_On->Text.ToInt();
            Reg->WriteInteger("BURNER_HHYST",pid_pars.burner_hyst_h);
            pid_pars.burner_hyst_l = ptmp->Burner_Off->Text.ToInt();
            Reg->WriteInteger("BURNER_LHYST",pid_pars.burner_hyst_l);
            switch (pid_pars.pid_model)
            {
               case 0 : init_pid1(&pid_pars); break; // First   ebrew PID controller
               case 1 : init_pid2(&pid_pars); break; // Updated ebrew PID controller
               case 2 : init_pid3(&pid_pars); break; // Allen Bradley Type A controller
               case 3 : init_pid4(&pid_pars); break; // Allen Bradley Type C controller
               default: pid_pars.pid_model = 3;
                        init_pid4(&pid_pars); break; // Allen Bradley Type C controller
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
            dac_a = ptmp->DAC_A_Edit->Text.ToDouble();
            Reg->WriteFloat("DAC_A",dac_a);
            dac_b = ptmp->DAC_B_Edit->Text.ToDouble();
            Reg->WriteFloat("DAC_B",dac_b);

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

void __fastcall TMainForm::MenuFileExitClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'File | Exit'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   exit_ebrew();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::exit_ebrew(void)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'File | Exit'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   int    err = 0; // error return code from I2C routines
   char   s[80];   // temp. string for error messages
   adda_t adc;     // struct containing 4 ADC values + DA value

   T50msec->Enabled = false; // Disable Interrupt Timer
   if (ShowDataGraphs)
   {
      ShowDataGraphs->GraphTimer->Enabled = false; // Stop Graph Update timer
   }
   if (ViewMashProgress)
   {
      ViewMashProgress->UpdateTimer->Enabled = false; // Stop Mash Progress Update timer
   }
   Sleep(51);                // Make sure that Timer is disabled
   if (ShowDataGraphs)
   {
      delete ShowDataGraphs;    // close modeless dialog
      ShowDataGraphs = 0;       // null the pointer
   }
   if (ViewMashProgress)
   {
      delete ViewMashProgress;  // close modeless dialog
      ViewMashProgress = 0;     // null the pointer
   }
   if (hw_status & ADDA_OK)
   {
      //-------------------------------------------------------------
      // Reset DA-Converter to 0 to prevent gas-burner from burning.
      //-------------------------------------------------------------
      adc.dac = 0;
      err     = read_adc(&adc); // Send value to DA-Converter
      sprintf(s,"Error %d while closing PCF8591 ADC-DAC",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if
   if (hw_status & DIG_IO_LSB_OK)
   {
      //-------------------------------------------------------------
      // Disable Heater, Pump and Alive LED (active-low)
      // Disable Gas Burner (active-high). See misc.h for declaration
      //-------------------------------------------------------------
      err = WriteIOByte(HEATERb | ALIVEb | PUMPb, LSB_IO);
      sprintf(s,"Error %d while closing LSB_IO",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if

   if ((hw_status & LED1_OK) && !err)     // clear LED1 display
   {
      err = set_led(-1,0,1,led1_vis);
      sprintf(s,"Error %d while closing LED1 Display",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if
   if ((hw_status & LED2_OK) && !err)     // clear LED2 display
   {
      err = set_led(-1,0,2,led2_vis);
      sprintf(s,"Error %d while closing LED2 Display",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if
   if ((hw_status & LED3_OK) && !err)     // clear LED3 display
   {
      err = set_led(-1,0,3,led3_vis);
      sprintf(s,"Error %d while closing LED3 Display",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if
   if ((hw_status & LED4_OK) && !err)     // clear LED4 display
   {
      err = set_led(-1,0,4,led4_vis);
      sprintf(s,"Error %d while closing LED4 Display",err);
      if (err) MessageBox(NULL,s,"ERROR",MB_OK);
   } // if

   if ((hw_status & LCD_OK) && !err)     // clear LCD display
   {
      //err = WriteInstrLCD(0x01); // clear display
      //err = WriteInstrLCD(0x08); // Display OFF, Blink OFF, Cursor Off
   } // if
   if ((hw_status > 0) && !err)
   {
      err = i2c_stop(PT_CLOSE); // Stop I2C Communication, close PortTalk
      if (err)
      {
         sprintf(s,"Error %d while closing I2C Bus",err);
         MessageBox(NULL,s,"ERROR",MB_OK);
      } // if
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
} //  TMainForm::MenuFileExitClick()
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

void __fastcall TMainForm::MenuOptionsI2CSettingsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Hardware Settings'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   int x1;  // temp. variable representing old I2C HW Base Address
   int x2;  // temp. variable representing new I2C HW Base Address
   char *endp; // temp. pointer for strtol() function
   char s[80]; // temp. array
   int  init_needed = false; // temp. flag, TRUE = Main_Initialisation to be called

   TRegistry *Reg = new TRegistry();
   TI2C_Settings *ptmp;

   ptmp = new TI2C_Settings(this);

   // Get I2C Bus Settings from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);

         x1 = Reg->ReadInteger("I2C_Base");  // Read HW IO address as an int.
         ptmp->HW_Base_Edit->Text = IntToHex(x1,3);

         known_hw_devices            = Reg->ReadInteger("KNOWN_HW_DEVICES");
         ptmp->Hw_devices_Edit->Text = IntToHex(known_hw_devices,3);

         fscl_prescaler              = Reg->ReadInteger("FSCL_PRESCALER");
         ptmp->fscl_combo->ItemIndex = fscl_prescaler;

         led1 = Reg->ReadInteger("LED1");     // Read LED1 from registry
         ptmp->RG1->ItemIndex = led1;         // Set radio-button
         led2 = Reg->ReadInteger("LED2");     // Read LED2 from registry
         ptmp->RG2->ItemIndex = led2;         // Set Radio-button
         led3 = Reg->ReadInteger("LED3");     // Read LED3 from registry
         ptmp->RG3->ItemIndex = led3;         // Set radio-button
         led4 = Reg->ReadInteger("LED4");     // Read LED4 from registry
         ptmp->RG4->ItemIndex = led4;         // Set radio-button

         led1_vis = Reg->ReadInteger("LED1_VIS"); // Read LED1 Visibility
         ptmp->UpDown1->Position = led1_vis;
         led2_vis = Reg->ReadInteger("LED2_VIS"); // Read LED2 Visibility
         ptmp->UpDown2->Position = led2_vis;
         led3_vis = Reg->ReadInteger("LED3_VIS"); // Read LED3 Visibility
         ptmp->UpDown5->Position = led3_vis;
         led4_vis = Reg->ReadInteger("LED4_VIS"); // Read LED4 Visibility
         ptmp->UpDown6->Position = led4_vis;

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM");
         ptmp->Thlim_edit->Text  = AnsiString(ttriac_hlim);
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM");
         ptmp->Tllim_edit->Text  = AnsiString(ttriac_llim);
         cb_i2c_err_msg = Reg->ReadBool("CB_I2C_ERR_MSG");
         ptmp->cb_i2c_err_msg->Checked = cb_i2c_err_msg;

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            strcpy(s,ptmp->HW_Base_Edit->Text.c_str());    // retrieve hex value
            x2 = (int)(strtol(s,&endp,16));                // convert to integer
            if (x2 != x1)
            {
               init_needed = true;
               Reg->WriteInteger("I2C_Base",x2); // save new I2C address
            } // if

            strcpy(s,ptmp->Hw_devices_Edit->Text.c_str()); // retrieve hex value
            known_hw_devices = (int)(strtol(s,&endp,16));  // convert to integer
            Reg->WriteInteger("KNOWN_HW_DEVICES",known_hw_devices);

            if (fscl_prescaler != ptmp->fscl_combo->ItemIndex)
            {
               init_needed    = true;
               fscl_prescaler = ptmp->fscl_combo->ItemIndex;
               Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler);
            } // if

            //---------------------------------------------------------------
            // For the LED Displays: 0=Thlt  , 1=Tmlt, 2=Tset_hlt, 3=Tset_mlt
            //                       4=Ttriac, 5=Vmlt, 6=Vhlt    , 7=gamma
            // Macro, used in TMainForm::T50msec2Timer()
            //---------------------------------------------------------------
            led1 = ptmp->RG1->ItemIndex;
            Reg->WriteInteger("LED1",led1);
            led2 = ptmp->RG2->ItemIndex;
            Reg->WriteInteger("LED2",led2);
            led3 = ptmp->RG3->ItemIndex;
            Reg->WriteInteger("LED3",led3);
            led4 = ptmp->RG4->ItemIndex;
            Reg->WriteInteger("LED4",led4);

            led1_vis = ptmp->Vis1_Edit->Text.ToInt();
            Reg->WriteInteger("LED1_VIS",led1_vis);
            led2_vis = ptmp->Vis2_Edit->Text.ToInt();
            Reg->WriteInteger("LED2_VIS",led2_vis);
            led3_vis = ptmp->Vis3_Edit->Text.ToInt();
            Reg->WriteInteger("LED3_VIS",led3_vis);
            led4_vis = ptmp->Vis4_Edit->Text.ToInt();
            Reg->WriteInteger("LED4_VIS",led4_vis);

            ttriac_hlim = ptmp->Thlim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
            tm_triac->SetPoint->Value = ttriac_hlim;
            ttriac_llim = ptmp->Tllim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);

            cb_i2c_err_msg = ptmp->cb_i2c_err_msg->Checked;
            Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);

            if (init_needed)
            {
               //--------------------------------------------------------
               // New I2C HW Base Address or SCL prescaler was changed,
               // call i2c_stop() and init I2C Bus communication again.
               //--------------------------------------------------------
               if (i2c_stop(PT_CLOSE) != I2C_NOERR)
               {  // i2c bus locked, i2c_stop() did not work
                  MessageBox(NULL,I2C_STOP_ERR_TXT,"ERROR",MB_OK);
                  exit_ebrew(); // Exit ebrew program
               }
               else
               {
                  Main_Initialisation(); // Init all I2C Hardware, ISR and PID controller
               } // else
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

void __fastcall TMainForm::Reset_I2C_Bus(int i2c_bus_id, int err)
/*------------------------------------------------------------------
  Purpose  : This routine reset the I2C bus in case of an error.
             The user needs to turn the power off and then on again
             (of the electronics, NOT the PC!), only when i2c_stop()
             was not successful.
  Returns  : None
  ------------------------------------------------------------------*/
{
   char tmp_str[80];    // temp string for calculations
   char err_txt[40];    // temp string for error message

   switch (err)
   {
      case I2C_BERR:     strcpy(err_txt,I2C_BERR_MSG);
                         break;
      case I2C_BB:       strcpy(err_txt,I2C_BB_MSG);
                         break;
      case I2C_TIMEOUT:  strcpy(err_txt,I2C_TO_MSG);
                         break;
      case I2C_ARGS:     strcpy(err_txt,I2C_ARGS_MSG);
                         break;
      case I2C_PT:       strcpy(err_txt,I2C_PT_MSG);
                         break;
      case I2C_LM92_ERR: strcpy(err_txt,I2C_LM92_MSG);
                         break;
      default:           sprintf(err_txt,"Unknown Error (%d)",err);
                         break;
   }
   sprintf(tmp_str,"%s while accessing I2C device 0x%2x",err_txt,i2c_bus_id);
   if (i2c_stop(PT_OPEN) != I2C_NOERR)
   {  // i2c bus locked, i2c_stop() did not work
      MessageBox(NULL,I2C_STOP_ERR_TXT,tmp_str,MB_OK);
      exit_ebrew(); // Exit ebrew program
   }
   else
   {
      if (cb_i2c_err_msg)
      {
         MessageBox(NULL,"i2c_stop() successful: Press OK button to continue reset process",tmp_str,MB_OK);
      } // if
      Main_Initialisation(); // continue with init. process
   } // else
} // TMainForm::Reset_I2C_bus()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Generate_IO_Signals(void)
/*------------------------------------------------------------------
  Purpose  : This routines does the following (called every 50 msec.)
             1) It calculates a new pulse for Gamma, the triac control.
                Example: Gamma = 60 % => output is 60 times '1' and
                40 times '0'.

                      '1'          |-----------|
                                   |           |
                      '0' |--------|  Gamma    |---------
                             40%       60%
             2) It calculates the Alive bit (blinking LED)
             3) It calculates the Pump bit (control pump on/off)
             4) It calculates the 'Gas Burner On' bit
             5) It sends all signals to the PCF8574 I2C IO device
  Variables: The timer variables in tmr (Unit1.h) are used and updated.
  Returns  : None
  ------------------------------------------------------------------*/
{
   int lsb_io = 0x00; // init. byte to write to DIG_IO_LSB_BASE
   int err    = 0;    // I2C error return value

   switch (tmr.isrstate)
   {
      case 0: //---------------------------------------------------------
              // State 0: Enable heating element
              // Goto 'disable heating element' if triac_too_hot     or
              //      pid output not routed to triac/electric heater or
              //      (timeout_htimer && !100%_gamma)
              //---------------------------------------------------------
              if (triac_too_hot || !(cb_pid_out & PID_OUT_ELECTRIC))
              {
                 lsb_io &= ~HEATERb; // set heater OFF
                 tmr.isrstate = 1;   // go to 'disable heater' state
                 tmr.ltimer   = tmr.time_low; // init. low-timer
              }
              else if (tmr.htimer == 0)
              {  // timer has counted down
                 if (tmr.time_low == 0)
                 {  // indication for 100% gamma, remain in this state
                    tmr.htimer = tmr.time_high; // init. high-timer again
                    lsb_io |= HEATERb; // Set heater ON
                 }
                 else
                 {
                    lsb_io &= ~HEATERb; // set heater OFF
                    tmr.isrstate = 1;   // go to 'disable heater' state
                    tmr.ltimer   = tmr.time_low; // init. low-timer
                 } // else
              } // if
              else
              {  // timer has not counted down yet, continue
                 lsb_io |= HEATERb; // Set heater ON
                 tmr.htimer--;      // decrement high-timer
              } /* else */
              break;

      case 1: //---------------------------------------------------------
              // State 1: Disable heating element
              // Goto 'enable heating element' if timeout_ltimer &&
              //      pid output routed to triac/electric heater &&
              //      !0%_gamma && !triac_too_hot
              //---------------------------------------------------------
              if (tmr.ltimer == 0)
              {  // timer has counted down
                 if (tmr.time_high == 0)
                 {
                    // indication for 0% gamma, remain in this state
                    tmr.ltimer = tmr.time_low; // init. timer again
                    lsb_io &= ~HEATERb; // set heater OFF
                 }
                 else if (!triac_too_hot && (cb_pid_out & PID_OUT_ELECTRIC))
                 {
                    lsb_io |= HEATERb; // set heater ON
                    tmr.isrstate = 0;  // go to 'enable heater' state
                    tmr.htimer   = tmr.time_high; // init. high-timer
                 } // else if
                 // Remain in this state if timeout && !0% && triac_too_hot
              } // if
              else
              {
                 // timer has not counted down yet, continue
                 lsb_io &= ~HEATERb; // Set heater OFF
                 tmr.ltimer--; // decrement low-timer
              } // else
              break;

      default:  tmr.isrstate = 1;
                break;
   } // case

   //------------------------------------------------------------------
   // Calculate alive toggle bit to see if this routine is still alive
   //------------------------------------------------------------------
   if (++tmr.alive_tmr >= ALIVE_TICKS)
   {
     tmr.alive_tmr = 0;          // reset timer
     tmr.alive     = !tmr.alive; // invert alive (bit 1 of IO port)
   } // if
   if (tmr.alive)
   {
      lsb_io &= ~ALIVEb; // Enable alive LED
   }
   else
   {
      lsb_io |= ALIVEb;  // Disable alive LED
   } // else

   //--------------------------------------------
   // Send Pump On/Off signal to DIG_IO_LSB_BASE.
   //--------------------------------------------
   if (std_out & P0b)
   {
      lsb_io |= PUMPb;
   }
   else
   {
      lsb_io &= ~PUMPb;
   } // else

   //-----------------------------------------------------------------
   // Send Non-Modulating Gas-Burner On/Off signal to DIG_IO_LSB_BASE.
   // Activate this only when the PID-output is routed to a non-
   // modulating gas-burner. Disable burner if deselected.
   // Safety: disable gas burner when time_switch is true
   //-----------------------------------------------------------------
   if (time_switch || !(cb_pid_out & PID_OUT_GAS_NON_MOD))
   {
      burner_on = false;
   }
   else
   {   // time-switch is disabled and non-modulating gas-burner is selected
       if (tmr.time_high > pid_pars.burner_hyst_h)
       {
          burner_on = true;
       }
       else if (tmr.time_high < pid_pars.burner_hyst_l)
       {
          burner_on = false;
       } // else if
       // else: do nothing (hysteresis)
   } // else

   if (burner_on)
   {
      lsb_io |= BURNERb; // Fire it up!
   }
   else
   {
      lsb_io &= ~BURNERb; // Disable gas-burner
   } // else

   //-------------------------------------------------
   // Output lsb_io to IO port every 50 msec.
   // (see misc.h for bit definitions)
   // Bits for the Heater and the PUMP are inverted,
   // because they are active low.
   //-------------------------------------------------
   if (hw_status & DIG_IO_LSB_OK)
   {
      lsb_io ^= PUMPb | HEATERb; // Invert PUMP and Heater bits
      err = WriteIOByte(lsb_io,LSB_IO); // Write inverted value to IO port
      if (err) Reset_I2C_Bus(DIG_IO_LSB_BASE,err);
   } // if
} // Generate_IO_Signals()
//---------------------------------------------------------------------------

void __fastcall TMainForm::T50msec2Timer(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the main Timer function which is called every
             50 milliseconds. This can be seen as the main control
             loop of the program. It utilises time-slices to divide
             computations over time.
             After 5 seconds (100 calls) the main control loop
             starts again
  Returns  : None
  ------------------------------------------------------------------*/
{
   int        err = 0;       // error return value, needed for SET_LED macro
   TDateTime  td_now;        // holds current date and time
   double     thlt_unf;      // unfiltered version of thlt
   double     tmlt_unf;      // unfiltered version of tmlt
   adda_t     adc;           // struct containing 4 ADC values + DA value
   double     dac_x;         // temp. variable for calculating DA value

   //--------------------------------------------------------------
   // This is the main control loop, executed once every 50 msec.
   // Do some time-slicing to reduce #computations in one loop.
   // Every time-slice is executed once every TS seconds.
   // tmr.pid_tmr runs from 1 to TWENTY_SECONDS
   //--------------------------------------------------------------

   //----------------------------------------------------------------
   // TIME-SLICE: this time-slice is executed once every second.
   //             Read HLT volume (Vhlt) and call MA filter for Vhlt.
   //----------------------------------------------------------------
   if (tmr.pid_tmr % ONE_SECOND == 1)
   {
       if (vhlt_src != NONE) // get real value from ADC
       {
          volumes.Vhlt = get_analog_input(hw_status, vhlt_src, vhlt_a, vhlt_b, &err);
          if (err) Reset_I2C_Bus(base_adc[vhlt_src],err);
       } // if
       //--------------------------------------------------------------------
       // else: in case Vhlt is simulated and not read from an ADC, the value
       //       is determined in the state transition diagram.
       //--------------------------------------------------------------------
       if (swfx.vhlt_sw)
       {
          volumes.Vhlt = swfx.vhlt_fx;
       }
       else
       {
          volumes.Vhlt = moving_average(&str_vhlt,volumes.Vhlt); // Call MA filter
       }
   } // if time-slice

   //----------------------------------------------------------------
   // TIME-SLICE: this time-slice is executed once every second.
   //             Read MLT volume (Vmlt) and call MA filter for Vmlt.
   //----------------------------------------------------------------
   if (tmr.pid_tmr % ONE_SECOND == 2)
   {
       if (vmlt_src != NONE) // get real value from ADC
       {
          volumes.Vmlt = get_analog_input(hw_status, vmlt_src, vmlt_a, vmlt_b, &err);
          if (err) Reset_I2C_Bus(base_adc[vmlt_src],err);
       } // if
       if (swfx.vmlt_sw)
       {
          volumes.Vmlt = swfx.vmlt_fx;
       }
       else
       {
          volumes.Vmlt = moving_average(&str_vmlt,volumes.Vmlt); // Call MA filter
       }
   } // if time-slice

   //----------------------------------------------------------------------
   // TIME-SLICE: this time-slice is executed once every second. Read
   //             Triac Temperature (Ttriac) and check if it is too high.
   //----------------------------------------------------------------------
   if (tmr.pid_tmr % ONE_SECOND == 3)
   {
       if (ttriac_src != NONE) // get real value from ADC
       {
          ttriac = get_analog_input(hw_status, ttriac_src, ttriac_a, ttriac_b, &err);
          if (err) Reset_I2C_Bus(base_adc[ttriac_src],err);
       } // if
       if (swfx.ttriac_sw)
       {
          ttriac = swfx.ttriac_fx;
       } // if

       //---------------------------------------------------
       // Triac Temperature Protection: hysteresis function
       //---------------------------------------------------
       if (triac_too_hot)
       {
          if (ttriac < ttriac_llim)
          {
             triac_too_hot = false;
          }
       }
       else
       {
          if (ttriac > ttriac_hlim)
          {
             triac_too_hot = true;
          }
       }
   } // if time-slice

   //----------------------------------------------------------------
   // TIME-SLICE: this time-slice is executed once every second.
   //             It reads the LM92 temperature sensor (HLT)
   //----------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 4)
   {
       if (hw_status & LM92_1_OK)
       {
          thlt_unf = lm92_read(0); // Read HLT temp. from LM92 device
          if (thlt_unf == LM92_ERR)
          {
             Reset_I2C_Bus(LM92_1_BASE, I2C_LM92_ERR);
          } // if
          else
          {
             thlt_unf += thlt_offset; // add calibration offset
             thlt = moving_average(&str_thlt,thlt_unf); // MA-filter
          } // else
       } // if
       if (swfx.thlt_sw)
       {
          thlt = (double)swfx.thlt_fx;
       } // if
   } // else if

   //----------------------------------------------------------------
   // TIME-SLICE: this time-slice is executed once every second.
   //             It reads the LM92 temperature sensor (MLT)
   //----------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 5)
   {
       if (hw_status & LM92_2_OK)
       {
          tmlt_unf = lm92_read(1); // Read MLT temp. from LM92 device
          if (tmlt_unf == LM92_ERR)
          {
             Reset_I2C_Bus(LM92_2_BASE, I2C_LM92_ERR);
          } // if
          else
          {
             tmlt_unf += tmlt_offset; // add calibration offset
             tmlt = moving_average(&str_tmlt,tmlt_unf); // MA-filter
          } // else
       } // if
       if (swfx.tmlt_sw)
       {
          tmlt = (double)swfx.tmlt_fx;
       } // if
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Time switch controller. Enable PI controller
   // [TS sec.]   if current date & time matches the date & time set.
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % pid_pars.ts_ticks == 6)
   {
      // Only useful if PID controller is disabled AND time_switch is enabled
      if ((PID_RB->ItemIndex != 1) && (time_switch == 1))
      {
         td_now = Now(); // Get Current Date and Time
         if ((td_now >= dt_time_switch) && (td_now < dt_time_switch + ONE_MINUTE))
         {
            PID_RB->ItemIndex = 1; // Enable PID Controller
         }
      } // if

      // PID_RB->ItemIndex = 1 => PID Controller On
      switch (pid_pars.pid_model)
      {
         case 0 : pid_reg1(thlt,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex);
                  break; // First ebrew PID controller
         case 1 : pid_reg2(thlt,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex);
                  break; // Updated ebrew PID controller
         case 2 : pid_reg3(thlt,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex);
                  break; // Allen Bradley Type A controller
         case 3 : pid_reg4(thlt,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex);
                  break; // Allen Bradley Type C controller
         default: pid_reg2(thlt,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex);
                  break;
      } // switch
      if (swfx.gamma_sw)
      {
         gamma = swfx.gamma_fx; // fix gamma
      } // if

      //--------------------------------------------------------------------
      // Now calculate high and low time for the timers
      // The Gamma is a value between 0-100%. The Gamma signal has a
      // period of 5 seconds, which is 100 * 50 msec.
      // Therefore every percent corresponds to one period of 50 msec.
      // These timer values are needed for Generate_IO_Signals();
      //--------------------------------------------------------------------
      tmr.time_high = (int)gamma;
      tmr.time_low  = 100 - tmr.time_high;
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Output values to DA-Converter to create a PWM signal for
   //             the modulating gas-burner
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 7)
   {
       //---------------------------------------------------------------------
       // Send Modulating Gas-Burner On/Off signal to the DAC of the PCF8591.
       // Activate this only when the PID-output is routed to a modulating
       // gas-burner. Disable burner if deselected.
       // Safety: disable gas burner when time_switch is true
       //---------------------------------------------------------------------
       if (!time_switch && (cb_pid_out & PID_OUT_GAS_MODULATE) && (hw_status & ADDA_OK))
       {
          dac_x   = gamma * dac_a + dac_b; // convert from [0..100%] to value for DAC
          if (dac_x > 255.0)
          {
             adc.dac = 255;
          }
          else if (dac_x < 0.0)
          {
             adc.dac = 0;
          }
          else
          {
             adc.dac = (byte)dac_x;
          } // else
          err = read_adc(&adc);                  // Send value to DA-Converter
          if (err) Reset_I2C_Bus(ADDA_BASE,err); // Reset if necessary
       } // if
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Output values to I2C LED Display every second
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 8)
   {
      // Now update the LEDs with the proper values by calling macro SET_LED
      //      HW_BIT, which display, which var., Visibility, LEDx_BASE
      SET_LED(LED1_OK,1,led1,led1_vis,LED1_BASE);
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Output values to I2C LED Display every second
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 9)
   {
      SET_LED(LED2_OK,2,led2,led2_vis,LED2_BASE);
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Output values to I2C LED Display every second
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 10)
   {
      SET_LED(LED3_OK,3,led3,led3_vis,LED3_BASE);
   } // else if

   //-----------------------------------------------------------------------
   // TIME-SLICE: Output values to I2C LED Display every second
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 11)
   {
      SET_LED(LED4_OK,4,led4,led4_vis,LED4_BASE);
   } // else if

   //--------------------------------------------------------------------------
   // TIME-SLICE: Calculate State Transition Diagram (STD) and determine
   //             new settings of the valves (every second)
   //--------------------------------------------------------------------------
   else if (tmr.pid_tmr % ONE_SECOND == 12)
   {
      int std_tmp;
      if (swfx.std_sw)
      {
         std_tmp = swfx.std_fx;
      }
      else
      {
         std_tmp = -1;
      }
      update_std(&volumes, tmlt, thlt, &tset_mlt, &tset_hlt, &std_out,
                 ms, &sp, &std, PID_RB->ItemIndex, std_tmp);

      if (swfx.tset_hlt_sw)
      {
         tset_hlt = swfx.tset_hlt_fx; // fix tset_hlt
      } // if
      //-----------------------------------------------------------------
      // Now output all valve bits to DIG_IO_MSB_BASE (if it is present).
      // NOTE: The pump bit is output to DIG_IO_LSB_IO!
      //-----------------------------------------------------------------
      if (hw_status & DIG_IO_MSB_OK)
      {
         err = WriteIOByte((byte)(std_out & 0x00FE),MSB_IO);
         if (err) Reset_I2C_Bus(DIG_IO_MSB_BASE,err);
      } // if
   } // else if

   //--------------------------------------------------------------------------
   // Reset timer if necessary
   //--------------------------------------------------------------------------
   if (++tmr.pid_tmr > TWENTY_SECONDS)
   {
      tmr.pid_tmr = 1; // reset timer
   } // else if
   //--------------------------------------------------------------------------
   // END OF TIME-SLICES
   //--------------------------------------------------------------------------

   //------------------------------------------------
   // Calculate IO signals (done every 50 msec.)
   // - Gamma (triac) pulse
   // - Alive bit (LED)
   // - Pump bit
   // - Gas burner bit
   // - Output value to PCF874 IO port every 50 msec.
   //------------------------------------------------
   Generate_IO_Signals();
} // TMainForm::T50msecTimer()
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
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewData_graphsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'View | Data (Graphs)'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   ShowDataGraphs->Show(); // Show modeless Dialog
}
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

void __fastcall TMainForm::ebrew_idle_handler(TObject *Sender, bool &Done)
{
   char tmp_str[80];   // temp string for calculations
   int  p;             // temp. variable
   int  i;             // temp. variable

   sprintf(tmp_str,"%4.2f",thlt);   // Display Thlt value on screen
   Val_Thlt->Caption    = tmp_str;
   tm_hlt->Value->Value = thlt;     // update HLT thermometer object

   sprintf(tmp_str,"%4.2f",tmlt);   // Display Tmlt value on screen
   Val_Tmlt->Caption    = tmp_str;
   tm_mlt->Value->Value = tmlt;     // update MLT thermometer object

   tm_triac->Value->Value = ttriac; // Update thermometer object
   sprintf(tmp_str,"%2.0f",ttriac);
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

   // Heater object shows kW (max=3), gamma = 100 % => PHEATER [W]
   Heater->Value = (gamma * PHEATER) / 100; // Update object on screen

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
      sprintf(tmp_str,"%6.2f %6.2f %6.2f %6.2f %6.2f",pid_pars.pp,
                                                     pid_pars.pi,
                                                     pid_pars.pd,
                                                     pid_pars.pp+pid_pars.pi+pid_pars.pd,
                                                     gamma);
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
   // Stop all I2C bus communication, then restart and print all devices found.
   //---------------------------------------------------------------------------
   if (i2c_stop(PT_OPEN) != I2C_NOERR)
   {  // i2c bus locked, i2c_stop() did not work
      Application->MessageBox(I2C_STOP_ERR_TXT,"ERROR",MB_OK);
      exit_ebrew(); // Exit ebrew program
   }
   else
   {
      Start_I2C_Communication(-1); // print all I2C devices found
      if (i2c_stop(PT_OPEN) != I2C_NOERR)
      {  // i2c bus locked, i2c_stop() did not work
         Application->MessageBox(I2C_STOP_ERR_TXT,"ERROR",MB_OK);
         exit_ebrew(); // Exit ebrew program
      }
   } // else
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
   int vhlt_start_old; // previous value of Vhlt_start;

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
         ptmp->UD_MA_HLT->Position      = Reg->ReadInteger("MA_THLT");
         ptmp->Thlt_Offset->Text        = Reg->ReadFloat("THLT_OFFSET");
         //------------------
         // MLT Temperature
         //------------------
         ptmp->UD_MA_MLT->Position      = Reg->ReadInteger("MA_TMLT");
         ptmp->Tmlt_Offset->Text        = Reg->ReadFloat("TMLT_OFFSET");
         //------------------
         // HLT Volume
         //------------------
         ptmp->UpDown1->Position        = Reg->ReadInteger("MA_VHLT");
         volumes.Vhlt_start             = Reg->ReadInteger("VHLT_START");
         vhlt_start_old                 = volumes.Vhlt_start; // save value
         ptmp->Vhlt_init_Edit->Text     = AnsiString(volumes.Vhlt_start);
         vhlt_src                       = (enum i2c_adc)Reg->ReadInteger("VHLT_SRC");
         ptmp->Vhlt_src->ItemIndex      = vhlt_src;
         vhlt_a                         = Reg->ReadFloat("VHLT_A");
         ptmp->Vhlt_a->Text             = vhlt_a;
         vhlt_b                         = Reg->ReadFloat("VHLT_B");
         ptmp->Vhlt_b->Text             = vhlt_b;
         //------------------
         // MLT Volume
         //------------------
         ptmp->UpDown4->Position        = Reg->ReadInteger("MA_VMLT");
         vmlt_src                       = (enum i2c_adc)Reg->ReadInteger("VMLT_SRC");
         ptmp->Vmlt_src->ItemIndex      = vmlt_src;
         vmlt_a                         = Reg->ReadFloat("VMLT_A");
         ptmp->Vmlt_a->Text             = vmlt_a;
         vmlt_b                         = Reg->ReadFloat("VMLT_B");
         ptmp->Vmlt_b->Text             = vmlt_b;
         //-------------------
         // Boil Kettle Volume
         //-------------------
         volumes.Vboil_simulated        = Reg->ReadBool("VBOIL_SIMULATED");
         ptmp->Vboil_simulated->Checked = volumes.Vboil_simulated;
         //------------------
         // Triac Temperature
         //------------------
         ttriac_src                     = (enum i2c_adc)Reg->ReadInteger("TTRIAC_SRC");
         ptmp->Ttriac_src->ItemIndex    = ttriac_src;
         ttriac_a                       = Reg->ReadFloat("TTRIAC_A");
         ptmp->Ttriac_a->Text           = ttriac_a;
         ttriac_b                       = Reg->ReadFloat("TTRIAC_B");
         ptmp->Ttriac_b->Text           = ttriac_b;

         ptmp->Vhlt_srcChange(this);   // enable/disable fields in form
         ptmp->Vmlt_srcChange(this);   // enable/disable fields in form

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            //------------------
            // HLT Temperature
            //------------------
            init_ma(&str_thlt,ptmp->UD_MA_HLT->Position,thlt); // order of MA filter
            Reg->WriteInteger("MA_THLT",ptmp->UD_MA_HLT->Position);
            thlt_offset = ptmp->Thlt_Offset->Text.ToDouble();
            Reg->WriteFloat("THLT_OFFSET",thlt_offset);
            //------------------
            // MLT Temperature
            //------------------
            init_ma(&str_tmlt,ptmp->UD_MA_MLT->Position,tmlt); // order of MA filter
            Reg->WriteInteger("MA_TMLT",ptmp->UD_MA_MLT->Position);
            tmlt_offset = ptmp->Tmlt_Offset->Text.ToDouble();
            Reg->WriteFloat("TMLT_OFFSET",tmlt_offset);
            //------------------
            // HLT Volume
            //------------------
            init_ma(&str_vhlt,ptmp->MA_HLT_Edit->Text.ToInt(),volumes.Vhlt); // order of MA filter
            Reg->WriteInteger("MA_VHLT",ptmp->MA_HLT_Edit->Text.ToInt());
            volumes.Vhlt_start = ptmp->Vhlt_init_Edit->Text.ToInt();
            Reg->WriteInteger("VHLT_START",volumes.Vhlt_start);
            vhlt_src = (enum i2c_adc)ptmp->Vhlt_src->ItemIndex;
            Reg->WriteInteger("VHLT_SRC",vhlt_src);
            volumes.Vhlt_simulated = vhlt_src;
            if ((vhlt_src == 0) && (volumes.Vhlt_start != vhlt_start_old))
            {  // Update Vhlt if start value has changed and Vhlt is simulated
               volumes.Vhlt += volumes.Vhlt_start - vhlt_start_old;
            } // if
            vhlt_a = ptmp->Vhlt_a->Text.ToDouble();
            Reg->WriteFloat("VHLT_A",vhlt_a);
            vhlt_b = ptmp->Vhlt_b->Text.ToDouble();
            Reg->WriteFloat("VHLT_B",vhlt_b);
            //------------------
            // MLT Volume
            //------------------
            init_ma(&str_vmlt,ptmp->NMA_edit->Text.ToInt(),volumes.Vmlt); // order of MA filter
            Reg->WriteInteger("MA_VMLT",ptmp->NMA_edit->Text.ToInt());
            vmlt_src = (enum i2c_adc)ptmp->Vmlt_src->ItemIndex;
            Reg->WriteInteger("VMLT_SRC",vmlt_src);
            vmlt_a = ptmp->Vmlt_a->Text.ToDouble();
            Reg->WriteFloat("VMLT_A",vmlt_a);
            vmlt_b = ptmp->Vmlt_b->Text.ToDouble();
            Reg->WriteFloat("VMLT_B",vmlt_b);
            //-------------------
            // Boil Kettle Volume
            //-------------------
            volumes.Vboil_simulated = ptmp->Vboil_simulated->Checked;
            Reg->WriteBool("VBOIL_SIMULATED",volumes.Vboil_simulated);
            //------------------
            // Triac Temperature
            //------------------
            ttriac_src = (enum i2c_adc)ptmp->Ttriac_src->ItemIndex;
            Reg->WriteInteger("TTRIAC_SRC",ttriac_src);
            ttriac_a = ptmp->Ttriac_a->Text.ToDouble();
            Reg->WriteFloat("TTRIAC_A",ttriac_a);
            ttriac_b = ptmp->Ttriac_b->Text.ToDouble();
            Reg->WriteFloat("TTRIAC_B",ttriac_b);

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


