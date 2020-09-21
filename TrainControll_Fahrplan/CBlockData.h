#pragma once
#include "pch.h"
#include <vector>
#include "XpressNetCom.h"
#include "CDataXpressNet.h"
#include "CDatenBankLok.h"
#include "CDataFahrplan.h"
#include "CInfoBlock.h"


struct Weichen_Gruppe
{
	Weichen_Gruppe()
	{
	}
	Weichen_Gruppe(CString InText)
	{
		Ein_Block = _ttoi(InText.Mid( 0, 3));
		Aus_Block = _ttoi(InText.Mid(11, 3));
		if (0 < _ttoi(InText.Mid(15, 2)))	
			WStellung.push_back(TrainCon_Paar(InText.Mid(15, 3)));
		if (0 < _ttoi(InText.Mid(19, 2)))	
			WStellung.push_back(TrainCon_Paar(InText.Mid(19, 3)));
		if (0 < _ttoi(InText.Mid(23, 2)))	
			WStellung.push_back(TrainCon_Paar(InText.Mid(23, 3)));
		if (0 < _ttoi(InText.Mid(27, 2)))	
			WStellung.push_back(TrainCon_Paar(InText.Mid(27, 3)));
		if (0 < _ttoi(InText.Mid(31, 2)))	
			WStellung.push_back(TrainCon_Paar(InText.Mid(31, 3)));
	}
	byte GetBlock(byte Block)
	{
		if (Block == Ein_Block) return (Aus_Block);
		if (Block == Aus_Block) return (Ein_Block);
		return (0);
	}
	bool isdiesderWeg(byte Ein, byte Aus)
	{
		if ((Ein == Ein_Block) && (Aus == Aus_Block)) return true;
		if ((Aus == Ein_Block) && (Ein == Aus_Block)) return true;
		return false;
	}
	bool isBlockinEin(byte Block)
	{
		return (Block == Ein_Block);
	}
	bool isBlockinAus(byte Block)
	{
	return (Block == Aus_Block);
	}
	byte Gruppen_Nr = 0;
	byte Ein_Block = 0;
	byte Aus_Block = 0;
	std::vector<TrainCon_Paar> WStellung;
};

class CDataBlock
{
public:
	CDataBlock();
	~CDataBlock();
	void Init();
	RingbufferM	   MelderRing;

	bool ist_Lok_onBlock( byte Block);
	void Load_Data_Block();
	

	// alles für Block
	byte GetAnzahl_Block();
	CInfoBlock GetWerte_Block(byte Nr);
	BlockInfo GetWerte_BlockInfo(byte Nr);
	bool ist_besetzt(byte Nr);
	bool ist_frei(byte Nr);
	CString GetZugName_in_Block(byte Nr);
	
	BlockType Get_BlockType(byte Nr);
		
	byte ClearBlockbyLok(byte LokNr, byte Melder);

	// alles für den Fahrplan
	byte FahrplanAnzahl = 0;
	bool Set_FahrPlan(byte Lok_Nr, byte Plan_Nr);
	void Do_Start_Plan_Zug(byte Zug_Nr);

	// alles im Block
	CString Get_Gleis_Name(byte Nr);
	CRect Get_Weichen_Feld(byte Nr);
	byte Get_Next_Block(byte Nr, bool PRichtung);
	byte Get_Next_MelderzuBlock(byte Lok_Nr, byte Block_Nr);
	byte Get_Soll_MelderzuBlock(byte Lok_Nr, byte Block_Nr);
	byte Get_Soll_MelderzuBlock(byte Lok_Nr, TrainCon_Paar Block);
	void besetzte_Block(byte Nr, CDataXpressNet* Lok);
	BlockRueckmeldung besetzte_bis_Next_Block_Hand( byte Lok);
	BlockRueckmeldung besetzte_bis_Next_Block_Auto(byte Lok);
	CDataXpressNet *Get_Block_Lok(byte Block_Nr);
	void setze_Block_Test(byte Nr, bool setze);
	// für Melder
	void Set_Melder(TrainCon_Paar Ml);
	byte GetAnzahl_Melder();
	byte GetMelder_Zeit();
	bool Show_Melder(byte Nr);
	bool Get_Melder(byte Nr);
	CPoint Get_Melder_Position(byte Nr);
	CPoint Get_Melder_PositionText(byte Nr);
	// für Taster
	bool Show_Taster(byte Nr, bool Ort);
	char GetCol_Taster(byte Nr, bool Ort);
	CRect Get_Taster_Position(byte Nr, bool Ort);
	void KlickTasterSchalten(CPoint point);
	// für Weichen
	bool is_DoppelWeiche(byte Nr);
	bool Get_Stellung_Weiche_Ein(byte Nr);
	bool Get_Stellung_Weiche_Aus(byte Nr);
	bool Get_Stellung_Weiche(byte Nr);
	void Set_Stellung_Weiche(TrainCon_Paar Wl);
	void Set_Stellung_WeicheWert(byte Nr, byte Wert);
	byte KlickWeicheNummer(CPoint KlickP);
	bool SchalteWeicheTest(byte WeichenNr);
	void SchalteWeiche(CPoint point);
	void SchalteWeiche(TrainCon_Paar Setto);
	void SchalteWeiche(byte Weiche, bool Weg);
	void SchalteWeichenStrasse(byte TasterA, byte TasterB);

	CPoint Get_Weiche_EinPos(byte Nr);
	CPoint Get_Weiche_EinPos(byte Nr, bool Bit);
	CPoint Get_Weiche_AusPos(byte Nr, bool Bit);
	CPoint Get_Weiche_MitPos(byte Nr);
	CPoint Get_Weiche_TexPos(byte Nr);
	// für Zug
	CDataXpressNet *Get_Block_besetzt_Wert(byte Bl_Nr);
	void doBlocks_byZug_Stop(byte LokNr);
	void doBlocks_byZug_Halt(byte LokNr);
	bool isNewUpdate_Gleis();
	bool isNewUpdate_Taster();
	bool isNewUpdate_Zug();

	std::vector <CInfoBlock> *GetBlockPointer();
	void copyWeg(std::vector <Weichen_Gruppe> *Tabelle);
	bool Wege_zuBlock(bool R, byte Block, std::vector <byte> *Tabelle);
protected:
	const int Rect_X = 1600;
	const int Rect_Y = 660;

	void _Block_besetzen(byte Nr, CDataXpressNet *Lok);
	void _Block_freimachen(byte Nr, byte Lok_Adr);

	CXpressNetCom   *XpressNet = NULL;
	CDatenBankLok   *Züge = NULL;
	CPoint Step;
	byte Melder_Zeit = 0;

	std::vector <MelderBlock> 	Melder; 
	std::vector <CInfoBlock>    Block;
	std::vector<Weichen_Gruppe> W_Gruppe;
	std::vector<CDataXpressNet> AktiveLoks;
	
	neueMeldungen   Neu_DataGleis  = neueMeldungen();
	neueMeldungen   Neu_DataTaster = neueMeldungen();
	neueMeldungen   Neu_DataZug    = neueMeldungen();
};

