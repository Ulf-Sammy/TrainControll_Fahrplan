#pragma once
#include "pch.h"
#include "CGleisPlan.h"
UINT Thread_Update_Status(LPVOID pParam);

class C3DMeterStatus: public CStatic
{
public:
	C3DMeterStatus(void);
	~C3DMeterStatus(void);
	void OnInitDialog(CGleisPlan * pPlan);
	void ReconstructControl();
	bool RunThread = false;
	CGleisPlan *DataPlan = NULL;
protected:
	
	CRect m_rectCtrl ;
	bool DrawInfo = true;
	bool Setup = false;
	
	CBrush Brush_Hinter_Wei;
	CBrush Brush_Hinter_Rot;
	CBrush Brush_Hinter_Gru;
	CBrush Brush_Hinter_Gel;
	CBrush Brush_Hinter_Org;
	CBrush* Brush_Old = NULL;

	CFont Font_Info;
	CFont *Font_Old = NULL;

	CDC *pDC = NULL;
	CDC  m_dcBackground ;
	
	CBitmap *m_pBitmapOldBackground = NULL;
	CBitmap m_bitmapBackground ;

	//{{AFX_MSG(C3DMeterStatus)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ZeichenHintergrund(CDC * pDC, CRect & rect);
	void ZeichneStatus(CDC* pDC);
	void ZeichneSetupStatus(CDC* pDC);
	
};
