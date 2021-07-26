#pragma once
#include "pch.h"
#include "MemDC.h"
#include "CGleisPlan.h"
#include "CDlg_Lok_Schuppen.h"
#include "CDlg_Block_Info.h"

enum { Zeichne_Gitter, Zeichne_Block_Nr, Zeichne_Gleis_Nr, Zeichne_Weichen_Nr, Zeichne_Tasten_Nr, Zeichne_Melder_Nr, Zeichnen_Ende };

class C3DMeterGleis: public CStatic
{
public:
	C3DMeterGleis(void);
	~C3DMeterGleis(void);
	void OnInitDialog(CGleisPlan* pBlock);
	void ReconstructControl();
	CGleisPlan *DataPlan = NULL;

protected:
	bool Zeige[Zeichnen_Ende];

	CRect m_rectCtrl ;
	int Bild_X = 0;
	int Bild_Y = 0;

	bool Setup = false;
	bool Error = false;

	CPen Pen_SW; // Schwarz
	CPen Pen_Melder;
	CPen Pen_Taster;
	CPen* Pen_Old = NULL; 
	

    CBrush Brush_Hinterg;

	CBrush* Brush_Old = NULL;
	
	CFont Font_Block_0;
	CFont Font_Block_90;
	CFont Font_Gleis_Info_0;
	CFont Font_Gleis_Info_90;
	CFont* Font_Old = NULL;

	CDC* pDC = NULL;
	CDC  m_dcBackground ;
	CBitmap *m_pBitmapOldBackground = NULL;
	CBitmap m_bitmapBackground ;
	CRect LokButton;
	CDlg_Lok_Schuppen* pDlgSchuppen = NULL;
	CDlg_Block_Info* pDlgBlockInfo = NULL;

	//{{AFX_MSG(C3DMeterGleis)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ZeichenHintergrund(CPaintDC *dc, CDC * pDC_H, bool Gitter);
	

	void ZeicheTaster(byte Nr);
	void ZeicheAchteck(CPoint P, byte Nr, BlockType Block);
	void ZeichenTest();
};
