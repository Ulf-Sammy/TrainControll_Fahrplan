#pragma once
#include "pch.h"
#include "CDataXpressNet.h"
#include <vector>


class CBlock_Taster
{
public:
	CBlock_Taster();
	~CBlock_Taster();
	void SkaliereDaten(char Step);
protected:
	byte Block_Nr;
	bool Showit = false; // Benutzte und Zeigen
	char Color = '#';   // Füll Farbe 
	CRect KlickRechteck; // Rechteck in dem gelickt wird
	CPoint Pos;
	/*
	byte 
	{
		switch (Step)
		{
		case 'L': // Links
			Pos = Pos + CPoint(-24,0);
			break;
		case 'R': // Rechts
			Pos = Pos + CPoint(24, 0);
			break;
		case 'U': // Up hoch
			Pos = Pos + CPoint(0,-24);
			break;
		case 'D': // Down runter
			Pos = Pos + CPoint(0,24);
			break;
		default:
			break;
		}
		KlickRechteck = CRect(-10, -10, 10, 10);
		KlickRechteck.OffsetRect(Pos);
	}
	*/
};

class CBlock_Weiche
{
public:
	CBlock_Weiche();
	CBlock_Weiche(CString InText, CPoint Step);
	void zeichneWeiche(CDC* pDC,BlockStatus Bl_Status,std::vector<TrainCon_Paar>* Weichen);
	bool klickedWeiche(CPoint KlickP);
	bool SetWeiche(TrainCon_Paar WeichenDaten);
	byte Weichenantrieb();
	~CBlock_Weiche();

private:

	byte Antrieb_Nr;
	WeichenType		Type = WeichenType::linksWeiche;
	CPoint TextPos = CPoint(-1, -1); // Position des Gleis Textes
	CPoint EinPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CPoint MitPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CPoint AusPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CRect  KlickRechteck; // Rechteck in dem gelickt wird
};



class CBlock_Strecke
{
public:
	CBlock_Strecke();
	CBlock_Strecke(const CBlock_Strecke&);
	CBlock_Strecke(CString InText, CPoint Step);
	bool empty();
	void Setup(CString InText, CPoint Step);
	void zeichneStrecke(CDC* pDC, BlockStatus Bl_Status, CString LokName, bool LokDir);
	bool klickedGleis(CPoint KlickP);
	bool schalteGleisRelais(TrainCon_Paar Relais);
	TrainCon_Paar Relais();
	byte Get_Relais_Nr();
	void SetRelais(TrainCon_Paar Relais); // beim Laden Daten
	void GetDebugData(BlockDebugData* Data);
	~CBlock_Strecke();

private:
	CString Gleis_Name;
	byte Relais_Nr;
	bool Relais_Power; // true = Power on
	char GliesPos = ' '; // Position des Gleises für die Lok
	char box = ' '; // Richtung der Box
	StreckenType Type = StreckenType::Strecke;
	CPoint TextPos = CPoint(-1, -1); // Position des Gleis Textes
	CPoint TextBes = CPoint(-1, -1); // Position Beschriftung Gleis
	std::vector<CPoint> StreckePunkte;
	CRect  KlickRechteck; // Rechteck in dem gelickt wird
};

class CBlock_Weg
{
public:
	CBlock_Weg();
	CBlock_Weg(CString InText);
	void WechselRichtung();
	bool isvonBlock(byte EBlock, byte ABlock);
	bool isWeg_geschaltet(std::vector<TrainCon_Paar>* Weichen);
	bool isWeg_nachgeschaltet(byte Block, std::vector<TrainCon_Paar>* Weichen);
	void Blocksaufweg(std::vector<byte>* Block);
	byte VonBlock(); // von Block  
	byte ZwiBlock();
	byte NachBlock(); // nach Block im Uhr zeiger Sinn

private:
	byte Block_von; // von Block ... nach im Uhrzeigersinn
	byte Block_nach;
	byte Block_zwich;
	std::vector<TrainCon_Paar> WeichenWeg;
};