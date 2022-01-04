// CDrawStatic.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CStaticDraw.h"


// CDrawStatic

IMPLEMENT_DYNAMIC(CStaticDraw, CStatic)

CStaticDraw::CStaticDraw()
{
	for (byte i = 0; i < MAX_STROM_ZEIT; i++)
	{
		Strom_Werte[i] = 0; // 1A
	}
}

CStaticDraw::~CStaticDraw()
{
}


BEGIN_MESSAGE_MAP(CStaticDraw, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()




void CStaticDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Fügen Sie hier Ihren Meldungshandlercode ein.
					   // CStatic::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;


	CPen* pOldPen ;
	CPen Pen_SW;
	Pen_SW.CreatePen(PS_SOLID, 1, colorDunkelGrau);

	pOldPen = pDC->SelectObject(&Pen_SW);

	pDC->MoveTo(0, Y_1A);	pDC->LineTo(MAX_STROM_ZEIT, Y_1A); // 1A
	pDC->MoveTo(0, Y_2A);	pDC->LineTo(MAX_STROM_ZEIT, Y_2A); // 1A
	pDC->MoveTo(0, Y_3A);	pDC->LineTo(MAX_STROM_ZEIT, Y_3A); // 1A
	pDC->MoveTo(0, Y_4A);	pDC->LineTo(MAX_STROM_ZEIT, Y_4A); // 1A


	int y;
//for (byte i = 0; i < MAX_STROM_ZEIT; i++)
	for (byte i = 0; i < 254 ; i++)
	{
		y = (Y_0A - (Strom_Werte[i] >> 1));
		pDC->SetPixel(i,y , colorRot);
	}

	pDC->SelectObject(pOldPen);
}

void CStaticDraw::Init()
{
	GetClientRect(&m_rectCtrl);
	Y_0A = (byte) m_rectCtrl.bottom;
	Y_1A = Y_0A - 19;
	Y_2A = Y_0A - (19 * 2);
	Y_3A = Y_0A - (19 * 3);
	Y_4A = Y_0A - (19 * 4);
}

void CStaticDraw::Set_Strom_Wert(byte* Data)
{
	if (St_Time < MAX_STROM_ZEIT)
	{
		Strom_Werte[St_Time] = Data[1];
		St_Time++;
	}
	// 0=  wert 0Ampere
	if (St_Time == MAX_STROM_ZEIT)
	{
		for (byte i = 0; i < (MAX_STROM_ZEIT - 1); i++)
		{
			Strom_Werte[i] = Strom_Werte[i + 1];
		}
		Strom_Werte[St_Time - 1] = Data[1];
	}
	Invalidate();
}

void CStaticDraw::Test_Strom()
{
	static byte A = 0;
	if (St_Time < MAX_STROM_ZEIT)
	{
		Strom_Werte[St_Time] = A;
		St_Time++;
	}
	// 0=  wert 0Ampere
	if (St_Time == MAX_STROM_ZEIT)
	{
		for (byte i = 0; i < (MAX_STROM_ZEIT - 1); i++)
		{
			Strom_Werte[i] = Strom_Werte[i + 1];
		}
		Strom_Werte[St_Time - 1] = A;
	}
	A++;
	if (A == 191) A = 0;
	Invalidate();
}
