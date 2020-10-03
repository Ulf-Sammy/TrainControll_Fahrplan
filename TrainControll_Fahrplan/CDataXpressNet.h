#pragma once


#include "ComXpressNet.h"
#include "CDataFahrplan.h"
#include "VC_TableInfo.h"
#include <vector>


class CDataXpressNet
{
public:
	CDataXpressNet();
	CDataXpressNet(CString LokName);
	bool FillData(CString InText);
	~CDataXpressNet();
	void Set_Dlg_Nr(byte Nr);
	byte Get_Dlg_Nr();

	bool Pr�fe_Plan_im_Block();
	void Set_Block(byte Ist, byte Soll, byte Best, byte Melder);
	void Set_Besetzt_Block(byte Best);
	void Set_Block_Pause(bool Stop);
	byte Get_Soll_Block();
	void Lade_Fahrplan();
	void ASK_LokData();
	void Get_LokData();
	void Set_Adresse();
	CString Text_Block();
	CString Text_Betrieb();
	CString Text_Adresse();
	CString Text_Hersteller();
	CString Text_Decoder();
	CString Text_Decoder_Sub();
	
	byte Get_Decoder_Nr();
	void Set_Startbedingungen(Zug_Status UserSet_Status);
	void Set_Lok_Nr(byte Nr_EDV);
	void Set_Funktion(byte Nr, bool bit);
	void Set_Funktion(FahrplanPos Befehl);
	void Set_Funktion_Sound(bool SW);
	void Set_Funktion_Rangieren(bool SW);
	void Set_Funktion_Verz�gerungsZeit(bool SW);
	bool Get_Funktion(byte Nr);
	void Set_Geschwindigkeit(byte Geschwindigkeit, bool FahrtRichtung);
	void Set_Geschwindigkeit(FahrplanPos Befehl);
	void ReSet_Geschwindigkeit();
	void Set_Stop();
	void Set_Halt();
	void Set_Halt(FahrplanPos Befehl);
	bool isNext_Melder(byte Melder);
	bool isHalt();
	bool isVorw�rtz();
	bool isOnGleis();
	bool isActive();
	bool isAutomaticOn();
	bool isHand();
	bool Pr�frichtung();
	Zug_Status Get_Status();

	CDecoderInfo  Decoder_Data;
	HBITMAP		  Bild = 0x00 ;			// Bild der Lok
	CString		  Name ;				// Name der Lok
	CString       LokNr  = 0;			// Lok-Nr so was wie eine Ulf EDV Nummer
	byte		  Dlg_Nr = 0xFF;		// Merke dir die Hand Dialog
	bool		  DoFahrplan = false;	// Fahrplan wird im Thread abgearbeitet 
	CDataFahrplan Fahr_Plan;
	byte		  Plan_Melder = 0;		// n�chster Melder im Fahrplan
	bool		  Plan_Warten = false;	// wartet nach Plan
	clock_t		  WarteZeit = 0;		// Zeit die der Zug warten soll
	bool		  Blick = false;		// true heist lok steht in Uhrzeigerrichtung
	byte		  Block_ist = 0;		// ist Position der Lok im Block
	byte		  Block_soll = 0;		// soll Position der Lok im Block
	byte		  Block_best = 0;		// der blokierte Block
	byte		  Melder_next = 0;		// Melder der f�r den Zug die n�chte Strecke berechnet/Blockiert 
	byte		  Next_Hand_ABlock = 0; // N�chster Start AUS Block aus Sicht vor dem Start
	byte		  Next_Hand_EBlock = 0; // N�chster Start Ein Block aus Sicht vor dem Start
	RingBuffer	  ClearList;            // Liste von Melder bei 0 die Strecke freigeben
	Zug_Steuerung Betriebs_Modus = Zug_Steuerung::nicht_Betriebs_bereit;
	//#############################################


protected:                 //0 1 2 3 4 5 6 7 8 9 0 1 2 3
	const std::vector<byte> BitNr { 4,0,1,2,3,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7 };
	const std::vector<byte> FunNr { 1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4 };
	

	// alles zum Fahren
	CCom_XpressNet   *XpressNet = NULL;
	Zug_Status    Status = Zug_Status::Zug_Stopped;       // Der Status vom Zug stop,f�hr, h�lt
	bool		Zug_active			= false; // ist Zug active
	bool		Zug_wartet_auf_Data = false; // Zug wartet auf Daten
	byte		FahrPlan_Nr			= 0;     // Nr des Fahrplans
	byte		Adresse				= 0;     // Adresse Lok
	byte		FahrGesch			= 0;     // diese Werte werden genommen um zu�bertragen
	bool		FahrRicht			= false; // diese Werte werden genommen um zu�bertragen
	std::vector<byte>	FunktionsGruppe{ 0x00,0x00,0x00,0x00,0x00 };

};
