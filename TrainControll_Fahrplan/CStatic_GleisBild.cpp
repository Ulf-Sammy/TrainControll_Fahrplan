#include "pch.h"
#include "CStatic_GleisBild.h"
#include "math.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"


CStatic_GleisBild::CStatic_GleisBild(void)
{

	Zeichnen_Gitter  = false;
	Zeichnen_Block_Nr = true;
	
	Brush_Hinterg.CreateSolidBrush(colorHinterGrund);

	LokButton = CRect(0, 0, 70, 151);
	LokButton.OffsetRect(0, 498);
}
//
CStatic_GleisBild::~CStatic_GleisBild(void)
{
}

//
BEGIN_MESSAGE_MAP(CStatic_GleisBild, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CStatic_GleisBild::Init()
{
	
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	pDlgSchuppen = APP->pDlgSchuppen;
	pDlgBlockInfo = APP->pDlgBlockInfo;
	DataPlan = &APP->Anlagen_Daten.Gleis_Data;
	
	GetClientRect(&m_rectCtrl);
}


void CStatic_GleisBild::OnPaint(void)
{
	CPaintDC dc(this);
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;

	pOldBrush = pDC->SelectObject(&Brush_Hinterg);

	pDC->Rectangle(m_rectCtrl);
	if (!DataPlan->isPower_onGleis()) 	{ theApp.WarnungNotAus.DrawTransparent(pDC, 800, 250, RGB(255, 255, 255));}
	

	pOldPen = pDC->SelectObject(&theApp.Stift_SW_1);
	pOldFont = pDC->SelectObject(&theApp.Font_Info_s);

	if(Zeichnen_Gitter) ZeichenHintergrund_Gitter(pDC);

	DataPlan->ZeicheStrecke(pDC);
	if (Zeichnen_Block_Nr) DataPlan->ZeicheStreckenInfo(pDC);
	DataPlan->ZeicheTaster(pDC);

	theApp.LokSchuppen.DrawTransparent(pDC, 0, 498, RGB(255, 255, 255));
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);

}

void CStatic_GleisBild::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (DataPlan->Klick_Block(point))
	{
		Invalidate();
		pDlgBlockInfo->NeueDaten();
	}
	if (LokButton.PtInRect(point))
	{
		if (pDlgSchuppen->IsWindowVisible())
		{
			pDlgSchuppen->ShowWindow(SW_HIDE);
		}
		else
		{
			DataPlan->Ask_Door_Status();
			pDlgSchuppen->ShowWindow(SW_SHOW);
		}
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CStatic_GleisBild::ZeichenHintergrund_Gitter(CDC* pDC_H)
{
	CString  Text;
	CPoint   P;
	CPoint   Step = CPoint(40,22);
	int X_Lang = 40;
	int Y_Lang = 30;
  

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

