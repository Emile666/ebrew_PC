// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Provides the code for the About... Screen.
// --------------------------------------------------------------------------
// $Log$
// ==========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#include <alloc.h>
#include <stdio.h>
#include "VersionAwareAbout.h"
#include "Unit1.h"
#pragma hdrstop

extern char *i2c_dll_revision; // defined in i2c_dll.cpp
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TVersionAwareAbout *VersionAwareAbout;

__fastcall TVersionAwareAbout::TVersionAwareAbout(TComponent* Owner)
  : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TVersionAwareAbout::FormCreate(TObject *Sender)
{
  int VerInfoSize =0;
  VS_FIXEDFILEINFO *VerInfo =0;

  VerInfoSize=
     GetFileVersionInfoSize(Application->ExeName.c_str(), 0);
  VerInfo =(VS_FIXEDFILEINFO*)malloc(VerInfoSize);

  /*Check if Version info exists & for success allocating memory*/
  if((VerInfoSize)&&(VerInfo))
  {
     GetFileVersionInfo(Application->ExeName.c_str(), 0, VerInfoSize,
                       VerInfo);

     /*Some needed items*/
     char *buf;
     AnsiString BaseString;
     AnsiString tmp;
     unsigned int TossThis;
     struct LANGANDCODEPAGE
     {
        WORD wLanguage;
        WORD wCodePage;
     } *lpTranslate =0;

     /*Read the Language ID & CodePage info*/
     VerQueryValue(VerInfo,
              TEXT("\\VarFileInfo\\Translation"),
              (LPVOID*)&lpTranslate, &TossThis);

     /*Now build a base string for getting the Version Info*/
     char LangID[8];
     sprintf(LangID,"%04x%04x",lpTranslate->wLanguage, lpTranslate->wCodePage);
     BaseString ="\\StringFileInfo\\" +AnsiString(LangID);

     VersionAwareAbout->Caption = "About ebrew";

     tmp = BaseString + "\\FileVersion";
     VerQueryValue(VerInfo,
              TEXT(tmp.c_str()),
              (LPVOID*)&buf, &TossThis);
     if(buf[0] !='\0')
     {
        char tmp_str[40];  // Temp. string for ebrew revision
        char tmp_str2[40]; // temp. string for i2c_dll revision
        // Extract the revision number from the ebrew cvs revision string
        strncpy(tmp_str, &(MainForm->ebrew_revision[11]),4);
        tmp_str[4] = '\0';
        // Extract the revision number from the i2c_dll cvs revision string
        strncpy(tmp_str2, &i2c_dll_revision[11],4);
        tmp_str2[4] = '\0';

        reVersionInfo->Lines->Add("Version: " + AnsiString(buf) + " (CVS Rev. "
                                              + AnsiString(tmp_str)
                                              + "), i2c_dll Rev. "
                                              + AnsiString(tmp_str2));
     }

    tmp =BaseString +"\\CompanyName";
    VerQueryValue(VerInfo,
              TEXT(tmp.c_str()),
              (LPVOID*)&buf, &TossThis);
     if(buf[0] !='\0')
        reVersionInfo->Lines->Add("Brewery  : "+AnsiString(buf));
     reVersionInfo->Lines->Add("Web-site : www.vandelogt.nl");
     reVersionInfo->Lines->Add("(c) 2003-2004: Ir. Emile van de Logt");
     free(VerInfo);
  }

  /*if the exe does not include version info or if there was an
  error allocating memory.*/
  else
  {
     VersionAwareAbout->Caption ="About";
     reVersionInfo->Clear();
     reVersionInfo->Lines->Add("No version information available.");
  }  
}
//---------------------------------------------------------------------------


