// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the parameters for the sparging session
//               can be modified.
// --------------------------------------------------------------------------
// $Log$
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
private:	// User declarations
public:		// User declarations
        __fastcall TSpargeSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSpargeSettings *SpargeSettings;
//---------------------------------------------------------------------------
#endif
