// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
// Revision 1.11  2003/07/11 18:34:46  emile
// - tset_mlt added. Also added to log-file (tset_mlt now replaces gamma).
// - Bug solved: transition to 'EMPTY_MLT' was 1 sparging cycle too early.
// - Log-file header updated
// - init_adc(): all vref initialisations are now the same (/ 2560).
//               Removed the / 10 division of AD4 in the main loop, this is
//               now done in init_adc().
// - Multiply and division changed into <<= and >>= (in lm76_read())
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

#define TS_INIT   (5)
#define KC_INIT   (4)
#define TI_INIT (110)
#define TD_INIT  (27)
#define LOGFILE "ebrewlog.txt"
#define MASH_FILE "maisch.sch"
#define REGKEY    "ebrew"
// 1 minute = 1/(60*24) part of one day, see TDateTime for details
#define ONE_MINUTE (6.94444E-04)

// Macro, used in TMainForm::Main_Initialisation()
#define PR_HW_STAT(x)   hw_status & x ? strcpy(s1,YES_TXT) : strcpy(s1,NOT_TXT)

typedef struct _swfx_struct
{
   bool   tset_sw;  // Switch valye for tset
   double tset_fx;  // Fix value for tset
   bool   gamma_sw; // Switch value for gamma
   double gamma_fx; // Fix value for gamma
   bool   tad1_sw;  // Switch value for Tad1
   double tad1_fx;  // Fix value for Tad1 [E-1 Celsius]
   bool   tad2_sw;  // Switch value for Tad2
   double tad2_fx;  // Fix value for Tad2 [E-1 Celsius]
   bool   std_sw;   // Switch value for STD state
   int    std_fx;   // Fix value for STD state
   bool   vmlt_sw;  // Switch value for Vmlt
   double vmlt_fx;  // Fix value for Vmlt
} swfx_struct;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *Edit1;
        TMenuItem *View1;
        TMenuItem *MenuOptions;
        TMenuItem *Help1;
        TMenuItem *MenuHelpAbout;
        TMenuItem *HowtoUseHelp1;
        TMenuItem *SearchforHelpOn1;
        TMenuItem *Contents1;
        TMenuItem *MenuOptionsPIDSettings;
        TMenuItem *MenuViewLogFile;
        TMenuItem *MenuViewData_graphs;
        TMenuItem *MenuEditMashScheme;
        TMenuItem *File2;
        TMenuItem *MenuFileExit;
        TMenuItem *N1;
        TMenuItem *PrintSetup1;
        TMenuItem *Print1;
        TMenuItem *MenuEditFixParameters;
        TImage *MainImage;
        TLabel *Val_gamma;
        TLabel *Val_ek;
        TLabel *Val_temp;
        TLabel *Val_tset;
        TLabel *Val_Tmlt;
        TRadioGroup *PID_RB;
        TAnimTimer *T50msec;
        TMenuItem *MenuViewMash_progress;
        TMenuItem *MenuOptionsI2CSettings;
        TLabel *Val_TTriac;
        TLabel *Val_Volume;
        TMenuItem *SpargeSettings1;
        TImage *Image1;
        TLabel *V5;
        TPopupMenu *PopupMenu1;
        TMenuItem *Auto1;
        TMenuItem *OFF1;
        TMenuItem *ON1;
        TLabel *V4;
        TLabel *V2;
        TLabel *V1;
        TLabel *V6;
        TLabel *V3;
        TLabel *V7;
        TLabel *Std_State;
        TMenuItem *N2;
        TMenuItem *ReadLogFile1;
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
private:	// User declarations
        void __fastcall Main_Initialisation(void);
        void __fastcall Init_Sparge_Settings(void);
        void __fastcall Restore_Settings(void);
        timer_vars      tmr;        // struct with timer variables
        swfx_struct     swfx;       // Switch & Fix settings for tset and gamma
        ma              str_vmlt;   // Struct for MA5 filter for pressure transducer
        int             led1;       // 0=Tad1, 1=Tad2, 2=Tset
        int             led2;       // 0=Tad1, 1=Tad2, 2=Tset
        int             led1_vis;   // 1..7: LED1 Visibility
        int             led2_vis;   // 1..7: LED2 Visibility
public:		// User declarations
        adda_t          padc;       // struct containing the 4 ADC values in mV
        double          gamma;      // PID controller output
        double          tset_hlt;   // HLT reference temperature
        double          tset_mlt;   // MLT reference temperature
        double          Vmlt;       // MLT volume: output of MA5 filter for pressure transducer
        pid_params      pid_pars;   // struct containing PID parameters
        int             ms_tot;     // tot. nr. of valid temp & time values
        int             ms_idx;     // index in ms[] array
        int             hw_status;  // I2C HW status, see i2c.h for bit settings
        maisch_schedule ms[MAX_MS]; // struct containing maisch schedule
        sparge_struct   sp;         // Values for Sparging
        std_struct      std;        // Values for State Transition Diagram
        unsigned int    std_out;    // position of valves
                                    // Bit 0 = Pump, Bit 1..7 = V1..V7
                                    // Bit 8..15: 0 = Auto, 1 = Manual Override
        unsigned int    time_switch;// 1: PID is controlled by a time-switch
        TDateTime       dt_time_switch; // object holding date and time
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
