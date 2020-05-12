#include "pch.h"
#include ".\3dmetergleis.h"
#include "math.h"
#include "resource.h"

UINT Thread_Update_Gleis(LPVOID pParam)
{
	C3DMeterGleis* Info_Gleis = (C3DMeterGleis*)pParam;
	Sleep(500);
	do
	{
		 if (Info_Gleis->DataBlock->isNewUpdate_Gleis()) 	 Info_Gleis->Invalidate();
		 if (Info_Gleis->DataBlock->isNewUpdate_Taster()) 	 Info_Gleis->Invalidate();
	} while (Info_Gleis->RunThread);
	return 0;
}


C3DMeterGleis::C3DMeterGleis(void)
{
	Zeige[Zeichne_Gitter] = true;
	Zeige[Zeichne_Block_Nr] = false;
	Zeige[Zeichne_Gleis_Nr] = true;
	Zeige[Zeichne_Weichen_Nr] = true;
	Zeige[Zeichne_Melder_Nr] = true;
	Zeige[Zeichne_Tasten_Nr] = false; 

	
	Pen_Gr.CreatePen(PS_SOLID, 5, RGB(  6,233,    13)); // Grün
	Pen_Ro.CreatePen(PS_SOLID, 5, RGB(225,  24,   32)); // Rot
	Pen_hR.CreatePen(PS_SOLID, 5, RGB(243, 141,  147)); // hell Rot
	Pen_Or.CreatePen(PS_SOLID, 5, RGB(232, 104,   13)); // Orange
	Pen_Ge.CreatePen(PS_SOLID, 5, RGB(255, 242,    0)); // Gelb
	Pen_Ga.CreatePen(PS_SOLID, 5, RGB(155,155,155)); // Grau
	Pen_White.CreatePen(PS_SOLID,5,RGB(255,255,255)); 
	Pen_Bl.CreatePen    (PS_SOLID,5,RGB( 42, 13,232)); // Blau
	Pen_SW.CreatePen    (PS_SOLID,1,RGB( 0,  0,  0)); // Schwarz
	Pen_Melder.CreatePen(PS_SOLID,2,RGB( 0,  0,  0)) ;
	Pen_Taster.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	Brush_Hinterg.CreateSolidBrush(colorHinterGrund);
	Brush_Weiche.CreateSolidBrush(colorHinterGrund);
	Brush_Melder_O.CreateSolidBrush(RGB(255,217,179)) ;
	Brush_Melder_I.CreateSolidBrush(RGB(255,  0,  0)) ;
	Brush_White.CreateSolidBrush(RGB(255,255,255));
	Brush_Yellow.CreateSolidBrush(RGB(255, 255, 0));
	Brush_Red.CreateSolidBrush(RGB(255, 0, 0));
	Brush_Green.CreateSolidBrush(RGB(0, 140, 0));



	Font_Info.CreateFont(15, 0, 0, 0, 400,
						 FALSE, FALSE, 0, ANSI_CHARSET,
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						 DEFAULT_QUALITY, 
						 DEFAULT_PITCH|FF_SWISS, _T("Arial Narrow")) ;
	Font_Block_0.CreateFont(16, 0, 0, 0, 400,
						 FALSE, FALSE, 0, ANSI_CHARSET,
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
						 DEFAULT_PITCH|FF_SWISS, _T("Arial Narrow")) ;
	Font_Block_90.CreateFont(14, 0, 2700, 900, 200,
						FALSE, FALSE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow"));
	Font_Gleis_Info_0.CreateFont(14, 0, 0, 0, 400,
						FALSE, FALSE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow"));
	Font_Gleis_Info_90.CreateFont(11, 0, 2700, 0, 400,
						FALSE, FALSE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial Narrow"));
	Lok_in_Uhr = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITLOK_MUHR));
	Lok_ge_Uhr = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITLOK_GUHR));
	Lok_Hoch   = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITLOK_HOCH));
	Lok_Runter = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITLOK_RUNTER));
}
//
C3DMeterGleis::~C3DMeterGleis(void)
{
	RunThread = false;
	Sleep(500);
}

//
BEGIN_MESSAGE_MAP(C3DMeterGleis, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void C3DMeterGleis::OnInitDialog(CGleisPlan *pPlan)
{
	DataPlan = pPlan;
	DataBlock = &pPlan->Block_Data;
	RunThread = true;
	AfxBeginThread(Thread_Update_Gleis, this);
}

void C3DMeterGleis::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	ReconstructControl() ;
}

void C3DMeterGleis::OnPaint(void)
{
	CPaintDC dc(this); 
	GetClientRect (&m_rectCtrl) ;
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;
	CMem_DC Test_DC(&dc, &m_rectCtrl);

	if ((m_dcBackground.GetSafeHdc() == NULL) || (m_bitmapBackground.m_hObject == NULL))
	{
		m_dcBackground.CreateCompatibleDC(&dc) ;
		m_bitmapBackground.CreateCompatibleBitmap(&dc,m_rectCtrl.Width(), m_rectCtrl.Height()) ;
		m_pBitmapOldBackground = m_dcBackground.SelectObject(&m_bitmapBackground) ;
		ZeichenHintergrund(&m_dcBackground, m_rectCtrl) ;				
	}
	// drop in the background
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),&m_dcBackground, 0, 0, SRCCOPY) ;
	
	
	Pen_Old   = pDC->SelectObject(&Pen_SW);
	Brush_Old = pDC->SelectObject(&Brush_Weiche) ;
	Font_Old  = pDC->SelectObject(&Font_Block_0) ;
	//ZeichenTest();

	for(int i = 1; i < (DataBlock->GetAnzahl_Block()); i++)
	{
		switch(DataBlock->Get_BlockType(i))
		{
			case BlockType::isWeiche:
				ZeichneWeiche(i);
			    break;
			case BlockType::isBlock:
			case BlockType::isGleis:
				ZeichneBlock(i);
				ZeicheLok(&Test_DC, i);
				pDC->SelectObject(&Pen_Taster);
				ZeicheTaster(i);
				break;
		}

	}
	pDC->SelectObject(&Pen_Melder);
	for(int i = 0; i < DataBlock->GetAnzahl_Melder(); i++)
	{
		ZeicheMelder(i);
	}

	pDC->SelectObject(&Pen_Old);
	pDC->SelectObject(&Brush_Old);
	pDC->SelectObject(&Font_Old) ;
	DataPlan->Set_Startup(true);
}
void C3DMeterGleis::ReconstructControl()
{
	if ((m_pBitmapOldBackground) && (m_bitmapBackground.GetSafeHandle()) && (m_dcBackground.GetSafeHdc()))
	{
			m_dcBackground.SelectObject(m_pBitmapOldBackground);
			m_dcBackground.DeleteDC() ;
			m_bitmapBackground.DeleteObject();
	}
	Invalidate () ;
}
void C3DMeterGleis::OnLButtonDown(UINT nFlags, CPoint point)
{
	DataBlock->KlickTasterSchalten(point);
	DataBlock->SchalteWeiche(point);
	CStatic::OnLButtonDown(nFlags, point);
}

void C3DMeterGleis::ZeichenHintergrund(CDC * pDC_H, CRect &rect)
{
   
	COLORREF Back_Pen = RGB(115, 115, 115);
	CPen	 Pen_Back  ;
	
	CBrush*  pOldBrush ;
	CPen*	 pOldPen   ;
	CFont*	 pOldFont  ;
	CString  Text;
	CPoint   P;
	int X_Lang = 80;
	int Y_Lang = 30;
   //Brush_Back.CreateSolidBrush(colorHinterGrund);
   Pen_Back.CreatePen(PS_SOLID, 1,Back_Pen);

   pOldBrush = pDC_H->SelectObject(&Brush_Hinterg);
   pOldPen   = pDC_H->SelectObject(&Pen_Back);
   pOldFont  = pDC_H->SelectObject(&Font_Info) ;
   pDC_H->Rectangle(rect);
   if (Zeige[Zeichne_Gitter])
   {
	   pDC_H->SetBkColor(colorHinterGrund);
	   pDC_H->SetTextColor(Back_Pen);
	   for(int x=0;x < X_Lang ;x++)
		{
			Text.Format(_T("%i"),x);
			P = CPoint(x* Step.x , 1);
			pDC_H->TextOut(P.x +5 , m_rectCtrl.top       ,Text);
			pDC_H->TextOut(P.x +5 , m_rectCtrl.bottom-Y_Lang ,Text);
			pDC_H->MoveTo( P.x    , m_rectCtrl.top    );
			pDC_H->LineTo( P.x    , m_rectCtrl.bottom);
		}
		for(int y=0;y < Y_Lang ;y++)
		{
			P = CPoint(1, y* Step.y);
			Text.Format(_T("%i"),y);
			pDC_H->TextOut(m_rectCtrl.left ,P.y , Text);
			pDC_H->MoveTo( m_rectCtrl.left ,P.y );
			pDC_H->LineTo( m_rectCtrl.right,P.y );
		}
   }
   pDC_H->SelectObject(pOldBrush);
   pDC_H->SelectObject(pOldPen);
   pDC_H->SelectObject(pOldFont);
}
void C3DMeterGleis::ZeichneBlock(byte Nr)
{
	CString LokTexT;
	CString GleisText;
	int x_txt_B , y_txt_B, x_txt_G, y_txt_G;
	CRect GleisFeld;
	CPoint Prellbock[2];
	SetzeBlockFarbe(Nr);

	pDC->MoveTo(DataBlock->GetWerte_Block(Nr).EinPos[0]);
	pDC->LineTo(DataBlock->GetWerte_Block(Nr).MitPos[0]);
	pDC->LineTo(DataBlock->GetWerte_Block(Nr).MitPos[1]);
	pDC->LineTo(DataBlock->GetWerte_Block(Nr).AusPos[0]);

	if (DataBlock->GetWerte_Block(Nr).EingangBlock[0] == 0xFF)
	{
		if (DataBlock->GetWerte_Block(Nr).EinPos[0].x == DataBlock->GetWerte_Block(Nr).AusPos[0].x)
		{
			Prellbock[0] = CPoint(-3, 0);
			Prellbock[1] = CPoint(+3, 0);
		}
		if (DataBlock->GetWerte_Block(Nr).EinPos[0].y == DataBlock->GetWerte_Block(Nr).AusPos[0].y)
		{
			Prellbock[0] = CPoint(0, -3);
			Prellbock[1] = CPoint(0, +3);
		}

		pDC->MoveTo(DataBlock->GetWerte_Block(Nr).EinPos[0] + Prellbock[0]);
		pDC->LineTo(DataBlock->GetWerte_Block(Nr).EinPos[0] + Prellbock[1]);
	}

	if (DataBlock->GetWerte_Block(Nr).AusgangBlock[0] == 0xFF)
	{
		if (DataBlock->GetWerte_Block(Nr).MitPos[1].x == DataBlock->GetWerte_Block(Nr).AusPos[0].x)
		{
			Prellbock[0] = CPoint(-3, 0);
			Prellbock[1] = CPoint(+3, 0);
		}
		if (DataBlock->GetWerte_Block(Nr).MitPos[1].y == DataBlock->GetWerte_Block(Nr).AusPos[0].y)
		{
			Prellbock[0] = CPoint(0, -3);
			Prellbock[1] = CPoint(0, +3);
		}

		pDC->MoveTo(DataBlock->GetWerte_Block(Nr).AusPos[0] + Prellbock[0]);
		pDC->LineTo(DataBlock->GetWerte_Block(Nr).AusPos[0] + Prellbock[1]);
	}
	// Das Gleis Feld Zeichnen
	if (DataBlock->GetWerte_Block(Nr).Block_Type == BlockType::isGleis)
	{
		CFont* Block = NULL;
		CFont* InfoGleis = NULL;
		GleisText = DataBlock->Get_Gleis_Name(Nr);
		if (DataBlock->ist_besetzt(Nr))
			LokTexT = DataBlock->GetZugName_in_Block(Nr);
			
		else
			LokTexT = _T("-");
		// Glies von Links nach Rechts
		if (DataBlock->GetWerte_Block(Nr).MitPos[0].y == DataBlock->GetWerte_Block(Nr).MitPos[1].y)
		{
			Block = &Font_Block_0;
			InfoGleis = &Font_Gleis_Info_0;
			GleisFeld.SetRect((DataBlock->GetWerte_Block(Nr).TextPos +CPoint(70, -10)), (DataBlock->GetWerte_Block(Nr).TextPos + CPoint(180, 10)));
			x_txt_B = DataBlock->GetWerte_Block(Nr).TextPos.x + 74;
			y_txt_B = DataBlock->GetWerte_Block(Nr).TextPos.y - 8;
			x_txt_G = x_txt_B;
			y_txt_G = y_txt_B - 18;
		}
		// Gleis von Oben nach unten
		if (DataBlock->GetWerte_Block(Nr).MitPos[0].x == DataBlock->GetWerte_Block(Nr).MitPos[1].x)
		{
			Block = &Font_Block_90;
			InfoGleis = &Font_Gleis_Info_90;
			GleisFeld.SetRect((DataBlock->GetWerte_Block(Nr).TextPos + CPoint(-10, 65)), (DataBlock->GetWerte_Block(Nr).TextPos + CPoint(10,180)));
			x_txt_B = DataBlock->GetWerte_Block(Nr).TextPos.x + 8;
			y_txt_B = DataBlock->GetWerte_Block(Nr).TextPos.y + 68;
			x_txt_G = x_txt_B + 16;
			y_txt_G = y_txt_B;
		}
		pDC->SelectObject(&Pen_SW);
		pDC->SelectObject(&Brush_White);
		pDC->SelectObject(Block);
		// Zug in Block schreiben
		pDC->Rectangle(GleisFeld);
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetBkColor(colorWeiß);
		pDC->TextOutW(x_txt_B, y_txt_B, LokTexT);
		// Gleisbeschriften
		pDC->SelectObject(InfoGleis);
		pDC->SetBkColor(colorHinterGrund);
		pDC->TextOutW(x_txt_G, y_txt_G, GleisText); // Beschreibung Gleis
		ZeicheAchteck(DataBlock->GetWerte_Block(Nr).TextPos, Nr, DataBlock->GetWerte_Block(Nr).Block_Type);
	}
}

void C3DMeterGleis::ZeichneWeiche(byte Nr)
{
	bool S = DataBlock->Get_Stellung_Weiche(Nr);
	//pDC->SelectObject(&Pen_SW);
    //pDC->Rectangle(DataBlock->Get_Weichen_Feld(i));

	// Zeiche erst leere Strecke
	pDC->SelectObject(&Pen_White);
	pDC->MoveTo(DataBlock->Get_Weiche_MitPos(Nr));   
	pDC->LineTo(DataBlock->Get_Weiche_AusPos(Nr, !S));

	SetzeBlockFarbe(Nr);
	pDC->MoveTo(DataBlock->Get_Weiche_EinPos(Nr));  
	pDC->LineTo(DataBlock->Get_Weiche_MitPos(Nr));  
	pDC->LineTo(DataBlock->Get_Weiche_AusPos(Nr,S));
	pDC->SelectObject(&Pen_Ro);
	ZeicheAchteck(DataBlock->Get_Weiche_TexPos(Nr), Nr, DataBlock->GetWerte_Block(Nr).Block_Type);
}

void C3DMeterGleis::ZeicheMelder(byte Nr)
{
	CString TextNr, TextInfo;
	CRect Kreis;
	if (DataBlock->Show_Melder(Nr))
	{
		pDC->SetTextAlign(TA_LEFT | TA_CENTER);

		Kreis = CRect(-6, -6, 6, 6);
		Kreis.OffsetRect(DataBlock->Get_Melder_Position(Nr));
		if (DataBlock->Get_Melder(Nr))
		{
			pDC->SelectObject(&Brush_Melder_I);
		}
		else
		{
			pDC->SelectObject(&Brush_Melder_O);
		}
		pDC->Ellipse(Kreis);
		if (Zeige[Zeichne_Melder_Nr])
		{
			pDC->SetBkColor(colorHinterGrund);
			TextNr.Format(_T("%3i"), Nr);
			pDC->TextOut(DataBlock->Get_Melder_PositionText(Nr).x, DataBlock->Get_Melder_PositionText(Nr).y, TextNr);
		}
	}
}

void C3DMeterGleis::ZeicheTaster(byte Block_Nr)
{
	CString TextNr, TextInfo;
	for (int i = 0; i < 2; i++)
	{
		if (DataBlock->Show_Taster(Block_Nr, (bool) i))
		{
			CBrush Bneu;

			switch (DataBlock->GetCol_Taster(Block_Nr,(bool) i))
			{
			case 'W':
				pDC->SelectObject(&Brush_White);
				break;
			case 'Y':
				pDC->SelectObject(&Brush_Yellow);
				break;
			case 'R':
				pDC->SelectObject(&Brush_Red);
				break;
			case 'G':
				pDC->SelectObject(&Brush_Green);
				break;
			default:
				Bneu.CreateSolidBrush(colorHinterGrund);
				pDC->SelectObject(&Bneu);
				break;
			}
			pDC->Ellipse(DataBlock->Get_Taster_Position(Block_Nr, (bool) i));
		}
	}
}

void C3DMeterGleis::ZeicheAchteck(CPoint P, byte Nr, BlockType Block)
{
	CBrush BCol;

	if (Block == BlockType::isBlock)
	{
		if (Zeige[Zeichne_Block_Nr])
		{
		}
		else
		{
			return;
		}
	}
	if (Block == BlockType::isGleis)
	{
		if (Zeige[Zeichne_Gleis_Nr])
		{
		}
		else
		{
			return;
		}
	}
	if (Block == BlockType::isWeiche)
	{
		if (Zeige[Zeichne_Weichen_Nr])
		{
		}
		else
		{
			return;
		}
	}
	int d=7;
	CPoint Pts[8];
	CString TextNr;

	BCol.CreateSolidBrush(colorGelb);
	P = P + CPoint(-14, -10);

	Pts[0] = CPoint(   d,    0);
	Pts[1] = CPoint(2 *d,    0);
	Pts[2] = CPoint(3 *d,    d);
	Pts[3] = CPoint(3 *d, 2* d);
	Pts[4] = CPoint(2 *d, 3* d);
	Pts[5] = CPoint(   d, 3* d);
	Pts[6] = CPoint(0   , 2* d);
	Pts[7] = CPoint(0   ,    d);
	for (int i = 0; i < 8; i++)
	{
		Pts[i].Offset(P);
	}
	pDC->SelectObject(&Pen_SW);
	pDC->SelectObject(&BCol);
	pDC->Polygon(Pts, 8);
	pDC->SetBkColor(colorGelb);
	pDC->SetTextAlign(TA_LEFT | TA_CENTER);
	TextNr.Format(_T("%2i"), Nr);
	pDC->SelectObject(&Font_Block_0);
	pDC->TextOut((P.x+10), (P.y+3), TextNr);

}

void C3DMeterGleis::SetzeBlockFarbe(byte Nr)
{
	if (DataBlock->ist_frei(Nr))
		pDC->SelectObject(&Pen_Gr);
	else
	{
		switch (DataBlock->Get_Block_Lok(Nr)->Get_Status())
		{
		case Zug_Status::Zug_Stopped:
			pDC->SelectObject(&Pen_Or);
			break;
		case Zug_Status::Zug_faehrt_vor:
		case Zug_Status::Zug_faehrt_rueck:
		case Zug_Status::Zug_haelt:
			if (DataBlock->ist_Lok_onBlock(Nr))
				pDC->SelectObject(&Pen_Ro);
			else
				pDC->SelectObject(&Pen_hR);
			break;
		default:
			pDC->SelectObject(&Pen_hR);
			break;
		}
	}

}
void C3DMeterGleis::ZeicheLok(CMem_DC LokBitmap, int Nr)
{
	if (DataBlock->Get_BlockType(Nr) == BlockType::isGleis)
	{
		if (DataBlock->ist_besetzt(Nr))
		{
			CPoint P;
			if (DataBlock->GetWerte_Block(Nr).GliesPos == 'U')
			{
				if (DataBlock->Get_Block_Lok(Nr)->Blick)
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_in_Uhr));
				else	
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_ge_Uhr));
				P = DataBlock->GetWerte_Block(Nr).TextPos + CPoint(12, -30);

			}
			if (DataBlock->GetWerte_Block(Nr).GliesPos == 'O')
			{
				if (DataBlock->Get_Block_Lok(Nr)->Blick)
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_ge_Uhr));
				else
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_in_Uhr));
				P = DataBlock->GetWerte_Block(Nr).TextPos + CPoint(12, -30);
			}
			if (DataBlock->GetWerte_Block(Nr).GliesPos == 'L')
			{
				if (DataBlock->Get_Block_Lok(Nr)->Blick)
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_Hoch));
				else
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_Runter));
				P = DataBlock->GetWerte_Block(Nr).TextPos + CPoint(-22, 13);
			}
			if (DataBlock->GetWerte_Block(Nr).GliesPos == 'R')
			{
				if (DataBlock->Get_Block_Lok(Nr)->Blick)
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_Runter));
				else
					LokBitmap->SelectObject(CBitmap::FromHandle(Lok_Hoch));
				P = DataBlock->GetWerte_Block(Nr).TextPos + CPoint(-22, 13);
			}

			pDC->SelectObject(&Brush_Hinterg);
			pDC->BitBlt(P.x, P.y, 100, 50, LokBitmap, 0, 0, MERGECOPY); // RCCOPY);
		}
	}
}

void C3DMeterGleis::ZeichenTest()
{
	pDC->SelectObject(&Font_Block_0);
	pDC->TextOutW(100, 100, _T(" 0° gedreht "));
	pDC->SelectObject(&Font_Block_90);
	pDC->TextOutW(100, 200, _T(" 90° gedreht "));
	pDC->SelectObject(&Font_Gleis_Info_0);
	pDC->TextOutW(400, 100, _T(" Info 0° gedreht "));
	pDC->SelectObject(&Font_Gleis_Info_90);
	pDC->TextOutW(400, 200, _T(" Info 90° gedreht "));

}
