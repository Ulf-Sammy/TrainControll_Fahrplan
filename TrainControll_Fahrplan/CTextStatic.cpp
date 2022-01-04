// CTextStatic.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CStaticText.h"


// CTextStatic

IMPLEMENT_DYNAMIC(CStaticText, CStatic)

CStaticText::CStaticText()
{

}

CStaticText::~CStaticText()
{
}


BEGIN_MESSAGE_MAP(CStaticText, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTextStatic-Meldungshandler




void CStaticText::OnPaint()
{
	// 		pDC->TextOutW(300, ((i * 8) + 725), Debug_Text_B[i]);
	//      pDC->TextOutW(460, ((i * 8) + 725), Debug_Text_W[i]);

	CPaintDC dc(this); 
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;

	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&theApp.Font_Info_small);

	for (int i = 0; i < 11; i++)
	{
		pDC->TextOutW(0, (i * 8), Debug_Text[i]);
	}
	pDC->SelectObject(pOldFont);

}

void CStaticText::Init(CString Info)
{
	Info_Text = Info;
	GetClientRect(&m_rectCtrl);
}

void CStaticText::Set_Debug_Text(byte* Data)
{
	CString Text;
	byte Nr = Data[1];
	bool Bit = (bool)Data[2];

	if (Bit)
	{
		 Text.Format(_T(" %2i : I"), Nr);
	}
	else
	{
		Text.Format(_T(" %2i : o"), Nr);
	}
	Text = Info_Text + Text;
	Text = theApp.Get_Time(Text);
	if (Pos == 11)
	{
		for (int i = 0; i < 10; i++)
		{
			Debug_Text[i] = Debug_Text[i + 1];
		}
		Debug_Text[10] = Text;
	}
	else
	{
		Debug_Text[Pos] = Text;
		Pos++;
	}
	Invalidate();
}
