// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where parameters for the PID Controller
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//----------------------------------------------------------------------------
#ifndef PID_Settings_DialogH
#define PID_Settings_DialogH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//----------------------------------------------------------------------------

class TPID_Settings : public TForm
{
__published:
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
        TEdit *TS_edit;
        TLabel *TS_Label;
        TEdit *Kc_Edit;
        TLabel *Label1;
        TEdit *Ti_Edit;
        TLabel *Label2;
        TEdit *Td_Edit;
        TLabel *Label3;
        TRadioGroup *RG1;
        TLabel *Label4;
        TEdit *Offs_Edit;
        TMaskEdit *Date_Edit;
        TMaskEdit *Time_Edit;
        TRadioGroup *RG2;
        TLabel *Date_Label;
        TLabel *Time_Label;
        void __fastcall RG2Click(TObject *Sender);
private:
public:
	virtual __fastcall TPID_Settings(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TPID_Settings *PID_Settings;
//----------------------------------------------------------------------------
#endif
