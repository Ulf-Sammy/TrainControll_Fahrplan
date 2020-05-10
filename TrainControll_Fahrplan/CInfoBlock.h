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


class CInfoBlock
{
public:
	CInfoBlock();
	~CInfoBlock();
	CInfoBlock(CString InText);
	BlockInfo Get_BlockInfo();
	void SetTaster(CString InText);
	void SkaliereDaten(CPoint Step);
	void Calac_Pos();
	byte GetNextBlock_Hand(bool Fahrrichtung);
	byte GetNextBlock_Auto(bool Fahrrichtung, bool S);
	CRect GetKlickField();

	byte Nr = 0;
	CDataXpressNet  *Besetzt_Zug = NULL;
	bool			WeichenStellung = false;
	byte			WeichenGruppe = 0;
	CString			Name;
	BlockType		Block_Type = BlockType::isBlock;
	WeichenType		Weiche_Type = WeichenType::linksWeiche;
	byte			EingangBlock[2] = { 0 ,0 };
	byte			AusgangBlock[2] = { 0 ,0 };
	byte			EingangMelder[2] = { 0 ,0 };
	byte			AusgangMelder[2] = { 0 ,0 };
	TasterData		EingangTaster ;
	TasterData		AusgangTaster ;
	
	char Richtung = ' ';
	char GliesPos = ' '; // Position des Gleises für die Lok
	CPoint TextPos   = CPoint(-1, -1); // Position des Gleis Textes
	CPoint EinPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CPoint AusPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CPoint MitPos[2] = { CPoint(-1, -1),CPoint(-1, -1) };
	CRect  KlickRechteck; // Rechteck in dem gelickt wird
};

