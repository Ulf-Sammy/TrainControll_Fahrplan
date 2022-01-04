#pragma once
#include "pch.h"
#include "MemDC.h"

// CDrawStatic

class CStaticDraw : public CStatic
{
	DECLARE_DYNAMIC(CStaticDraw)

public:
	CStaticDraw();
	virtual ~CStaticDraw();

protected:
	CPen Pen_SW; // Schwarz

	CRect m_rectCtrl;
	CDC* pDC = NULL;
	byte Strom_Werte[MAX_STROM_ZEIT];
	byte St_Time = 0;
	byte Y_0A = 0;
	byte Y_1A = 0; 
	byte Y_2A = 0; 
	byte Y_3A = 0; 
	byte Y_4A = 0; 

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void Init();
	void Set_Strom_Wert(byte* Data);
	void Test_Strom();
};


