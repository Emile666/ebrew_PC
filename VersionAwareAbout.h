// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where for the About... Screen.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef VesionAwareAboutH
#define VesionAwareAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TVersionAwareAbout : public TForm
{
__published:	// IDE-managed Components
  TBitBtn *btnOK;
  TRichEdit *reVersionInfo;
        TImage *Image1;
  void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TVersionAwareAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVersionAwareAbout *VersionAwareAbout;
//---------------------------------------------------------------------------
#endif
