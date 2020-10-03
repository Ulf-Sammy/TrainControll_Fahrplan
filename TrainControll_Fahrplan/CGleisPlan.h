#pragma once
#include "pch.h"
#include "CBlock.h"
#include "ComXpressNet.h"
#include "Com_BlockMelderNet.h"
#include "CDatenBankLok.h"

struct InfoConStat
{
	InfoConStat(ControlStatus M, byte U)
	{
		hauptMode = M;
		unterMode = U;
	}
	ControlStatus hauptMode;
	byte		  unterMode;
};

UINT Thread_Update(LPVOID pParam);


class CGleisPlan: public CStatic
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
	void ChangeSetupMode(ControlStatus	Mode, byte Sub);

	bool isNewUpdate_Status();
	bool isNewUpdate_Gleis();
	bool isNewUpdate_Taster();
	// für den Task	
	void NewDataMelder();
	void NewDataWeiche();
	void NewDataRelais();
	void NewDataZug();
	void NewTimeZug(clock_t Zeit);
	void New_Daten_LVZ();
	void New_CV_Daten();
	void New_Daten_Senden();
	void New_Controll_Mode();

	void DoCheckIt(byte Lok_Nr, clock_t Zeit);
	void showInfo_Dlg(byte LokNr, CString Info);
	void showPlan_Dlg(byte LokNr, CString Info);

	void Set_Lok_Geschwindigkeit(byte LokNr, Zug_Status SetTo, byte Geschwindikkeit); // und sendet die Daten

	
	void Set_Startup(bool B);
	void Reeady_ToRun();
	byte GetStatusZentrale();
	InfoConStat GetStatusMode();
	void SetStatusPower();
	bool Schalte_Power_LVZ();
	bool isPower_onGleis();
	bool isreadyforStartup();
	void Setup_TrainControl();
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
	int				StartUpCount	= -1;
	ControlStatus	ModeControl		= ControlStatus::No_Arduino;
	byte		    ModeSub			= 0;
	byte			StatusZentrale	= 0;
	neueMeldungen   Neu_DataStatus	= neueMeldungen();
	neueMeldungen   Neu_DataGleis   = neueMeldungen(); // Melder und Weiche
	neueMeldungen   Neu_DatTaster   = neueMeldungen();
	byte			max_active_Loks = 0;
	bool Zug_wartet_auf_Data = false;

	void Lade_Daten();

};
