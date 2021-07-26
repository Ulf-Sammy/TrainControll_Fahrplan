#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Selten verwendete Komponenten aus Windows-Headern ausschließen
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // Einige CString-Konstruktoren sind explizit.

// Deaktiviert das Ausblenden einiger häufiger und oft ignorierter Warnungen durch MFC
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen


#include <afxdisp.h>        // MFC-Automatisierungsklassen



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC-Unterstützung für allgemeine Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC-Unterstützung für Menübänder und Steuerleisten


#include "CustomTrace.h"
#include <vector>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


#define COM_LEN(L)      (L & 0x07)     //                       Setup - Test - Auto - Program
#define COM_WRITE_MOD       0x08+0x03  // 1 MOD	+ Sub				| S | T | A | P | modus des Prog
#define COM_ACKN_MOD        0x10+0x04  // 3 MOD + Sub + STA			| S | T | A | P |
#define COM_SEND_LVZ_STA    0x18+0x02  // 3 Status					| S | T | A | P | Status vom LVZ
#define COM_SEND_ERROR      0x20+0x03  // 4 ADR + Error				| S | T | A | P |
#define COM_SEND_DISPLAY    0x28+0x07  // 5 Test 1H2o3l4l5o			| S | T | A | P |

#define COM_WRITE_ZUG_D0    0x30+0x03  // 6 ADR + Speed-Dir			| _ | T | A | _ |
#define COM_WRITE_ZUG_D1    0x38+0x03  // 7 ADR + GRUPPE 1			| _ | T | A | _ |
#define COM_WRITE_ZUG_D2    0x40+0x03  // 8 ADR + GRUPPE 2			| _ | T | A | _ |
#define COM_WRITE_ZUG_D3    0x48+0x03  // 9 ADR + GRUPPE 3			| _ | T | A | _ |
#define COM_WRITE_ZUG_D4    0x50+0x03  //10 ADR + GRUPPE 4			| _ | T | A | _ |
#define COM_WRITE_ZUG_D5    0x58+0x03  //11 ADR + GRUPPE 5			| _ | T | A | _ |
#define COM_WRITE_WEICHE    0x60+0x03  //12 Weiche + Wert			| _ | T | A | _ |
#define COM_READ_ZUG_DA     0x68+0x03  //13 ADR	+ Lok_Nr_Liste		| _ | T | A | _ |
#define COM_READ_ZUG_D1     0x70+0x02  //14 ADR	(frei)					| _ | T | A | _ |
#define COM_SEND_RELAIS     0x78+0x02  //15 Relais + Bit						| _ | T | A | _ |
#define COM_WRITE_RELAIS    0x80+0x02  //16 Relais + Bit						| _ | T | A | _ |;
#define COM_SEND_TIME	    0x88+0x02  //17 TIME + Wert				| _ | T | A | _ | Timer ist aus gelöst
#define COM_SEND_BLOCK      0x90+0x03  //18 Block + Wert			| _ | T | A | _ |
#define COM_SEND_WEICHE     0x98+0x03  //19 Weiche + Wert			| _ | T | A | _ |
#define COM_SEND_ZUG_DA     0xA0+0x07  //20 Gr0 .. Gr5 + Nr			| _ | T | A | _ |
#define COM_SEND_ZUG_D1     0xA8+0x03  //21 ADR + GRUPPE 1			| _ | T | A | _ |
#define COM_SEND_ZUG_D2     0xB0+0x03  //22 ADR + GRUPPE 2			| _ | T | A | _ |
#define COM_SEND_ZUG_D3     0xB8+0x03  //23 ADR + GRUPPE 3			| _ | T | A | _ |
#define COM_SEND_ZUG_D4     0xC0+0x03  //24 ADR + GRUPPE 4			| _ | T | A | _ |
#define COM_SEND_ZUG_D5     0xC8+0x03  //25 ADR + GRUPPE 5			| _ | T | A | _ |

#define COM_WRITE_MELDER_TI	0xD0+0x02  //26 Wert für Zeit			| S | _ | _ | _ |
#define COM_WRITE_MELDER_ST 0xD8+0x02  //27 Melderanzahl		   	| S | _ | _ | _ |
#define COM_WRITE_WEICHE_ST 0xE0+0x02  //28 Weichenanzahl			| S | _ | _ | _ |
#define COM_WRITE_LVZ_POWER 0xE8+0x02  //30 Wert LVZ ON/OFF			| S | _ | _ | _ |

#define COM_SEND_CV         0xE8+0x04  //30 CV + Wert + MOD 		| _ | _ | _ | P |
#define COM_WRITE_CV        0xF0+0x03  //32 CV + Wert				| _ | _ | _ | P |
#define COM_READ_CV         0xF8+0x03  //33 CV + Wert				| _ | _ | _ | P |


#define WM_NOTIFY_DESCRIPTION_EDITED             WM_APP + 1
//#define TESTSTRECKE 

#ifdef TESTSTRECKE  // Teste auf TestStrecke
	#define FILE_ANLAGE   "Daten\\GLEIS_BILD - OVAL.TXT"
	#define FILE_FAHRPLAN "FAHRPLANTEST_%d_DATA.txt"
	#define FILE_AKTIVE_ZUEGE "Daten\\ULF_%d_AKTIVEZUGDATA.txt"
#else
	#define FILE_ANLAGE   "Daten\\GLEIS_BILD - GARTEN.TXT"
	#define FILE_FAHRPLAN "FAHRPLANGARTEN_%d_DATA.txt"
	#define FILE_AKTIVE_ZUEGE "Daten\\ULF_%d_AKTIVEZUGDATA - Garten.txt"
	#define	FILE_ANHANG "Daten\\ANHANG.txt"
#endif // TESTSTRECKE

#define FILE_ALLE_ZUEGE   "Daten\\ULF_%d_ZUGDATA.txt"
#define FILE_ALLE_ZUEGE_BILDER "Images\\%s.bmp"
#define FILE_ALLE_BILDER "Images\\%s"

#define MAXARRAY 100
#define MAXDECODERTYPE 5
#define MAX_FUNKTION 29
#define MAX_MASSOTH_FUNKTION 17
#define MAX_ZUEGE      (byte)20
#define MAX_ZUEGE_AKT (byte)5 // maximal 3 Züge mit Hand steuern oder Automatisch
#define MAX_WEICHE 32    // 4 Karten x 8 Antriebe
#define MAX_WEICHEN_WEGE 40
#define MAX_BLOCK 100
#define MAX_GLEISE 10
#define MAX_MELDER 64  //	3 ADR = 8 => 8 * 8 = 64 // Der wert darf nicht über oxFF gehen
#define MAX_TASTER 50
#define MAX_WEI_GRUPPE 20
#define MAX_TASTER_IN_GRUPPE 10
#define MAX_FAHRPLAN 2
#define MAX_BLOCK_MELDER 4 // angeschlossenen Block Melder
#define MAX_WEICHEN_MOTOR (MAX_BLOCK_MELDER * 8)

const COLORREF colorHinterGrund = RGB(186, 252, 189);
const COLORREF GleisGruen = RGB(6, 233, 13);
const COLORREF GleisOrange = RGB(232,104,13);
const COLORREF GleisRot = RGB(225, 24, 32);
const COLORREF GleisGelb = RGB(255, 242, 0);
const COLORREF GleishellRot = RGB(242, 141, 147);
const COLORREF GleisWeiss = RGB(255, 255, 255);
const COLORREF GleisErrorA = RGB(  0, 255, 255);
const COLORREF GleisErrorB = RGB(255, 255,   0);
const COLORREF colorGelb = RGB(253, 240, 2);
const COLORREF colorRot = RGB(255, 0, 0);
const COLORREF colorSchwarz = RGB(0, 0, 0);
const COLORREF colorWeiss = RGB(255, 255, 255);
const COLORREF colorSchuppen = RGB(128, 0, 0);
const COLORREF colorSchuppenTor = RGB(255, 128, 0);
const COLORREF colorDunkelGrau = RGB(150, 150, 150);

const tagLOGPEN Stift_Schwarz = { PS_SOLID , 1 , 1, colorSchwarz };

const tagLOGPEN StiftGleis_frei    = { PS_SOLID , 5 , 5, GleisGruen };
const tagLOGPEN StiftGleis_besetzt = { PS_SOLID , 5 , 5, GleisOrange };
const tagLOGPEN StiftGleis_ErrorA  = { PS_SOLID , 5 , 5, GleisErrorA };
const tagLOGPEN StiftGleis_ErrorB  = { PS_SOLID , 5 , 5, GleisErrorB };

const tagLOGPEN StiftGleisRot    = { PS_SOLID , 4 , 4, GleisRot};
const tagLOGPEN StiftGleisGelb   = { PS_SOLID , 5 , 5, GleisGelb };
const tagLOGPEN StiftGleisNull  = { PS_SOLID , 5 , 5, GleisWeiss };
const tagLOGPEN StiftLokSchuppen = { PS_SOLID , 3,  3, colorSchuppen };
const tagLOGPEN StiftTorSchuppen = { PS_SOLID , 1,  1, colorSchuppenTor };

const tagLOGFONTW FontType_Ar_9_0 =  {  9, 0, 0, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial") };
const tagLOGFONTW FontType_Ar_14_0 = { 14, 0, 0, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow") };
const tagLOGFONTW FontType_Ar_15_0 = { 15, 0, 0, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow") };
const tagLOGFONTW FontType_Ar_16_0 = { 16, 0, 0, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow") };
const tagLOGFONTW FontType_Ar_11_270 = { 11, 0, 2700, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow") };
const tagLOGFONTW FontType_Ar_14_270 = { 14, 0, 2700, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow") };

const tagLOGFONTW FontType_ArR_20_0 = { 20, 0, 0, 0, 200, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial Rounded MT Bold") };





const int Rect_X = 1600;
const int Rect_Y = 660;

enum class BlockStatus {Frei, Besetzt, BesetztError_A, BesetztError_B};
enum class ControlStatus { No_Arduino = -1, Setup =0, Program, Fahren, Ende_COM };
enum class XpNSendwas { FGruppe0 = 0, FGruppe1, FGruppe2, FGruppe3, FGruppe4, FGruppe5 };
enum class BlockType { isWeiche, isBlock, isGleis };
enum class WeichenType { linksWeiche, rechtsWeiche, L_DoppelWeiche, R_DoppelWeiche};
enum class StreckenType {Strecke, Gleis, Abstellgleis};

enum class FahrPlanDo { begin_Block, stoppen, vorwaerz_fahren, rueckwaerz_fahren, warten_fahren, warten_stoppen, schalten_Funk, schalten_Weiche, letzte_Zeile };

enum class Zug_Status { Zug_Stopped = 0, Zug_faehrt_vor = 1, Zug_faehrt_rueck = 2, Zug_haelt = 3};
enum class Zug_Steuerung { nicht_Betriebs_bereit, Hand_Betrieb, Automatik_Betrieb };

enum class BlockRueckmeldung { Frei_Fahrt = 0, Weichenweg_nichtfrei = 1, Block_besetzt = 9 };
enum class DecoderHersteller { Tams = 0, Massoth };
enum class DecoderTypen { Tams_Normal = 0, Massoth_No_Sound, Massoth_Sound, NoName, NoDecoder = 999 };



struct neueMeldungen
{
public:
	void neueMeldung()
	{
		Meld_In = 0;
		Meld_Out = 0;
	}
	void Putin()
	{
		Meld_In++;
	}
	bool Get_Out()
	{
		if (Meld_In != Meld_Out)
		{
			Meld_Out++;
			return true;
		}
		return false;
	}
protected:
	volatile unsigned int Meld_In;
	volatile unsigned int Meld_Out;

};

class BlockInfo
{
public:
	byte Nr;
	BlockType		Block_Type;
	WeichenType		Weiche_Type;
	bool			WeichenStellung;
	byte			EingangBlock[2] = { 0 ,0 };
	byte			AusgangBlock[2] = { 0 ,0 };
	byte			EingangMelder[2] = { 0 ,0 };
	byte			AusgangMelder[2] = { 0 ,0 };
};

class TrainCon_Paar
{
public:
	TrainCon_Paar()
	{
		Wert = 0;
		Bit = false;
	}
	TrainCon_Paar(byte W, bool B)
	{
		Wert = W;
		Bit = B;
	}
	TrainCon_Paar(CString T)
	{// "011" Wert = 1 Bit = true
		Wert = _ttoi(T.Mid(0, 2));
		Bit = (bool)_ttoi(T.Mid(2, 1));
	}
	void Set_Paar(TrainCon_Paar TP)
	{
		Wert = TP.GetWert();
		Bit = TP.GetBit();
	}
	void Set_Paar(byte W, bool B)
	{
		Wert = W;
		Bit = B;
	}
	void SetWert(byte W)
	{
		Wert = W;
	}
	void SetBit(bool B)
	{
		Bit = B;
	}
	TrainCon_Paar GetInvBit()
	{
		return(TrainCon_Paar(Wert, !Bit));
	}
	byte GetWert()
	{
		return(Wert);
	}
	bool GetBit()
	{
		return(Bit);
	}
	CString GetText()
	{
		CString T;
		T.Format(_T(" %02i%1i"), Wert, Bit);
		return (T);
	}
	void SetbyText(CString T)
	{// "011" Wert = 1 Bit = true
		Wert = _ttoi(T.Mid(0, 2));
		Bit = (bool)_ttoi(T.Mid(2, 1));
	}
	void TraceInfo()
	{
		TRACE2(" Nr %02i = %1i \n", Wert, Bit);
	}
protected:
	byte Wert = 0;
	bool Bit = false;
};

struct BlockDebugData
{
	byte Block_Nr;
	CString Gleis_Name;
	CString Lok_Name;
	bool Lok_Richtung;
	byte Relais_Nr;
	bool Relais_Power;
	bool Block_gemeldet;
	byte Weichen_Anzahl;
	std::vector<byte> EinBlock;
	std::vector<byte> AusBlock;
	std::vector<byte> Block_Wege_nach;
	std::vector<byte> Block_Wege_von;
	BlockDebugData()
	{
		Block_Nr = 0;
		Lok_Richtung = false;
		Relais_Nr = 0;
		Relais_Power = false;
		Block_gemeldet = false ;
		Weichen_Anzahl = 0 ;
	};
};

#define BUFFER 16
struct RingBuffer
{
	byte ClearBlock[BUFFER];
	byte ClearMelder[BUFFER];
	int head = 0;
	int tail = 0;
	void clearBuffer()
	{
		head = 0;
		tail = 0;
	}
	bool isnextMelder(byte Ml)
	{
		return (Ml == ClearMelder[tail]);
	}
	byte GetBlock()
	{
		if (head != tail)
		{
			byte Bl = ClearBlock[tail];
			tail = (tail + 1) % BUFFER;
			return Bl;
		}
		return 0;
	}
	void SetBlock(byte Ml, byte Bl)
	{
		ClearMelder[head] = Ml;
		ClearBlock[head] = Bl;
		head = (head + 1) % BUFFER;
	}
};

struct RingbufferM
{
	TrainCon_Paar Melder[BUFFER];
	int head = 0;
	int tail = 0;

	bool isNeueData()
	{
		return (head != tail);
	}

	void SetMelder(TrainCon_Paar Ml)
	{
		Melder[head] = Ml;
		head = (head + 1) % BUFFER;
	}
	TrainCon_Paar GetMelder()
	{
		TrainCon_Paar Ml;
		Ml = Melder[tail];
		tail = (tail + 1) % BUFFER;
		return (Ml);
	}
};

struct Funktion_Init
{
	CString FunktionsName;
	bool Wert;
	bool zeigen;
};
