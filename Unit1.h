// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).  
// --------------------------------------------------------------------------
// $Log$
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

//#include "i2c.h"
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
private:	// User declarations
        void __fastcall Main_Initialisation(void);
        timer_vars      tmr;        // struct with timer variables
        swfx_struct     swfx;       // Switch & Fix settings for tset and gamma
        ma              str_vmlt;   // Struct for MA5 filter for pressure transducer
        int             led1;       // 0=Tad1, 1=Tad2, 2=Tset
        int             led2;       // 0=Tad1, 1=Tad2, 2=Tset
        int             led1_vis;   // 1..7: LED1 Visibility
        int             led2_vis;   // 1..7: LED2 Visibility
        sparge_struct   sp;         // Values for Sparging
public:		// User declarations
        adda_t          padc;       // struct containing the 4 ADC values in mV
        double          gamma;      // PID controller output
        double          tset;       // HLT reference temperature
        double          Vmlt;       // MLT volume: output of MA5 filter for pressure transducer
        pid_params      pid_pars;   // struct containing PID parameters
        int             ms_tot;     // tot. nr. of valid temp & time values
        int             ms_idx;     // index in ms[] array
        int             hw_status;  // I2C HW status, see i2c.h for bit settings
        maisch_schedule ms[MAX_MS]; // struct containing maisch schedule
        int             std_state;  // current version of STD state
        unsigned int    std_out;    // position of valves
                                    // Bit 0 = Pump, Bit 1..7 = V1..V7
                                    // Bit 8..15: 0 = Auto, 1 = Manual Override
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
