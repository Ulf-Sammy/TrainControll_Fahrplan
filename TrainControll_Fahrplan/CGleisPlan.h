#pragma once
#include "pch.h"
#include "XpressNetCom.h"
#include "CBlockData.h"
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
	CDataBlock      Block_Data;
	bool ListentoCom = false;
	bool Thread_Run = false;

	CGleisPlan();
	~CGleisPlan();
	void Init();
	byte GetNextMessage();

	void Start_Com_Thread();
	void Stop_Com_Thread();
	void ChangeSetupMode(ControlStatus	Mode, byte Sub);

	bool isNewUpdate_Status();
		
	// für den Task	
	void NewDataMelder();
	void NewDataWeiche();
	void NewDataZug();
	void NewTimeZug(clock_t Zeit);
	void New_Daten_LVZ();
	void New_CV_Daten();
	void New_Daten_Senden();
	void New_Controll_Mode();

	void DoMelder_I(byte Melder_Nr, byte Lok_Nr); // arbeite die Melder ab
	void DoMelder_O(byte Melder_Nr, byte Lok_Nr); // arbeite die Melder ab
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

protected:
	CXpressNetCom   *XpressNet		= NULL;
	CDatenBankLok   *Züge			= NULL;
	BlockRueckmeldung PruefeZugWeg	= BlockRueckmeldung::Frei_Fahrt;
	int				StartUpCount	= -1;
	ControlStatus	ModeControl		= ControlStatus::No_Arduino;
	byte		    ModeSub			= 0;
	byte			StatusZentrale	= 0;
	neueMeldungen   Neu_DataStatus	= neueMeldungen();
	byte			max_active_Loks = 0;
	//bool StreckenPlan = true; // true = Test false = im Garten
	bool Zug_wartet_auf_Data = false;

};
