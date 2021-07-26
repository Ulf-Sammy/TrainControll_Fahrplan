#pragma once
#include "pch.h"
#include "CBlock.h"
#include "Com_XpressNet.h"
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
	bool isPower_onBlock(byte Nr);
	void Setup_TrainControl();
	void Setup_MelderControl();
	void ZeicheStrecke(CDC* pDC);
	bool Kick_Block(CPoint Klick);
	bool Set_Weiche(TrainCon_Paar WeichenData);
	bool Get_Weiche(TrainCon_Paar WeichenDaten);
	bool Set_Relais(TrainCon_Paar RelayData);
	void Schalte_Relais(byte Nr, bool Bit);
	void GetAnschlussBlocks(byte Nr, std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks);
	bool Weg_Von_bis_frei(byte BlockNr, byte BlockTo, std::vector<byte>*WegeBlocks);
	bool Weg_nach_von_Block(byte BlockNr, bool Richtung,std::vector<byte>* WegeBlocks);
	bool isWeg_frei(std::vector<byte> WegeBlocks);
	bool Besetze_Weg_mit_Lok(CDataXpressNet* XpressNet, std::vector<byte> WegeBlocks);
	CString Get_LokName_in_Block(byte Nr);
	BlockStatus GetStatus_Block(byte Nr, CString *Lok_Name );
	BlockDebugData Get_DebugData(byte Nr);
	void TestBlock_mitZug(byte Nr, bool Bit, bool leeren );
	CDataXpressNet* TestZug(bool Bit);

protected:
	std::vector <CBlock> Block;
	std::vector<TrainCon_Paar> WeichenAntrieb;

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
