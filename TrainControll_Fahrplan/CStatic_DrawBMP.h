#pragma once
#include "pch.h"
#include "MemDC.h"


// CStatic_DrawBMP

class CStatic_DrawBMP : public CStatic
{
	DECLARE_DYNAMIC(CStatic_DrawBMP)

public:
	CStatic_DrawBMP();
	virtual ~CStatic_DrawBMP();

protected:
	CRect m_rectCtrl;
	CDC* pDC = NULL;
	CBrush* pOldBrush;
	CPen* pOldPen;
	CFont* pOldFont;

	CBrush Brush_Hinterg;
	CPen Stift;

	byte Pos = 0;
	byte Status = 0;
	CString Status_Text;
	CImage* Status_Bild;
	byte Max_Status = 0;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void Init(byte Size, const CString* DateilisteBild);
	void Set_Status(byte St);
	void Set_Status(byte St, CString Stxt);
};


