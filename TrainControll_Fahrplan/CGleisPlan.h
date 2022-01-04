#pragma once
#include "pch.h"
#include "CBlock.h"
#include "COM_LZV200.h"
#include "Com_BlockMelderNet.h"
#include "CDatenBankLok.h"
#include "TrainControll_Fahrplan.h"



class CGleisPlan
{
public:
	CGleisPlan();
	~CGleisPlan();
	void Init();
	// für den Task	
	void NewTimeZug(clock_t Zeit);
	void GetNextMessage_Mega();
	void GetNextMessage_LZV();

	bool isNewUpdate_Taster();

	
	void DoCheckIt(byte Lok_Nr, clock_t Zeit);
	void showInfo_Dlg(byte LokNr, CString Info);
	void showPlan_Dlg(byte LokNr, CString Info);

	void Set_Lok_Geschwindigkeit(byte LokNr, Zug_Status SetTo, byte Geschwindikkeit); // und sendet die Daten

	bool isPower_onGleis();
	bool isPower_onBlock(byte Nr);
	void ZeicheStrecke(CDC* pDC);
	bool Kick_Block(CPoint Klick);
	bool Set_Block(byte* Data);
	bool Set_Weiche(byte* Data);
	bool Set_Weiche(TrainCon_Paar WeichenDaten);
	bool Get_Weiche(TrainCon_Paar WeichenDaten);
	byte Get_Weichen_Anzahl();
	bool Set_Relais(byte* Data);
	bool Set_Relais(TrainCon_Paar RelayData);
	void Set_Door(byte* Data);
	void Ask_Door_Status(); // Status abfragen
	bool Get_Door_open();
	bool Get_Door_free();
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
	DoorInfo Tor;
	CCom_LZV200         *XpressNet	  = NULL;
	CDatenBankLok       *Züge		  = NULL;
	CCom_BlockMelderNet *BlockMelder  = NULL;
	BlockRueckmeldung	PruefeZugWeg	  = BlockRueckmeldung::Frei_Fahrt;
	ControlStatus		ModeControl	  = ControlStatus::Begin_COM;
	byte				ModeSub			= 0;
	byte				StatusZentrale	= 0;
	neueMeldungen		Neu_DatTaster   = neueMeldungen();
	bool				Zug_wartet_auf_Data = false;

	void Lade_Daten();

};
