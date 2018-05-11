// ==========================================================================
// Filename    : Unit1.h
// Author      : Emile
// Purpose     : Main Unit of the ebrew program. Contains Init. functions,
//               functions for every menu command and it contains the main
//               program loop (TMainForm::T50msec2Timer()).
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
        TMenuItem *CIP_Start;
        TMenuItem *N1;
        TMenuItem *AddingMalt1;
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
        void __fastcall CIP_StartClick(TObject *Sender);
        void __fastcall AddingMalt1Click(TObject *Sender);
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
