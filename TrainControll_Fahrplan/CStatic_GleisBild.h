#pragma once
#include "pch.h"
#include "MemDC.h"
#include "CGleisPlan.h"
#include "CDlg_Lok_Schuppen.h"
#include "CDlg_Block_Info.h"


class CStatic_GleisBild: public CStatic
{
public:
	CStatic_GleisBild(void);
	~CStatic_GleisBild(void);
	void Init();

protected:
	bool Zeichnen_Gitter;
	bool Zeichnen_Block_Nr;

	CRect m_rectCtrl ;

	CGleisPlan *DataPlan = NULL;

	CBrush* pOldBrush = NULL;
	CPen*	pOldPen     = NULL ;
	CFont*	pOldFont   = NULL;

    CBrush Brush_Hinterg;

	CDC* pDC = NULL;

	CRect LokButton;
	CDlg_Lok_Schuppen* pDlgSchuppen = NULL;
	CDlg_Block_Info* pDlgBlockInfo = NULL;

	//{{AFX_MSG(C3DMeterGleis)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClick();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ZeichenHintergrund_Gitter( CDC * pDC_H);

};
