#pragma once
#include "pch.h"
#include "CBlockInfo.h"
#include "Com_XpressNet.h"
#include "Com_BlockMelderNet.h"

class CBlock
{
public:
	CBlock(CString InText);
	void AddWeiche( CString InText, CPoint Step);
	void AddStrecke(CString InText, CPoint Step);
	void AddWeg( bool Richtung, CBlock_Weg Weg);
	void ZeicheBlock(CDC* pDC, std::vector<TrainCon_Paar>* Weichen);
	CString GetLok_NameonBlock();
	byte OnKlick(CPoint Klick, byte *WeicheData);
	bool set_Relais(TrainCon_Paar RelaisDaten);
	bool Weg_zuBlock(bool Richtung, std::vector<TrainCon_Paar>* Weichen, std::vector<byte>*WegeBlocks);
	void GetAnschlussBlocks(std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks);
	void bestetzen(CDataXpressNet* Lok);
	void bestetzen(bool Bit);
	void freimachen();
	bool ist_frei();
	BlockStatus GetStatus_Block(CString *Lok_Name, bool* Lok_Dir);
	TrainCon_Paar Get_Relais_Data(bool Inv); // wenn true invertiere Bit
	BlockDebugData Get_Debug();
	~CBlock();
protected:
	CDataXpressNet* LokData = NULL;
	
	bool Block_gemeldet = false;// Melder gemeldet
	// Strecke 
	CBlock_Strecke GleisStrecke;
	byte BlockNr; // gelese Block Nummer

	std::vector<CPoint> StreckePunkte;
	// Weichen Gruppe
	std::vector <CBlock_Weiche> Weiche;
	std::vector<CBlock_Weg> nach_Weg;
	std::vector<CBlock_Weg> von_Weg;
};

