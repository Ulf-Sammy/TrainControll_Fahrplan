#include "pch.h"
#include "CInfoBlock.h"

CInfoBlock::CInfoBlock()
{
}

CInfoBlock::CInfoBlock(CString InText)
{
	static bool NextAntrieb = false;
	static byte erstAntreib = 0;
	Besetzt_Zug = NULL;
	WeichenStellung = 0;
	CString Text = InText.Mid(4, 6);
	Nr = _ttoi(InText.Mid(0, 3));
	TextPos = CPoint(_ttoi(InText.Mid(45, 3)), _ttoi(InText.Mid(49, 3)));

	if ((Text == "Block ") || (Text == "Gleis "))
	{
		if (Text == "Block ") Block_Type = BlockType::isBlock;
		if (Text == "Gleis ") Block_Type = BlockType::isGleis;
		EingangBlock[0]  = _ttoi(InText.Mid(55, 3));
		AusgangBlock[0]  = _ttoi(InText.Mid(59, 3));
		EingangMelder[0] = _ttoi(InText.Mid(63, 3));
		AusgangMelder[0] = _ttoi(InText.Mid(67, 3));
		
		if (Block_Type == BlockType::isGleis)
		{
			Name = InText.Mid(72, 25);
			GliesPos =  (char)InText[98];
		}
		EinPos[0] = CPoint(_ttoi(InText.Mid(11, 3)), _ttoi(InText.Mid(15, 3)));
		MitPos[0] = CPoint(_ttoi(InText.Mid(19, 3)), _ttoi(InText.Mid(23, 3))); // MitPos
		MitPos[1] = CPoint(_ttoi(InText.Mid(27, 3)), _ttoi(InText.Mid(31, 3))); //MitPos
		AusPos[0] = CPoint(_ttoi(InText.Mid(35, 3)), _ttoi(InText.Mid(39, 3)));
	}
	if (Text == "Weiche")
	{
		Block_Type = BlockType::isWeiche;

		Text = InText.Mid(21, 8);
		if (Text == "WEICH__L") Weiche_Type = WeichenType::linksWeiche;
		if (Text == "WEICH__R") Weiche_Type = WeichenType::rechtsWeiche;
		if (Text == "WEICH_DO")	Weiche_Type = WeichenType::DoppelWeiche;
		Richtung = (char)InText[19];
		WeichenGruppe    = _ttoi(InText.Mid(39, 3));
		EingangBlock[0]  = _ttoi(InText.Mid(55, 3));
		EingangBlock[1]  = _ttoi(InText.Mid(71, 3));
		EingangMelder[0] = _ttoi(InText.Mid(63, 3));
		EingangMelder[1] = _ttoi(InText.Mid(79, 3));

		AusgangBlock[0]  = _ttoi(InText.Mid(59, 3));
		AusgangBlock[1]  = _ttoi(InText.Mid(75, 3));
		AusgangMelder[0] = _ttoi(InText.Mid(67, 3));
		AusgangMelder[1] = _ttoi(InText.Mid(83, 3));

		EinPos[0] = CPoint(_ttoi(InText.Mid(11, 3)), _ttoi(InText.Mid(15, 3)));

	}
}

BlockInfo CInfoBlock::Get_BlockInfo()
{
	BlockInfo Data;
	Data.Nr = Nr;
	Data.AusgangBlock[0] = AusgangBlock[0];
	Data.AusgangBlock[1] = AusgangBlock[1];
	Data.EingangBlock[0] = EingangBlock[0];
	Data.EingangBlock[1] = EingangBlock[1];
	Data.EingangMelder[0] = EingangMelder[0];
	Data.EingangMelder[1] = EingangMelder[1];
	Data.AusgangMelder[0] = AusgangMelder[0];
	Data.AusgangMelder[1] = AusgangMelder[1];
	Data.Block_Type = Block_Type;
	Data.Weiche_Type = Weiche_Type;
	Data.WeichenStellung = WeichenStellung;
	return Data; 
}

void CInfoBlock::Set_ZweitenAntrieb(byte Nr)
{
	DoppelWecheAntrieb_2 = Nr;
}

void CInfoBlock::SetTaster(CString InText, CPoint Step)
{
	int x = _ttoi(InText.Mid(15, 3)) * Step.x;
	int y = _ttoi(InText.Mid(19, 3)) * Step.y;
	if ('E' == (char)InText[11])
	{
		EingangTaster.Pos = CPoint(x, y);
		EingangTaster.Showit = true;
		EingangTaster.Color = (char)InText[13];
		EingangTaster.SkaliereDaten((char)InText[12]);
	}

	if ('A' == (char)InText[11])
	{
		AusgangTaster.Pos = CPoint(x,y);
		AusgangTaster.Showit = true;
		AusgangTaster.Color = (char)InText[13];
		AusgangTaster.SkaliereDaten((char)InText[12]);
	}
}

void CInfoBlock::Calac_Pos()
{
	if (Weiche_Type == WeichenType::rechtsWeiche)
	{
		if (Richtung == 'L')
		{
			MitPos[0] = EinPos[0] + CPoint(-1, 0);
			AusPos[0] = EinPos[0] + CPoint(-2, 0);
			AusPos[1] = EinPos[0] + CPoint(-2, -1);
			KlickRechteck = CRect(0, 0, 2, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-2, -1));
		}
		if (Richtung == 'R')
		{
			MitPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[0] = EinPos[0] + CPoint(2, 0);
			AusPos[1] = EinPos[0] + CPoint(2, 1);
			KlickRechteck = CRect(0, 0, 2, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(0, 0));
		}
		if (Richtung == 'U')
		{
			MitPos[0] = EinPos[0] + CPoint( 0, 1);
			AusPos[0] = EinPos[0] + CPoint( 0, 2);
			AusPos[1] = EinPos[0] + CPoint(-1, 2);
			KlickRechteck = CRect(0, 0, 1, 2);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-1 , 0));
		}
		if (Richtung == 'O')
		{
			MitPos[0] = EinPos[0] + CPoint(0, -1);
			AusPos[0] = EinPos[0] + CPoint(0, -2);
			AusPos[1] = EinPos[0] + CPoint(1, -2);
			KlickRechteck = CRect(0, 0, 1 , 2 );
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(0 , -2 ));
		}
	}
	if (Weiche_Type == WeichenType::linksWeiche)
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
			MitPos[0] = EinPos[0] + CPoint( 1,  0);
			AusPos[0] = EinPos[0] + CPoint( 2,  0);
			AusPos[1] = EinPos[0] + CPoint( 2, -1);
			KlickRechteck = CRect(0, 0, 2, 1);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(0, -1));
		}
		if (Richtung == 'U')
		{
			MitPos[0] = EinPos[0] + CPoint(0, 1);
			AusPos[0] = EinPos[0] + CPoint(0, 2);
			AusPos[1] = EinPos[0] + CPoint(1, 2);
			KlickRechteck = CRect(0, 0, 1 , 2 );
			KlickRechteck.OffsetRect(EinPos[0]);
		}
		if (Richtung == 'O')
		{
			MitPos[0] = EinPos[0] + CPoint( 0, -1);
			AusPos[0] = EinPos[0] + CPoint( 0, -2);
			AusPos[1] = EinPos[0] + CPoint(-1, -2);
			KlickRechteck = CRect(0, 0, 1 , 2);
			KlickRechteck.OffsetRect(EinPos[0] + CPoint(-1,-2));
		}
	}
	if (Weiche_Type == WeichenType::DoppelWeiche)
	{// Hier brauche ich nur links :-)
		KlickRechteck = CRect(0, 0, 1, 2);
		if (Richtung == 'R')
		{
			EinPos[1] = EinPos[0] + CPoint(0, -1);
			MitPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[0] = EinPos[0] + CPoint(2, 0);
			AusPos[1] = EinPos[0] + CPoint(2, 1);
			if (Nr > DoppelWecheAntrieb_2)			
				KlickRechteck.OffsetRect(MitPos[0] + CPoint(-1, -1));
			if (Nr < DoppelWecheAntrieb_2)			
				KlickRechteck.OffsetRect(MitPos[0] + CPoint( 0, -1));
		}
		if (Richtung == 'L')
		{
			EinPos[1] = EinPos[0] + CPoint(0, 1);
		    MitPos[0] = EinPos[0] + CPoint(1, 0);
			AusPos[0] = EinPos[0] + CPoint(2, 0);
			AusPos[1] = EinPos[0] + CPoint(2,-1);
			if (Nr > DoppelWecheAntrieb_2)			
				KlickRechteck.OffsetRect(MitPos[0] + CPoint(-1, -1));
			if (Nr < DoppelWecheAntrieb_2)			
				KlickRechteck.OffsetRect(MitPos[0] + CPoint(0, -1));

		}
	}
}

byte CInfoBlock::GetNextBlock_Hand(bool Fahrrichtung)
{
	byte NextBlock;
	bool S = 0;
	if (Block_Type == BlockType::isWeiche) S = WeichenStellung;
	if (Fahrrichtung)
		NextBlock = AusgangBlock[S];
	else
		NextBlock = EingangBlock[S];
	return(NextBlock);
}
byte CInfoBlock::GetNextBlock_Auto(bool Fahrrichtung, bool S)
{
	byte NextBlock;
	if ((Block_Type == BlockType::isWeiche) || (Block_Type == BlockType::isBlock)) S = 0;
	if (Fahrrichtung)
		NextBlock = AusgangBlock[S];
	else
		NextBlock = EingangBlock[S];
	return(NextBlock);
}

CRect CInfoBlock::GetKlickField()
{
	return KlickRechteck;
}




CInfoBlock::~CInfoBlock()
{
	
}

void CInfoBlock::SkaliereDaten(CPoint Step)
{
	Calac_Pos();
	EinPos[0].x = EinPos[0].x * Step.x;
	EinPos[1].x = EinPos[1].x * Step.x;
	MitPos[0].x = MitPos[0].x * Step.x;
	MitPos[1].x = MitPos[1].x * Step.x;
	AusPos[0].x = AusPos[0].x * Step.x;
	AusPos[1].x = AusPos[1].x * Step.x;
	TextPos.x   = TextPos.x   * Step.x;

	EinPos[0].y = EinPos[0].y * Step.y;
	EinPos[1].y = EinPos[1].y * Step.y;
	MitPos[0].y	= MitPos[0].y * Step.y;
	MitPos[1].y = MitPos[1].y * Step.y;
	AusPos[0].y = AusPos[0].y * Step.y;
	AusPos[1].y = AusPos[1].y * Step.y;
	TextPos.y   = TextPos.y   * Step.y;

	KlickRechteck.bottom = KlickRechteck.bottom * Step.y;
	KlickRechteck.top    = KlickRechteck.top    * Step.y;
	KlickRechteck.left   = KlickRechteck.left   * Step.x;
	KlickRechteck.right  = KlickRechteck.right  * Step.x;
}
