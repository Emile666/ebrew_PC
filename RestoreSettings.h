// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides a form where the contents of the log-file are
//               displayed (in a Combobox), so that the proper log-entry to
//               restore from, can be selected.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#ifndef RestoreSettingsH
#define RestoreSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TRestore_Program_Settings : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TComboBox *ComboBox1;
        TButton *Button1;
        TButton *Button2;
        TButton *Help_Button;
        void __fastcall Help_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TRestore_Program_Settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRestore_Program_Settings *Restore_Program_Settings;
//---------------------------------------------------------------------------
#endif
