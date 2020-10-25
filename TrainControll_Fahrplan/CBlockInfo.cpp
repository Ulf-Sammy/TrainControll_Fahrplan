#include "pch.h"
#include "CBlockInfo.h"

CBlock_Weiche::CBlock_Weiche()
{
	Antrieb_Nr = 0;
	Stellung = false;
}

CBlock_Weiche::CBlock_Weiche(CString InText, CPoint Step)
{
	CString Text = InText.Mid(8, 8);
	char Richtung = (char)InText[27];
	char Antrieb = (char)InText[17];
	Stellung = false;

	Antrieb_Nr = _ttoi(InText.Mid(4, 3));
	EinPos[0] = CPoint(_ttoi(InText.Mid(19, 3)), _ttoi(InText.Mid(23, 3)));
	TextPos   = CPoint(_ttoi(InText.Mid(31, 3)), _ttoi(InText.Mid(35, 3)));

	if (Text == "WEICH__L") Type = WeichenType::linksWeiche;
	if (Text == "WEICH__R") Type = WeichenType::rechtsWeiche;
	if (Text == "WEICH_DL")	Type = WeichenType::L_DoppelWeiche;
	if (Text == "WEICH_DR")	Type = WeichenType::R_DoppelWeiche;

	if (Type == WeichenType::rechtsWeiche)
	{
		if (Richtung == 'L')
		{
			MitPos[0] = EinPos[0] + CPoint(-1, 0);
			AusPos[0] = EinPos[0] + CPoint(-2, 0);
			AusPos[1] = EinPos[0] + CPoint(-2, -1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-2, -1));
		}
		if (Richtung == 'R')
		{
			MitPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[0] = EinPos[0] + CPoint(2, 0);
			AusPos[1] = EinPos[0] + CPoint(2, 1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(1, 0));
		}
		if (Richtung == 'U')
		{
			MitPos[0] = EinPos[0] + CPoint(0, 1);
			AusPos[0] = EinPos[0] + CPoint(0, 2);
			AusPos[1] = EinPos[0] + CPoint(-1, 2);
			KlickRechteck = CRect(0, 0, 1, 2);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-1, 0));
		}
		if (Richtung == 'O')
		{
			MitPos[0] = EinPos[0] + CPoint(0, -1);
			AusPos[0] = EinPos[0] + CPoint(0, -2);
			AusPos[1] = EinPos[0] + CPoint(1, -2);
			KlickRechteck = CRect(0, 0, 1, 2);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(0, -2));
		}
	}
	if (Type == WeichenType::linksWeiche)
	{
		if (Richtung == 'L')
		{
			MitPos[0] = EinPos[0] + CPoint(-1, 0);
			AusPos[0] = EinPos[0] + CPoint(-2, 0);
			AusPos[1] = EinPos[0] + CPoint(-2, 1);
			KlickRechteck = CRect(0, 0, 2, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-2, 0));
		}
		if (Richtung == 'R')
		{
			MitPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[0] = EinPos[0] + CPoint(2, 0);
			AusPos[1] = EinPos[0] + CPoint(2, -1);
			KlickRechteck = CRect(0, 0, 2, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(0, -1));
		}
		if (Richtung == 'U')
		{
			MitPos[0] = EinPos[0] + CPoint(0, 1);
			AusPos[0] = EinPos[0] + CPoint(0, 2);
			AusPos[1] = EinPos[0] + CPoint(1, 2);
			KlickRechteck = CRect(0, 0, 1, 2);
			KlickRechteck.OffsetRect(EinPos[0]);
		}
		if (Richtung == 'O')
		{
			MitPos[0] = EinPos[0] + CPoint(0, -1);
			AusPos[0] = EinPos[0] + CPoint(0, -2);
			AusPos[1] = EinPos[0] + CPoint(-1, -2);
			KlickRechteck = CRect(0, 0, 1, 2);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-1, -2));
		}
	}
	if (Type == WeichenType::R_DoppelWeiche)
	{
		if (Richtung == 'R')
		{
			EinPos[1] = EinPos[0];
			MitPos[0] = EinPos[0];
			AusPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[1] = EinPos[0] + CPoint(1, 1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(MitPos[0]);
		}
		if (Richtung == 'L')
		{
			EinPos[1] = EinPos[0];
			MitPos[0] = EinPos[0];
			AusPos[0] = EinPos[0] + CPoint(-1,  0);
			AusPos[1] = EinPos[0] + CPoint(-1, -1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(MitPos[0] + CPoint(-1, -1));
		}
	}
	if (Type == WeichenType::L_DoppelWeiche)
	{
		if (Richtung == 'R')
		{
			EinPos[1] = EinPos[0];
			MitPos[0] = EinPos[0];
			AusPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[1] = EinPos[0] + CPoint(1, -1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(MitPos[0] + CPoint( 0, -1));
		}
		if (Richtung == 'L')
		{
			EinPos[1] = EinPos[0];
			MitPos[0] = EinPos[0];
			AusPos[0] = EinPos[0] + CPoint(-1, 0);
			AusPos[1] = EinPos[0] + CPoint(-1, 1);
			KlickRechteck = CRect(0, 0, 1, 1);
			KlickRechteck.OffsetRect(MitPos[0] + CPoint(-1, 0));
		}
	}
	for (int i = 0; i < 2; i++)
	{
		EinPos[i].x = EinPos[i].x * Step.x;
		MitPos[i].x = MitPos[i].x * Step.x;
		AusPos[i].x = AusPos[i].x * Step.x;

		EinPos[i].y = EinPos[i].y * Step.y;
		MitPos[i].y = MitPos[i].y * Step.y;
		AusPos[i].y = AusPos[i].y * Step.y;
	}
	TextPos.x = TextPos.x * Step.x;
	TextPos.y = TextPos.y * Step.y;

	KlickRechteck.bottom = KlickRechteck.bottom * Step.y;
	KlickRechteck.top = KlickRechteck.top * Step.y;
	KlickRechteck.left = KlickRechteck.left * Step.x;
	KlickRechteck.right = KlickRechteck.right * Step.x;

}

void CBlock_Weiche::zeichneWeiche(CDC* pDC, CPen *Penstatus)
{
	CPen PenWhite_B(PS_SOLID, 5, RGB(255, 255, 255));

	byte S0, S1;
	if (Stellung)
	{
		S0 = 0;
		S1 = 1;
	}
	else
	{
		S0 = 1;
		S1 = 0;
	}
	pDC->SelectObject(&PenWhite_B);
	if ((Type == WeichenType::L_DoppelWeiche) ||(Type == WeichenType::R_DoppelWeiche))
	{ 
		pDC->MoveTo(MitPos[0]);
		pDC->LineTo(AusPos[S0]);
		pDC->SelectObject(Penstatus);
		pDC->MoveTo(MitPos[0]);
		pDC->LineTo(AusPos[S1]);
	}
	else
	{ // Weichen
		pDC->MoveTo(MitPos[0]);
		pDC->LineTo(AusPos[S0]);
		pDC->SelectObject(Penstatus);
		pDC->MoveTo(EinPos[0]);
		pDC->LineTo(MitPos[0]);
		pDC->LineTo(AusPos[S1]);
	}
}

bool CBlock_Weiche::klickedWeiche(CPoint KlickP)
{
	return KlickRechteck.PtInRect(KlickP);
}

bool CBlock_Weiche::SetWeiche(TrainCon_Paar WeichenDaten)
{
	if (WeichenDaten.GetWert() == Antrieb_Nr)
	{
		Stellung = WeichenDaten.GetBit();
		return true;
	}
	return false;
}


TrainCon_Paar CBlock_Weiche::Weichenantrieb()
{
	return TrainCon_Paar(Antrieb_Nr,Stellung);
}

CBlock_Weiche::~CBlock_Weiche()
{
}
//###############################################################################
//
//###############################################################################
CBlock_Strecke::CBlock_Strecke(){}

CBlock_Strecke::CBlock_Strecke(const CBlock_Strecke&){}

CBlock_Strecke::CBlock_Strecke(CString InText, CPoint Step)
{
	Setup(InText, Step);
}

bool CBlock_Strecke::empty()
{
	return StreckePunkte.empty();
}

void CBlock_Strecke::Setup(CString InText, CPoint Step)
{
	Relais_Nr = 0;
	Relais_Power = false;
	CString Text = InText.Mid(4, 7);
	Relais_Nr = _ttoi(InText.Mid(12, 2));
	char box = (char)InText[23]; // Richtung der Box
	GliesPos = (char)InText[27]; // Position der Beschriftung
	TextPos = CPoint(_ttoi(InText.Mid(16, 3)), _ttoi(InText.Mid(20, 3)));
	TextPos.x = TextPos.x * Step.x;
	TextPos.y = TextPos.y * Step.y;

	Gleis_Name = InText.Mid(29, 25);


	if (Text == "Abstell") Type = StreckenType::Abstellgleis;
	if (Text == "Strecke") Type = StreckenType::Strecke;
	if (Text == "Gleis  ") Type = StreckenType::Gleis;
	KlickRechteck = CRect(0, 0, 0, 0);
	if (InText.GetLength() > 56)
	{
		int Reads = (InText.GetLength() - 55) / 8;
		int x, y;
		for (int i = 0; i < Reads; i++)
		{
			x = (i * 8) + 56;
			y = (i * 8) + 60;
			x = _ttoi(InText.Mid(x, 3)) * Step.x;
			y = _ttoi(InText.Mid(y, 3)) * Step.y;
			StreckePunkte.push_back(CPoint(x, y));
		}
	}
	if ((Type == StreckenType::Gleis) || (Type == StreckenType::Abstellgleis))
	{
		if (box == '-')
		{
			KlickRechteck = CRect(TextPos + CPoint(70, -10), TextPos + CPoint(_ttoi(InText.Mid(24, 3)), 10));
			TextPos = TextPos + CPoint(74, -8);
			TextBes = TextPos + CPoint(0, 20);
			BeschriftungBlock.CreateFontIndirectW(&FontType_Ar_16_0);
			GleisBeschriftung.CreateFontIndirectW(&FontType_Ar_14_0);
		}
		if (box == '|')
		{
			KlickRechteck = CRect(TextPos + CPoint(-10, 65), TextPos + CPoint(10, _ttoi(InText.Mid(24, 3))));
			TextPos = TextPos + CPoint(8, 68);
			TextBes = TextPos + CPoint(-20, 0);
			BeschriftungBlock.CreateFontIndirectW(&FontType_Ar_14_270);
			GleisBeschriftung.CreateFontIndirectW(&FontType_Ar_11_270);
		}
	}

}

CBlock_Strecke::~CBlock_Strecke()
{
}

void CBlock_Strecke::zeichneStrecke(CDC* pDC, CPen* Penstatus, CString LokName)
{
	CPen Pen_SW(PS_SOLID,1,colorSchwarz);
	CPen Pen_PowerOn(PS_SOLID, 5, colorGelb);
	CPen Pen_PowerOFF(PS_SOLID, 5, colorRot);
	CBrush Brush_White(colorWeiss);

	byte i = 0;
	pDC->SelectObject(Penstatus);
	for (CPoint P : StreckePunkte)
	{
		if (i == 0) pDC->MoveTo(P);
		else pDC->LineTo(P);
		i++;
	}
	if ((Type == StreckenType::Gleis) || (Type == StreckenType::Abstellgleis))
	{ 
		if (Relais_Nr != 0)
		{
			if(Relais_Power) pDC->SelectObject(&Pen_PowerOn);
			else pDC->SelectObject(&Pen_PowerOFF);
			pDC->Rectangle(KlickRechteck);
		}
		pDC->SelectObject(&Brush_White);
		pDC->SelectObject(&Pen_SW);
		pDC->Rectangle(KlickRechteck);
		pDC->SelectObject(BeschriftungBlock);
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetBkColor(colorWeiss);
		pDC->TextOutW(TextPos.x, TextPos.y, LokName); // Lok in Block text
		pDC->SelectObject(GleisBeschriftung);
		pDC->SetBkColor(colorHinterGrund);
		pDC->TextOutW(TextBes.x, TextBes.y, Gleis_Name); // Beschreibung Gleis


	}
}

bool CBlock_Strecke::klickedGleis(CPoint KlickP)
{
	return KlickRechteck.PtInRect(KlickP);
}

bool CBlock_Strecke::schalteGleisRelais(TrainCon_Paar Relais)
{
	if (Relais_Nr == Relais.GetWert())
	{
		Relais_Power = Relais.GetBit();
		return true;
	}
	else return false;
}

TrainCon_Paar CBlock_Strecke::Relais()
{
	return TrainCon_Paar(Relais_Nr, Relais_Power);
}

void CBlock_Strecke::SetRelais(TrainCon_Paar Relais)
{
	Relais_Nr   = Relais.GetWert();
	Relais_Power = Relais.GetBit();
}
