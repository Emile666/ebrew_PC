// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
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
#include "EditMashScheme.h"
#include "ViewMashProgressForm.h"
#include "DataGraphForm.h"
#include "Sparge_Settings.h"
#include "RestoreSettings.h"
#include "misc.h"
#include "VersionAwareAbout.h"
#include <Dialogs.hpp>

#include "i2c_dll.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AnimTimer"
#pragma link "VrControls"
#pragma link "VrTank"
#pragma link "VrThermoMeter"
#pragma link "VrPowerMeter"
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
                                                         p1[k].sp_idx,
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
         ms_idx           = p1[k].lms_idx;
         for (j = 0; j < ms_idx; j++)
         {
            ms[j].timer = ms[j].time; // set previous timers to time-out
         } // for j
         if (p1[k].std_val >= S05_SPARGING_REST)
         {
            ms[ms_idx].timer = ms[ms_idx].time; // set to time-out if mashing is finished
         }
         else
         {
            ms[ms_idx].timer = p1[k].tmr_ms_idx; // do a best guess for the timer value
         } // else
         //----------------------------
         // Restore Sparging parameters
         //----------------------------
         std.ebrew_std = p1[k].std_val; // Current state
         std.sp_idx    = p1[k].sp_idx;  // Sparging sessions done
         std.vmash     = p1[k].vmash;   // Vmash value
         x             = p1[k].eline + 1 - p1[k].start_lstd;
         x            *= 5;             // Log-file -> 5 sec.: STD called -> 1 sec.
         switch (std.ebrew_std) // init. timers for states that have timers
         {
            case S05_SPARGING_REST:        std.timer1 = x;
                                           break;
            case S08_DELAY_xSEC:           std.timer2 = x;
                                           break;
            case S10_BOILING:              if (p1[k].max_std > S10_BOILING)
                                           {
                                              std.timer3 = NOT_STARTED;
                                              std.timer4 = sp.to4;
                                              std.timer5 = x + sp.to3 + sp.to4;
                                           }
                                           else
                                           {
                                              std.timer3 = x;
                                              std.timer4 = 0;
                                              std.timer5 = x;
                                           }
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
   char s[80];          // temp. string
   char s1[20];         // temp. string
   char st[1024];       // string for MessageBox
   int  i;              // temp. variable
   int  x1;             // Temp. var. for I2C HW base address
   int  fscl_prescaler; // index into PCF8584 prescaler values
   //---------------------------------------------------------------------------
   // The PCF8584 uses register S2 to set the proper SCL frequency. It contains
   // bytes S4,S3,S2 (control CLK frequency) and S1, S0 (control fscl).
   // This results in the following modes (assume CLK = 12 MHz):
   //
   // Div_by     Reg_S2     fscl(kHz)     Div_by     Reg_S2     fscl(kHz)
   //   8192   11111 (0x1F)   1.46           512   10110 (0x16)  22.44
   //   5120   11011 (0x1B)   2.34           384   10010 (0x12)  31.25
   //   4096   10111 (0x17)   2.93           256   00010 (0x02)  46.88
   //   3072   10011 (0x13)   3.91           160   11001 (0x19)  75.00
   //   2048   00011 (0x03)   5.86           128   10101 (0x15)  93.75
   //   1024   11110 (0x1E)  11.72            96   10001 (0x11) 125.00
   //    640   11010 (0x1A)  18.75
   //---------------------------------------------------------------------------
   byte fscl_values[] = {0x1F, 0x1B, 0x17, 0x13, 0x03, 0x1E, 0x1A,
                         0x16, 0x12, 0x02, 0x19, 0x15, 0x11};

   //----------------------------------------
   // Initialise all variables from Registry
   //----------------------------------------
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         x1 = Reg->ReadInteger("I2C_Base");    // Read HW IO address as an int.
         known_hw_devices = Reg->ReadInteger("KNOWN_HW_DEVICES");
         fscl_prescaler   = Reg->ReadInteger("FSCL_PRESCALER");
         if ((fscl_prescaler < 0) || (fscl_prescaler > 12))
         {
            fscl_prescaler = 5; // set fscl to 11.72 kHz
            Reg->WriteInteger("FSCL_PRESCALER",fscl_prescaler);
         } // if
         i = Reg->ReadInteger("TS");  // Read TS from registry
         pid_pars.ts = (double)i;
         i = Reg->ReadInteger("Kc");  // Read Kc from registry
         pid_pars.kc = (double)i;
         i = Reg->ReadInteger("Ti");  // Read Ti from registry
         pid_pars.ti = (double)i;
         i = Reg->ReadInteger("Td");  // Read Td from registry
         pid_pars.td = (double)i;
         pid_pars.temp_offset = Reg->ReadInteger("TOffset");
         pid_pars.mash_control = Reg->ReadInteger("Mash_Control"); // 0 = Tad1, 1 = Tad2

         led1 = Reg->ReadInteger("LED1"); // Read led1 from registry
         led2 = Reg->ReadInteger("LED2"); // Read led2 from registry
         led3 = Reg->ReadInteger("LED3"); // Read led3 from registry
         led4 = Reg->ReadInteger("LED4"); // Read led4 from registry

         led1_vis = Reg->ReadInteger("LED1_VIS"); // Read led1 Visibility
         led2_vis = Reg->ReadInteger("LED2_VIS"); // Read led2 Visibility
         led3_vis = Reg->ReadInteger("LED3_VIS"); // Read led3 Visibility
         led4_vis = Reg->ReadInteger("LED4_VIS"); // Read led4 Visibility

         padc.vref1 = Reg->ReadInteger("VREF1"); // Read Vref1 from registry
         padc.vref2 = Reg->ReadInteger("VREF2"); // Read Vref2 from registry
         padc.vref3 = Reg->ReadInteger("VREF3"); // Read Vref3 from registry
         padc.vref4 = Reg->ReadInteger("VREF4"); // Read Vref4 from registry
         padc.dac   = Reg->ReadInteger("DAC");   // Read DAC Value

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM"); // Read high limit
         tm_triac->SetPoint->Value = ttriac_hlim;
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM"); // Read low limit

         volumes.Vhlt_start      = Reg->ReadInteger("VHLT_START"); // Read initial volume
         volumes.Vhlt_simulated  = Reg->ReadBool("VHLT_SIMULATED");
         volumes.Vboil_simulated = Reg->ReadBool("VBOIL_SIMULATED");
         cb_i2c_err_msg          = Reg->ReadBool("CB_I2C_ERR_MSG"); // display message

         Reg->CloseKey();      // Close the Registry
         init_adc(&padc);      // Calculate ADC conversion constants
         init_pid2(&pid_pars); // Init. PID controller
         // Do NOT delete Reg yet, since we need it further on
      } // if
   } // try
   catch (ERegistryException &E)
   {
      ShowMessage(E.Message);
   } // catch

   //----------------------------------------------------------------------
   // Start I2C Communication: ISA PCB Card with Base Address from Registry
   //----------------------------------------------------------------------
   hw_status = i2c_init(x1,TRUE,fscl_values[fscl_prescaler]);
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
                        if (hw_status != known_hw_devices)
                        {  // Print only if HW device configuration has changed
                           MessageBox(NULL,st,"Results of I2C Hardware Check",MB_OK);
                        }
                        break; /* NO_ERR */
      } // switch
   } // else

   //-------------------------------------
   // Read Mash Scheme for maisch.sch file
   //-------------------------------------
   if (!read_input_file(MASH_FILE,ms,&ms_tot,pid_pars.ts))
   {
       MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
   } /* if */
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
         Reg->WriteInteger("ms_idx",ms_idx); // update registry setting
         tset_hlt = ms[ms_idx].temp; // Set tset value for HLT
         Reg->CloseKey();        // Close the Registry
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

   // Init. 5th order Moving Average (MA) filter for pressure transducer
   //-------------------------------------------------------------------
   init_ma(&str_vmlt,5); // 5th order MA filter

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
      fprintf(fd,"Kc = %6.2f, Ti = %6.2f, Td = %6.2f, Ts = %6.2f\n",pid_pars.kc,pid_pars.ti,pid_pars.td,pid_pars.ts);
      fprintf(fd,"k0 = %6.2f, k1 = %6.2f, k2 = %6.2f\n",pid_pars.k0,pid_pars.k1,pid_pars.k2);
      fprintf(fd,"hw_status = 0x%2X, ms_tot =%2d\n",hw_status,ms_tot);
      fprintf(fd,"Temp Offset = %4.1f, Mash progress controlled by Tad%1d ",pid_pars.temp_offset,1+pid_pars.mash_control);
      (pid_pars.mash_control == 0) ? fprintf(fd,"(Thlt)\n") : fprintf(fd,"(Tmlt)\n");
      fprintf(fd,"Vref1 = %3d, Vref2 = %3d, Vref3 = %3d, Vref4 = %3d, DAC-value = %3d\n\n",
                 padc.vref1,padc.vref2,padc.vref3,padc.vref4,padc.dac);
      fprintf(fd,"Time-stamp Tset_mlt Tset_hlt Thlt   Tmlt  TTriac   Vmlt PID ms_ ebrew\n");
      fprintf(fd,"[hh:mm:ss]  [°C]    [°C]     [°C]   [°C]   [°C]    [L]  _on idx _std\n");
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
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
/*------------------------------------------------------------------
  Purpose  : This is the main constructor for the program.
  Returns  : None
  ------------------------------------------------------------------*/
{
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
          Reg->WriteInteger("FSCL_PRESCALER",5); // set fscl to 11.72 kHz
          // PID Settings Dialog
          Reg->WriteInteger("TS",TS_INIT);  // Set Default value
          Reg->WriteInteger("Kc",KC_INIT);  // Controller gain
          Reg->WriteInteger("Ti",TI_INIT);  // Ti constant
          Reg->WriteInteger("Td",TD_INIT);  // Td constant

          //-------------------------------------------------------
          // For the LED Displays: 0=Thlt    , 1=Tmlt  , 2=Tset_hlt
          //                       3=Tset_mlt, 4=Ttriac, 5=Vmlt
          //-------------------------------------------------------
          led1 = 0; Reg->WriteInteger("LED1",led1);   // Thlt
          led2 = 1; Reg->WriteInteger("LED2",led2);   // Tmlt
          led3 = 2; Reg->WriteInteger("LED3",led3);   // Tset_hlt
          led4 = 3; Reg->WriteInteger("LED4",led4);   // Tset_mlt

          led1_vis = 4; // 12 mA visibility
          Reg->WriteInteger("LED1_VIS",led1_vis); // LED1 Visibility
          led2_vis = 4; // 12 mA visibility
          Reg->WriteInteger("LED2_VIS",led2_vis); // LED2 Visibility
          led3_vis = 4; // 12 mA visibility
          Reg->WriteInteger("LED3_VIS",led3_vis); // LED3 Visibility
          led4_vis = 4; // 12 mA visibility
          Reg->WriteInteger("LED4_VIS",led4_vis); // LED4 Visibility

          ttriac_hlim = 70; // Upper limit for triac temp.
          Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
          ttriac_llim = 60; // Lower limit for triac temp.
          Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);
          volumes.Vhlt_start = 90; // Starting volume of HLT
          Reg->WriteInteger("VHLT_START",volumes.Vhlt_start);
          volumes.Vhlt_simulated  = true;
          Reg->WriteBool("VHLT_SIMULATED",volumes.Vhlt_simulated);
          volumes.Vboil_simulated = true;
          Reg->WriteBool("VBOIL_SIMULATED",volumes.Vboil_simulated);
          cb_i2c_err_msg          = true;
          Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);
          known_hw_devices = DIG_IO_LSB_OK | LED1_OK | LED2_OK   | LED3_OK |
                             LED4_OK       | ADDA_OK | LM92_1_OK | LM92_2_OK;

          // Init values for mash scheme variables
          Reg->WriteInteger("ms_idx",MAX_MS);   // init. index in mash scheme
          Reg->WriteInteger("VREF1",VREF_INIT); // init. ADC1 VREF
          Reg->WriteInteger("VREF2",VREF_INIT); // init. ADC2 VREF
          Reg->WriteInteger("VREF3",VREF_INIT); // init. ADC3 VREF
          Reg->WriteInteger("VREF4",VREF_INIT); // init. ADC4 VREF
          Reg->WriteInteger("DAC",0);           // Init. DA Converter
          Reg->WriteInteger("TOffset",3);       // HLT - MLT heat loss
          Reg->WriteInteger("Mash_Control",1);  // 0 = Tad1, 1 = Tad2
          // Init values for Sparge Settings
          Reg->WriteInteger("SP_BATCHES",4);    // #Sparge Batches
          Reg->WriteInteger("SP_TIME",20);      // Time between sparge batches
          Reg->WriteInteger("MASH_VOL",30);     // Total Mash Volume (L)
          Reg->WriteInteger("SP_VOL",50);       // Total Sparge Volume (L)
          Reg->WriteInteger("BOIL_TIME",90);    // Total Boil Time (min.)
          // Init values for STD
          Reg->WriteFloat("TMLT_HLIMIT",0.5); // TMLT_HLIMIT [E-1 C]
          Reg->WriteFloat("TMLT_LLIMIT",0.0); // TMLT_LLIMIT [E-1 C]
          Reg->WriteFloat("VMLT_EMPTY", 3.0); // Vmlt_EMPTY [L]
          Reg->WriteInteger("TO_XSEC",1);     // TIMEOUT_xSEC [sec]
          Reg->WriteInteger("TO3",300);       // TIMEOUT3 [sec]
          Reg->WriteInteger("TO4",20);        // TIMEOUT4 [sec
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
    // Init. volumes. Should be done only once
    //----------------------------------------
    if (volumes.Vhlt_simulated)
    {  // Vhlt is not measured but calculated from Vmlt
       volumes.Vhlt  = volumes.Vhlt_start;
    }
    if (volumes.Vboil_simulated)
    {  // Vboil is not measured but calculated from Vmlt
       volumes.Vboil  = VBOIL_START;
    }
    Main_Initialisation(); // Init all I2C Hardware, ISR and PID controller
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
   int i;
   char tmp[255]; // temp. array

   ptmp = new TPID_Settings(this);

   // Get PID Controller setting from the Registry
   try
   {
      if (Reg->KeyExists(REGKEY))
      {
         Reg->OpenKey(REGKEY,FALSE);
         i = Reg->ReadInteger("TS");      // Read TS from registry
         ptmp->TS_edit->Text = AnsiString(i);
         i = Reg->ReadInteger("Kc");      // Read Kc from registry
         ptmp->Kc_Edit->Text = AnsiString(i);
         i = Reg->ReadInteger("Ti");      // Read Ti from registry
         ptmp->Ti_Edit->Text = AnsiString(i);
         i = Reg->ReadInteger("Td");      // Read Ti from registry
         ptmp->Td_Edit->Text = AnsiString(i);
         i = Reg->ReadInteger("TOffset"); // Read Temp. Offset from registry
         ptmp->Offs_Edit->Text = AnsiString(i);
         i = Reg->ReadInteger("Mash_Control"); // Read Mash Control from registry
         ptmp->RG1->ItemIndex = i;
         ptmp->RG2->ItemIndex = time_switch;   // Value of time-switch [off, on]
         if (time_switch)
         {
            ptmp->Date_Edit->Text = DateToStr(dt_time_switch);
            AnsiString s = TimeToStr(dt_time_switch);
            s.SetLength(s.Length()-3); // remove seconds
            ptmp->Time_Edit->Text = s;
         } // if
         if (ptmp->ShowModal() == 0x1) // mrOK
         {
            i = ptmp->TS_edit->Text.ToInt();
            pid_pars.ts = (double)i;
            Reg->WriteInteger("TS",i);  // Set to value from form
            i = ptmp->Kc_Edit->Text.ToInt();
            pid_pars.kc = (double)i;
            Reg->WriteInteger("Kc",i);  // Set to value from form
            i = ptmp->Ti_Edit->Text.ToInt();
            pid_pars.ti = (double)i;
            Reg->WriteInteger("Ti",i);  // Set to value from form
            i = ptmp->Td_Edit->Text.ToInt();
            pid_pars.td = (double)i;
            Reg->WriteInteger("Td",i);  // Set to value from form
            init_pid2(&pid_pars);        // Init. PID controller

            i = ptmp->Offs_Edit->Text.ToInt();
            pid_pars.temp_offset = (double)i;
            Reg->WriteInteger("TOffset",i);  // Set to value from form
            pid_pars.mash_control = ptmp->RG1->ItemIndex;
            Reg->WriteInteger("Mash_Control",pid_pars.mash_control);
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
   TRegistry *Reg = new TRegistry();
   int err = 0;
   char s[80];

   T50msec->Enabled = false; // Disable Interrupt Timer
   ShowDataGraphs->GraphTimer->Enabled = false; // Start Graph Update timer
   ViewMashProgress->UpdateTimer->Enabled = false; // Start Mash Progress Update timer
   Sleep(51);                // Make sure that Timer is disabled
   delete ShowDataGraphs;    // close modeless dialog
   ShowDataGraphs = 0;       // null the pointer
   delete ViewMashProgress;  // close modeless dialog
   ViewMashProgress = 0;     // null the pointer

   if (hw_status & DIG_IO_LSB_OK)
   {
      err = WriteIOByte(0xFF,LSB_IO); // disable heater, alive LED & PUMP
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
   if ((hw_status >= 0) && !err)
   {
      err = i2c_stop(); // Stop I2C Communication
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
   ptmp->CB_Gamma->Checked = swfx.gamma_sw; // Set Checkbox for Gamma
   if (swfx.gamma_sw)
   {
      ptmp->Gamma_MEdit->Text = AnsiString(swfx.gamma_fx); // Set fix value
   } // if
   // Set Ref. Temp (Tset_hlt)
   ptmp->CB_Tset->Checked = swfx.tset_sw; // Set Checkbox for Tset
   if (swfx.tset_sw)
   {
      ptmp->Tset_MEdit->Text = AnsiString(swfx.tset_fx); // Set fix value
   } // if
   ptmp->CB_Tad1->Checked = swfx.tad1_sw; // Set Checkbox for Tad1
   if (swfx.tad1_sw)
   {
      ptmp->Tad1_MEdit->Text = AnsiString(swfx.tad1_fx); // Set fix value
   } // if
   ptmp->CB_Tad2->Checked = swfx.tad2_sw; // Set Checkbox for Tad1
   if (swfx.tad2_sw)
   {
      ptmp->Tad2_MEdit->Text = AnsiString(swfx.tad2_fx); // Set fix value
   } // if
   ptmp->CB_std->Checked = swfx.std_sw; // Set Checkbox for Tad1
   if (swfx.std_sw)
   {
      ptmp->STD_MEdit->Text = AnsiString(swfx.std_fx); // Set fix value
   } // if
   ptmp->CB_vmlt->Checked = swfx.vmlt_sw; // Set Checkbox for Vmlt
   if (swfx.vmlt_sw)
   {
      ptmp->Vmlt_MEdit->Text = AnsiString(swfx.vmlt_fx); // Set fix value
   } // if

   if (ptmp->ShowModal() == 0x1) // mrOK
   {
      // Get PID Output (Gamma)
      swfx.gamma_sw = ptmp->CB_Gamma->Checked;
      if (swfx.gamma_sw)
      {
         swfx.gamma_fx = ptmp->Gamma_MEdit->Text.ToDouble();
      } // if
      // Get Ref. Temp (Tset_hlt)
      swfx.tset_sw = ptmp->CB_Tset->Checked;
      if (swfx.tset_sw)
      {
         swfx.tset_fx = ptmp->Tset_MEdit->Text.ToDouble();
      } // if
      // Get Temp from AD1(Tad1)
      swfx.tad1_sw = ptmp->CB_Tad1->Checked;
      if (swfx.tad1_sw)
      {
         // Max. value for Tad1 = ADDA_VREF E-1 °C
         swfx.tad1_fx = ptmp->Tad1_MEdit->Text.ToDouble();
      } // if
      // Get Temp from AD2(Tad2)
      swfx.tad2_sw = ptmp->CB_Tad2->Checked;
      if (swfx.tad2_sw)
      {
         // Max. value for Tad2 = ADDA_VREF E-1 °C
         swfx.tad2_fx = ptmp->Tad2_MEdit->Text.ToDouble();
      } // if
      // Get STD state
      swfx.std_sw = ptmp->CB_std->Checked;
      if (swfx.std_sw)
      {
         // Value must be between 0 and 12
         swfx.std_fx = ptmp->STD_MEdit->Text.ToInt();
         if (swfx.std_fx < 0 || swfx.std_fx > S12_CHILL)
         {
            swfx.std_fx = 0; // reset to a save value
         }
      } // if
      // Get Vmlt value
      swfx.vmlt_sw = ptmp->CB_vmlt->Checked;
      if (swfx.vmlt_sw)
      {
         swfx.vmlt_fx = ptmp->Vmlt_MEdit->Text.ToDouble();
      }
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
   int  fscl_prescaler;      // index into PCF8584 prescaler values

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

         ptmp->Vref1_edit->Text  = AnsiString(padc.vref1);
         ptmp->Vref2_edit->Text  = AnsiString(padc.vref2);
         ptmp->Vref3_edit->Text  = AnsiString(padc.vref3);
         ptmp->Vref4_edit->Text  = AnsiString(padc.vref4);
         ptmp->UpDown3->Position = padc.dac;   // value for DA Converter
         ptmp->UpDown4->Position = str_vmlt.N; // order of MA filter

         ttriac_hlim = Reg->ReadInteger("TTRIAC_HLIM");
         ptmp->Thlim_edit->Text  = AnsiString(ttriac_hlim);
         ttriac_llim = Reg->ReadInteger("TTRIAC_LLIM");
         ptmp->Tllim_edit->Text  = AnsiString(ttriac_llim);
         volumes.Vhlt_start = Reg->ReadInteger("VHLT_START");
         ptmp->Vhlt_init_Edit->Text = AnsiString(volumes.Vhlt_start);
         volumes.Vhlt_simulated = Reg->ReadBool("VHLT_SIMULATED");
         ptmp->Vhlt_simulated->Checked = volumes.Vhlt_simulated;
         volumes.Vboil_simulated = Reg->ReadBool("VBOIL_SIMULATED");
         ptmp->Vboil_simulated->Checked = volumes.Vboil_simulated;
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

            //-------------------------------------------------------
            // For the LED Displays: 0=Thlt    , 1=Tmlt  , 2=Tset_hlt
            //                       3=Tset_mlt, 4=Ttriac, 5=Vmlt
            //-------------------------------------------------------
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

            padc.vref1 = ptmp->Vref1_edit->Text.ToInt();
            padc.vref2 = ptmp->Vref2_edit->Text.ToInt();
            padc.vref3 = ptmp->Vref3_edit->Text.ToInt();
            padc.vref4 = ptmp->Vref4_edit->Text.ToInt();
            padc.dac   = ptmp->DAC_edit->Text.ToInt();  // DAC value
            init_ma(&str_vmlt,ptmp->NMA_edit->Text.ToInt()); // order of MA filter
            Reg->WriteInteger("VREF1",padc.vref1);
            Reg->WriteInteger("VREF2",padc.vref2);
            Reg->WriteInteger("VREF3",padc.vref3);
            Reg->WriteInteger("VREF4",padc.vref4);
            Reg->WriteInteger("DAC"  ,padc.dac);

            ttriac_hlim = ptmp->Thlim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_HLIM",ttriac_hlim);
            tm_triac->SetPoint->Value = ttriac_hlim;
            ttriac_llim = ptmp->Tllim_edit->Text.ToInt();
            Reg->WriteInteger("TTRIAC_LLIM",ttriac_llim);
            volumes.Vhlt_start = ptmp->Vhlt_init_Edit->Text.ToInt();
            Reg->WriteInteger("VHLT_START",volumes.Vhlt_start);
            volumes.Vhlt_simulated = ptmp->Vhlt_simulated->Checked;
            Reg->WriteBool("VHLT_SIMULATED",volumes.Vhlt_simulated);
            volumes.Vboil_simulated = ptmp->Vboil_simulated->Checked;
            Reg->WriteBool("VBOIL_SIMULATED",volumes.Vboil_simulated);
            cb_i2c_err_msg = ptmp->cb_i2c_err_msg->Checked;
            Reg->WriteBool("CB_I2C_ERR_MSG",cb_i2c_err_msg);

            if (init_needed)
            {
               //--------------------------------------------------------
               // New I2C HW Base Address or SCL prescaler was changed,
               // call i2c_stop() and init I2C Bus communication again.
               //--------------------------------------------------------
               if (i2c_stop() != I2C_NOERR)
               {  // i2c bus locked, i2c_stop() did not work
                  MessageBox(NULL,"i2c_stop() not successful: Cycle power Off -> On, then press OK button.","ERROR",MB_OK);
                  MenuFileExitClick(this); // Exit ebrew program
               }
               else
               {
                  Main_Initialisation(); // Init all I2C Hardware, ISR and PID controller
               } // else
            } // if
            init_adc(&padc); // recalculate conversion constants
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
   if (i2c_stop() != I2C_NOERR)
   {  // i2c bus locked, i2c_stop() did not work
      MessageBox(NULL,"i2c_stop() not successful: Cycle power Off -> On, then press OK button.",tmp_str,MB_OK);
      MenuFileExitClick(this); // Exit ebrew program
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

void __fastcall TMainForm::T50msec2Timer(TObject *Sender)
/*------------------------------------------------------------------
  Purpose  : This is the main Timer function which is called every
             50 milliseconds. This can be seen as the main control
             loop of the program. It utilises time-slices to divide
             computations over time.
  Returns  : None
  ------------------------------------------------------------------*/
{
   int        value = 0x00;  // init. byte to write to DIG_IO_LSB_BASE
   int        err = 0;       // error return value, needed for SET_LED macro
   TDateTime  td_now;        // holds current date and time

   switch (tmr.isrstate)
   {
      case 0: /* enable heater */
              if (triac_too_hot)
              {
                 value &= ~HEATERb; // set heater OFF
                 tmr.isrstate = 1;  // go to 'disable heater' state
                 tmr.ltimer   = tmr.time_low; // init. timer
              }
              else if (tmr.htimer == 0)
              {  /* timer has counted down */
                 if (tmr.time_low == 0)
                 {  /* indication for 100% gamma, remain in this state */
                    tmr.htimer = tmr.time_high; /* init. timer again */
                    value |= HEATERb; // Set heater ON
                 }
                 else
                 {
                    value &= ~HEATERb; // set heater OFF
                    tmr.isrstate = 1; /* go to 'disable heater' state */
                    tmr.ltimer   = tmr.time_low; /* init. timer */
                 } /* else */
              } /* if */
              else
              {  /* timer has not counted down yet, continue */
                 value |= HEATERb; // Set heater ON
                 tmr.htimer--; /* decrement timer */
              } /* else */
              break;

      case 1: /* disable heater */
              if (tmr.ltimer == 0)
              {  /* timer has counted down */
                 if (tmr.time_high == 0)
                 {
                    /* indication for 0% gamma, remain in this state */
                    tmr.ltimer = tmr.time_low; /* init. timer again */
                    value &= ~HEATERb; // set heater OFF
                 }
                 else if (!triac_too_hot)
                 {
                    value |= HEATERb; // set heater ON
                    tmr.isrstate = 0; /* go to 'enable heater' state */
                    tmr.htimer   = tmr.time_high; /* init. timer */
                 } /* else if*/
                 // Remain in this state if timeout && !0% && triac_too_hot
              } /* if */
              else
              {
                 /* time has not counted down yet, continue */
                 value &= ~HEATERb; // Set heater OFF
                 tmr.ltimer--; /* decrement timer */
              } /* else */
              break;

      default:  tmr.isrstate = 1;
                break;
   } /* case */

   //--------------------------------------------------------------
   // This is the main control loop, executed once every 50 msec.
   // Do some time-slicing to reduce #computations in one loop.
   // Every time-slice is executed once every TS seconds.
   // tmr.pid_tmr runs from 1 to pid_pars.ts_ticks
   //--------------------------------------------------------------
   tmr.pid_tmr++; // increment timer

   //----------------------------------------------------------------
   // First time-slice: this time-slice is executed once every second.
   //                   Read AD-Converters and call MA filter for Vmlt.
   //----------------------------------------------------------------
   if (tmr.pid_tmr % 20 == 1)
   {
       // Read values from all AD Coverters
       if (hw_status & ADDA_OK)
       {
          err = read_adc(&padc); // Read the value of all ADCs
          if (err) Reset_I2C_Bus(ADDA_BASE,err);
       }
       else
       {
          padc.ad1 = 0.0; // HLT temperature
          padc.ad2 = 0.0; // MLT temperature
          padc.ad3 = 0.0; // Triac temperature
          padc.ad4 = 0.0; // Pressure transducer
       } // else
       //-----------------------------------------------------------------
       // If a LM92 is connected, overwrite the LM35 values on AD1 and AD2
       //-----------------------------------------------------------------
       if (hw_status & LM92_1_OK)
       {
          padc.ad1 = lm92_read(0); // Read HLT temp. from LM92 device
          if (padc.ad1 == LM92_ERR)
          {
             Reset_I2C_Bus(LM92_1_BASE, I2C_LM92_ERR);
          } // if
       } // if
       if (hw_status & LM92_2_OK)
       {
          padc.ad2 = lm92_read(1); // Read MLT temp. from LM92 device
          if (padc.ad2 == LM92_ERR)
          {
             Reset_I2C_Bus(LM92_2_BASE, I2C_LM92_ERR);
          } // if
       } // if

       if (swfx.tad1_sw)
       {
          padc.ad1 = (double)swfx.tad1_fx;
       } // if

       if (swfx.tad2_sw)
       {
          padc.ad2 = (double)swfx.tad2_fx;
       } // if

       //---------------------------------------------------
       // Triac Temperature Protection: hysteresis function
       //---------------------------------------------------
       if (triac_too_hot)
       {
          if (padc.ad3 < ttriac_llim)
          {
             triac_too_hot = false;
          }
       }
       else
       {
          if (padc.ad3 > ttriac_hlim)
          {
             triac_too_hot = true;
          }
       }

       //-------------------------------------
       // Pressure sensor, AD4, Volume of MLT
       //-------------------------------------
       if (swfx.vmlt_sw)
       {
          volumes.Vmlt = swfx.vmlt_fx;
       }
       else
       {
          volumes.Vmlt = moving_average(&str_vmlt,padc.ad4); // Call MA filter
       }
   } // if
   //-----------------------------------------------------------------------
   // Second time-slice: Time switch controller. Enable PI controller
   //                    if current date & time matches the date & time set.
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr == 2)
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
   } // else
   //--------------------------------------------------------------------------
   // Third time-slice: Calculate Gamma value with the PID controller.
   //                   Update Tset_hlt and update values on screen every TS seconds.
   //--------------------------------------------------------------------------
   else if (tmr.pid_tmr == 3)
   {
      // PID_RB->ItemIndex = 1 => PID Controller On
      pid_reg2(padc.ad1,&gamma,tset_hlt,&pid_pars,PID_RB->ItemIndex); // gamma = pid_reg2(temp)
      if (swfx.gamma_sw)
      {
         gamma = swfx.gamma_fx; // fix gamma
      } // if

      // check if tset_hlt should be increased
      if (pid_pars.mash_control == 0) // 0 = Tad1 (HLT), 1 = Tad2 (MLT)
      {
         tset_mlt = update_tset(&tset_hlt,padc.ad1,pid_pars.temp_offset,ms,&ms_idx,ms_tot);
      }
      else
      {
         tset_mlt = update_tset(&tset_hlt,padc.ad2,pid_pars.temp_offset,ms,&ms_idx,ms_tot);
      } // else

      if (swfx.tset_sw)
      {
         tset_hlt = swfx.tset_fx; // fix tset
      } // if
   } // else if
   //-----------------------------------------------------------------------
   // Fourth time-slice: Output values to I2C LED Displays every TS seconds.
   //-----------------------------------------------------------------------
   else if (tmr.pid_tmr % 20 == 4)
   {
      // Now update the LEDs with the proper values by calling macro SET_LED
      //      HW_BIT, which display, which var., Visibility
      SET_LED(LED1_OK,1,led1,led1_vis);
      if (err) Reset_I2C_Bus(LED1_BASE,err);
      SET_LED(LED2_OK,2,led2,led2_vis);
      if (err) Reset_I2C_Bus(LED2_BASE,err);
      SET_LED(LED3_OK,3,led3,led3_vis);
      if (err) Reset_I2C_Bus(LED3_BASE,err);
      SET_LED(LED4_OK,4,led4,led4_vis);
      if (err) Reset_I2C_Bus(LED4_BASE,err);
   } // else if
   //--------------------------------------------------------------------------
   // Fifth time-slice: Calculate State Transition Diagram (STD) and determine
   //                   new settings of the valves (every second).
   //--------------------------------------------------------------------------
   else if (tmr.pid_tmr % 20 == 5)
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
      /* Tmlt = AD2, Thlt = AD1 */
      update_std(&volumes,padc.ad2,padc.ad1,tset_hlt,&std_out,
                 ms,ms_idx,ms_tot,&sp,&std,PID_RB->ItemIndex,std_tmp);
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
   // Last time-slice
   //--------------------------------------------------------------------------
   else if (tmr.pid_tmr == pid_pars.ts_ticks)
   {
      tmr.pid_tmr = 1; // reset timer
      /*------------------------------------------------*/
      /* Now calculate high and low time for the timers */
      /*------------------------------------------------*/
      tmr.time_high = (int)((gamma * pid_pars.ts_ticks) / 100);
      tmr.time_low  = pid_pars.ts_ticks - tmr.time_high;
   } // else if
   //--------------------------------------------------------------------------

   //------------------------------------------------------------------
   // Calculate alive toggle bit to see if this routine is still alive.
   //------------------------------------------------------------------
   if (++tmr.alive_tmr >= ALIVE_TICKS)
   {
     tmr.alive_tmr = 0;          // reset timer
     tmr.alive     = !tmr.alive; // invert alive (bit 1 of IO port)
   } // if
   if (tmr.alive)
   {
      value |= ALIVEb;  // Set alive LED on
   }
   else
   {
      value &= ~ALIVEb; // Disable alive LED
   } // else

   //--------------------------------------------
   // Send Pump On/Off signal to DIG_IO_LSB_BASE.
   //--------------------------------------------
   if (std_out & P0b)
   {
      value |= PUMPb;
   }
   else
   {
      value &= ~PUMPb;
   } // else

   //-------------------------------------------------
   // Output value to IO port every 50 msec.
   // See misc.h for bit definitions
   // Bits are inverted, because they are active low
   //-------------------------------------------------
   if (hw_status & DIG_IO_LSB_OK)
   {
      err = WriteIOByte(~value,LSB_IO); // Write inverted value to IO port
      if (err) Reset_I2C_Bus(DIG_IO_MSB_BASE,err);
   } // if
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
      // Read Mash Scheme again
      //-----------------------
      if (!read_input_file(MASH_FILE,ms,&ms_tot,pid_pars.ts))
      {
          MessageBox(NULL,"File " MASH_FILE " not found","error in read_input_file()",MB_OK);
      } /* if */
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
        sp.tmlt_hlimit = Reg->ReadFloat("TMLT_HLIMIT");
        sp.tmlt_llimit = Reg->ReadFloat("TMLT_LLIMIT");
        sp.vmlt_empty  = Reg->ReadFloat("VMLT_EMPTY");
        sp.to_xsec     = Reg->ReadInteger("TO_XSEC");
        sp.to3         = Reg->ReadInteger("TO3");
        sp.to4         = Reg->ReadInteger("TO4");
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
        ptmp->Ehlimit->Text     = AnsiString(sp.tmlt_hlimit); // TMLT_HLIMIT [C]
        ptmp->Ellimit->Text     = AnsiString(sp.tmlt_llimit); // TMLT_LLIMIT [C]
        ptmp->Evmlt_empty->Text = AnsiString(sp.vmlt_empty);  // Vmlt_EMPTY [L]
        ptmp->Eto_xsec->Text    = AnsiString(sp.to_xsec);     // TIMEOUT_xSEC [sec]
        ptmp->Eto3->Text        = AnsiString(sp.to3);         // TIMEOUT3 [sec]
        ptmp->Eto4->Text        = AnsiString(sp.to4);         // TIMEOUT4 [sec]
        ptmp->EVmash->Text      = AnsiString(std.vmash);      // Vmash [L]

        if (ptmp->ShowModal() == 0x1) // mrOK
        {
           Reg->WriteInteger("SP_BATCHES",ptmp->EBatches->Text.ToInt());  // Number of Sparge Batches
           Reg->WriteInteger("SP_TIME",   ptmp->EBTime->Text.ToInt());    // Time (min.) between two Sparge Batches
           Reg->WriteInteger("MASH_VOL",  ptmp->EMVol->Text.ToInt());     // Total Mash Volume (L)
           Reg->WriteInteger("SP_VOL",    ptmp->ESVol->Text.ToInt());     // Total Sparge Volume (L)
           Reg->WriteInteger("BOIL_TIME", ptmp->EBoilTime->Text.ToInt()); // Total Boil Time (min.)

           Reg->WriteFloat("TMLT_HLIMIT", ptmp->Ehlimit->Text.ToDouble());  // TMLT_HLIMIT [C]
           Reg->WriteFloat("TMLT_LLIMIT", ptmp->Ellimit->Text.ToDouble());  // TMLT_LLIMIT [C]
           Reg->WriteFloat("VMLT_EMPTY",  ptmp->Evmlt_empty->Text.ToDouble()); // Vmlt_EMPTY [L]
           Reg->WriteInteger("TO_XSEC",   ptmp->Eto_xsec->Text.ToInt());    // TIMEOUT_xSEC [sec]
           Reg->WriteInteger("TO3",       ptmp->Eto3->Text.ToInt());        // TIMEOUT3 [sec]
           Reg->WriteInteger("TO4",       ptmp->Eto4->Text.ToInt());        // TIMEOUT4 [sec]
           Init_Sparge_Settings(); // Init. struct with sparge settings with new values
           std.vmash = ptmp->EVmash->Text.ToDouble(); // Special case: Vmash value
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

void __fastcall TMainForm::PopupMenu1Popup(TObject *Sender)
{
   if (PopupMenu1->PopupComponent->Name.AnsiCompare("P0") == 0)
   {
      if (std_out & P0M) // Manual Override Mode
      {
         if (std_out & P0b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V1") == 0)
   {
      if (std_out & V1M) // Manual Override Mode
      {
         if (std_out & V1b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V2") == 0)
   {
      if (std_out & V2M) // Manual Override Mode
      {
         if (std_out & V2b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V3") == 0)
   {
      if (std_out & V3M) // Manual Override Mode
      {
         if (std_out & V3b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V4") == 0)
   {
      if (std_out & V4M) // Manual Override Mode
      {
         if (std_out & V4b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V5") == 0)
   {
      if (std_out & V5M) // Manual Override Mode
      {
         if (std_out & V5b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else if (PopupMenu1->PopupComponent->Name.AnsiCompare("V6") == 0)
   {
      if (std_out & V6M) // Manual Override Mode
      {
         if (std_out & V6b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
   else
   {
      if (std_out & V7M) // Manual Override Mode
      {
         if (std_out & V7b) PopupMenu1->Items->Items[2]->Checked = true; // On (M)
         else               PopupMenu1->Items->Items[1]->Checked = true; // Off (M)
      }
      else PopupMenu1->Items->Items[0]->Checked = true; // Auto
   }
}  // TMainForm::PopupMenu1Popup()
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
   MenuFileExitClick(Sender);
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
         Restore_Settings();                 // Read Log File & Restore Settings
         Reg->WriteInteger("ms_idx",ms_idx); // Update registry setting
         tset_hlt = ms[ms_idx].temp;         // Set tset value for HLT
         Reg->CloseKey();                    // Close the Registry
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

   sprintf(tmp_str,"%4.2f",padc.ad1); // Display AD1 value to screen
   Val_Thlt->Caption    = tmp_str;
   tm_hlt->Value->Value = padc.ad1;   // update HLT thermometer object

   sprintf(tmp_str,"%4.2f",padc.ad2); // Display AD2 value to screen
   Val_Tmlt->Caption    = tmp_str;
   tm_mlt->Value->Value = padc.ad2;   // update MLT thermometer object

   tm_triac->Value->Value = padc.ad3; // Update thermometer object
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
      case  4: Std_State->Caption = "04. Bypass Heat Exchanger"; break;
      case  5: Std_State->Caption = "05. Sparging Rest"        ; break;
      case  6: Std_State->Caption = "06. Pump from MLT to Boil"; break;
      case  7: Std_State->Caption = "07. Pump from HLT to MLT" ; break;
      case  8: Std_State->Caption = "08. Delay"                ; break;
      case  9: Std_State->Caption = "09. Empty MLT"            ; break;
      case 10: Std_State->Caption = "10. Boiling"              ; break;
      case 11: Std_State->Caption = "11. Empty Heat Exchanger" ; break;
      case 12: Std_State->Caption = "12. Chill"                ; break;
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
} // ebrew_idle_handler()

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
   Application->OnIdle = ebrew_idle_handler;
} // TMainForm::FormCreate()
//---------------------------------------------------------------------------



