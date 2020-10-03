#pragma once
#include "pch.h"
#include "CDataXpressNet.h"
#include <vector>


struct TasterData
{
	bool Showit = false; // Benutzte und Zeigen
	char Color  = '#';   // Füll Farbe 
	CRect KlickRechteck; // Rechteck in dem gelickt wird
	CPoint Pos;
	void SkaliereDaten(char Step)
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
};

class CBlock_Weiche
{
public:
	CBlock_Weiche();
	CBlock_Weiche(CString InText, CPoint Step);
	void zeichneWeiche(CDC* pDC,CPen *Penstatus);
	bool klickedWeiche(CPoint KlickP);
	bool SetWeiche(TrainCon_Paar WeichenDaten);
	TrainCon_Paar Weichenantrieb();
	~CBlock_Weiche();

private:

	bool Stellung;
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
	void zeichneStrecke(CDC* pDC, CPen* Penstatus, CString LokName);
	bool klickedGleis(CPoint KlickP);
	bool schalteGleisRelais(TrainCon_Paar Relais);
	TrainCon_Paar Relais();
	void SetRelais(TrainCon_Paar Relais); // beim Laden Daten
	~CBlock_Strecke();

private:
	CString Gleis_Name;
	byte Relais_Nr;
	bool Relais_Power; // true = Power on
	char GliesPos = ' '; // Position des Gleises für die Lok
	StreckenType Type = StreckenType::Strecke;
	CPoint TextPos = CPoint(-1, -1); // Position des Gleis Textes
	CPoint TextBes = CPoint(-1, -1); // Position Beschriftung Gleis
	std::vector<CPoint> StreckePunkte;
	CRect  KlickRechteck; // Rechteck in dem gelickt wird
	CFont BeschriftungBlock;
	CFont GleisBeschriftung ;
};

