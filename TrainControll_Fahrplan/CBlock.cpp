#include "pch.h"
#include "CBlock.h"
#include "TrainControll_FahrplanDlg.h"


CBlock::CBlock(CString InText)
{
	LokData = NULL;
	TRACE1("Block= %s \n", InText.Mid(0, 3));
	BlockNr = _ttoi(InText.Mid(0,3));

	/*

	CString TextL;
	int Pos = 10;
	int Ar = 0;

	do
	{
		Pos = Ar * 3 + 10;
		Ar++;
		EinBlock.push_back(_ttoi(InText.Mid(Pos, 2)));

	} while (InText[Pos + 2] != '|');
	*/
	//for (int i = 0; i < Reads; i++)
	//{
	//	
	//	if (Eingang)
	//	{
	//		EinBlock.push_back(CBlock_Weg(InText.Mid(Pos, 2)));
	//	}
	//	else
	//	{
	//		AusBlock.push_back(CBlock_Weg(InText.Mid(Pos, 2)));
	//	}
	//	Ar++;
	//	if (InText[Pos + 2] == '|')
	//	{
	//		Ar = 0;
	//		Eingang = false;
	//	}
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
	if (!Weiche.empty())
	{
		for (CBlock_Weiche W : Weiche)
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

bool CBlock::Weg_zuBlock(bool Richtung, std::vector<TrainCon_Paar>* Weichen, std::vector<byte>*WegeBlocks)
{
	
	if (Richtung)
	{
		for (auto Weg : nach_Weg)
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
		for (auto Weg : von_Weg)
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
	for (auto Weg : nach_Weg)
	{
		EinBlocks->push_back(Weg.NachBlock());
	}
	for (auto Weg : von_Weg)
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

TrainCon_Paar CBlock::Get_Relais_Data(bool Inv)
{
	if (Inv)
		return GleisStrecke.Relais().GetInvBit();
	else
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
