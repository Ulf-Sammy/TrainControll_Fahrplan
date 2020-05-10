#include "pch.h"
#include ".\3DMeterStatus.h"
#include "MemDC.h"
#include "math.h"



UINT Thread_Update_Status(LPVOID pParam)
{
	C3DMeterStatus* Info_Status = (C3DMeterStatus*) pParam;
	Sleep(500);
	do
	{
		if (Info_Status->DataPlan->isNewUpdate_Status()) Info_Status->Invalidate();
	} while (Info_Status->RunThread);
	return 0;
}


C3DMeterStatus::C3DMeterStatus(void)
{
	Brush_Hinter_Rot.CreateSolidBrush(RGB(255,   0,  0));
	Brush_Hinter_Gru.CreateSolidBrush(RGB(  0, 255,  0));
	Brush_Hinter_Gel.CreateSolidBrush(RGB(155, 155,  0));
	Brush_Hinter_Org.CreateSolidBrush(RGB(155, 155, 0));
	Brush_Hinter_Wei.CreateSolidBrush(RGB(255, 255, 255));;
	
	Font_Info.CreateFont(25, 0, 0, 0, 400,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Arial Rounded MT Bold"));
}

void C3DMeterStatus::OnInitDialog(CGleisPlan * pPlan)
{
	DataPlan  = pPlan;
	RunThread = true;
	AfxBeginThread(Thread_Update_Status, this);
}


//
C3DMeterStatus::~C3DMeterStatus(void)
{
	RunThread = false;
}


BEGIN_MESSAGE_MAP(C3DMeterStatus, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void C3DMeterStatus::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	ReconstructControl() ;
}

void C3DMeterStatus::OnPaint(void)
{
	CPaintDC dc(this); 
	GetClientRect (&m_rectCtrl) ;
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;

	if ((m_dcBackground.GetSafeHdc() == NULL) || (m_bitmapBackground.m_hObject == NULL))
	{
		m_dcBackground.CreateCompatibleDC(&dc) ;
		m_bitmapBackground.CreateCompatibleBitmap(&dc,m_rectCtrl.Width(), m_rectCtrl.Height()) ;
		m_pBitmapOldBackground = m_dcBackground.SelectObject(&m_bitmapBackground) ;
		ZeichenHintergrund(&m_dcBackground, m_rectCtrl) ;				
	}
	// drop in the background
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),&m_dcBackground, 0, 0, SRCCOPY) ;
	Brush_Old = pDC->SelectObject(&Brush_Hinter_Rot);
	Font_Old = pDC->SelectObject(&Font_Info);
	ZeichneStatus(&memDC);
	ZeichneSetupStatus(&memDC);
	// TRACE(_T("on Paint  Status ! \n "));
	DataPlan->Set_Startup(true);
}
void C3DMeterStatus::ReconstructControl()
{
	if ((m_pBitmapOldBackground) && 
		(m_bitmapBackground.GetSafeHandle()) && 
		(m_dcBackground.GetSafeHdc()))
	{
			m_dcBackground.SelectObject(m_pBitmapOldBackground);
			m_dcBackground.DeleteDC() ;
			m_bitmapBackground.DeleteObject();
	}
	Invalidate () ;
}

void C3DMeterStatus::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

// Setze Werte für Weichenrechtecke
void C3DMeterStatus::ZeichenHintergrund(CDC * pDC, CRect &rect)
{
	COLORREF Back_Pen = RGB(115, 115, 115);
	CPen	 Pen_Back;
	CBrush*  pOldBrush;
	CPen*	 pOldPen;

	Pen_Back.CreatePen(PS_SOLID, 1, Back_Pen);

	pOldBrush = pDC->SelectObject(&Brush_Hinter_Wei);
	pOldPen = pDC->SelectObject(&Pen_Back);

	pDC->Rectangle(rect) ;


	pDC->SelectObject(pOldPen) ;
	pDC->SelectObject(pOldBrush) ;
}
void C3DMeterStatus::ZeichneStatus(CDC* pDC)
{
	CPen penDraw, *pPenOld;
	CFont *pFontOld,SkFont ;
	CString M_Text;
	CString ZText[] = { _T("Zentrale nicht definiert !"),_T("Power ist am Gleis") ,_T("Keine Power am Gleis !") ,_T("BC Alle Loks Aus") ,_T("im Programiermode") ,_T("BC Rückmeldung") ,_T("6. ??") };
	SkFont.DeleteObject() ;
	SkFont.CreateFont (25, 0, 0, 0, 400,
						 FALSE, FALSE, 0, ANSI_CHARSET,
						 OUT_DEFAULT_PRECIS, 
						 CLIP_DEFAULT_PRECIS,
						 DEFAULT_QUALITY, 
						 DEFAULT_PITCH|FF_SWISS, _T("Arial Narrow")) ;
	pFontOld = pDC->SelectObject(&SkFont) ;
	pDC->SetBkColor(RGB(255,255,255));
	penDraw.DeleteObject() ;
	penDraw.CreatePen(PS_SOLID,1,RGB(255,155,155)) ;
	pPenOld = pDC->SelectObject(&penDraw);

	pDC->TextOut(1100, 10, _T("                             "));
	if (DataPlan->GetStatusZentrale() == 0xFF)
		pDC->TextOut(1100, 10, _T("Zentrale noch nicht eingeschaltet !"));
	else
		pDC->TextOut(1100, 10, ZText[DataPlan->GetStatusZentrale()]);
	penDraw.DeleteObject() ;
	pDC->SelectObject(&pPenOld);
}

void C3DMeterStatus::ZeichneSetupStatus(CDC* pDC)
{
	CString M_Text;
	switch (DataPlan->GetStatusMode().hauptMode)
	{
	case ControlStatus::No_Arduino:
		M_Text = "Kein Arduino:";
		pDC->SetBkColor(RGB(255, 0, 0));
		DataPlan->Reeady_ToRun();
		break;
	case ControlStatus::Begin_COM:
		M_Text = "Starte Verbindung";
		pDC->SetBkColor(RGB(255, 200, 15));
		break;
	case ControlStatus::Setup_Controller:
		pDC->SetBkColor(RGB(255, 200, 15));
		if (DataPlan->GetStatusMode().unterMode == 0)		M_Text = "Starte mit dem  Setup.....!";
		if (DataPlan->GetStatusMode().unterMode == 1)		M_Text = "Start Setup der Melder....!";
		if (DataPlan->GetStatusMode().unterMode == 2)		M_Text = "Start Setup der Weichen...!";
		if (DataPlan->GetStatusMode().unterMode == 3)		M_Text = "Start Setup der Blöcke....!";
		if (DataPlan->GetStatusMode().unterMode == 4)		M_Text = "Start Setup XpressNet.....!";
		if (DataPlan->GetStatusMode().unterMode == 5)		M_Text = "Start Setup des Lichts....!";
		break;
	case ControlStatus::Program:
		pDC->SetBkColor(RGB(0, 255, 0));
		M_Text = "Lok programmieren";
		break;
	case ControlStatus::Testen:
		pDC->SetBkColor(RGB(255, 255, 255));
		M_Text = "Der Betrieb / Lok fahren"; 
		DataPlan->Reeady_ToRun();
		break;
	case ControlStatus::Ende_COM:
		pDC->SetBkColor(RGB(255, 0, 0));
		M_Text = "Ende der Komunikation";
		break;
	}
	pDC->TextOut(5, 10, _T("                                                   "));
	pDC->TextOut(5, 10, M_Text);
}
