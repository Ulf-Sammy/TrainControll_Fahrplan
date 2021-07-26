#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CBlockInfo.h"

CBlock_Weiche::CBlock_Weiche()
{
	Antrieb_Nr = 0;
}

CBlock_Weiche::CBlock_Weiche(CString InText, CPoint Step)
{
	CString Text = InText.Mid(8, 8);
	char Richtung = (char)InText[27];
	char Antrieb = (char)InText[17];
	
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

void CBlock_Weiche::zeichneWeiche(CDC* pDC, BlockStatus Bl_Status, std::vector<TrainCon_Paar>* Weichen)
{
	byte S0, S1;
	if (Weichen->at(Antrieb_Nr).GetBit())
	{
		S0 = 0;
		S1 = 1;
	}
	else
	{
		S0 = 1;
		S1 = 0;
	}
	pDC->SelectObject(&theApp.Gleis_Null);
	pDC->MoveTo(MitPos[0]);
	pDC->LineTo(AusPos[S0]);

	switch (Bl_Status)
	{
		case BlockStatus::Frei:
			pDC->SelectObject(&theApp.Gleis_Frei);
			break;
		case BlockStatus::Besetzt:
			pDC->SelectObject(&theApp.Gleis_Besetzt);
			break;
		case BlockStatus::BesetztError_A:
			pDC->SelectObject(&theApp.Gleis_ErrorA);
			break;
		case BlockStatus::BesetztError_B:
			pDC->SelectObject(&theApp.Gleis_ErrorB);
			break;
		default:
			pDC->SelectObject(&theApp.Gleis_Null);
			break;
	}	
	if ((Type == WeichenType::L_DoppelWeiche) || (Type == WeichenType::R_DoppelWeiche))
	{
		pDC->MoveTo(MitPos[0]);
	}
	else
	{
		pDC->MoveTo(EinPos[0]);
		pDC->LineTo(MitPos[0]);
	}
	pDC->LineTo(AusPos[S1]);
}

bool CBlock_Weiche::klickedWeiche(CPoint KlickP)
{
	return KlickRechteck.PtInRect(KlickP);
}

bool CBlock_Weiche::SetWeiche(TrainCon_Paar WeichenDaten)
{
	if (WeichenDaten.GetWert() == Antrieb_Nr) {	return true;}
	return false;
}


byte CBlock_Weiche::Weichenantrieb()
{
	return Antrieb_Nr;
}

CBlock_Weiche::~CBlock_Weiche()
{
}
//###############################################################################
//
//###############################################################################
CBlock_Strecke::CBlock_Strecke()
{
	CBlock_Strecke::Relais_Nr = 0;
	CBlock_Strecke::Relais_Power = 0;
}

CBlock_Strecke::CBlock_Strecke(const CBlock_Strecke&)
{
	CBlock_Strecke::Relais_Nr = 0;
	CBlock_Strecke::Relais_Power = 0;
}

CBlock_Strecke::CBlock_Strecke(CString InText, CPoint Step)
{
	CBlock_Strecke::Relais_Nr = 0;
	CBlock_Strecke::Relais_Power = 0;
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
	box = (char)InText[23]; // Richtung der Box
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
		if (box == '-') // 0°
		{
			KlickRechteck = CRect(TextPos + CPoint(70, -10), TextPos + CPoint(_ttoi(InText.Mid(24, 3)), 10));
			TextPos = TextPos + CPoint(74, -8);
			TextBes = TextPos + CPoint(0, 20);
		}
		if (box == '|') // 270°
		{
			KlickRechteck = CRect(TextPos + CPoint(-10, 65), TextPos + CPoint(10, _ttoi(InText.Mid(24, 3))));
			TextPos = TextPos + CPoint(8, 68);
			TextBes = TextPos + CPoint(-20, 0);
		}
	}

}

CBlock_Strecke::~CBlock_Strecke()
{
}

void CBlock_Strecke::zeichneStrecke(CDC* pDC, BlockStatus Bl_Status, CString LokName, bool LokDir)
{
	CPen Pen_PowerOn(PS_SOLID, 5, colorGelb);
	CPen Pen_PowerOFF(PS_SOLID, 5, colorRot);

	byte i = 0;
	switch (Bl_Status)
	{
	case BlockStatus::Frei:
		pDC->SelectObject(theApp.Gleis_Frei);
		break;
	case BlockStatus::Besetzt:
		pDC->SelectObject(theApp.Gleis_Besetzt);
		break;
	case BlockStatus::BesetztError_A:
		pDC->SelectObject(theApp.Gleis_ErrorA);
		break;
	case BlockStatus::BesetztError_B:
		pDC->SelectObject(theApp.Gleis_ErrorB);
		break;
	default:
		pDC->SelectObject(theApp.Gleis_Null);
		break;
	}
	for (CPoint P : StreckePunkte)
	{
		if (i == 0) pDC->MoveTo(P);
		else pDC->LineTo(P);
		i++;
	}
	if ((Type == StreckenType::Gleis) || (Type == StreckenType::Abstellgleis))
	{ 
		CPoint LokPosH = TextBes;
		CPoint LokPosV = TextBes;
		
		if (Relais_Nr != 0)
		{
			if(Relais_Power) pDC->SelectObject(&Pen_PowerOn);
			else pDC->SelectObject(&Pen_PowerOFF);
			pDC->Rectangle(KlickRechteck);
		}
		pDC->SelectObject(&theApp.Brush_White);
		pDC->SelectObject(&theApp.Stift_SW_1);
		pDC->Rectangle(KlickRechteck);
		if (box == '|') 	pDC->SelectObject(&theApp.BeschriftungBlock_270);
		if (box == '-')     pDC->SelectObject(&theApp.BeschriftungBlock_0);
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetBkColor(GleisWeiss);
		pDC->TextOutW(TextPos.x, TextPos.y, LokName); // Lok in Block text
		if (box == '|') 	pDC->SelectObject(&theApp.GleisBeschriftung_270);
		if (box == '-')     pDC->SelectObject(&theApp.GleisBeschriftung_0);
		pDC->SetBkColor(colorHinterGrund);
		pDC->TextOutW(TextBes.x, TextBes.y, Gleis_Name); // Beschreibung Gleis
		if (!LokName.IsEmpty())
		{
			if (box == '-')
			{
				if (TextBes.y > 400)
				{
					if (LokDir)
					{
						LokPosH.Offset(CPoint(75, -50));
						if(Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_ge_Uhr.DrawTransparent(pDC, LokPosH.x, LokPosH.y, RGB(255, 255, 255));
					}
					else
					{
						LokPosH.Offset(CPoint(-52, -50));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_in_Uhr.DrawTransparent(pDC, LokPosH.x, LokPosH.y, RGB(255, 255, 255));
					}
				}
				else
				{
					if (LokDir)
					{
						LokPosH.Offset(CPoint(-52, -50));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_in_Uhr.DrawTransparent(pDC, LokPosH.x, LokPosH.y, RGB(255, 255, 255));
					}
					else
					{
						LokPosH.Offset(CPoint(75, -50));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_ge_Uhr.DrawTransparent(pDC, LokPosH.x, LokPosH.y, RGB(255, 255, 255));
					}
			}
			}
			if (box == '|')
			{
				if (TextBes.x > 800)
				{
					if (LokDir)
					{
						LokPosV.Offset(CPoint(-25, -52));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_Hoch.DrawTransparent(pDC, LokPosV.x, LokPosV.y, RGB(255, 255, 255));
					}
					else
					{
						LokPosV.Offset(CPoint(-25, 82));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_Runter.DrawTransparent(pDC, LokPosV.x, LokPosV.y, RGB(255, 255, 255));
					}
				}
				else
				{
					if (LokDir)
					{
						LokPosV.Offset(CPoint(-25, +81));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_Runter.DrawTransparent(pDC, LokPosV.x, LokPosV.y, RGB(255, 255, 255));
					}
					else
					{
						LokPosV.Offset(CPoint(-25, -52));
						if (Bl_Status == BlockStatus::Besetzt)
							theApp.Lok_Hoch.DrawTransparent(pDC, LokPosV.x, LokPosV.y, RGB(255, 255, 255));
					}
				}
			}
		}


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

byte CBlock_Strecke::Get_Relais_Nr()
{
	return Relais_Nr;
}

void CBlock_Strecke::SetRelais(TrainCon_Paar Relais)
{
	Relais_Nr   = Relais.GetWert();
	Relais_Power = Relais.GetBit();
}

void CBlock_Strecke::GetDebugData(BlockDebugData *Data)
{
	Data->Gleis_Name = Gleis_Name;
	Data->Relais_Nr = Relais_Nr;
	Data->Relais_Power = Relais_Power;
}

CBlock_Weg::CBlock_Weg()
{
	Block_von = 0;
	Block_nach = 0;
	Block_zwich = 0;
}

CBlock_Weg::CBlock_Weg(CString InText)
{
	Block_zwich = 0;
	Block_von = _ttoi(InText.Mid(0,2));
	Block_nach = _ttoi(InText.Mid(10, 2));
	
	int Reads = (InText.GetLength() - 10) / 4;
	int i = 0; 
	if ('B' == InText[InText.GetLength() - 3])
	{
		Block_zwich = _ttoi(InText.Mid(InText.GetLength() - 2, 2));
		Reads--;
	}
	do
	{
		WeichenWeg.push_back(TrainCon_Paar(InText.Mid((i * 4 + 13), 3)));
		i++;
	} while (Reads != i);
}

void CBlock_Weg::WechselRichtung()
{
	byte tmp = Block_nach;
	Block_nach = Block_von;
	Block_von = tmp;
}

bool CBlock_Weg::isvonBlock(byte EBlock,byte ABlock)
{
	return ((EBlock == Block_von) && (ABlock == Block_nach));
}

bool CBlock_Weg::isWeg_geschaltet(std::vector<TrainCon_Paar>* Weichen)
{
	bool result = true;
	bool bit;

	for (auto N : WeichenWeg)
	{
		bit = (N.GetBit() == Weichen->at(N.GetWert()).GetBit());
		result = result && bit;
	}
	return result;
}

bool CBlock_Weg::isWeg_nachgeschaltet(byte Block, std::vector<TrainCon_Paar>* Weichen)
{
	if (isWeg_geschaltet(Weichen))
	{
		if (Block == Block_von) return true;
		if (Block == Block_nach) return true;
	}
	return false;
}

void CBlock_Weg::Blocksaufweg(std::vector<byte>* Block)
{
	if (Block_zwich != 0) Block->push_back(Block_zwich);
	Block->push_back(Block_nach);
}


byte CBlock_Weg::VonBlock()
{
	return Block_von;
}

byte CBlock_Weg::ZwiBlock()
{
	return Block_zwich;
}

byte CBlock_Weg::NachBlock()
{
	return Block_nach;
}
