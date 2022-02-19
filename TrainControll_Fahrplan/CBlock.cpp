#include "pch.h"
#include "CBlock.h"
#include "TrainControll_FahrplanDlg.h"


CBlock::CBlock()
{
	BlockNr = 0;
}

CBlock::CBlock(CString InText, CPoint Step)
{
	
	int L = InText.GetLength();
	LokData = NULL;
	TRACE1("Block= %s \n", InText.Mid(0, 3));
	BlockNr = _ttoi(InText.Mid(0,3));
	if (InText.GetLength() > 17)
	{
		if (InText[18] == 'E')
			Taster_Ein = CBlock_Taster(InText.Mid(18, 10), Step);
		if (InText[18] == 'A')
			Taster_Aus = CBlock_Taster(InText.Mid(18, 10), Step);
	}
	if (InText.GetLength() > 28)
	{
		if (InText[29] == 'E')
			Taster_Ein = CBlock_Taster(InText.Mid(29, 10), Step);
		if (InText[29] == 'A')
			Taster_Aus = CBlock_Taster(InText.Mid(29, 10), Step);
	}
}

void CBlock::AddStrecke(CString InText, CPoint Step)
{
	GleisStrecke.Setup(InText, Step);
}

void CBlock::AddWeg(bool Richtung, CBlock_Weg Weg)
{
	if(Richtung)
		nach_Weg.push_back(Weg);
	else
	{
		Weg.WechselRichtung();
		von_Weg.push_back(Weg);
	}
}

void CBlock::ZeicheBlock(CDC* pDC, std::vector<TrainCon_Paar>* Weichen)
{
	BlockStatus Status;
	CString Lok_Name;
	bool Lok_Richtung;

	Status = GetStatus_Block(&Lok_Name,&Lok_Richtung);

	if (!GleisStrecke.empty())
	{
		GleisStrecke.zeichneStrecke(pDC, Status, Lok_Name, Lok_Richtung);
	}
	if (!Weiche.empty())
	{
		for (CBlock_Weiche W : Weiche)
		{
			W.zeichneWeiche(pDC, Status, Weichen);
		}
	}
}

void CBlock::ZeicheTaster(CDC* pDC)
{
	if (!GleisStrecke.empty())
	{
		Taster_Ein.zeichneTaster(pDC, BlockNr);
		Taster_Aus.zeichneTaster(pDC, BlockNr);
	}
}

void CBlock::ZeicheBlockInfo(CDC* pDC, byte Nr)
{
	if (!GleisStrecke.empty())
	{
		GleisStrecke.zeichneBlockNr(pDC, Nr);
	}
}

CString CBlock::GetLok_NameonBlock()
{
	if (LokData == NULL)
	{
		return _T("");
	}
	else
	{
		return LokData->Name;
	}
}

byte CBlock::OnKlick(CPoint Klick, byte* Data)
{
	if (Taster_Ein.OnKlick(Klick)) return 3;
	if (Taster_Aus.OnKlick(Klick)) return 4;
	if (!Weiche.empty())
	{
		
		for (auto& W : Weiche)
		{
			if (W.klickedWeiche(Klick))
			{
				*Data = W.Weichenantrieb();
				return 1;
			};
		}
	}
	if (GleisStrecke.klickedGleis(Klick))
	{
			*Data = GleisStrecke.Relais().GetWert();
			return 2;
	}
	return 0;
}


bool CBlock::set_Relais(TrainCon_Paar RelaisDaten)
{
	if (GleisStrecke.schalteGleisRelais(RelaisDaten))
	{
		return true;
	}
	return false;
}

void CBlock::Set_EinTasterFarbe(char TC)
{
	Taster_Ein.SetFarbe(TC);
}

void CBlock::Set_AusTasterFarbe(char TC)
{ 
	Taster_Aus.SetFarbe(TC);
}

std::vector<TrainCon_Paar>* CBlock::Get_zuschaltener_Weg(byte NachBlock)
{
	for (auto& W : nach_Weg)
	{
		if (W.NachBlock() == NachBlock)
		{
			return(W.Hole_den_Weg());
		}
	}
	for (auto& W : von_Weg)
	{
		if (W.NachBlock() == NachBlock)
		{
			return(W.Hole_den_Weg());
		}
	}
	return(NULL);
}

bool CBlock::Weg_zuBlock(bool Richtung, std::vector<TrainCon_Paar>* Weichen, std::vector<byte>*WegeBlocks)
{
	
	if (Richtung)
	{
		for (auto &Weg : nach_Weg)
		{
			if (Weg.isWeg_geschaltet(Weichen))
			{
				Weg.Blocksaufweg(WegeBlocks);
				return true;
			}
		}
	}
	else
	{
		for (auto &Weg : von_Weg)
		{
			if (Weg.isWeg_geschaltet(Weichen))
			{
				Weg.Blocksaufweg(WegeBlocks);
				return true;
			}
		}
	}
	return false;
}


void CBlock::GetAnschlussBlocks(std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks)
{
	for (auto &Weg : nach_Weg)
	{
		EinBlocks->push_back(Weg.NachBlock());
	}
	for (auto &Weg : von_Weg)
	{
		AusBlocks->push_back(Weg.NachBlock());
	}
}

void CBlock::bestetzen(CDataXpressNet* Lok)
{
	LokData = Lok;
}

void CBlock::bestetzen(bool Bit)
{
	Block_gemeldet = Bit;
}

void CBlock::freimachen()
{
	LokData = NULL;
}

bool CBlock::ist_frei()
{
	return (LokData == NULL);
}

byte CBlock::Get_Aus_nextBlock(std::vector<TrainCon_Paar>* Weichen, std::vector<byte>* nBlock)
{
	byte B = 0;
	for (auto& Weg : nach_Weg)
	{
		nBlock->push_back(Weg.NachBlock());
		if (Weg.isWeg_geschaltet(Weichen))
		{
			B = Weg.NachBlock();
		}
	}
	return B;
}

byte CBlock::Get_Ein_nextBlock(std::vector<TrainCon_Paar>* Weichen, std::vector<byte>* nBlock)
{
	byte B = 0;
	for (auto& Weg : von_Weg)
	{
		nBlock->push_back(Weg.NachBlock());
		if (Weg.isWeg_geschaltet(Weichen))
		{
			B = Weg.NachBlock();
		}
	}
	return B;
}
BlockStatus CBlock::GetStatus_Block(CString *Lok_Name, bool *Lok_Dir)
{
	BlockStatus Status;

	if (LokData == NULL)
	{ // Keine Lok eingetragen
		if (Block_gemeldet) Status = BlockStatus::BesetztError_A;
		else    			Status = BlockStatus::Frei;
		Lok_Name->Empty();
		*Lok_Dir = true;
	}
	else
	{ // Lokeingetragen
		if (Block_gemeldet) Status = BlockStatus::Besetzt;
		else			    Status = BlockStatus::BesetztError_B;
		*Lok_Name = LokData->Name;
		*Lok_Dir = LokData->Blick;
	}
	return Status;
}

TrainCon_Paar CBlock::Get_Relais_Data()
{
	return GleisStrecke.Relais();
}

BlockDebugData CBlock::Get_Debug()
{
	BlockDebugData Data;

	GleisStrecke.GetDebugData(&Data);
	Data.Block_Nr = BlockNr;
	if (LokData == NULL) Data.Lok_Name.Empty();
	else
	{
		Data.Lok_Name = LokData->Name;
		Data.Lok_Richtung = LokData->Blick;
	}
	Data.Block_gemeldet = Block_gemeldet;
	Data.Weichen_Anzahl = (byte) Weiche.size();
	GetAnschlussBlocks(&Data.EinBlock, &Data.AusBlock);
	return (Data);
}

void CBlock::AddWeiche(CString InText, CPoint Step)
{
	Weiche.push_back(CBlock_Weiche(InText, Step));
}

CBlock::~CBlock()
{}

CDataXpressNet* CBlock::Get_Lok_onBlock()
{
	return LokData;
}

Start_Lok_Block CBlock::Get_StartLokInfo()
{
	Start_Lok_Block Data;
	Data.Block = BlockNr;
	if (LokData != nullptr)
	{
		Data.Lok_Name = LokData->Name;
		Data.Blick = LokData->Blick;
		Data.Betriebs_Modus = LokData->Betriebs_Modus;
	}
	return Data;
}
