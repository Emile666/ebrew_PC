// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2002/12/30 13:33:44  emile
// - Headers with CVS tags added to every source file
// - Restore Settings function is added
// - "ebrew" registry key now in a define REGKEY
//
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef Sparge_SettingsH
#define Sparge_SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSpargeSettings : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *EBatches;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TEdit *EBTime;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *EMVol;
        TLabel *Label4;
        TEdit *ESVol;
        TEdit *EBoilTime;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Ehlimit;
        TLabel *Label7;
        TEdit *Ellimit;
        TLabel *Label8;
        TEdit *Eto_xsec;
        TLabel *Label9;
        TEdit *Evmlt_empty;
        TLabel *Label10;
        TEdit *Eto3;
        TLabel *Label11;
        TEdit *Eto4;
        TLabel *Label12;
        TLabel *Label13;
private:	// User declarations
public:		// User declarations
        __fastcall TSpargeSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
#endif
