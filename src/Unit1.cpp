// ==========================================================================
// Filename    : Unit1.cpp
// Author      : Emile
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).
// --------------------------------------------------------------------------
// Revision 1.94  2018/11/24
// - Audio alarm added for sensors, new command-line argument (Xx) added
// - Version works with firmware r1.34.
//
// Revision 1.93  2018/06/17
// - Bug-fix MLT set-point temp. not set to offset0 in state RDY_TO_ADD_MALT
// - Bug-fix Boiling start-time not shown in Mash progress screen
//
// Revision 1.92  2018/05/11
// - Toffset0 Reg. Var. added to compensate for mash dough-in temperature loss
// - CB_Boil_Rest Reg. Var. added to select 5 min. rest after boiling
// - CB_pumps_on Reg. Var. added to select pumps running after MLT Temp. reached
//
// Revision 1.91  2018/01/03
// - Bug-fix: CB_dpht and HLT_Bcap now also added to MainForm constructor
// - flow_rate_low() now checks if flow-sensor gives a good reading. If not,
//   measurement is repeated. Sometimes the CFC flow-sensor gets stuck with
//   hop particles in the beginning. This should prevent a switch-off.
//
// Revision 1.90  2017/08/03
// - Added dynamic preheat timing instead of only fixed timing
// - Two new Registry variables: CB_dpht and HLT_Bcap
// - State labels extended with more information
// - Feed-forward control instead of PID control in CIP state HEAT_UP
//
// Revision 1.89  2017/07/27
// - Switched from CVS to GIT version control
// - All CVS logs in headers removed (except this one)
// - View Mash Progress Form made larger to prevent roll-over bug
//
// Revision 1.88  2017/02/17 13:29:25  Emile
// - CIP cleaning inputs and outputs now per valve instead of all together.
//
// Revision 1.87  2016/10/09 12:44:45  Emile
// - Bugfix from brewsession 153: Temp. offset were not added to temps. Corrected.
// - First version of CIP STD added. TODO: add parameter menu instead of hardcoded values.
// - Several minor changes
//
// Revision 1.86  2016/09/23 09:51:55  Emile
// - Bug-fix: Switches/Fixes for Tset_boil, gamma_boil, Tboil and Vboil now work properly.
// - Separate key (Q) for Pump 2 instead of one key (P) for both pumps.
// - Added: All valves off (V0 command) when exiting program.
// - Boiling-detection added. New Registry variable BOIL_DETECT, adjustable in Sparge, Mash
//   & Boil Settings Dialog Box.
// - Flowrate low detection also added for boil-kettle to fermentation bin. New Registry
//   variables added: MIN_FR_MLT_PERC and MIN_FR_BOIL_PERC, adjustable in Measurements dialog box.
//
// Revision 1.85  2016/08/07 14:26:43  Emile
// - Version works with firmware r1.29.
// - Pump 2 (HLT heat-exchanger) support added in Px command
// - V8 now also works, action bits in STD reorganised for this.
// - Tboil Ref. Temp. is now properly set (bug-fix from session 151).
// - New Registry parameter BOIL_MIN_TEMP instead of hard-coded value.
//
// Revision 1.84  2016/06/11 16:57:28  Emile
// - Indy UDP components removed. udp routines added.
// - Ethernet / UDP communication now works.
//
// Revision 1.83  2016/05/22 13:51:16  Emile
// Bugfixes brewing session 21-05-'16 with v3.30 PCB and HW r1.27
// - Temp.sensor error value is now '-99.99'
// - Double updates to Vboil removed in std
// - HLT and MLT thermometer objects removed
//
// Revision 1.82  2016/05/15 14:20:45  Emile
// - Logfile updated with new volumes and temperatures + date added.
//
// Revision 1.81  2016/04/17 13:00:54  Emile
// - Version after Integration Testing. Works with firmware R1.26.
// - Bug-fix PID-controller.
// - Temps (A0) and Flows (A9) now combined instead of separate tasks.
//
// Revision 1.80  2016/04/09 12:58:50  Emile
// - First version for new V3.30 PCB HW. Now support 4 temperatures, 4 flowsensors
//   and Boil-Kettle PID-Controller. Various changes to User Interface, Registry
//   parameters and scheduler/tasks.
// - Only 6 parameters left to send to HW. In line with firmware R1.23.
// - New switched/fixes added for tset_boil, gamma_boil and Tboil.
//
// Revision 1.79  2016/01/24 19:36:55  Emile
// - Valves and Pump now show colours: RED (on) and GREEN (off)
// - Pipes are now highlighted to show actual direction of fluid movement
// - Initial delay of tasks changed to multiples of 100 msec. scheduler tick
// - Mash-rest in new state now set to 5 min. instead of 10 min.
//
// Revision 1.78  2015/12/13 14:20:27  Emile
// - Size of all 3 brew-kettles now adjustable. New Reg. par. VBOIL_MAX added.
// - New 'Mash_Rest' checkbox added to 'Sparge & Mash Settings. New. Reg. par.
//   CB_Mash_Rest. New state 18 'Mast Rest (10 minutes)' added to STD.
// - Pump and Valves are now all off in state 'Add Malt to MLT'.
// - Statusbar now also shows mash and sparge litres (valves indicators removed).
// - Auto-All option added to set all valves and the pump to Auto when 'A' pressed.
//
// Revision 1.77  2015/11/21 10:36:38  Emile
// - Task-time was > 50 msec. resulting in inaccurate timing. All sleeps(20)
//   removed in communication routines.
//
// Revision 1.76  2015/08/07 10:59:42  Emile
// - HW revision number now also written to logfile
// - 'Check I2C Hardware' removed from menu-bar, is no longer used
// - Update to HW Settings Dialog screen: unused items are removed
// - Communication Read Timeout now set to 30 msec.
//
// Revision 1.75  2015/07/21 19:42:45  Emile
// - Setting Mash- and Sparge Volume now via maisch.sch and not in Dialog screen anymore.
// - Flow-rate indicators added (HLT->MLT and MLT->Boil) to Main-Screen.
// - Transition from 'Empty MLT' to 'Wait for Boil' now detected automatically with
//   new function flow_rate_low().
// - Registry vars VMLT_EMPTY, MASH_VOL and SPARGE_VOL removed.
// - Functionality and Checkbox for 'Double initial Sparge Volume' added.
// - Registry var CB_VSP2 added.
//
// Revision 1.74  2015/06/28 12:19:41  Emile
// - All temperature reading tasks from 1 -> 2 seconds
// - Comm port debug logging improved
// - Q8.7 format for Thlt and Tmlt parameters (instead of Q8.4)
// - Read retries added to reading temperatures
//
// Revision 1.73  2015/06/09 21:00:21  Emile
// Bugfix STD: Valve V4 was ON in state 14, should be OFF. Now corrected.
//
// Revision 1.72  2015/06/06 14:02:33  Emile
// - User Interaction now with PopupMenu to State-label
// - PID Controller now made with a TvrPowerButton instead of a radiobutton box
// - View Mash Progress Form improved
//
// Revision 1.71  2015/06/05 19:18:40  Emile
// - STD optimized for new solenoid valves. User Interaction dialog added
// - Calibration & Temp. correction added for flowsensors
//
// Revision 1.70  2015/05/11 13:22:07  Emile
// - Comm. debug log now with timestamps
// - Communication with HW (R1.15) made more robust
//
// Revision 1.69  2015/03/21 11:12:51  Emile
// - Bugfix W command (PWM output), W0 was always sent
// - V command enabled (Valves output)
// - scheduler.h and scheduler.c added to CVS library
//
// Revision 1.68  2015/03/21 09:27:21  Emile
// - Vboil_simulated removed, VHLT_START added
// - task_read_vmlt_boil() with command A6 added (works with ebrew HW R1.12)
// - task_read_vhlt_vmlt() with command A5 added
// - Flow1_hlt_mlt and Flow2_mlt_boil objects added to main-screen
// - New Registry var USE_FLOWSENSORS. Switches between tasks 03/04 and 03F/04F
//
// Revision 1.67  2014/10/26 12:50:12  Emile
// - In case of an I2C HW error, the font-colour for Thlt and Tmlt will be red
//   and the old temperature is displayed. With this, sensors can be hot-swapped!
// - This version works with Ebrew HW R1.11.
//
// Revision 1.66  2014/06/01 13:59:16  Emile
// - Ethernet UDP Communication added.
// - New Registry variable UDP_IP_PORT and USB_COM_PORT renamed in COMM_CHANNEL
// - This version works with Ebrew HW R1.9
//
// Revision 1.65  2013/07/24 14:08:57  Emile
// - Bug-fix version numbering in statusbar.
//
// Revision 1.64  2013/07/24 14:00:00  Emile
// - Version ready for Integration Testing with Ebrew HW R1.07!
// - Writing parameters to Ebrew HW now works with new task writing_pars
// - HW Revision is now displayed in statusbar if comm. is established
// - Task hw_debug added to display S1, S2 & S3 command info from Ebrew HW.
// - Key-press 'D' now initiates hw_debug task
//
// Revision 1.63  2013/07/23 09:42:46  Emile
// - Fourth intermediate version: several Registry Settings added / removed.
// - Dialog Screens updated: better lay-out and matches new Registry Settings
// - Source-code improved for readability
//
// Revision 1.62  2013/07/21 22:32:47  Emile
// - 3rd intermediate version to support ebrew 2.0 rev.1.5 hardware
// - Changes to Measurement Dialog Screen: VHLT, VMLT, THLT, TMLT
// - Registry: several parameters removed + parameters renamed
// - Ttriac & DAC code & parameters removed
//
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
extern flow_rate_low_struct frl_empty_boil; // Needed for detection of low flowrate

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AnimTimer"
#pragma link "VrControls"
#pragma link "VrTank"
#pragma link "VrThermoMeter"
#pragma link "VrPowerMeter"
#pragma link "VrLeds"
#pragma link "VrButtons"
#pragma link "VrBanner"
#pragma link "VrGradient"
#pragma link "VrAnimate"
#pragma resource "*.dfm"

TMainForm *MainForm;

// GLOBAL VARIABLES for COM Port Communication
HANDLE       hComm = NULL;
COMMTIMEOUTS ctmoNew = {0}, ctmoOld;
FILE         *fdbg_com; // COM-port debug file-descriptor

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Read all 5 Temperature values from hardware
  Period-Time: 2 seconds
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void task_read_temps(void)
{
    char       s[MAX_BUF_READ];
    const char s_exp[] = "T=";
    int        x = 0;
    bool       str_ok = false;
    char       *p;
    double     temp1, temp2, temp3, temp4, temp5;

    MainForm->comm_port_write("A0\n"); // A0 = Read All Temperatures
    do
    {
        MainForm->comm_port_read(s); // Read Temperatures from Ebrew hardware
    } while ((++x < MAX_READ_RETRIES) && strncmp(s,s_exp,2));
    // Check string received for header and length "T=0.00,0.00,0.00,0.00,0.00"
    if ((!strncmp(s,s_exp,2)) && (strlen(s) >= 26)) str_ok = true;
    if (str_ok)
    {
        p = strtok(&s[2],","); temp1 = atof(p); // Ttriac
        p = strtok(NULL ,","); temp2 = atof(p); // Thlt
        p = strtok(NULL ,","); temp3 = atof(p); // Tmlt
        p = strtok(NULL ,","); temp4 = atof(p); // Tboil
        p = strtok(NULL ,","); temp5 = atof(p); // Tcfc
    } // if
    //------------------ TEMP1 (LM35) -----------------------------------------
    if (str_ok)
    {
         MainForm->Ttriac_lbl->Font->Color = clLime;
         MainForm->ttriac = temp1; // Update Ttriac with new value
    }
    else MainForm->Ttriac_lbl->Font->Color = clRed;
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
    //------------------ TEMP2 (THLT) -----------------------------------------
    if (str_ok && (temp2 > SENSOR_VAL_LIM_OK))
    {
         MainForm->Val_Thlt->Font->Color = clLime;
         MainForm->thlt = temp2 + MainForm->thlt_offset; // update THLT with new value
    } // if
    else
    {
         MainForm->Val_Thlt->Font->Color = clRed; // + do NOT update THLT
         if ((MainForm->no_sound == ALARM_TEMP_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS))
             MainForm->comm_port_write("X3\n");
    } // else
    if (MainForm->swfx.thlt_sw)
    {  // Switch & Fix
       MainForm->thlt = (double)(MainForm->swfx.thlt_fx);
    } // if
    //------------------ TEMP3 (TMLT) -----------------------------------------
    if (str_ok && (temp3 > SENSOR_VAL_LIM_OK))
    {
         MainForm->Val_Tmlt->Font->Color = clLime;
         MainForm->tmlt = temp3 + MainForm->tmlt_offset; // update TMLT with new value
    }
    else
    {
        MainForm->Val_Tmlt->Font->Color = clRed; // + do NOT update TMLT
        if ((MainForm->no_sound == ALARM_TEMP_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS))
            MainForm->comm_port_write("X3\n");
    } // else
    if (MainForm->swfx.tmlt_sw)
    {  // Switch & Fix
       MainForm->tmlt = (double)(MainForm->swfx.tmlt_fx);
    } // if
    //------------------ TEMP4 (TBOIL) ----------------------------------------
    if (str_ok && (temp4 > SENSOR_VAL_LIM_OK))
    {
         MainForm->Temp_Boil->Font->Color = clLime;
         MainForm->tboil = temp4 + MainForm->tboil_offset; // update TBOIL with new value
    }
    else
    {
         MainForm->Temp_Boil->Font->Color = clRed; // + do NOT update TBOIL
         if ((MainForm->no_sound == ALARM_TEMP_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS))
             MainForm->comm_port_write("X3\n");
    } // else
    if (MainForm->swfx.tboil_sw)
    {  // Switch & Fix
       MainForm->tboil = (double)(MainForm->swfx.tboil_fx);
    } // if
    //------------------ TEMP5 (TCFC) -----------------------------------------
    if (str_ok && (temp5 > SENSOR_VAL_LIM_OK))
    {
         MainForm->Temp_CFC->Font->Color = clLime;
         MainForm->tcfc = temp5 + MainForm->tcfc_offset; // update TCFC with new value
    }
    else
    {
         MainForm->Temp_CFC->Font->Color = clRed; // + do NOT update TCFC
         if ((MainForm->no_sound == ALARM_TEMP_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS))
             MainForm->comm_port_write("X3\n");
    } // else
    // No switch/fix needed for TCFC
} // task_read_temps()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Read all 4 flowsensor values from hardware
  Period-Time: 2 seconds
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_read_flows(void)
{
    char       s[MAX_BUF_READ];
    const char s_exp[] = "F=";
    double     err, temp;
    int        x = 0;
    char       *p;
    bool       str_ok = false;

    MainForm->comm_port_write("A9\n"); // A9 = Read all Flowsensor values
    do
    {
        MainForm->comm_port_read(s); // Read flowsensor
    } while ((++x < MAX_READ_RETRIES) && strncmp(s,s_exp,2));
    // Check string received for header and length "F=0.00,0.00,0.00,0.00"
    if ((!strncmp(s,s_exp,2)) && (strlen(s) >= 21)) str_ok = true;
    MainForm->volumes.Flow_hlt_mlt_old  = MainForm->volumes.Flow_hlt_mlt;
    MainForm->volumes.Flow_mlt_boil_old = MainForm->volumes.Flow_mlt_boil;
    MainForm->volumes.Flow_cfc_out_old  = MainForm->volumes.Flow_cfc_out;
    MainForm->volumes.Flow4_old         = MainForm->volumes.Flow4;
    if (str_ok)
    {
        p = strtok(&s[2],","); MainForm->volumes.Flow_hlt_mlt  = atof(p);
        p = strtok(NULL ,","); MainForm->volumes.Flow_mlt_boil = atof(p);
        p = strtok(NULL ,","); MainForm->volumes.Flow_cfc_out  = atof(p);
        p = strtok(NULL ,","); MainForm->volumes.Flow4         = atof(p);
        MainForm->Flow1_hlt_mlt->Font->Color  = clLime;
        MainForm->Flow2_mlt_boil->Font->Color = clLime;
        MainForm->Flow3_cfc->Font->Color      = clLime;
        MainForm->Flow4->Font->Color          = clLime;
    } // if
    else
    {
        MainForm->Flow1_hlt_mlt->Font->Color  = clRed;
        MainForm->Flow2_mlt_boil->Font->Color = clRed;
        MainForm->Flow3_cfc->Font->Color      = clRed;
        MainForm->Flow4->Font->Color          = clRed;
    } // else
    //------------------ FLOW1 ------------------------------------------------
    if (MainForm->flow_temp_corr)
    {   // Apply correction for increased volume at higher temperatures
        err = (1.0 + 0.00021 * (MainForm->thlt - 20.0));
        MainForm->volumes.Flow_hlt_mlt /= err;
    } // if
    // Now apply Calibration error correction
    MainForm->volumes.Flow_hlt_mlt *= 1.0 + 0.01 * MainForm->flow1_err;
    // Calculate Flow-rate in L per minute ; this task is called once per 2 seconds
    temp = 30.0 * (MainForm->volumes.Flow_hlt_mlt - MainForm->volumes.Flow_hlt_mlt_old);
    if ((temp < 0.1) && MainForm->flow1_running &&
        ((MainForm->no_sound == ALARM_FLOW_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS)))
    {
        MainForm->comm_port_write("X2\n"); // sound alarm
    } // if
    MainForm->volumes.Flow_rate_hlt_mlt = moving_average(&MainForm->flow1_ma,temp);
    // Calculate VHLT volume here
    MainForm->volumes.Vhlt = MainForm->vhlt_max - MainForm->volumes.Flow_hlt_mlt;
    if (MainForm->swfx.vhlt_sw)
    {  // Switch & Fix
       MainForm->volumes.Vhlt = MainForm->swfx.vhlt_fx;
    } // if
    //------------------ FLOW2 ------------------------------------------------
    if (MainForm->flow_temp_corr)
    {   // Apply correction for increased volume at higher temperatures
        err = (1.0 + 0.00021 * (MainForm->tmlt - 20.0));
        MainForm->volumes.Flow_mlt_boil /= err;
    } // if
    // Now apply Calibration error correction
    MainForm->volumes.Flow_mlt_boil *= 1.0 + 0.01 * MainForm->flow2_err;
    // Calculate Flow-rate in L per minute ; this task is called once per 2 seconds
    temp = 30.0 * (MainForm->volumes.Flow_mlt_boil - MainForm->volumes.Flow_mlt_boil_old);
    if ((temp < 0.1) && MainForm->flow2_running &&
        ((MainForm->no_sound == ALARM_FLOW_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS)))
    {
        MainForm->comm_port_write("X2\n"); // sound alarm
    } // if
    MainForm->volumes.Flow_rate_mlt_boil = moving_average(&MainForm->flow2_ma,temp);
    // Calculate VMLT volume here
    MainForm->volumes.Vmlt = MainForm->volumes.Flow_hlt_mlt  -
                             MainForm->volumes.Flow_mlt_boil;
    if (MainForm->swfx.vmlt_sw)
    {  // Switch & Fix
       MainForm->volumes.Vmlt = MainForm->swfx.vmlt_fx;
    } // if
    //------------------ FLOW3 ------------------------------------------------
    if (MainForm->flow_temp_corr)
    {   // Apply correction for increased volume at higher temperatures
        err = (1.0 + 0.00021 * (MainForm->tcfc - 20.0));
        MainForm->volumes.Flow_cfc_out /= err;
    } // if
    // Now apply Calibration error correction
    MainForm->volumes.Flow_cfc_out *= 1.0 + 0.01 * MainForm->flow3_err;
    // Set Flow_cfc_out to 0 by subtracting reset value (set in STD when Boiling is finished)
    MainForm->volumes.Flow_cfc_out -= MainForm->volumes.Flow_cfc_out_reset_value;
    // Calculate Flow-rate in L per minute ; this task is called once per 2 seconds
    temp = 30.0 * (MainForm->volumes.Flow_cfc_out - MainForm->volumes.Flow_cfc_out_old);
    if ((temp < 0.1) && MainForm->flow3_running &&
        ((MainForm->no_sound == ALARM_FLOW_SENSORS) || (MainForm->no_sound == ALARM_TEMP_FLOW_SENSORS)))
    {
        MainForm->comm_port_write("X2\n"); // sound alarm
    } // if
    MainForm->volumes.Flow_rate_cfc_out = moving_average(&MainForm->flow3_ma,temp);
    // Calculate VBOIL volume here
    MainForm->volumes.Vboil = MainForm->volumes.Flow_mlt_boil -
                              MainForm->volumes.Flow_cfc_out;
    if (MainForm->swfx.vboil_sw)
    {  // Switch & Fix
       MainForm->volumes.Vboil = MainForm->swfx.vboil_fx;
    } // if
    //------------------ FLOW4 ------------------------------------------------
    if (MainForm->flow_temp_corr)
    {   // Apply correction for increased volume at higher temperatures
        err = (1.0 + 0.00021 * (MainForm->tmlt - 20.0));
        MainForm->volumes.Flow4 /= err;
    } // if
    // Now apply Calibration error correction
    MainForm->volumes.Flow4 *= 1.0 + 0.01 * MainForm->flow4_err;
    // Calculate Flow-rate in L per minute
    temp = 30.0 * (MainForm->volumes.Flow4 - MainForm->volumes.Flow4_old);
    MainForm->volumes.Flow_rate4 = moving_average(&MainForm->flow4_ma,temp);
} // task_read_flows()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Run PID Controller and generate Gamma value [0%..100%]
  Period-Time: Controlled by Parameter TS [sec.]
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_pid_ctrl(void)
{
    char      s[MAX_BUF_READ];
    TDateTime td_now; // holds current date and time

    // Only useful if PID controller is disabled AND time_switch is enabled
    if (!MainForm->PID_Ctrl->Active && (MainForm->time_switch == 1))
    {
       td_now = Now(); // Get Current Date and Time
       if ((td_now >= MainForm->dt_time_switch) &&
           (td_now <  MainForm->dt_time_switch + ONE_MINUTE))
       {
            MainForm->PID_Ctrl->Active = true;
       } // if
    } // if

    // PID_Ctrl->Active = true => PID Controller On
    switch (MainForm->pid_pars_hlt.pid_model)
    {
         case 0 : pid_reg2(MainForm->thlt            , &MainForm->gamma_hlt,
                           MainForm->tset_hlt        , &MainForm->pid_pars_hlt,
                           MainForm->PID_Ctrl->Active, &MainForm->sys_id_pars);
                  break; // Self-Tuning Takahashi Type C
         case 1 : pid_reg4(MainForm->thlt            , &MainForm->gamma_hlt,
                           MainForm->tset_hlt        , &MainForm->pid_pars_hlt,
                           MainForm->PID_Ctrl->Active);
                  break; // Takahashi Type C, NO filtering of D-action
         default: pid_reg4(MainForm->thlt            , &MainForm->gamma_hlt,
                           MainForm->tset_hlt        , &MainForm->pid_pars_hlt,
                           MainForm->PID_Ctrl->Active);
                  break; // default to Type C, NO filtering of D-action
    } // switch
    if (MainForm->swfx.gamma_hlt_sw)
    {
       MainForm->gamma_hlt = MainForm->swfx.gamma_hlt_fx; // fix gamma for HLT
    } // if
    sprintf(s,"H%d\n", (int)(MainForm->gamma_hlt)); // PID-Output [0%..100%]
    MainForm->comm_port_write(s); // output Hxxx to Ebrew hardware

    //----------------------------------------------------------------------
    // PID-Controller for Boil-Kettle: only use Takahashi type C
    //----------------------------------------------------------------------
    pid_reg4(MainForm->tboil, &MainForm->gamma_boil, MainForm->tset_boil,
             &MainForm->pid_pars_boil, MainForm->sp.pid_ctrl_boil_on == 1);
    if (MainForm->sp.pid_ctrl_boil_on == 2)
    {   // Feed-Forward, gamma_boil == 100%, disable PID-control
        // Only used in CIP-program
        MainForm->gamma_boil = 100.0; // Boil-kettle Burner at full-power
    } // if
    if (MainForm->swfx.gamma_boil_sw)
    {
       MainForm->gamma_boil = MainForm->swfx.gamma_boil_fx; // fix gamma for Boil-Kettle
    } // if
    sprintf(s,"B%d\n", (int)(MainForm->gamma_boil)); // PID-Output [0%..100%]
    MainForm->comm_port_write(s); // output Bxxx to Ebrew hardware
} // task_pid_ctrl()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Run Main State Transition Diagram (STD) that controls
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
    double old_tset_boil; // previous value of tset_boil
    int    ui;            // various user commands
    int    pump_bits;     // Bits for Pump

    if (MainForm->swfx.std_sw)
    {
       std_tmp = MainForm->swfx.std_fx;
    }
    old_tset_hlt  = MainForm->tset_hlt;  // Previous value of HLT Temp. Ref.
    old_tset_boil = MainForm->tset_boil; // Previous value of Boil-Kettle Temp. Ref.
    ui = (MainForm->PID_Ctrl->Active ? 1 : 0);
    if (MainForm->AddingMalt1->Checked)       ui |= UI_ADDING_MALT_TO_MLT;
    if (MainForm->MaltaddedtoMLT1->Checked)   ui |= UI_MALT_ADDED_TO_MLT;
    if (MainForm->Boilingstarted1->Checked)   ui |= UI_BOILING_STARTED;
    if (MainForm->StartChilling1->Checked)    ui |= UI_START_CHILLING;
    if (MainForm->ChillingFinished1->Checked) ui |= UI_CHILLING_FINISHED;
    if (MainForm->CIP_Start->Checked)         ui |= UI_CIP_INIT;
    else                                      ui &= ~UI_CIP_INIT;

    switch (MainForm->std.ebrew_std)
    {
       case S20_CIP_INIT:
            MessageBox(NULL,"- Fill Boil-Kettle with 1% NaOH solution\n"
                            "- Place MLT-Return hose into Boil-Kettle\n"
                            "- Place CFC-Output hose into Boil-Kettle\n"
                            "Press OK to continue.",
                            "Cleaning in Place (CIP): Initialisation",MB_OK);
            ui |= UI_CIP_BOIL_FILLED;
            break;
       case S24_CIP_DRAIN_BOIL1:
            MessageBox(NULL,"- Place MLT-Return hose into drain\n"
                            "- Place CFC-Output hose into drain\n"
                            "Press OK to continue.",
                            "Cleaning in Place (CIP): Draining Boil-Kettle",MB_OK);
            ui |= UI_CIP_HOSES_IN_DRAIN;
            break;
       case S26_CIP_FILL_HLT:
            MessageBox(NULL,"- NEW: Fill HLT with fresh water\n"
                            "- NEW: Place Boil-Kettle return into drain\n"
                            "- Leave MLT-Return hose in drain\n"
                            "- Leave CFC-Output hose in drain\n"
                            "Press OK to continue.",
                            "Cleaning in Place (CIP): Fill HLT with Fresh Water",MB_OK);
            ui |= UI_CIP_HLT_FILLED;
            break;
       case S32_CIP_END:
            MainForm->CIP_Start->Checked = false; // Reset CIP checkbox
            ui &= ~UI_CIP_INIT;                   // Reset UI checkbox
            MainForm->std.ebrew_std = S00_INITIALISATION;
            break;
       default:
            break;
    } // switch

    update_std(&MainForm->volumes , MainForm->thlt, MainForm->tmlt, MainForm->tboil,
               &MainForm->tset_hlt, &MainForm->tset_mlt, &MainForm->tset_boil,
               &MainForm->std_out, MainForm->ms, &MainForm->sp, &MainForm->std,
               ui, std_tmp);
    if (MainForm->swfx.tset_hlt_sw)
    {  // Set Temperature Setpoint for HLT to a fixed value
       MainForm->tset_hlt = MainForm->swfx.tset_hlt_fx; // fix tset_hlt
    } // if
    slope_limiter(MainForm->tset_slope_limit, old_tset_hlt, &MainForm->tset_hlt);
    if (MainForm->swfx.tset_boil_sw)
    {  // Set Temperature Setpoint for Boil-Kettle to a fixed value
       MainForm->tset_boil = MainForm->swfx.tset_boil_fx;
    } // if
    slope_limiter(MainForm->tset_slope_limit, old_tset_boil, &MainForm->tset_boil);

    if (MainForm->PID_Ctrl->Active && (MainForm->tset_hlt < 5.0))
    {   // Disable PID controller when sparging is finished
        MainForm->PID_Ctrl->Active = false;
    } // if
    //-----------------------------------------------------------------
    // Now output all valve bits to Ebrew hardware.
    // NOTE: The pump bit is sent using the P0/P1 command
    //-----------------------------------------------------------------
    sprintf(s,"V%d\n",(MainForm->std_out & ALL_VALVES)); // Output all valves
    MainForm->comm_port_write(s); // output Vxxx to Ebrew hardware

    //--------------------------------------------
    // Send Pump On/Off signals to ebrew hardware.
    //--------------------------------------------
    if (MainForm->std_out & P0b) pump_bits  = 0x01;
    else                         pump_bits  = 0x00;
    if (MainForm->std_out & P1b) pump_bits |= 0x02;
    sprintf(s,"P%d\n",pump_bits);
    MainForm->comm_port_write(s); // Send Px command to ebrew hardware
} // task_update_std()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: ALIVE Led toggle
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
   MainForm->comm_port_write(s); // Send L1/L0 command to ebrew hardware
} // task_alive_led()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Write all relevant data to a log-file
  Period-Time: 5 seconds
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_log_file(void)
{
   FILE   *fd;
   struct time t1;

   if ((fd = fopen(LOGFILE,"a")) != NULL)
   {
      gettime(&t1);
      fprintf(fd,"%02d:%02d:%02d,",t1.ti_hour,t1.ti_min,t1.ti_sec);
      fprintf(fd,"%5.2f,%5.2f,%5.2f,%5.2f,%4.1f,%5.1f,%1d,%1d,%2d,%5.1f,%5.1f,%5.1f,%5.1f,%4.1f,%5.1f\n",
                 MainForm->tset_mlt,
                 MainForm->tset_hlt,
                 MainForm->thlt,
                 MainForm->tmlt,
                 MainForm->ttriac,
                 MainForm->volumes.Vmlt,
                 MainForm->std.sp_idx,
                 MainForm->std.ms_idx,
                 MainForm->std.ebrew_std,
                 MainForm->gamma_hlt,
                 MainForm->volumes.Vhlt,
                 MainForm->volumes.Vboil,
                 MainForm->tboil,
                 MainForm->tcfc,
                 MainForm->gamma_boil);
      fclose(fd);
   } // if
} // task_log_file()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Write all changed parameters to the Ebrew hardware
  Period-Time: 5 seconds
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_write_pars(void)
{
   int i;
   char s[100], s1[10];

   s[0] = '\0';
   for (i = 0 ; i < MAX_PARS; i++)
   {
     if (MainForm->pars_changed[i])
     {
       MainForm->pars_changed[i] = false; // reset flag
       sprintf(s1,"N%d ",i); // Compose command
       strcat(s,s1);         // add to previous commands
       switch (i)
       {
         case  0: sprintf(s1,"%d\n",MainForm->system_mode)         ; break;
         case  1: sprintf(s1,"%d\n",MainForm->gas_non_mod_llimit)  ; break;
         case  2: sprintf(s1,"%d\n",MainForm->gas_non_mod_hlimit)  ; break;
         case  3: sprintf(s1,"%d\n",MainForm->gas_mod_pwm_llimit)  ; break;
         case  4: sprintf(s1,"%d\n",MainForm->gas_mod_pwm_hlimit)  ; break;
         case  5: sprintf(s1,"%d\n",MainForm->ttriac_llim * 100)   ; break;
         case  6: sprintf(s1,"%d\n",MainForm->ttriac_hlim * 100)   ; break;
       } // switch
       strcat(s,s1); // add to existing commands
     } // if
   } // for
   if (strlen(s) > 0)
   {
      MainForm->comm_port_write(s); // Send Nxx yy command to ebrew hardware
   } // if
} // task_write_pars()

void decode_S3_response(FILE *fd, char *s)
{
   char *p, *q;
   char s2[100];

   for (p = strtok(s,"\n"); p != NULL; p = strtok(p+strlen(p)+1,"\n"))
   {
      strncpy(s2, p, sizeof(s2));
      q = strtok(s2,",");
      if (q) fprintf(fd,"%-11s ",q);
      q = strtok(q+strlen(q)+1, ",");
      if (q) fprintf(fd,"%5d ",atoi(q));
      q = strtok(q+strlen(q)+1, ",");
      if (q) fprintf(fd,"0x%02x   ",atoi(q));
      q = strtok(q+strlen(q)+1, ",");
      if (q) fprintf(fd,"%03d   ",atoi(q));
      q = strtok(q+strlen(q)+1, ",");
      if (q) fprintf(fd,"%03d\n",atoi(q));
   } // for
} // decode_S3_response()

/*-----------------------------------------------------------------------------
  Purpose    : TASK: Write HW debug info to a log-file
  Period-Time: 1 second
  Variables  : hw_debug_logging: true = write info to log-file
  Returns    : -
  ---------------------------------------------------------------------------*/
void task_hw_debug(void)
{
   FILE *fd;
   struct time t1;
   struct date d1;
   char   s1[255];
   static time_slice = 0;

   if (MainForm->hw_debug_logging)
   {
      //---------------------------------------------------------------
      // At 38400 Baud ; 1 char. takes approx. 0.26 msec.
      // 200 characters take approx. 52 msec.
      // Max. time to use for this task before disturbing other tasks
      // is approx. 300 msec. So stay below this!
      //---------------------------------------------------------------
      s1[0] = '\0';
      if ((fd = fopen("_debug_ebrew_hw.txt","a")) != NULL)
      {
        switch (time_slice)
        {
          case 0: MainForm->comm_port_write("S1\n"); // List all parameters
                  gettime(&t1);
                  getdate(&d1);
                  fprintf(fd,"\n%02d-%02d-%d, %02d:%02d:%02d\n",d1.da_day,d1.da_mon,
                          d1.da_year,t1.ti_hour,t1.ti_min,t1.ti_sec);
                  list_all_tasks(fd); // print SW tasks (PC program)
                  MainForm->comm_port_read(s1);
                  fprintf(fd,"\n%s\n",s1);
                  time_slice = 1; // goto next time-slice
                  break;
          case 1: // Reading 5 I2C-channels (128 addresses / channel) at 400 kHz
                  // takes 5 x 11 msec. = 55 msec. total, which is < LONG_READ_TIMEOUT
                  MainForm->comm_port_write("S2\n"); // List all I2C devices
                  MainForm->comm_port_set_read_timeout(LONG_READ_TIMEOUT);
                  MainForm->comm_port_read(s1);
                  fprintf(fd,"%s\n",s1);
                  time_slice = 2;
                  break;
          case 2: MainForm->comm_port_write("S3\n"); // List all tasks (approx. 200 chars)
                  fprintf(fd,"Task-Name   T(ms) Stat T(ms) M(ms)\n");
                  fprintf(fd,"----------------------------------\n");
                  MainForm->comm_port_set_read_timeout(LONG_READ_TIMEOUT);
                  MainForm->comm_port_read(s1); // takes approx. 50 msec.
                  decode_S3_response(fd,s1);    // print formatted
                  time_slice = 0;
                  MainForm->hw_debug_logging = false; // disable flag
                  break;
        } // time_slice
        fclose(fd); // close file again
      } // if
      MainForm->comm_port_set_read_timeout(NORMAL_READ_TIMEOUT); // back to normal timeout
   } // if
} // task_hw_debug()

/*-----------------------------------------------------------------------------
  Purpose    : Split Registry string with IP:Port into IP string and Port number
  Variables  :
         port: the port number (0-65535)
           ip: string containing IP address (e.g.'"192.168.1.1")
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::split_ip_port(int *port, char *ip)
{
   char *p;

   p     = strtok(udp_ip_port,":");
   strcpy(ip,p); // copy ebrew IP address
   p     = strtok(NULL,":");
   *port = atoi(p); // copy ebrew port number
} // split_ip_port()

/*-----------------------------------------------------------------------------
  Purpose    : Initialize UDP communication with time-out (non-blocking)
               A time udp_wait_msec_before_read is waited before the call to
               udp_read() is made. In the case that no data is received, udp_read()
               times out 10 msec. after this time-out. This is initialized here.
  Variables  : --
  Returns    : 1=error, 0=OK
  ---------------------------------------------------------------------------*/
int __fastcall TMainForm::udp_init(void)
{
   WSADATA        wsd;
   int            udp_port; // local Port and ebrew-hw port number
   char           ebrew_hw_ip[30],namebuf[50];
   struct hostent *local = NULL;
   int    timeout = 10; // On Windows: timeout in milliseconds!

   if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
   {
       MessageBox(NULL,"WSAStartup failed!","UDP_init() ERROR:",MB_OK);
       return 1;
   } // if
   sock_udp = socket(AF_INET, SOCK_DGRAM, 0); // create the socket
   if (sock_udp == INVALID_SOCKET)
   {
       MessageBox(NULL,"socket failed!","UDP_init() ERROR:",MB_OK);
       return 1;
   } // if
   if (setsockopt(sock_udp,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) != 0)
   {
       MessageBox(NULL,"setsockopt() failed!","UDP_init() ERROR:",MB_OK);
       return 1;
   } // if
   // Get ebrew hardware IP address and Port number
   split_ip_port(&udp_port,ebrew_hw_ip);
   ebrew_hw.sin_family      = AF_INET;
   ebrew_hw.sin_port        = htons((short)udp_port);
   ebrew_hw.sin_addr.s_addr = inet_addr(ebrew_hw_ip);

   // Resolve the local IP address (the machine where this program runs)
   local_ip.sin_family      = AF_INET;
   local_ip.sin_port        = htons((short)udp_port);
   gethostname(namebuf,sizeof(namebuf)); // get local IP address
   local = gethostbyname(namebuf);
   if (local)
   {
      CopyMemory(&local_ip.sin_addr,local->h_addr_list[0],local->h_length);
      //MessageBox(NULL,inet_ntoa(local_ip.sin_addr),"UDP_init()",MB_OK);
   } // if
   if (bind(sock_udp,(SOCKADDR *)&local_ip, sizeof(local_ip)) == SOCKET_ERROR)
   {
      MessageBox(NULL,"bind() failed","UDP_init() ERROR:",MB_OK);
      WSACleanup();
      return 1;
   } // if
   return 0;
} // udp_init()

/*-----------------------------------------------------------------------------
  Purpose    : Read from UDP socket. This routine should be called after a
               specific time. In case no-data is received, this routine
               times out after 10 msec.
  Variables  :
            s: string that contains the read result
        bytes: Number of bytes read
  Returns    : 1=error, 0=OK
  ---------------------------------------------------------------------------*/
int __fastcall TMainForm::udp_read(char *s, int *bytes)
{
   int         err = 0, ret;
   SOCKADDR_IN sender;
   int         sender_size,lerr;
   char        spr[30];

   if (sock_udp)
   {
      sender_size = sizeof(sender);
      ret = recvfrom(sock_udp,s,MAX_BUF_READ,0,(SOCKADDR *)&sender,&sender_size);
      if (ret == SOCKET_ERROR)
      {  // In Windows 2000 and later, if no data arrives within the period specified
         // in SO_RCVTIMEO, the recv function returns WSAETIMEDOUT, and if data is
         // received, recv returns SUCCESS.
         lerr   = WSAGetLastError();
         if (lerr == WSAETIMEDOUT)
         {  // nothing was received, no error
            s[0] = '\0';
            *bytes = 0;
         } // if
         else
         {
            sprintf(spr,"recvfrom() failed: %d",lerr);
            MessageBox(NULL,spr,"udp_read() ERROR:",MB_OK);
            err    = 1;
            *bytes = 0;
         } // else
      } // if
      else
      {
         s[ret] = '\0';
         *bytes = ret;
      } // else
   } // if
   else err = 1;
   return err;
} // udp_read()

/*-----------------------------------------------------------------------------
  Purpose    : Write to UDP socket
  Variables  :
            s: string to write to ebrew-hardware
  Returns    : 1=error, 0=OK
  ---------------------------------------------------------------------------*/
int __fastcall TMainForm::udp_write(char *s)
{
   int err = 0, ret;

   if (sock_udp)
   {
      ret = sendto(sock_udp,s,strlen(s),0,(SOCKADDR *)&ebrew_hw,sizeof(ebrew_hw));
      if (ret == SOCKET_ERROR)
      {
         MessageBox(NULL,"sendto() failed","udp_write() ERROR:",MB_OK);
         err = 1;
      } // if
   } // if
   else err = 1;
   return err;
} // udp_write()

/*-----------------------------------------------------------------------------
  Purpose    : Close UDP socket
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::udp_close(void)
{
   closesocket(sock_udp);
   WSACleanup();
} // udp_close()

/*-----------------------------------------------------------------------------
  Purpose    : Set the COM Port Read Time-out value
  Variables  : msec: the number of millisec. to wait for a read
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::comm_port_set_read_timeout(DWORD msec)
{
     if (comm_channel_nr == 0) // Ethernet UDP
     {
        udp_wait_msec_before_read = msec;
     } // if
     else
     {
        // Set the Communication Timeouts
        // ReadTotalTimeoutConstant: A constant used to calculate the total time-out
        //                           period for read operations, in milliseconds.
        GetCommTimeouts(hComm,&ctmoOld);
        ctmoNew.ReadTotalTimeoutConstant    = msec;
        ctmoNew.ReadTotalTimeoutMultiplier  =   0;
        ctmoNew.WriteTotalTimeoutMultiplier =   0;
        ctmoNew.WriteTotalTimeoutConstant   =   0;
        SetCommTimeouts(hComm, &ctmoNew);
     } // else
} // COM_port_set_read_timeout()

/*-----------------------------------------------------------------------------
  Purpose    : Open Virtual COM Port (Virtual: COM Port emulated via USB)
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::comm_port_open(void)
{
   DCB  dcbCommPort;
   char s[50];

   if (comm_channel_nr > 0) // Any of the Virtual USB COM ports
   {
        // OPEN THE COM PORT.
        sprintf(s,"COM%d",comm_channel_nr); // communication channel
        hComm = CreateFile(s,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

        // IF THE PORT CANNOT BE OPENED, BAIL OUT.
        if(hComm == INVALID_HANDLE_VALUE)
        {
           strcat(s," could not be opened. Please check COM Port Settings.");
           MessageBox(NULL,s,"ERROR Opening Virtual USB COM Port",MB_OK);
           com_port_is_open = false;
        } // if
        else
        {
           comm_port_set_read_timeout(LONG_READ_TIMEOUT); // Set the Communication Timeouts

           // Set Baud-rate, Parity, wordsize and stop-bits.
           // There are other ways of doing these setting, but this is the easiest way.
           // If you want to add code for other baud-rates, remember that the argument
           // for BuildCommDCB must be a pointer to a string. Also note that
           // BuildCommDCB() defaults to NO Handshaking.
           dcbCommPort.DCBlength = sizeof(DCB);
           GetCommState(hComm, &dcbCommPort);
           BuildCommDCB(com_port_settings, &dcbCommPort); // "38400,N,8,1"
           SetCommState(hComm, &dcbCommPort);
           com_port_is_open = true;
        } // else
   } // if
   else
   {    // Ethernet UDP communication
        com_port_is_open = (udp_init() == 0); // Ethernet: returns 0 if all is well
        comm_port_set_read_timeout(LONG_READ_TIMEOUT); // Set the Communication Timeouts
   } // else
   com_port_opened  = comm_channel_nr; // remember communication channel
   if ((fdbg_com = fopen(COM_PORT_DEBUG_FNAME,"a")) == NULL)
   {  // Open COM-port debugging file
      MessageBox(NULL,"Could not open COM-port debug log-file","Error during Initialisation",MB_OK);
   } // if
   else if (cb_debug_com_port)
   {
      struct time t1;
      struct date d1;
      gettime(&t1);
      getdate(&d1);
      fprintf(fdbg_com,"\nFile opened (%02d-%02d-%d, %02d:%02d:%02d)\n",
                       d1.da_day,d1.da_mon,d1.da_year,t1.ti_hour,t1.ti_min,t1.ti_sec);
   } // else if
} // comm_port_open()

/*-----------------------------------------------------------------------------
  Purpose    : Close Virtual COM Port (Virtual: COM Port emulated via USB)
               that was opened by COM_port_open().
  Variables  : -
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::comm_port_close(void)
{
   if (com_port_opened > 0) // any of the USB Virtual COM ports
   {
      PurgeComm(hComm, PURGE_RXABORT);
      SetCommTimeouts(hComm, &ctmoOld);
      CloseHandle(hComm); // close Virtual USB COM-port
   } // if
   else if (com_port_is_open)
   {  // UDP communication and udp_init() was OK
      udp_close();
   } // else if
   if (cb_debug_com_port) fprintf(fdbg_com,"\nFile closed\n\n");
   fclose(fdbg_com);   // close COM-port debugging file
   com_port_is_open   = false; // set flag to 'not open'
} // comm_port_close()

/*-----------------------------------------------------------------------------
  Purpose    : Read a string from the Virtual COM Port opened by COM_port_open()
  Variables  : s: contains the null-terminated string that is read
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::comm_port_read(char *s)
{
   char rbuf[MAX_BUF_READ]; // contains string read from RS232 port
   DWORD i, j, dwBytesRead = 0;
   int   ok, udp_bytes;
   struct time t1;

   if (comm_channel_nr == 0) // Use Ethernet UDP as communication channel
   {
        // UDP data is read by the UDPRead() method of UDP_Server
        // If data is available, it is stored in the array udp_read[]
        // This function is normally called after a comm_port_write(),
        // so introduce a little delay here, to give the uC time to respond
        ::Sleep(udp_wait_msec_before_read); // give ebrew HW time to react and send response
        ok = (udp_read(s,&udp_bytes) == 0);
        dwBytesRead = udp_bytes;
   } // if
   else if (com_port_is_open)
   { // Virtual COM-port and port is opened
     ReadFile(hComm, s, MAX_BUF_READ-1, &dwBytesRead, NULL);
     if(dwBytesRead)
     {
        s[dwBytesRead] = '\0'; // Null-Terminate the string
     } // if
   } // else if
   if (cb_debug_com_port)
   {
      for (i = j = 0; i < dwBytesRead; i++)
      {
         if ((s[i] != '\r') && (s[i] != '\n'))
         {
             rbuf[j++] = s[i];
         } // if
      } // if
      rbuf[j] = '\0';
      gettime(&t1);
      fprintf(fdbg_com,"R%1d.%02d.%03d[%s]",comm_channel_nr,t1.ti_sec,t1.ti_hund*10,rbuf);
   } // if
} // comm_port_read()

/*-----------------------------------------------------------------------------
  Purpose    : Write a string to the Virtual COM Port opened by COM_port_open()
  Variables  : s: contains the null-terminated string to write to the COM port.
  Returns    : -
  ---------------------------------------------------------------------------*/
void __fastcall TMainForm::comm_port_write(const char *s)
{
   char send_buffer[MAX_BUF_WRITE]; // contains string to send to RS232 port
   char s2[MAX_BUF_WRITE];          // contains string for debugging
   int  bytes_to_send = 0;          // Number of bytes to send
   int  ok, i, j, bytes_sent = 0;   // Number of bytes sent to COM port
   struct time t1;
   int    xmit_err = 0;

   strcpy(send_buffer,s);    // copy command to send into send_buffer
   bytes_to_send = strlen(send_buffer);
   if (comm_channel_nr == 0) // Use Ethernet UDP as communication channel
   {
        ok = (udp_write((char *)s) == 0);
   } // if
   else if (com_port_is_open)
   { // Virtual COM-port and port is opened
     bytes_sent    = 0;
     while ((bytes_sent < bytes_to_send) && (xmit_err < 3))
     {
        if (!TransmitCommChar(hComm, send_buffer[bytes_sent++]))
        {
           MessageBox(NULL,"TransmitCommChar() Error","COM_port_write()",MB_OK);
           xmit_err++;
        } // if
     } // while()
   } // else if
   if (cb_debug_com_port)
   {
      j = 0;
      for (i = 0; i < bytes_to_send; i++)
      {
         if (send_buffer[i] != '\n')
         {
            s2[j++] = send_buffer[i];
         } // else
      } // for i
      s2[j] = '\0'; // terminate string
      gettime(&t1);
      fprintf(fdbg_com,"\nW%02d.%03d[%s]",t1.ti_sec,t1.ti_hund*10,s2);
      if (xmit_err >= 3)
      {
         fprintf(fdbg_com,"[xmit_err]");
         comm_port_close();
      } // if
   } // if
} // COM_port_write()

/*------------------------------------------------------------------
  Purpose  : This is the main constructor for the program.
  Returns  : None
  ------------------------------------------------------------------*/
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
   ebrew_revision   = "$Revision: 1.94 $";
   ViewMashProgress = new TViewMashProgress(this); // create modeless Dialog
   TRegistry *Reg   = new TRegistry();
   power_up_flag    = true;  // indicate that program power-up is active
   com_port_is_open = false; // indicate that COM-port is closed at power-up
   hw_debug_logging = false; // Keypress 'D' to enable this (task hw_debug)

   try
    {
       if (!Reg->KeyExists(REGKEY))
       {  // No entry in Registry, create all keys
          Reg->CreateKey(REGKEY); // Create key if it does not exist yet
          Reg->OpenKey(REGKEY,FALSE);

          //------------------------------------
          // Ebrew 2.0 Hardware Settings Dialog
          //------------------------------------
          Reg->WriteInteger("SYSTEM_MODE",GAS_MODULATING);
          Reg->WriteInteger("COMM_CHANNEL",0);   // Select Ethernet as Comm. Channel
          Reg->WriteString("COM_PORT_SETTINGS","38400,N,8,1"); // COM port settings
          Reg->WriteString("UDP_IP_PORT","192.168.1.177:8888"); // IP & Port number
          Reg->WriteBool("CB_DEBUG_COM_PORT",true);

          Reg->WriteInteger("GAS_NON_MOD_LLIMIT",30);
          Reg->WriteInteger("GAS_NON_MOD_HLIMIT",35);
          Reg->WriteInteger("GAS_MOD_PWM_LLIMIT",2);
          Reg->WriteInteger("GAS_MOD_PWM_HLIMIT",4);
          Reg->WriteInteger("TTRIAC_LLIM",65);
          Reg->WriteInteger("TTRIAC_HLIM",75);

          //------------------------------------
          // PID Settings Dialog
          //------------------------------------
          Reg->WriteInteger("PID_Model",1);      // Takahashi PID Controller
          Reg->WriteFloat("TS",20.0);            // Set Default sample time
          Reg->WriteFloat("Kc",80.0);            // Controller gain
          Reg->WriteFloat("Ti",282.0);           // Ti constant
          Reg->WriteFloat("Td",20.0);            // Td constant
          Reg->WriteFloat("K_LPF",0);            // LPF filter time-constant
          Reg->WriteInteger("STC_N",1);          // order N for system identification
          Reg->WriteInteger("STC_TD",4); // Time-Delay estimate for system identification
          Reg->WriteFloat("TSET_SLOPE_LIM",1.0); // Slope Limit for Temperature Setpoints
          sys_id_pars.stc_adf = 1; // true = use Adaptive directional forgetting
          Reg->WriteBool("STC_ADF",(sys_id_pars.stc_adf > 0));
          cb_pid_dbg        = false; // no PID debug to screen (not a Reg. variable)
          PID_dbg->Visible  = false;
          PID_dbg2->Visible = false;

          //------------------------------------
          // Sparge, Mash & Boil Settings Dialog
          //------------------------------------
          // Sparge Settings
          Reg->WriteInteger("SP_BATCHES",5);   // #Sparge Batches
          Reg->WriteInteger("SP_TIME",12);     // Time between sparge batches
          Reg->WriteBool("CB_VSP2",1);         // Double Initial Sparge Volume to Boil-Kettle
          // Mash Settings
          Reg->WriteInteger("ms_idx",MAX_MS);  // init. index in mash scheme
          Reg->WriteFloat("TOffset0",3.5);     // Compensation for dough-in of malt
          Reg->WriteFloat("TOffset",1.0);      // Compensation HLT-MLT heat-loss
          Reg->WriteFloat("TOffset2",-0.5);    // Early start of mash-timer
          Reg->WriteInteger("PREHEAT_TIME",15);// PREHEAT_TIME [min.]
          Reg->WriteBool("CB_Mash_Rest",1);    // Mash Rest for 5 minutes after Malt is added
          Reg->WriteBool("CB_dpht",1);         // 1= use dynamic preheat timing
          Reg->WriteInteger("HLT_Bcap",90);    // HLT burner capacity in sec./°C
          Reg->WriteBool("CB_pumps_on",1);     // 1= Leave pumps runnings after MLT temp. is reached
          // Boil Settings
          Reg->WriteInteger("BOIL_MIN_TEMP",60); // Min. Temp. for Boil-Kettle (Celsius)
          Reg->WriteInteger("BOIL_TIME",90);   // Total Boil Time (min.)
          Reg->WriteInteger("SP_PREBOIL",95);  // Pre-Boil Temperature (Celsius)
          Reg->WriteFloat("BOIL_DETECT",99.3); // Boiling-Detection minimum Temperature (Celsius)
          Reg->WriteInteger("SP_BOIL",105);    // Boil Temperature (Celsius)
          Reg->WriteBool("CB_Boil_Rest",1);    // Let wort rest for 5 minutes after boiling

          //------------------------------------
          // Measurements Dialog
          //------------------------------------
          Reg->WriteFloat("THLT_OFFSET",0.0);  // Offset for Thlt
          Reg->WriteFloat("TMLT_OFFSET",0.0);  // Offset for Tmlt
          Reg->WriteFloat("TBOIL_OFFSET",0.0); // Offset for Tboil
          Reg->WriteFloat("TCFC_OFFSET",0.0);  // Offset for Tcfc

          Reg->WriteFloat("VHLT_MAX",200.0);   // Max. HLT volume
          Reg->WriteFloat("VMLT_MAX",110.0);   // Max. MLT volume
          Reg->WriteFloat("VBOIL_MAX",140.0);  // Max. Boil kettle volume
          Reg->WriteInteger("FLOW1_ERR",0);    // Error Correction for FLOW1
          Reg->WriteInteger("FLOW2_ERR",0);    // Error Correction for FLOW2
          Reg->WriteInteger("FLOW3_ERR",0);    // Error Correction for FLOW3
          Reg->WriteInteger("FLOW4_ERR",0);    // Error Correction for FLOW4
          Reg->WriteBool("FLOW_TEMP_CORR",1);  // Use Temperature Correction
          Reg->WriteInteger("MIN_FR_MLT_PERC",10); // Min. Flowrate for MLT Empty detection
          Reg->WriteInteger("MIN_FR_BOIL_PERC",2); // Min. Flowrate for Boil-Kettle Empty detection
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

    Main_Initialisation(); // Init all ebrew Hardware, ISR and PID controller
    //----------------------------------------
    // Init. volumes. Should be done only once
    //----------------------------------------
    volumes.Vboil = VBOIL_START;
    power_up_flag = false; // power-up is finished
} // TMainForm::TMainForm()
//---------------------------------------------------------------------------

void __fastcall TMainForm::Main_Initialisation(void)
/*------------------------------------------------------------------
  Purpose  : This function Initialises all ebrew Hardware and checks
             to see if it is present. It also initialises the
             Interrupt Service Routine (ISR) and the PID controller.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg   = new TRegistry();
   FILE *fd;             // Log File Descriptor
   int  i;               // temp. variable
   char s[MAX_BUF_READ]; // Temp. string
   char *result;

   //----------------------------------------
   // Initialise all variables from Registry
   //----------------------------------------
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);

         //------------------------------------
         // Ebrew 2.0 Hardware Settings Dialog
         //------------------------------------
         system_mode       = Reg->ReadInteger("SYSTEM_MODE");
         comm_channel_nr   = Reg->ReadInteger("COMM_CHANNEL");   // Communication Channel
         strcpy(com_port_settings,Reg->ReadString("COM_PORT_SETTINGS").c_str()); // COM port settings
         strcpy(udp_ip_port,Reg->ReadString("UDP_IP_PORT").c_str()); // UDP IP & Port number settings
         cb_debug_com_port = Reg->ReadBool("CB_DEBUG_COM_PORT"); // display message

         comm_port_open();  // Start Communication with Ebrew Hardware

         gas_non_mod_llimit = Reg->ReadInteger("GAS_NON_MOD_LLIMIT");
         gas_non_mod_hlimit = Reg->ReadInteger("GAS_NON_MOD_HLIMIT");
         gas_mod_pwm_llimit = Reg->ReadInteger("GAS_MOD_PWM_LLIMIT");
         gas_mod_pwm_hlimit = Reg->ReadInteger("GAS_MOD_PWM_HLIMIT");
         ttriac_llim        = Reg->ReadInteger("TTRIAC_LLIM");
         ttriac_hlim        = Reg->ReadInteger("TTRIAC_HLIM");
         tm_triac->SetPoint->Value = ttriac_hlim;  // update object on screen

         //------------------------------------
         // PID Settings Dialog
         //------------------------------------
         pid_pars_hlt.pid_model  = Reg->ReadInteger("PID_Model"); // [0..1]
         pid_pars_hlt.ts         = Reg->ReadFloat("TS");  // Read TS from registry
         pid_pars_hlt.kc         = Reg->ReadFloat("Kc");  // Read Kc from registry
         pid_pars_hlt.ti         = Reg->ReadFloat("Ti");  // Read Ti from registry
         pid_pars_hlt.td         = Reg->ReadFloat("Td");  // Read Td from registry
         pid_pars_hlt.k_lpf      = Reg->ReadFloat("K_LPF");

         pid_pars_boil.pid_model = 1; // Use Type C Takahashi for Boil-Kettle
         pid_pars_boil.ts        = Reg->ReadFloat("TS");  // Read TS from registry
         pid_pars_boil.kc        = Reg->ReadFloat("Kc");  // Read Kc from registry
         pid_pars_boil.ti        = Reg->ReadFloat("Ti");  // Read Ti from registry
         pid_pars_boil.td        = Reg->ReadFloat("Td");  // Read Td from registry
         pid_pars_boil.k_lpf     = Reg->ReadFloat("K_LPF");

         sys_id_pars.N       = Reg->ReadInteger("STC_N");     // [1,2,3]
         sys_id_pars.stc_td  = Reg->ReadInteger("STC_TD");    // [0..100]
         tset_slope_limit    = Reg->ReadFloat("TSET_SLOPE_LIM");
         sys_id_pars.stc_adf = Reg->ReadBool("STC_ADF");      // true = use ADF

         //------------------------------------
         // Measurements Dialog
         //------------------------------------
         thlt_offset  = Reg->ReadFloat("THLT_OFFSET");  // offset calibration
         tmlt_offset  = Reg->ReadFloat("TMLT_OFFSET");  // offset calibration
         tboil_offset = Reg->ReadFloat("TBOIL_OFFSET"); // offset calibration
         tcfc_offset  = Reg->ReadFloat("TCFC_OFFSET");  // offset calibration

         vhlt_max    = Reg->ReadFloat("VHLT_MAX");    // Read max. HLT volume
         vmlt_max    = Reg->ReadFloat("VMLT_MAX");    // Read max. MLT volume
         vboil_max   = Reg->ReadFloat("VBOIL_MAX");   // Boil kettle volume

         flow1_err   = Reg->ReadInteger("FLOW1_ERR"); // Compensation error for FLOW1
         flow2_err   = Reg->ReadInteger("FLOW2_ERR"); // Compensation error for FLOW2
         flow3_err   = Reg->ReadInteger("FLOW3_ERR"); // Compensation error for FLOW3
         flow4_err   = Reg->ReadInteger("FLOW4_ERR"); // Compensation error for FLOW4
         flow_temp_corr = Reg->ReadBool("FLOW_TEMP_CORR"); // Temp. correction for flowmeters
         volumes.min_flowrate_mlt_perc  = Reg->ReadInteger("MIN_FR_MLT_PERC");
         volumes.min_flowrate_boil_perc = Reg->ReadInteger("MIN_FR_BOIL_PERC");

         Reg->SaveKey(REGKEY,"ebrew_reg");
         Reg->CloseKey();      // Close the Registry
         switch (pid_pars_hlt.pid_model)
         {
            case 0 : // Self-Tuning Takahashi, N = 1 .. 3
                     init_pid2(&pid_pars_hlt,&sys_id_pars);
                     break;
            case 1 : init_pid4(&pid_pars_hlt);  // Takahashi Type C controller
                     break;
            default: pid_pars_hlt.pid_model = 1; // Takahashi Type C controller
                     init_pid4(&pid_pars_hlt);
                     break;
         } // switch
         init_pid4(&pid_pars_boil); // Init. PID-controller for Boil-Kettle
         // Do NOT delete Reg yet, since we need it further on
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch

   //--------------------------------------------------------------------------
   // Read Mash Scheme for maisch.sch file
   // Mash scheme is used in the STD, sample time of STD is 1 second
   // Init mash. timers only when doing a power-up, not after an I2C reset
   //--------------------------------------------------------------------------
   if (!read_input_file(MASH_FILE,ms,&(std.ms_tot),1.0,power_up_flag ? INIT_TIMERS : NO_INIT_TIMERS,&sp.mash_vol,&sp.sp_vol))
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
      sp.boil[0][0]  = '\0'; // boil-start
      sp.boil[1][0]  = '\0'; // boil-end
      sp.chill[0][0] = '\0'; // chill-start
      sp.chill[1][0] = '\0'; // chill-end
   } // if

   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         i = Reg->ReadInteger("ms_idx");
         if ((i < MAX_MS) && (power_up_flag == true))
         {  //--------------------------------------------------------------
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

   //--------------------------------------------------------------------------
   // Init. the position of all valves & the Pump to OFF (closed).
   // No Manual Override of valves yet.
   //--------------------------------------------------------------------------
   std_out = 0x0000;

   // Reset all flags for sending parameters to hardware (task write_pars)
   for (i = 0 ; i < MAX_PARS; i++)
   {
     MainForm->pars_changed[i] = false; // reset flag
   } // for

   //-----------------------------------------
   // Now add all the tasks for the scheduler
   //-----------------------------------------
   add_task(task_alive_led     , "alive_led"    ,   0,  500); /* 0, 500, 1000, 1500 */
   add_task(task_read_temps    , "read_temps"   , 100, 2000);
   add_task(task_pid_ctrl      , "pid_control"  , 300, (uint16_t)(pid_pars_hlt.ts * 1000));
   add_task(task_update_std    , "update_std"   , 400, 1000); /* 400, 1400 */
   add_task(task_hw_debug      , "hw_debug"     , 600, 2000);
   add_task(task_read_flows    , "read_flows"   ,1100, 2000);
   add_task(task_log_file      , "wr_log_file"  ,1600, 5000);
   add_task(task_write_pars    , "write_pars"   ,1700, 5000);

   init_ma(&MainForm->flow1_ma,10,0.0); // init moving_average filter for flowrate
   init_ma(&MainForm->flow2_ma,10,0.0); // init moving_average filter for flowrate
   init_ma(&MainForm->flow3_ma,10,0.0); // init moving_average filter for flowrate
   init_ma(&MainForm->flow4_ma,10,0.0); // init moving_average filter for flowrate

   //-----------------------------------------------
   // Set HW and SW rev. numbers in Tstatusbar panel
   //-----------------------------------------------
   ::Sleep(1600);  // Give E-brew HW (Arduino) time to power up
   strcpy(srev,"SW r");
   strncat(srev,&ebrew_revision[11],4); // extract the CVS revision number
   srev[9] = '\0';
   strcat(srev," HW r");
   bool s0_response = false;
   int  len, count = 6;
   struct time t1;
   while (!s0_response && (count-- > 0))
   {
        comm_port_write("S0\n");
        s[0] = '\0';
        comm_port_read(s);
        result = strtok(s,"\n");
        if (result != NULL)
        {
           s0_response = !strncmp(result,EBREW_HW_ID,strlen(EBREW_HW_ID));
           while (!s0_response && (result = strtok(NULL,"\n")) != NULL)
           { // another line was found in the buffer
             s0_response = !strncmp(result,EBREW_HW_ID,strlen(EBREW_HW_ID));
           } // while
        } // if
        ::Sleep(100);
   } // while
   if (count > 0) strncat(srev,&s[16],strlen(s)-16);
   else           strcat(srev,"?.?");
   StatusBar->Panels->Items[PANEL_REVIS]->Text = AnsiString(srev);

   // Now update Kettle volumes (Label and Tank objects) in GUI
   sprintf(s,"Boil %3.0fL", vboil_max); // Boil Kettle
   Boil_Label->Caption = s;
   Tank_Boil->Max = vboil_max;
   sprintf(s,"HLT %3.0fL", vhlt_max); // HLT
   HLT_Label->Caption = s;
   Tank_HLT->Max = vhlt_max;
   sprintf(s,"MLT %3.0fL", vmlt_max); // MLT
   MLT_Label->Caption = s;
   Tank_MLT->Max = vmlt_max;

   MainForm->volumes.Flow_cfc_out_reset_value = 0.0; // Init. reset value for Flow_cfc_out
   comm_port_write("R0\n"); // Reset all Flows to 0.0 L in ebrew hardware
   no_sound = ALARM_TEMP_FLOW_SENSORS; // sound alarm in case of Temp. and Flow sensor error

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
      fprintf(fd,"Kc:%6.2f, Ti:%6.2f, Td:%6.2f, K_lpf:%6.2f, Ts:%5.2f, ",
                 pid_pars_hlt.kc, pid_pars_hlt.ti, pid_pars_hlt.td, pid_pars_hlt.k_lpf, pid_pars_hlt.ts);
      fprintf(fd,"PID_Model:%2d\n",pid_pars_hlt.pid_model);
      fprintf(fd,"Version used: %s\n",srev);
      fprintf(fd,"ms_tot:%2d\n", std.ms_tot);
      fprintf(fd,"Temp Offset:%4.1f, Temp Offset2:%4.1f\n",sp.temp_offset,sp.temp_offset2);
      fprintf(fd,"Vhlt_max:%5.1f, Vmlt_max:%5.1f\n\n", vhlt_max, vmlt_max);
      fprintf(fd," Time    TsetM TsetH  Thlt  Tmlt Telc  Vmlt s m st  GmaH  Vhlt VBoil TBoil  Tcfc GmaB\n");
      fprintf(fd,"[h:m:s]   [°C]  [°C]  [°C]  [°C] [°C]   [L] p s  d   [%]   [L]   [L]  [°C]  [°C]  [%]\n");
      fprintf(fd,"-------------------------------------------------------------------------------------\n");
      fclose(fd);
   } // else

   comm_port_set_read_timeout(NORMAL_READ_TIMEOUT); // Now change Read time-out to default value

   //----------------------------------
   // We came all the way! Start Timers
   //----------------------------------
   if (T100msec)
   {
      T100msec->Interval = (int)(1000 / TICKS_PER_SEC); /* scheduler.h */
      T100msec->Enabled  = true; // start Interrupt Timer
   } // if
   if (ViewMashProgress) ViewMashProgress->UpdateTimer->Enabled = true; // Start Mash Progress Update timer
   PID_Ctrl->Enabled = true;
   time_switch     = 0;    // Time switch disabled at power-up
   delete Reg; // Delete Registry object to prevent memory leak
} // Main_Initialisation()
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
        // Sparge Settings
        sp.sp_batches     = Reg->ReadInteger("SP_BATCHES"); // Number of Sparge Batches
        sp.sp_time        = Reg->ReadInteger("SP_TIME");    // Time between two Sparge Batches
        MainForm->cb_vsp2 = Reg->ReadBool("CB_VSP2");     // Duble Initial Sparge Volume to Boil-Kettle
        sp.sp_vol_batch   = ((double)(sp.sp_vol)) / sp.sp_batches;
        if (MainForm->cb_vsp2)
             sp.sp_vol_batch0 = 2.0 * sp.sp_vol_batch;
        else sp.sp_vol_batch0 = sp.sp_vol_batch;
        // Mash Settings
        sp.temp_offset0   = Reg->ReadFloat("TOffset0");
        sp.temp_offset    = Reg->ReadFloat("TOffset");
        sp.temp_offset2   = Reg->ReadFloat("TOffset2");
        sp.ph_time        = 60 * Reg->ReadInteger("PREHEAT_TIME"); // ph_time is in seconds
        sp.use_dpht       = Reg->ReadBool("CB_dpht");              // 1= use dynamic preheat timing
        sp.hlt_bcap       = Reg->ReadInteger("HLT_Bcap");          // HLT Burner capacity
        sp.leave_pumps_on = Reg->ReadBool("CB_pumps_on");          // 1= leave pumps on when MLT temp. is reached
        std.mash_rest     = Reg->ReadBool("CB_Mash_Rest");         // Mash rest 5 minutes after malt is added
        std.boil_rest     = Reg->ReadBool("CB_Boil_Rest");         // Let wort rest for 5 minutes after boiling
        for (i = 0; i < std.ms_tot; i++)
        {
            ms[i].preht = ms[i].time;
            if (sp.use_dpht)
            {   // dynamic preheat time
                if (i < std.ms_tot - 1)
                     ms[i].preht -= sp.hlt_bcap * (int)(ms[i+1].temp - ms[i].temp);
                else ms[i].preht  = 0;
            } // if
            else
            {   // fixed preheat time
                ms[i].preht -= sp.ph_time;
            } // else
            if (ms[i].preht < 0) ms[i].preht = 0;
        } // for i
        // Boil-Time Settings
        sp.boil_min_temp= Reg->ReadInteger("BOIL_MIN_TEMP"); // Min. Temp. for Boil-Kettle
        sp.boil_time    = Reg->ReadInteger("BOIL_TIME");  // Total Boil Time (min.)
        sp.sp_preboil   = Reg->ReadInteger("SP_PREBOIL"); // Setpoint Pre-Boil Temperature
        sp.boil_detect  = Reg->ReadFloat("BOIL_DETECT");  // Boiling-Detection minimum Temperature (Celsius)
        sp.sp_boil      = Reg->ReadInteger("SP_BOIL");    // Setpoint Boil Temperature
        sp.pid_ctrl_boil_on = 0;                          // Disable PID-Controller for Boil-Kettle

        // STD Settings
        //---------------------------------------------------------------
        // Now calculate the internal values for the timers. Since the
        // State Transition Diagram is called every second, every 'tick'
        // is therefore a second.
        // SP_TIME [min.], BOIL_TIME [min.]
        //---------------------------------------------------------------
        sp.sp_time_ticks   = sp.sp_time * 60;
        sp.boil_time_ticks = sp.boil_time * 60;

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

/*------------------------------------------------------------------
  Purpose  : This is the main Timer function which is called every
             100 milliseconds. It is only used for the interrupt part
             of the task-scheduler.
  Returns  : None
  ------------------------------------------------------------------*/
void __fastcall TMainForm::T100msecTimer(TObject *Sender)
{
   scheduler_isr(); // Scheduler for all tasks
} // TMainForm::T100msecTimer()
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
         x  = p1[k].eline + 1 - p1[k].start_lstd;
         x *= p1[k].time_period; // Log-file -> 5 sec.: STD called -> 1 sec.
         switch (std.ebrew_std) // init. timers for states that have timers
         {
            case S05_SPARGE_TIMER_RUNNING:
                                    if (std.sp_idx > 0)
                                    {
                                       std.timer1 = x;
                                    }
                                    else
                                    {  // if crash occurs in state 5 when sp_idx==0
                                       std.timer1 = sp.sp_time_ticks;
                                    }
                                    break;
            case S08_DELAY_xSEC:    std.timer2 = 1;
                                    break;
            case S11_BOILING:       std.timer5 = x;
                                    break;
            case S14_PUMP_PREFILL:  std.timer3 = x;
                                    break;
            default:                break;
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
         ptmp->PID_Model->ItemIndex = Reg->ReadInteger("PID_Model");
         ptmp->TS_edit->Text        = AnsiString(Reg->ReadFloat("TS"));
         ptmp->Kc_Edit->Text        = AnsiString(Reg->ReadFloat("Kc"));
         ptmp->Ti_Edit->Text        = AnsiString(Reg->ReadFloat("Ti"));
         ptmp->Td_Edit->Text        = AnsiString(Reg->ReadFloat("Td"));
         ptmp->K_LPF_Edit->Text     = AnsiString(Reg->ReadFloat("K_LPF"));
         ptmp->STC_N_Edit->Text     = AnsiString(Reg->ReadInteger("STC_N"));
         ptmp->STC_TD_Edit->Text    = AnsiString(Reg->ReadInteger("STC_TD"));
         ptmp->Tset_hlt_slope->Text = AnsiString(Reg->ReadFloat("TSET_SLOPE_LIM"));
         ptmp->CB_adf->Checked      = Reg->ReadBool("STC_ADF");
         ptmp->RG2->ItemIndex       = time_switch; // Value of time-switch [off, on]
         ptmp->CB_PID_dbg->Checked  = cb_pid_dbg;   // PID debug info

         if (time_switch)
         {
            ptmp->Date_Edit->Text = DateToStr(dt_time_switch);
            AnsiString s = TimeToStr(dt_time_switch);
            s.SetLength(s.Length()-3); // remove seconds
            ptmp->Time_Edit->Text = s;
         } // if

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            pid_pars_hlt.pid_model = ptmp->PID_Model->ItemIndex;
            Reg->WriteInteger("PID_Model",pid_pars_hlt.pid_model);
            pid_pars_hlt.ts        = ptmp->TS_edit->Text.ToDouble();
            Reg->WriteFloat("TS",pid_pars_hlt.ts);
            pid_pars_hlt.kc        = ptmp->Kc_Edit->Text.ToDouble();
            Reg->WriteFloat("Kc",pid_pars_hlt.kc);
            pid_pars_hlt.ti        = ptmp->Ti_Edit->Text.ToDouble();
            Reg->WriteFloat("Ti",pid_pars_hlt.ti);
            pid_pars_hlt.td        = ptmp->Td_Edit->Text.ToDouble();
            Reg->WriteFloat("Td",pid_pars_hlt.td);
            pid_pars_hlt.k_lpf     = ptmp->K_LPF_Edit->Text.ToDouble();
            Reg->WriteFloat("K_LPF",pid_pars_hlt.k_lpf);
            sys_id_pars.N          = ptmp->STC_N_Edit->Text.ToInt();
            if ((sys_id_pars.N < 1) || (sys_id_pars.N > 3))
            {
                sys_id_pars.N = 1;
            } // if
            Reg->WriteInteger("STC_N",sys_id_pars.N);
            sys_id_pars.stc_td = ptmp->STC_TD_Edit->Text.ToInt();
            if ((sys_id_pars.stc_td < 0) || (sys_id_pars.stc_td > MAX_MA))
            {
                sys_id_pars.stc_td = 0;
            } // if
            Reg->WriteInteger("STC_TD",sys_id_pars.stc_td);
            tset_slope_limit = ptmp->Tset_hlt_slope->Text.ToDouble();
            Reg->WriteFloat("TSET_HLT_SLOPE",tset_slope_limit);
            if (ptmp->CB_adf->Checked)
                 sys_id_pars.stc_adf = 1; // use adaptive directional forgetting
            else sys_id_pars.stc_adf = 0; // NO ADF
            Reg->WriteBool("STC_ADF",(sys_id_pars.stc_adf > 0));
            cb_pid_dbg         = ptmp->CB_PID_dbg->Checked; // PID debug info
            PID_dbg->Visible   = cb_pid_dbg;
            PID_dbg2->Visible  = cb_pid_dbg;
            switch (pid_pars_hlt.pid_model)
            {
               case 0 : // Self-Tuning Takahashi, N = 1 .. 3
                        init_pid2(&pid_pars_hlt,&sys_id_pars);
                        break;
               case 1 : init_pid4(&pid_pars_hlt);   // Takahashi Type C
                        break;
               default: pid_pars_hlt.pid_model = 1; // Takahashi Type C
                        init_pid4(&pid_pars_hlt);
                        break;
            } // switch
            pid_pars_boil.pid_model = 1; // always use Takahashi Type C for Boil-Kettle
            init_pid4(&pid_pars_boil);   // Init. PID parameters for Boil-Kettle PID-controller
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

#define CH_I_PAR(nr, par, par_gui, reg) \
        if (par != par_gui)             \
        {                               \
           par = par_gui;               \
           pars_changed[nr] = true;     \
           Reg->WriteInteger(reg, par); \
        }

void __fastcall TMainForm::MenuOptionsI2CSettingsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Hardware Settings'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry     *Reg = new TRegistry();
   TI2C_Settings *ptmp;
   AnsiString    S;
   int           x1;  // temp. variable representing new USB COM Port number
   int           init_needed = false; // TRUE = Re-Init. Communication with HW

   ptmp = new TI2C_Settings(this);
   // Get Hardware Settings from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);

         ptmp->System_Mode->ItemIndex       = Reg->ReadInteger("SYSTEM_MODE");
         ptmp->update_i2c_gui();  // Start with correct labels enabled
         ptmp->Comm_Setting->ItemIndex      = Reg->ReadInteger("COMM_CHANNEL");
         ptmp->update_comm_gui(); // Start with correct labels enabled
         ptmp->COM_Port_Settings_Edit->Text = Reg->ReadString("COM_PORT_SETTINGS");
         ptmp->UDP_Settings->Text           = Reg->ReadString("UDP_IP_PORT");
         ptmp->cb_debug_com_port->Checked   = Reg->ReadBool("CB_DEBUG_COM_PORT");

         ptmp->S0L_Edit->Text = AnsiString(Reg->ReadInteger("GAS_MOD_PWM_LLIMIT"));
         ptmp->S0U_Edit->Text = AnsiString(Reg->ReadInteger("GAS_MOD_PWM_HLIMIT"));
         ptmp->S1L_Edit->Text = AnsiString(Reg->ReadInteger("GAS_NON_MOD_LLIMIT"));
         ptmp->S1U_Edit->Text = AnsiString(Reg->ReadInteger("GAS_NON_MOD_HLIMIT"));
         ptmp->S2L_Edit->Text = AnsiString(Reg->ReadInteger("TTRIAC_LLIM"));
         ptmp->S2U_Edit->Text = AnsiString(Reg->ReadInteger("TTRIAC_HLIM"));

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            x1 = ptmp->Comm_Setting->ItemIndex;  // Retrieve Comm. Channel Nr.
            if (x1 != comm_channel_nr)
            {
               init_needed     = true;
               comm_channel_nr = x1;
               Reg->WriteInteger("COMM_CHANNEL",x1); // Save new COM port nr.
            } // if
            S = ptmp->COM_Port_Settings_Edit->Text;
            if (strcmp(com_port_settings,S.c_str())) // COM Port Settings
            {
               init_needed = true;
               strcpy(com_port_settings, S.c_str());
               Reg->WriteString("COM_PORT_SETTINGS",AnsiString(com_port_settings));
            } // if
            S = ptmp->UDP_Settings->Text;
            if (strcmp(udp_ip_port,S.c_str())) // UDP IP & Port Number
            {
               init_needed = true;
               strcpy(udp_ip_port, S.c_str());
               Reg->WriteString("UDP_IP_PORT",AnsiString(udp_ip_port));
            } // if

            cb_debug_com_port = ptmp->cb_debug_com_port->Checked;
            Reg->WriteBool("CB_DEBUG_COM_PORT",cb_debug_com_port);

            CH_I_PAR( 0,system_mode       ,ptmp->System_Mode->ItemIndex,"SYSTEM_MODE");
            CH_I_PAR( 3,gas_mod_pwm_llimit,ptmp->S0L_Edit->Text.ToInt(),"GAS_MOD_PWM_LLIMIT");
            CH_I_PAR( 4,gas_mod_pwm_hlimit,ptmp->S0U_Edit->Text.ToInt(),"GAS_MOD_PWM_HLIMIT");
            CH_I_PAR( 1,gas_non_mod_llimit,ptmp->S1L_Edit->Text.ToInt(),"GAS_NON_MOD_LLIMIT");
            CH_I_PAR( 2,gas_non_mod_hlimit,ptmp->S1U_Edit->Text.ToInt(),"GAS_NON_MOD_HLIMIT");
            CH_I_PAR( 5,ttriac_llim       ,ptmp->S2L_Edit->Text.ToInt(),"TTRIAC_LLIM");
            CH_I_PAR( 6,ttriac_hlim       ,ptmp->S2U_Edit->Text.ToInt(),"TTRIAC_HLIM");

            if (init_needed)
            {  //--------------------------------------------------------
               // Comm. settings were changed. Close COM-port if
               // still open and re-open with new settings.
               //--------------------------------------------------------
               if (com_port_is_open)
               {
                  if ((com_port_opened > 0) && (comm_channel_nr == 0))
                  {  // Virtual COMx port was open && now ethernet selected
                     comm_port_write("E1\n"); // enable ethernet communication
                  }
                  else if ((com_port_opened == 0) && (comm_channel_nr > 0))
                  {  // Ethernet (UDP) was open && now virtual COMx port selected
                     comm_port_write("E0\n"); // enable USB communication
                  } // else
                  comm_port_close(); // Close current communication channel
               } // if
               comm_port_open(); // Open COM-port with new settings
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

void __fastcall TMainForm::SpargeSettings1Click(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Sparge, Mash & Boil Settings'.
             Is assumes the Init_Sparge_Settings() has been called
             during Initialisation and that the struct sp is filled.
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
        // Sparge Settings
        ptmp->EBatches->Text    = AnsiString(sp.sp_batches); // Number of Sparge Batches
        ptmp->EBTime->Text      = AnsiString(sp.sp_time);    // Time between two Sparge Batches
        ptmp->CB_VSp2->Checked  = MainForm->cb_vsp2;         // Double the Initial Sparge Volume to Boil-kettle
        // Mash Settings
        ptmp->Offs0_Edit->Text  = AnsiString(sp.temp_offset0);
        ptmp->Offs_Edit->Text   = AnsiString(sp.temp_offset);
        ptmp->Offs2_Edit->Text  = AnsiString(sp.temp_offset2);
        ptmp->Eph_time->Text    = AnsiString(sp.ph_time/60);  // PREHEAT_TIME [minutes]
        ptmp->CB_mash_rest->Checked = std.mash_rest;          // Mash rest for 5 min after malt is added
        ptmp->CB_dpht->Checked  = sp.use_dpht;                // 1= Use Dynamic preheat Timing
        ptmp->HLT_Bcap->Text    = sp.hlt_bcap;                // HLT burner capacity in sec./°C
        ptmp->CB_pumps_on->Checked = sp.leave_pumps_on;       // 1= leave pumps running when MLT temp. is reached
        // Boil-Time Settings
        ptmp->Boil_Min_Temp->Text = AnsiString(sp.boil_min_temp); // Min. Temp. for Boil-Kettle
        ptmp->EBoilTime->Text   = AnsiString(sp.boil_time);   // Total Boil Time (min.)
        ptmp->SP_PreBoil->Text  = AnsiString(sp.sp_preboil);  // Setpoint Temperature for pre-boil
        ptmp->Boil_Det->Text    = AnsiString(sp.boil_detect); // Boiling-Detection minimum Temperature (Celsius)
        ptmp->SP_Boil->Text     = AnsiString(sp.sp_boil);     // Setpoint Temperature during boiling
        ptmp->CB_boil_rest->Checked = std.boil_rest;          // Let wort rest for 5 minutes after boiling

        if (ptmp->ShowModal() == 0x1) // mrOK
        {
           // Sparge Settings
           Reg->WriteInteger("SP_BATCHES",  ptmp->EBatches->Text.ToInt());
           Reg->WriteInteger("SP_TIME",     ptmp->EBTime->Text.ToInt());
           Reg->WriteBool("CB_VSP2",        ptmp->CB_VSp2->Checked);
           MainForm->cb_vsp2 = ptmp->CB_VSp2->Checked;

           // Mash Settings
           Reg->WriteFloat("TOffset0",      ptmp->Offs0_Edit->Text.ToDouble());
           Reg->WriteFloat("TOffset",       ptmp->Offs_Edit->Text.ToDouble());
           Reg->WriteFloat("TOffset2",      ptmp->Offs2_Edit->Text.ToDouble());
           Reg->WriteInteger("PREHEAT_TIME",ptmp->Eph_time->Text.ToInt());
           Reg->WriteBool("CB_Mash_Rest",   ptmp->CB_mash_rest->Checked);
           std.mash_rest = ptmp->CB_mash_rest->Checked;
           Reg->WriteBool("CB_dpht",        ptmp->CB_dpht->Checked);
           Reg->WriteInteger("HLT_Bcap",    ptmp->HLT_Bcap->Text.ToInt());
           Reg->WriteBool("CB_pumps_on",    ptmp->CB_pumps_on->Checked);
           sp.leave_pumps_on = ptmp->CB_pumps_on->Checked;
           // Boil-Time Settings
           Reg->WriteInteger("BOIL_MIN_TEMP",ptmp->Boil_Min_Temp->Text.ToInt());
           Reg->WriteInteger("BOIL_TIME",   ptmp->EBoilTime->Text.ToInt());
           Reg->WriteInteger("SP_PREBOIL",  ptmp->SP_PreBoil->Text.ToInt());
           Reg->WriteFloat("BOIL_DETECT",   ptmp->Boil_Det->Text.ToDouble());
           Reg->WriteInteger("SP_BOIL",     ptmp->SP_Boil->Text.ToInt());
           Reg->WriteBool("CB_Boil_Rest",   ptmp->CB_boil_rest->Checked);
           std.boil_rest = ptmp->CB_boil_rest->Checked;

           Init_Sparge_Settings(); // Init. struct with new sparge settings
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

void __fastcall TMainForm::MeasurementsClick(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the function which is called whenever the user
             presses 'Options | Measurements'.
  Returns  : None
  ------------------------------------------------------------------*/
{
   TRegistry *Reg = new TRegistry();
   TMeasurements *ptmp;
   char tmp_str[30];

   ptmp = new TMeasurements(this);

   // Get Measurements Settings from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         //--------------------------
         // Temperature Measurements
         //--------------------------
         ptmp->Thlt_Offset_Edit->Text  = Reg->ReadFloat("THLT_OFFSET");
         ptmp->Tmlt_Offset_Edit->Text  = Reg->ReadFloat("TMLT_OFFSET");
         ptmp->Tboil_Offset_Edit->Text = Reg->ReadFloat("TBOIL_OFFSET");
         ptmp->Tcfc_Offset_Edit->Text  = Reg->ReadFloat("TCFC_OFFSET");

         //--------------------------
         // Volume Measurements
         //--------------------------
         ptmp->Vhlt_Max_Edit->Text     = Reg->ReadFloat("VHLT_MAX");
         ptmp->Vmlt_Max_Edit->Text     = Reg->ReadFloat("VMLT_MAX");
         ptmp->Boil_Max_Edit->Text     = Reg->ReadFloat("VBOIL_MAX"); // For Screen only
         ptmp->Flow1_Err->Text         = flow1_err;
         ptmp->Flow2_Err->Text         = flow2_err;
         ptmp->Flow3_Err->Text         = flow3_err;
         ptmp->Flow4_Err->Text         = flow4_err;
         ptmp->Flow_Temp_Corr->Checked = flow_temp_corr;
         ptmp->Min_FR_MLT->Text        = volumes.min_flowrate_mlt_perc;
         ptmp->Min_FR_Boil->Text       = volumes.min_flowrate_boil_perc;

         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            //--------------------------
            // Temperature Measurements
            //--------------------------
            thlt_offset  = ptmp->Thlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("THLT_OFFSET", thlt_offset);
            tmlt_offset  = ptmp->Tmlt_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("TMLT_OFFSET", tmlt_offset);
            tboil_offset = ptmp->Tboil_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("TBOIL_OFFSET", tboil_offset);
            tcfc_offset  = ptmp->Tcfc_Offset_Edit->Text.ToDouble();
            Reg->WriteFloat("TCFC_OFFSET", tcfc_offset);

            //--------------------------
            // Volume Measurements
            //--------------------------
            vhlt_max     = ptmp->Vhlt_Max_Edit->Text.ToDouble();
            Reg->WriteFloat("VHLT_MAX", vhlt_max);
            sprintf(tmp_str,"HLT %dL", ptmp->Vhlt_Max_Edit->Text.ToInt());    // HLT
            HLT_Label->Caption = tmp_str;
            Tank_HLT->Max = ptmp->Vhlt_Max_Edit->Text.ToInt();

            vmlt_max     = ptmp->Vmlt_Max_Edit->Text.ToDouble();
            Reg->WriteFloat("VMLT_MAX", vmlt_max);
            sprintf(tmp_str,"MLT %dL", ptmp->Vmlt_Max_Edit->Text.ToInt());    // MLT
            MLT_Label->Caption = tmp_str;
            Tank_MLT->Max = ptmp->Vmlt_Max_Edit->Text.ToInt();

            Reg->WriteFloat("VBOIL_MAX",ptmp->Boil_Max_Edit->Text.ToDouble()); // Boil Kettle
            sprintf(tmp_str,"Boil %dL", ptmp->Boil_Max_Edit->Text.ToInt());
            Boil_Label->Caption = tmp_str;
            Tank_Boil->Max = ptmp->Boil_Max_Edit->Text.ToInt();

            flow1_err = ptmp->Flow1_Err->Text.ToInt();
            Reg->WriteInteger("FLOW1_ERR",flow1_err);
            flow2_err = ptmp->Flow2_Err->Text.ToInt();
            Reg->WriteInteger("FLOW2_ERR",flow2_err);
            flow3_err = ptmp->Flow3_Err->Text.ToInt();
            Reg->WriteInteger("FLOW3_ERR",flow3_err);
            flow4_err = ptmp->Flow4_Err->Text.ToInt();
            Reg->WriteInteger("FLOW4_ERR",flow4_err);
            flow_temp_corr = ptmp->Flow_Temp_Corr->Checked;
            Reg->WriteBool("FLOW_TEMP_CORR",flow_temp_corr);
            volumes.min_flowrate_mlt_perc = ptmp->Min_FR_MLT->Text.ToInt();
            Reg->WriteInteger("MIN_FR_MLT_PERC",volumes.min_flowrate_mlt_perc);
            volumes.min_flowrate_boil_perc = ptmp->Min_FR_Boil->Text.ToInt();
            Reg->WriteInteger("MIN_FR_BOIL_PERC",volumes.min_flowrate_boil_perc);
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

   if (T100msec) T100msec->Enabled = false; // Disable Interrupt Timer
   if (ViewMashProgress)
   {
      ViewMashProgress->UpdateTimer->Enabled = false; // Stop Mash Progress Update timer
   }
   ::Sleep(51);                // Make sure that Timer is disabled
   if (ViewMashProgress)
   {
      delete ViewMashProgress;  // close modeless dialog
      ViewMashProgress = 0;     // null the pointer
   }
   // Disable Gas-Burner PWM, Heater and Alive LEDs
   comm_port_write("B0\n"); // Disable Boil-kettle gas-burner
   comm_port_write("H0\n"); // Disable HLT gas-burner
   comm_port_write("L0\n"); // Disable Alive-LED
   comm_port_write("P0\n"); // Disable Pump
   comm_port_write("V0\n"); // Disable All Valves
   if (com_port_is_open)
   {
        comm_port_close();
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
   ptmp->CheckBox1->Checked = swfx.tset_boil_sw; // Set Checkbox for Tset_boil
   if (swfx.tset_boil_sw)
   {
      ptmp->MaskEdit1->Text = AnsiString(swfx.tset_boil_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_Gamma->Checked = swfx.gamma_hlt_sw; // Set Checkbox for Gamma_HLT
   if (swfx.gamma_hlt_sw)
   {
      ptmp->Gamma_MEdit->Text = AnsiString(swfx.gamma_hlt_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CheckBox2->Checked = swfx.gamma_boil_sw; // Set Checkbox for Gamma_Boil
   if (swfx.gamma_boil_sw)
   {
      ptmp->MaskEdit2->Text = AnsiString(swfx.gamma_boil_fx); // Set fix value
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
   ptmp->CheckBox3->Checked = swfx.tboil_sw; // Set Checkbox for Tboil
   if (swfx.tboil_sw)
   {
      ptmp->MaskEdit3->Text = AnsiString(swfx.tboil_fx); // Set fix value
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
   ptmp->CheckBox4->Checked = swfx.vboil_sw; // Set Checkbox for Vboil
   if (swfx.vboil_sw)
   {
      ptmp->MaskEdit4->Text = AnsiString(swfx.vboil_fx); // Set fix value
   } // if
   //-------------------------------------------------------------------------
   ptmp->CB_std->Checked = swfx.std_sw; // Set Checkbox for ebrew_std
   if (swfx.std_sw)
   {
      ptmp->STD_MEdit->Text = AnsiString(swfx.std_fx); // Set fix value
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
      if (!read_input_file(MASH_FILE,ms,&std.ms_tot,1.0,NO_INIT_TIMERS,&sp.mash_vol,&sp.sp_vol))
      {
          MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
      } /* if */
      Init_Sparge_Settings(); // Init. struct with new sparge settings
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

void __fastcall TMainForm::Auto1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out &= ~(P0M | P0b); // Pump = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("P1") == 0)
   {
      std_out &= ~(P1M | P1b); // P1 = Auto mode + OFF
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
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V7") == 0)
   {
      std_out &= ~(V7M | V7b); // V7 = Auto mode + OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V8") == 0)
   {
      std_out &= ~(V8M | V8b); // V8 = Auto mode + OFF
   }
} // TMainForm::Auto1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::OFF1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out |=  P0M; // Pump = Manual Override mode
      std_out &= ~P0b; // Pump = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("P1") == 0)
   {
      std_out |=  P1M; // Pump = Manual Override mode
      std_out &= ~P1b; // Pump = OFF
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
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V7") == 0)
   {
      std_out |=  V7M; // V7 = Manual Override mode
      std_out &= ~V7b; // V7 = OFF
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V8") == 0)
   {
      std_out |=  V8M; // V8 = Manual Override mode
      std_out &= ~V8b; // V8 = OFF
   }
} //  TMainForm::OFF1Click()
//---------------------------------------------------------------------------

void __fastcall TMainForm::ON1Click(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      std_out |= (P0M | P0b); // Pump = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("P1") == 0)
   {
      std_out |= (P1M | P1b); // Pump = Manual Override mode + ON
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
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V7") == 0)
   {
      std_out |= (V7M | V7b); // V7 = Manual Override mode + ON
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V8") == 0)
   {
      std_out |= (V8M | V8b); // V8 = Manual Override mode + ON
   }
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
   else SET_POPUPMENU("P1",P1M,P1b)
   else SET_POPUPMENU("V1",V1M,V1b)
   else SET_POPUPMENU("V2",V2M,V2b)
   else SET_POPUPMENU("V3",V3M,V3b)
   else SET_POPUPMENU("V4",V4M,V4b)
   else SET_POPUPMENU("V5",V5M,V5b)
   else SET_POPUPMENU("V6",V6M,V6b)
   else SET_POPUPMENU("V7",V7M,V7b)
   else SET_POPUPMENU("V8",V8M,V8b)
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

   //---------------------
   // Actual Temperatures
   //---------------------
   sprintf(tmp_str,"%4.2f°C",thlt);   // Display Thlt value on screen
   Val_Thlt->Caption    = tmp_str;

   sprintf(tmp_str,"%4.2f°C",tmlt);   // Display Tmlt value on screen
   Val_Tmlt->Caption    = tmp_str;

   sprintf(tmp_str,"%4.2f°C",tboil);  // Display Tboil on screen
   Temp_Boil->Caption     = tmp_str;

   sprintf(tmp_str,"%4.2f°C",tcfc);  // Display Tcfc on screen
   Temp_CFC->Caption      = tmp_str;

   tm_triac->Value->Value = ttriac;   // Update Ttriac object
   sprintf(tmp_str,"%3.1f°C",ttriac);
   Ttriac_lbl->Caption    = tmp_str;

   // Triac Temperature Protection: hysteresis function
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

   //------------------------
   // Temperature Setpoints
   //------------------------
   sprintf(tmp_str,"SP %4.1f°C",tset_hlt);  // Setpoint HLT Temperature
   Val_Tset_hlt->Caption = tmp_str;
   if (MainForm->PID_Ctrl->Active) Val_Tset_hlt->Font->Color = clLime;
   else                            Val_Tset_hlt->Font->Color = clRed;
   
   sprintf(tmp_str,"SP %3.0f°C",tset_mlt);  // Setpoint MLT Temperature
   Val_Tset_mlt->Caption   = tmp_str;

   sprintf(tmp_str,"SP %3.0f°C",tset_boil); // Setpoint Boil-Kettle Temperature
   Val_Tset_Boil->Caption = tmp_str;
   if (MainForm->sp.pid_ctrl_boil_on) Val_Tset_Boil->Font->Color = clLime;
   else                               Val_Tset_Boil->Font->Color = clRed;

   //--------------------------
   // Actual Volumes and Flows
   //--------------------------
   sprintf(tmp_str,"%4.1f L",volumes.Vhlt);  // Actual Volume HLT
   Vol_HLT->Caption = tmp_str;
   if (volumes.Vhlt >= 0.0) Tank_HLT->Position = volumes.Vhlt;

   sprintf(tmp_str,"%4.1f L",volumes.Vmlt);  // Actual Volume MLT
   Vol_MLT->Caption   = tmp_str;
   if (volumes.Vmlt >= 0.0) Tank_MLT->Position = volumes.Vmlt;

   sprintf(tmp_str,"%4.1f L",volumes.Vboil); // Actual Volume Boil-Kettle
   Vol_Boil->Caption = tmp_str;
   if (volumes.Vboil >= 0.0) Tank_Boil->Position = volumes.Vboil;

   sprintf(tmp_str,"%4.1f L",volumes.Flow_hlt_mlt);            // Display flow from HLT -> MLT
   Flow1_hlt_mlt->Caption = tmp_str;
   sprintf(tmp_str,"%4.1f L/min.",volumes.Flow_rate_hlt_mlt);  // Display flowrate from HLT -> MLT
   Flow1_rate->Caption = tmp_str;

   sprintf(tmp_str,"%4.1f L",volumes.Flow_mlt_boil);           // Display flow from MLT -> Boil
   Flow2_mlt_boil->Caption = tmp_str;
   sprintf(tmp_str,"%4.1f L/min.",volumes.Flow_rate_mlt_boil); // Display flowrate from MLT -> Boil
   Flow2_rate->Caption = tmp_str;

   sprintf(tmp_str,"%4.1f L",volumes.Flow_cfc_out);            // Display flow from CFC-output
   Flow3_cfc->Caption = tmp_str;
   sprintf(tmp_str,"%3.1f L/min. (%d, %d, %4.1f)",volumes.Flow_rate_cfc_out,
           frl_empty_boil.frl_std,frl_empty_boil.frl_tmr,frl_empty_boil.frl_det_lim);  // Display flowrate from CFC-output
   Flow3_rate->Caption = tmp_str;

   sprintf(tmp_str,"%4.1f L",volumes.Flow4);                   // Display flow from CFC-output
   Flow4->Caption = tmp_str;
   sprintf(tmp_str,"%4.1f L/min.",volumes.Flow_rate4);         // Display flowrate4
   Flow4_rate->Caption = tmp_str;

   //------------------------
   // PID-Controller Outputs
   //------------------------
   Heater->Value = gamma_hlt; // PID-output for HLT
   sprintf(tmp_str,"%d %%",(int)gamma_hlt);
   Gamma_Perc->Caption = tmp_str;

   Boil->Value = gamma_boil; // PID-output for Boil-Kettle
   sprintf(tmp_str,"%d %%",(int)gamma_boil);
   Gamma_Boil->Caption = tmp_str;

   switch (std.ebrew_std)
   {
     case S00_INITIALISATION:
          Std_State->Caption = "00. Initialisation";
          break;
     case S01_WAIT_FOR_HLT_TEMP:
          sprintf(tmp_str,"01. Wait for HLT Temperature (%2.1f °C)",MainForm->tset_hlt);
          Std_State->Caption = tmp_str;
          break;
     case S02_FILL_MLT:
          sprintf(tmp_str,"02. Fill MLT with %d L water",sp.mash_vol);
          Std_State->Caption = tmp_str;
          break;
     case S03_WAIT_FOR_MLT_TEMP:
          sprintf(tmp_str,"03. Wait for MLT Temperature (%2.1f °C)",ms[std.ms_idx].temp + sp.temp_offset2);
          Std_State->Caption = tmp_str;
          break;
     case S04_MASH_TIMER_RUNNING:
          sprintf(tmp_str,"04. Mash-Timer Running (%d/%2.0f min.)",ms[std.ms_idx].timer/60,ms[std.ms_idx].time/60.0);
          Std_State->Caption = tmp_str;
          break;
     case S05_SPARGE_TIMER_RUNNING:
          sprintf(tmp_str,"05. Sparge-Timer Running (%d/%d min.)",std.timer1/60,sp.sp_time);
          Std_State->Caption = tmp_str;
          break;
     case S06_PUMP_FROM_MLT_TO_BOIL:
          sprintf(tmp_str,"06. Pump from MLT to Boil-Kettle (%2.1f L)",std.sp_idx ? sp.sp_vol_batch : sp.sp_vol_batch0);
          Std_State->Caption = tmp_str;
          break;
     case S07_PUMP_FROM_HLT_TO_MLT:
          sprintf(tmp_str,"07. Pump fresh water from HLT to MLT (%2.1f L)",sp.sp_vol_batch);
          Std_State->Caption = tmp_str;
          break;
     case S08_DELAY_xSEC:
          sprintf(tmp_str,"08. Delay: %d seconds",TMR_DELAY_xSEC);
          Std_State->Caption = tmp_str;
          break;
     case S09_EMPTY_MLT:
          Std_State->Caption = "09. Empty MLT";
          break;
     case S10_WAIT_FOR_BOIL:
          Std_State->Caption = "10. Wait for Boil (M)";
          break;
     case S11_BOILING:
          sprintf(tmp_str,"11. Now Boiling (%d/%d min.)",std.timer5/60,sp.boil_time);
          Std_State->Caption = tmp_str;
          break;
     case S12_BOILING_FINISHED:
          if (std.boil_rest)
          {
             sprintf(tmp_str,"12. Boiling Finished, wait %d/%d min., prepare Chiller (M)",std.brest_tmr/60,TMR_BOIL_REST_5_MIN/60);
             Std_State->Caption = tmp_str;
          }
          else Std_State->Caption = "12. Boiling Finished, prepare Chiller (M)";
          break;
     case S13_MASH_PREHEAT_HLT:
          sprintf(tmp_str,"13. Mash Preheat HLT (%d/%2.0f min.)",ms[std.ms_idx].timer/60,ms[std.ms_idx].time/60.0);
          Std_State->Caption = tmp_str;
          break;
     case S14_PUMP_PREFILL:
          sprintf(tmp_str,"14. Pump Pre-Fill / Priming (%d/%d sec.)",std.timer3,TMR_PREFILL_PUMP);
          Std_State->Caption = tmp_str;
          break;
     case S15_ADD_MALT_TO_MLT:
          Std_State->Caption = "15. Add Malt to MLT (M)";
          break;
     case S16_CHILL_PUMP_FERMENTOR:
          Std_State->Caption = "16. Chill && Pump to Fermentation Bin (M)";
          break;
     case S17_FINISHED:
          Std_State->Caption = "17. Finished!";
          break;
     case S18_MASH_REST_5_MIN:
          sprintf(tmp_str,"18. Mash-Rest (%d/%d sec.)",std.mrest_tmr,TMR_MASH_REST_5_MIN);
          Std_State->Caption = tmp_str;
          break;
     case S19_RDY_TO_ADD_MALT:
          Std_State->Caption = "19. Ready to add Malt to MLT (M)";
          break;
     //---------------------------------------------------------
     // These are the Cleaning-In-Place (CIP) states
     //---------------------------------------------------------
     case S20_CIP_INIT:
          Std_State->Caption = "20. CIP: Initialisation";
          break;
     case S21_CIP_HEAT_UP:
          Std_State->Caption = "21. CIP: Heat up and Circulate";
          break;
     case S22_CIP_CIRC_5_MIN:
          sprintf(tmp_str,"22. CIP: Circulating (%d/%d sec.)",std.cip_tmr1,TMR_CIP_CIRC_TIME);
          Std_State->Caption = tmp_str;
          break;
     case S23_CIP_REST_5_MIN:
          sprintf(tmp_str,"23. CIP: Resting (%d/%d sec.)",std.cip_tmr1,TMR_CIP_REST_TIME);
          Std_State->Caption = tmp_str;
          break;
     case S24_CIP_DRAIN_BOIL1:
          Std_State->Caption = "24. CIP: Drain Boil-Kettle 1";
          break;
     case S25_CIP_DRAIN_BOIL2:
          Std_State->Caption = "25. CIP: Drain Boil-Kettle 2";
          break;
     case S26_CIP_FILL_HLT:
          Std_State->Caption = "26. CIP: Fill HLT with fresh water";
          break;
     case S27_CIP_CLEAN_OUTPUT_V7:
          Std_State->Caption = "27. CIP: Clean Output V7";
          break;
     case S28_CIP_CLEAN_OUTPUT_V6:
          Std_State->Caption = "28. CIP: Clean Output V6";
          break;
     case S29_CIP_CLEAN_OUTPUT_V4:
          Std_State->Caption = "29. CIP: Clean Output V4";
          break;
     case S30_CIP_CLEAN_INPUT_V3:
          Std_State->Caption = "30. CIP: Clean Input V3";
          break;
     case S31_CIP_CLEAN_INPUT_V1:
          Std_State->Caption = "31. CIP: Clean Input V1";
          break;
     case S32_CIP_END:
          Std_State->Caption = "32. CIP: End of Program";
          break;
     default:
          Std_State->Caption = "xx. Unknown State";
          break;
   } // switch

   //--------------------------------------------------------------------------
   // Update the Captions for all valves (they might have
   // changed as a result of the call to update_std().
   // The change in Manual Override settings is taken care of
   // in the PopupMemu. Therefore only update the valve captions
   // in Auto Mode in case of a bit change.
   //--------------------------------------------------------------------------
   switch (std_out & (P0M | P0b))
   {
      case P0M | P0b: P0->Caption = P01MTXT; P0->Font->Color = clRed;   break;
      case P0M      : P0->Caption = P00MTXT; P0->Font->Color = clGreen; break;
      case P0b      : P0->Caption = P01ATXT; P0->Font->Color = clRed;   break;
      default       : P0->Caption = P00ATXT; P0->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (P1M | P1b))
   {
      case P1M | P1b: P1->Caption = P11MTXT; P1->Font->Color = clRed;   break;
      case P1M      : P1->Caption = P10MTXT; P1->Font->Color = clGreen; break;
      case P1b      : P1->Caption = P11ATXT; P1->Font->Color = clRed;   break;
      default       : P1->Caption = P10ATXT; P1->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V1M | V1b))
   {
      case V1M | V1b: V1->Caption = V11MTXT; V1->Font->Color = clRed;   break;
      case V1M      : V1->Caption = V10MTXT; V1->Font->Color = clGreen; break;
      case V1b      : V1->Caption = V11ATXT; V1->Font->Color = clRed;   break;
      default       : V1->Caption = V10ATXT; V1->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V2M | V2b))
   {
      case V2M | V2b: V2->Caption = V21MTXT; V2->Font->Color = clRed;   break;
      case V2M      : V2->Caption = V20MTXT; V2->Font->Color = clGreen; break;
      case V2b      : V2->Caption = V21ATXT; V2->Font->Color = clRed;   break;
      default       : V2->Caption = V20ATXT; V2->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V3M | V3b))
   {
      case V3M | V3b: V3->Caption = V31MTXT; V3->Font->Color = clRed;   break;
      case V3M      : V3->Caption = V30MTXT; V3->Font->Color = clGreen; break;
      case V3b      : V3->Caption = V31ATXT; V3->Font->Color = clRed;   break;
      default       : V3->Caption = V30ATXT; V3->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V4M | V4b))
   {
      case V4M | V4b: V4->Caption = V41MTXT; V4->Font->Color = clRed;   break;
      case V4M      : V4->Caption = V40MTXT; V4->Font->Color = clGreen; break;
      case V4b      : V4->Caption = V41ATXT; V4->Font->Color = clRed;   break;
      default       : V4->Caption = V40ATXT; V4->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V6M | V6b))
   {
      case V6M | V6b: V6->Caption = V61MTXT; V6->Font->Color = clRed;   break;
      case V6M      : V6->Caption = V60MTXT; V6->Font->Color = clGreen; break;
      case V6b      : V6->Caption = V61ATXT; V6->Font->Color = clRed;   break;
      default       : V6->Caption = V60ATXT; V6->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V7M | V7b))
   {
      case V7M | V7b: V7->Caption = V71MTXT; V7->Font->Color = clRed;   break;
      case V7M      : V7->Caption = V70MTXT; V7->Font->Color = clGreen; break;
      case V7b      : V7->Caption = V71ATXT; V7->Font->Color = clRed;   break;
      default       : V7->Caption = V70ATXT; V7->Font->Color = clGreen; break;
   } // switch
   switch (std_out & (V8M | V8b))
   {
      case V8M | V8b: V8->Caption = V81MTXT; V8->Font->Color = clRed;   break;
      case V8M      : V8->Caption = V80MTXT; V8->Font->Color = clGreen; break;
      case V8b      : V8->Caption = V81ATXT; V8->Font->Color = clRed;   break;
      default       : V8->Caption = V80ATXT; V8->Font->Color = clGreen; break;
   } // switch

   //-------------------------------------------
   // Debug PID Controller
   //-------------------------------------------
   if (cb_pid_dbg)
   {
      if (pid_pars_hlt.pid_model == 0)
      {  // pid_model = 0: Self-Tuning controllers
         sprintf(tmp_str,"%6.2f %6.2f %6.2f T=%6.2f G=%6.2f (%6.2f %6.2f %6.2f)",
                         pid_pars_hlt.pp, pid_pars_hlt.pi, pid_pars_hlt.pd,
                         pid_pars_hlt.pp + pid_pars_hlt.pi + pid_pars_hlt.pd, gamma_hlt,
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
         sprintf(tmp_str,"%6.2f %6.2f %6.2f T=%6.2f G=%6.2f",
                         pid_pars_hlt.pp, pid_pars_hlt.pi, pid_pars_hlt.pd,
                         pid_pars_hlt.pp + pid_pars_hlt.pi + pid_pars_hlt.pd, gamma_hlt);
      } // else
      PID_dbg->Caption = tmp_str;
      sprintf(tmp_str,"%6.2f %6.2f %6.2f T=%6.2f G=%6.2f",
                      pid_pars_boil.pp, pid_pars_boil.pi, pid_pars_boil.pd,
                      pid_pars_boil.pp + pid_pars_boil.pi + pid_pars_boil.pd, gamma_boil);
      PID_dbg2->Caption = tmp_str;
   } // if

   bool any_input_on  = std_out & (V1b | V2b | V3b);
   bool any_output_on = std_out & (V4b | V6b | V7b);
   //-------------------------------------------------------------
   // Output lines: clNavy (OFF) - clAqua (ON)
   //-------------------------------------------------------------
   if (((std_out & (P0b | V4b)) == (P0b | V4b)) && any_input_on)
   {  // Output Pump -> Heat-Exchanger HLT -> MLT
      VrGradient1->StartColor = clAqua; VrGradient1->EndColor = clAqua;
      VrGradient2->StartColor = clAqua; VrGradient2->EndColor = clAqua;
      VrGradient3->StartColor = clAqua; VrGradient3->EndColor = clAqua;
      VrGradient4->StartColor = clAqua; VrGradient4->EndColor = clAqua;
      VrGradient5->StartColor = clAqua; VrGradient5->EndColor = clAqua;
   }
   else
   {
      VrGradient1->StartColor = clNavy; VrGradient1->EndColor = clNavy;
      VrGradient2->StartColor = clNavy; VrGradient2->EndColor = clNavy;
      VrGradient3->StartColor = clNavy; VrGradient3->EndColor = clNavy;
      VrGradient4->StartColor = clNavy; VrGradient4->EndColor = clNavy;
      VrGradient5->StartColor = clNavy; VrGradient5->EndColor = clNavy;
   } // else
   //-------------------------------------------------------------
   if (((std_out & (P0b | V6b)) == (P0b | V6b) ||
        (std_out & (P0b | V7b)) == (P0b | V7b)) && any_input_on)
   {  // Pump Output -> CFC or Boil-kettle
      VrGradient6->StartColor = clAqua; VrGradient6->EndColor = clAqua;
   }
   else
   {
      VrGradient6->StartColor = clNavy; VrGradient6->EndColor = clNavy;
   } // else
   //-------------------------------------------------------------
   if (((std_out & (P0b | V7b)) == (P0b | V7b)) && any_input_on)
   {  // From CFC -> Boil kettle
      VrGradient7->StartColor = clAqua; VrGradient7->EndColor = clAqua;
      VrGradient8->StartColor = clAqua; VrGradient8->EndColor = clAqua;
      flow2_running = true;
   }
   else
   {
      VrGradient7->StartColor = clNavy; VrGradient7->EndColor = clNavy;
      VrGradient8->StartColor = clNavy; VrGradient8->EndColor = clNavy;
      flow2_running = false;
   } // else
   //-------------------------------------------------------------
   // INPUT LINES: clMaroon (OFF) - clRed (ON)
   //-------------------------------------------------------------
   if (((std_out & (P0b | V2b)) == (P0b | V2b)) && any_output_on)
   {  // From HLT -> Pump input
      VrGradient9->StartColor = clRed; VrGradient9->EndColor = clRed;
   }
   else
   {
      VrGradient9->StartColor = clMaroon; VrGradient9->EndColor = clMaroon;
   } // else
   //-------------------------------------------------------------
   if (((std_out & (P0b | V1b)) == (P0b | V1b) ||
        (std_out & (P0b | V3b)) == (P0b | V3b)) && any_output_on)
   {  // From MLT -> Pump input
      VrGradient10->StartColor = clRed; VrGradient10->EndColor = clRed;
      flow1_running = true;
   }
   else
   {
      VrGradient10->StartColor = clMaroon; VrGradient10->EndColor = clMaroon;
      flow1_running = false;
   } // else
   //-------------------------------------------------------------
   if (((std_out & (P0b | V3b)) == (P0b | V3b)) && any_output_on)
   {  // From Boil-kettle -> Pump input
      VrGradient11->StartColor = clRed; VrGradient11->EndColor = clRed;
   }
   else
   {
      VrGradient11->StartColor = clMaroon; VrGradient11->EndColor = clMaroon;
   } // else
   //-------------------------------------------------------------
   if ((std_out & P0b) && any_input_on && any_output_on)
   {  // 12: Pump input, 13: pump output
      VrGradient12->StartColor = clRed; VrGradient12->EndColor = clRed;
      VrGradient13->StartColor = clAqua; VrGradient13->EndColor = clAqua;
   }
   else
   {
      VrGradient12->StartColor = clMaroon; VrGradient12->EndColor = clMaroon;
      VrGradient13->StartColor = clNavy;   VrGradient13->EndColor = clNavy;
   } // else
   //-------------------------------------------------------------
   if (((std_out & (P0b | V6b)) == (P0b | V6b)) && any_input_on)
   {  // CFC
      VrGradient14->StartColor = clAqua; VrGradient14->EndColor = clAqua;
      VrGradient15->StartColor = clAqua; VrGradient15->EndColor = clAqua;
      flow3_running = true;
   }
   else
   {
      VrGradient14->StartColor = clNavy; VrGradient14->EndColor = clNavy;
      VrGradient15->StartColor = clNavy; VrGradient15->EndColor = clNavy;
      flow3_running = false;
   } // else
   //-------------------------------------------------------------

   //-------------------------------------------
   // Update the various panels of the Statusbar
   //-------------------------------------------
   strcpy(tmp_str,"Sensor Alarm: ");
   switch (no_sound)
   {
        case ALARM_OFF:               strcat(tmp_str,"OFF"); break;
        case ALARM_TEMP_SENSORS:      strcat(tmp_str,"TEMP"); break;
        case ALARM_FLOW_SENSORS:      strcat(tmp_str,"FLOW"); break;
        case ALARM_TEMP_FLOW_SENSORS: strcat(tmp_str,"TEMP+FLOW"); break;
   } // if
   StatusBar->Panels->Items[PANEL_SYS_MODE]->Text = AnsiString(tmp_str);
   sprintf(tmp_str,"ms_idx = %d",std.ms_idx);
   StatusBar->Panels->Items[PANEL_MSIDX]->Text = AnsiString(tmp_str);
   sprintf(tmp_str,"sp_idx = %d",std.sp_idx);
   StatusBar->Panels->Items[PANEL_SPIDX]->Text = AnsiString(tmp_str);
   sprintf(tmp_str,"Mash: %dL, Sparge: %dL",sp.mash_vol,sp.sp_vol);
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
   if ((UpCase(Key) == 'D') && !hw_debug_logging)
   {  // Ebrew Hardware Debug Command
      hw_debug_logging = true;
   } // if
   else if (UpCase(Key) == 'P')
   {
      std_out |= P0M; // Set Pump Manual bit
      std_out ^= P0b; // Toggle Pump On/Off
   } // else if
   else if (UpCase(Key) == 'Q')
   {
      std_out |= P1M; // Set Pump 2 Manual bit
      std_out ^= P1b; // Toggle Pump 2 On/Off
   } // else if
   else if (UpCase(Key) == 'R')
   {
      comm_port_write("R0\n"); // Reset all Flows to 0.0 L in ebrew hardware
   } // else if
   else if (UpCase(Key) == 'S')
   {  // toggle between various alarms or no alarm
      if (++no_sound > ALARM_TEMP_FLOW_SENSORS) no_sound = ALARM_OFF;
   } // else if
   else if (UpCase(Key) == 'H')
   {
      swfx.gamma_hlt_sw = true; // Set switch for gamma
      if (swfx.gamma_hlt_fx > 99.9)
      {
         swfx.gamma_hlt_fx = 0.0; // fix gamma to 0 %
      } // if
      else
      {
         swfx.gamma_hlt_fx = 100.0; // fix gamma to 100 %
      } // else
   } // else if
   else if ((Key >= '1') && (Key <= '8'))
   {
      // This code only works if V8 is the MSB and V1 is the LSB!! (see misc.h)
      std_out |= (V1M << (Key - '1')); // set corresponding V1M..V8M bit
      std_out ^= (V1b << (Key - '1')); // set corresponding V1b..V8b bit
   } // else if
   else if (UpCase(Key) == 'A')
   {
      // Auto All
      std_out &= ~ALL_MANUAL;    // set everything to Auto again
      swfx.gamma_hlt_sw = false; // Release switch for gamma
   } // else if
} // FormKeyPress()
//---------------------------------------------------------------------------

void __fastcall TMainForm::MaltaddedtoMLT1Click(TObject *Sender)
{
    MaltaddedtoMLT1->Checked = True;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Boilingstarted1Click(TObject *Sender)
{
  Boilingstarted1->Checked = True;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartChilling1Click(TObject *Sender)
{
  StartChilling1->Checked = True;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ChillingFinished1Click(TObject *Sender)
{
  ChillingFinished1->Checked = True;
}

void __fastcall TMainForm::CIP_StartClick(TObject *Sender)
{
   CIP_Start->Checked = !CIP_Start->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddingMalt1Click(TObject *Sender)
{
   AddingMalt1->Checked = True;
}
//---------------------------------------------------------------------------

