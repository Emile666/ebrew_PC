// ==================================================================
// Functienaam : -
// Filenaam    : pt_ioctl.h
// Auteur      : E. van de Logt
// Datum       : 11-09-2002
// Versie      : V0.1
// ------------------------------------------------------------------
// Doel : Deze file bevat de includes voor pt_ioctl.c. Hiermee is het
//        mogelijk om direct naar IO poorten onder Windows 2000/XP/NT
//        te schrijven.  
// ------------------------------------------------------------------
// Gewijzigd :
//
// Datum    Auteur Versie & Omschrijving
// ------------------------------------------------------------------
// 09-02-02 LGT    - Eerste versie afgeleid van porttalk V2.2
// ==================================================================
#ifndef PT_IOCTL_H
#define PT_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned char OpenPortTalk(void);
void ClosePortTalk(void);

void nt_out(unsigned short PortAddress, unsigned char byte);
unsigned char nt_in(unsigned short PortAddress);

void InstallPortTalkDriver(void);
unsigned char StartPortTalkDriver(void);

#ifdef __cplusplus
};
#endif
#endif
