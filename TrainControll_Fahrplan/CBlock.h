#pragma once
#include "pch.h"
#include "CBlockInfo.h"
#include "ComXpressNet.h"
#include "Com_BlockMelderNet.h"

class CBlock
{
public:
	CBlock(CString InText);
	void AddWeiche( CString InText, CPoint Step);
	void AddStrecke(CString InText, CPoint Step);
	void ZeicheBlock(CDC* pDC);
	byte OnKlick(CPoint Klick, TrainCon_Paar *WeichenDaten);
	bool set_Weiche(TrainCon_Paar WeichenDaten);
	bool set_Relais(TrainCon_Paar RelaisDaten);
	void bestetzen(CDataXpressNet* Lok);
	void bestetzen(bool Bit);
	void freimachen();
	bool ist_frei();
	~CBlock();
protected:
	CDataXpressNet* LokData = NULL;
	// Melder gemeldet
	bool Block_gemeldet = false;
	//bool Block_Zugein = false;
	// Strecke 
	CBlock_Strecke GleisStrecke;
	// Weichen Gruppe
	std::vector <CBlock_Weiche> Weiche;
	// Zug im Block
};

