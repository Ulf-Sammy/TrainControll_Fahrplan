#include "pch.h"
#include "3dmetergleis.h"
#include "math.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"


C3DMeterGleis::C3DMeterGleis(void)
{
	Zeige[Zeichne_Gitter] = true;
	Zeige[Zeichne_Block_Nr] = false;
	Zeige[Zeichne_Gleis_Nr] = false;
	Zeige[Zeichne_Weichen_Nr] = false;
	Zeige[Zeichne_Melder_Nr] = false;
	Zeige[Zeichne_Tasten_Nr] = true; 
	
	Pen_SW.CreatePen    (PS_SOLID, 1, RGB( 0, 0, 0)); // Schwarz
	Pen_Melder.CreatePen(PS_SOLID, 2, RGB( 0, 0, 0)) ;
	Pen_Taster.CreatePen(PS_SOLID, 3, RGB( 0, 0, 0));

	Brush_Hinterg.CreateSolidBrush(colorHinterGrund);

	LokButton = CRect(0, 0, 70, 151);
	LokButton.OffsetRect(0, 498);
}
//
C3DMeterGleis::~C3DMeterGleis(void)
{
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
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	pDlgSchuppen = APP->pDlgSchuppen;
	pDlgBlockInfo = APP->pDlgBlockInfo;
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


	
	ZeichenHintergrund(&dc, &m_dcBackground, Zeige[Zeichne_Gitter]);

	// drop in the background
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),&m_dcBackground, 0, 0, SRCCOPY) ;
	

	DataPlan->ZeicheStrecke(pDC);
	
	theApp.LokSchuppen.DrawTransparent(pDC, 0, 498, RGB(255, 255, 255));
	if (!DataPlan->isPower_onGleis())
	{
		theApp.WarnungNotAus.DrawTransparent(pDC, 800, 250, RGB(255, 255, 255));
	}



	//ZeicheLok(&memDC, 0);
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
	if (DataPlan->Kick_Block(point))
	{
		Invalidate();
		pDlgBlockInfo->NeueDaten();
	}
//	DataBlock->KlickTasterSchalten(point);
	if (LokButton.PtInRect(point))
	{
		if (pDlgSchuppen->IsWindowVisible()) pDlgSchuppen->ShowWindow(SW_HIDE);
		else pDlgSchuppen->ShowWindow(SW_SHOW);
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

void C3DMeterGleis::ZeichenHintergrund(CPaintDC *dc,  CDC* pDC_H, bool Gitter)
{
   
	
	CBrush*  pOldBrush ;
	CPen*	 pOldPen   ;
	CFont*	 pOldFont  ;
	CString  Text;
	CPoint   P;
	CPoint   Step = CPoint(40,22);
	int X_Lang = 40;
	int Y_Lang = 30;
   //Brush_Back.CreateSolidBrush(colorHinterGrund);

   if ((m_dcBackground.GetSafeHdc() == NULL) || (m_bitmapBackground.m_hObject == NULL))
   {
	   m_dcBackground.CreateCompatibleDC(dc);
	   m_bitmapBackground.CreateCompatibleBitmap(dc, m_rectCtrl.Width(), m_rectCtrl.Height());
	   m_pBitmapOldBackground = m_dcBackground.SelectObject(&m_bitmapBackground);
   }
   pOldBrush = pDC_H->SelectObject(&Brush_Hinterg);
   pOldPen = pDC_H->SelectObject(&theApp.Stift_SW_1);
   pOldFont = pDC_H->SelectObject(&theApp.Font_Info_s);
   pDC_H->Rectangle(m_rectCtrl);
   if (Gitter)
   {
	   pDC_H->SetBkColor(colorHinterGrund);
	   pDC_H->SetTextColor(colorSchwarz);
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
//   pDC_H->SelectObject(pOldBrush);
   pDC_H->SelectObject(pOldPen);
   pDC_H->SelectObject(pOldFont);
}

void C3DMeterGleis::ZeicheTaster(byte Block_Nr)
{
	CString TextNr, TextInfo;
	for (int i = 0; i < 2; i++)
	{
		
		//if (DataBlock->Show_Taster(Block_Nr, (bool) i))
		{
			/*CBrush Bneu;

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
			*/
		} 
	}
}


void C3DMeterGleis::ZeicheAchteck(CPoint P, byte Nr, BlockType Block)
{
	CBrush BCol ;
	COLORREF colorWert; // , , Gelb = RGB(253, 240, 2);
	colorWert = RGB(0, 0, 0);
	if (Block == BlockType::isBlock)
	{
		if (Zeige[Zeichne_Block_Nr])
		{
			colorWert = RGB(253,240, 2);
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
			colorWert = colorGelb;
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
			colorWert = RGB(255, 128, 0);
		}
		else
		{
			return;
		}
	}
	int d=7;
	CPoint Pts[8];
	CString TextNr;

	BCol.CreateSolidBrush(colorWert);
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
	pDC->SetBkColor(colorWert);
	pDC->SetTextAlign(TA_LEFT | TA_CENTER);
	TextNr.Format(_T("%2i"), Nr);
	pDC->SelectObject(&Font_Block_0);
	pDC->TextOut((P.x+10), (P.y+3), TextNr);

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
