#pragma once
#include "pch.h"
#include "CBlockInfo.h"

class CBlock
{
public:
	CBlock();
	CBlock(CString InText, CPoint Step);
	void AddWeiche( CString InText, CPoint Step);
	void AddStrecke(CString InText, CPoint Step);
	void AddWeg( bool Richtung, CBlock_Weg Weg);
	void ZeicheBlock(CDC* pDC, std::vector<TrainCon_Paar>* Weichen);
	void ZeicheTaster(CDC* pDC);
	void ZeicheBlockInfo(CDC* pDC, byte Nr);
	CString GetLok_NameonBlock();
	byte OnKlick(CPoint Klick, byte *WeicheData);
	bool set_Relais(TrainCon_Paar RelaisDaten);
	void Set_EinTasterFarbe(char TC);
	void Set_AusTasterFarbe(char TC);
	std::vector<TrainCon_Paar>* Get_zuschaltener_Weg(byte NachBlock);
	bool Weg_zuBlock(bool Richtung, std::vector<TrainCon_Paar>* Weichen, std::vector<byte>*WegeBlocks);
	void GetAnschlussBlocks(std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks);
	void bestetzen(CDataXpressNet* Lok);
	void bestetzen(bool Bit);
	void freimachen();
	bool ist_frei();
	byte Get_Aus_nextBlock(std::vector<TrainCon_Paar>* Weichen, std::vector<byte>*nBlock);
	byte Get_Ein_nextBlock(std::vector<TrainCon_Paar>* Weichen, std::vector<byte>* nBlock);

	BlockStatus GetStatus_Block(CString *Lok_Name, bool* Lok_Dir);
	TrainCon_Paar Get_Relais_Data(); 
	BlockDebugData Get_Debug();
	~CBlock();
	CDataXpressNet* Get_Lok_onBlock();
	Start_Lok_Block Get_StartLokInfo();

protected:
	CDataXpressNet* LokData = NULL;
	
	bool Block_gemeldet = false;// Melder gemeldet
	// Strecke 
	CBlock_Strecke GleisStrecke;
	byte BlockNr; // gelese Block Nummer
	CBlock_Taster Taster_Ein;
	CBlock_Taster Taster_Aus;
	// Weichen Gruppe
	std::vector <CBlock_Weiche> Weiche;
	std::vector<CBlock_Weg> nach_Weg;
	std::vector<CBlock_Weg> von_Weg;
};

