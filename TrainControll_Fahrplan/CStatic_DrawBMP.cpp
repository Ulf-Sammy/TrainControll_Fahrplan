// CStatic_DrawBMP.cpp: Implementierungsdatei
//

#include "pch.h"
#include "CStatic_DrawBMP.h"
#include "TrainControll_Fahrplan.h"

// CStatic_DrawBMP

IMPLEMENT_DYNAMIC(CStatic_DrawBMP, CStatic)

CStatic_DrawBMP::CStatic_DrawBMP()
{
	Status_Bild = NULL;
	pOldBrush = NULL;
	pOldPen = NULL;
	pOldFont = NULL;

	Brush_Hinterg.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	Stift.CreatePen(0, 0, ::GetSysColor(COLOR_3DFACE));


}

CStatic_DrawBMP::~CStatic_DrawBMP()
{
	delete[] Status_Bild;
	Status_Bild = NULL;
}


BEGIN_MESSAGE_MAP(CStatic_DrawBMP, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStatic_DrawBMP-Meldungshandler




void CStatic_DrawBMP::OnPaint()
{
	CPaintDC dc(this);
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;

	pOldFont = pDC->SelectObject(&theApp.Font_Info_s);		
	pOldBrush = pDC->SelectObject(&Brush_Hinterg);
	pOldPen = pDC->SelectObject(&Stift);

	pDC->Rectangle(m_rectCtrl);
	if (Status < Max_Status)
	{
		int y = Status_Bild[Status].GetHeight();
		int x = Status_Bild[Status].GetWidth();
		CRect m_rectBild = CRect(0, 0, x, y);

	
		Status_Bild[Status].TransparentBlt(pDC->m_hDC, m_rectBild, RGB(0, 0, 255));
		pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
		pDC->TextOutW((x + 5), (y / 2) - 5, Status_Text);
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

void CStatic_DrawBMP::Init(byte Size, const CString* DateilisteBild)
{
	Max_Status = Size / sizeof(DateilisteBild[0]);
	Status_Bild = new CImage[Max_Status];
	GetClientRect(&m_rectCtrl);

	for (byte i = 0; i < Max_Status; i++)
	{
		Status_Bild[i].Load(DateilisteBild[i]);
	}
}

void CStatic_DrawBMP::Set_Status(byte St)
{
	Status = St;
	Invalidate();
}

void CStatic_DrawBMP::Set_Status(byte St, CString Stxt)
{
	Status = St;
	Status_Text = Stxt;
	Invalidate();
}
