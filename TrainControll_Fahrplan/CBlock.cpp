#include "pch.h"
#include "CBlock.h"
#include "TrainControll_FahrplanDlg.h"


CBlock::CBlock(CString InText)
{
	TRACE1("Block; %s \n", InText.Mid(0, 3));
	LokData = NULL;
}

void CBlock::AddStrecke(CString InText, CPoint Step)
{
	GleisStrecke.Setup(InText, Step);
}

void CBlock::ZeicheBlock(CDC* pDC)
{
	CPen Status;
	CString Lok_Name;
	if (LokData == NULL)
	{
		Lok_Name.Empty();
		if (Block_gemeldet)
			Status.CreatePen(PS_SOLID, 5, GleisOrange); //Orange		
		else
			Status.CreatePen(PS_SOLID, 5, GleisGruen); // Grün
	}
	else
	{
		Lok_Name = LokData->Name;
		if (Block_gemeldet)
			Status.CreatePen(PS_SOLID, 5, GleisRot); // Rot
		else
			Status.CreatePen(PS_SOLID, 5, GleisOrange); //Orange
	}
	if (!GleisStrecke.empty())
	{
		GleisStrecke.zeichneStrecke(pDC, &Status, Lok_Name);
	}
	if (!Weiche.empty())
	{
		for (CBlock_Weiche W : Weiche)
		{
			W.zeichneWeiche(pDC, &Status);
		}
	}
}

byte CBlock::OnKlick(CPoint Klick, TrainCon_Paar* WeichenDaten)
{
	if (!Weiche.empty())
	{
		for (CBlock_Weiche W : Weiche)
		{
			if (W.klickedWeiche(Klick))
			{
				*WeichenDaten = W.Weichenantrieb();
				WeichenDaten->SetBit(!WeichenDaten->GetBit());
				return 1;
			};
		}
	}
	if (GleisStrecke.klickedGleis(Klick))
	{
			*WeichenDaten = GleisStrecke.Relais();
			WeichenDaten->SetBit(!WeichenDaten->GetBit());
			return 2;
	}
	return 0;
}

bool CBlock::set_Weiche(TrainCon_Paar WeichenDaten)
{
	if (!Weiche.empty())
	{
		for (auto&  W : Weiche)
		{
			auto i = &W - &Weiche[0];
			if (Weiche[i].SetWeiche(WeichenDaten))
			{
				return true;
			}
		}
	}
	return false;

}

bool CBlock::set_Relais(TrainCon_Paar RelaisDaten)
{
	if (GleisStrecke.schalteGleisRelais(RelaisDaten))
	{
		return true;
	}
	return false;
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

void CBlock::AddWeiche(CString InText, CPoint Step)
{
	Weiche.push_back(CBlock_Weiche(InText, Step));
}

CBlock::~CBlock()
{}
