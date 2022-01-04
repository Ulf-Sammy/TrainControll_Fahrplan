#pragma once
#include "pch.h"
#include "MemDC.h"
// CTextStatic

class CStaticText : public CStatic
{
	DECLARE_DYNAMIC(CStaticText)

public:
	CStaticText();
	virtual ~CStaticText();

protected:
	CRect m_rectCtrl;
	CDC* pDC = NULL;
	byte Pos = 0;
	CString Info_Text;
	CString Debug_Text[11];
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void Init(CString Info);
	void Set_Debug_Text(byte* Data);
};


