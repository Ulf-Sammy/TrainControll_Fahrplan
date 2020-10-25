#pragma once
#include "pch.h"
#include "CBlock.h"
#include "ComXpressNet.h"
#include "Com_BlockMelderNet.h"
#include "CDatenBankLok.h"
#include "TrainControll_Fahrplan.h"



UINT Thread_Update_UNO(LPVOID pParam);
UINT Thread_Update_MEGA(LPVOID pParam);
UINT Thread_Update_Time(LPVOID pParam);

class CGleisPlan
{
public:
	bool ListentoCom = false;
	bool Thread_Run = false;

	CGleisPlan();
	~CGleisPlan();
	void Init();
	byte GetNextMessage_Mega();
	byte GetNextMessage_Uno();

	void Start_Com_Thread();
	void Stop_Com_Thread();

	bool isNewUpdate_Taster();
	// für den Task	
	void Verarbeite_Mega_Daten(byte neueInfo);
	void Verarbeite_Uno_Daten(byte neueInfo);
	void NewTimeZug(clock_t Zeit);
	void New_Daten_Senden();
	
	void DoCheckIt(byte Lok_Nr, clock_t Zeit);
	void showInfo_Dlg(byte LokNr, CString Info);
	void showPlan_Dlg(byte LokNr, CString Info);

	void Set_Lok_Geschwindigkeit(byte LokNr, Zug_Status SetTo, byte Geschwindikkeit); // und sendet die Daten

	byte GetStatusZentrale();

	void SetStatusPower();
	bool Schalte_Power_LVZ();
	bool isPower_onGleis();
	void Setup_TrainControl();
	void Setup_MelderControl();
	void ZeicheStrecke(CDC* pDC);
	void ZeicheBlock(byte Nr, CDC* pDC);
	void Kick_Block(CPoint Klick);
	void Set_Weiche(TrainCon_Paar WeichenData);
	void Set_Relais(TrainCon_Paar RelayData);

protected:
	std::vector <CBlock> Block;
	CCom_XpressNet      *XpressNet	  = NULL;
	CDatenBankLok       *Züge		  = NULL;
	CCom_BlockMelderNet * BlockMelder = NULL;
	BlockRueckmeldung PruefeZugWeg	= BlockRueckmeldung::Frei_Fahrt;
	ControlStatus	ModeControl		= ControlStatus::No_Arduino;
	byte		    ModeSub			= 0;
	byte			StatusZentrale	= 0;
	neueMeldungen   Neu_DatTaster   = neueMeldungen();
	byte			max_active_Loks = 0;
	bool Zug_wartet_auf_Data = false;

	void Lade_Daten();

};
