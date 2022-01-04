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
#define lowByte(w) ((WORD) ((w) & 0xff))
#define highByte(w) ((WORD) ((w) >> 8))


#define COM_LEN(L)          (L & 0x07)     //                       Setup - Test - Auto - Program
#define COM_SEND_MOD          0x08+0x02  // 1 MOD-status			    | 
#define COM_SEND_GLPOWER      0x18+0x03  // 2 Power on Gleis  over I2C  | 
#define COM_SEND_RELAIS       0x20+0x03  // 3 Relais Nr + Bit			| New
#define COM_SEND_DOOR         0x28+0x04  // 4 fahern					|
#define COM_SEND_DOOR_STAT    0x30+0x04  // 5 offen + belegt			| 
#define COM_SEND_WEICHE       0x38+0x03  // 6 Weiche + Wert			    |
#define COM_TC_WIRTE_BLOCK	  0x40+0x03  // 7 BLOCK + Wert			    | New											

#define COM_I2C_DEV1		  0x50+0x03  // 8 Melder Info
#define COM_I2C_DEV2		  0x58+0x03  // 9 Gleis Power Info
#define COM_I2C_DEV3          0x60+0x03  //10 Gleis Position Info


#define COM_TC_WRITE_CUR1     0xB0+0x02  //11 Strom abfragen | A.A in Ampere
#define COM_TC_WRITE_CUR2     0xB8+0x02  //12 Strom abfragen | 


#define COM_PC_ASK_VERSION    0xD0+0x03  //13 Frage SoftwareVersion 3 - 0 |
#define COM_PC_WRITE_WEICHE   0xE0+0x02  //14 setze WeichenAnzahl		  |
#define COM_PC_WRITE_BC_ALL   0xE8+0x02  //15 NotAus LED / Normal LED     | LED Rot und Grün
#define COM_PC_WRITE_DISPLAY  0xF0+0x07  //16 Test 1H2o3l4l5o			  |

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
#define FILE_ALLE_ZUEGE_BILDER "Images\\"
#define FILE_ALLE_BILDER "Images\\%s"

#define COM_MAX_LISTE 10
#define COM_MEGA_PC 3
#define COM_MEGA_LOG 9
#define	COM_LZV200 20

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
#define MAX_STROM_ZEIT 255


const byte DCC_SPEED_coded[] = { 0x00,0x02,0x12,0x03,0x13,0x04,0x14,0x05,0x15,0x06,0x16,0x07,0x17,0x08,0x18,0x09,0x19,0x0A,0x1A,0x0B,0x1B,0x0C,0x1C,0x0D,0x1D,0x0E,0x1E,0x0F,0x1F };
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
const COLORREF colorgruen = RGB(10,233,20);
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

const CString LED_Images[] = { L"Images\\LED_Rot.png" ,L"Images\\LED_greun.png",L"Images\\LED_blau.png",L"Images\\LED_Orange.png", L"Images\\LED_Violet.png" };
const CString LVZ_Images[] = { L"Images\\Button_off.png",L"Images\\Button_on.png" };
const CString MOD_Images[] = { L"Images\\Train_Test.png",L"Images\\Train_RUN.png",L"Images\\Train_PROG.png" };

enum class COM_Status  { No_Com, Com_Found, Com_Timeout, Com_Error};

enum class BlockStatus  { Frei, Besetzt, BesetztError_A, BesetztError_B};
enum class BC_Meldungen {  BC_Alles_Aus,BC_Alles_An,	BC_Alles_Loks_Aus,	BC_Programmiermode,	BC_Rueckmeldung};
enum class XpNSendwas   { FGruppe0 = 0, FGruppe1, FGruppe2, FGruppe3, FGruppe4, FGruppe5 };
enum class BlockType    { isWeiche, isBlock, isGleis };
enum class WeichenType  { linksWeiche, rechtsWeiche, L_DoppelWeiche, R_DoppelWeiche};
enum class StreckenType { Strecke, Gleis, Abstellgleis};

enum class FahrPlanDo { begin_Block, stoppen, vorwaerz_fahren, rueckwaerz_fahren, warten_fahren, warten_stoppen, schalten_Funk, schalten_Weiche, letzte_Zeile };

enum class Zug_Status { Zug_Stopped = 0, Zug_faehrt_vor = 1, Zug_faehrt_rueck = 2, Zug_haelt = 3};
enum class Zug_Steuerung { nicht_Betriebs_bereit, Hand_Betrieb, Automatik_Betrieb };
enum class ControlStatus { Begin_COM = 0, Setup, Fahren, Program, Ende_COM };

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

struct COM_Info
{
	CString COM_FriendlyName;
	CString COM_PORT;
	CString COM_Status;
	CString COM_Conect;
	bool COM_Active = false;
	bool COM_Error = false;
	HANDLE COM_Handel = NULL;
	DWORD BaudRate;
	void Com_off()
	{
		;
	}
};

class DoorInfo
{
	public:	
		bool Tor_offen;
		byte Tor_frei;
};

class Lok_Adresse
{
	public:
		Lok_Adresse()
		{
			Adr = 0;
			Adr_H = 0;
			Adr_L = 0;
		}
		Lok_Adresse(DWORD adr)
		{
			Adr = adr;
			Adr_H = (byte)highByte(Adr);
			Adr_L = (byte)lowByte(Adr);
		}
	byte LowAdr()
		{
			return Adr_L;
		}
		byte HighAdr()
		{
			return Adr_H;
		}
	protected:
		DWORD Adr;
		byte Adr_L;
		byte Adr_H;
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

	TrainCon_Paar(byte* D)
	{
		Wert = D[1];
		Bit = (bool)D[2];
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
