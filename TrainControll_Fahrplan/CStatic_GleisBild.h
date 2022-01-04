#pragma once
#include "pch.h"
#include "MemDC.h"
#include "CGleisPlan.h"
#include "CDlg_Lok_Schuppen.h"
#include "CDlg_Block_Info.h"

enum { Zeichne_Gitter, Zeichne_Block_Nr, Zeichne_Gleis_Nr, Zeichne_Weichen_Nr, Zeichne_Tasten_Nr, Zeichne_Melder_Nr, Zeichnen_Ende };

class CStatic_GleisBild: public CStatic
{
public:
	CStatic_GleisBild(void);
	~CStatic_GleisBild(void);
	void Init();
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

	CBrush* pOldBrush = NULL;
	CPen*	pOldPen     = NULL ;
	CFont*	pOldFont   = NULL;


    CBrush Brush_Hinterg;

	CFont Font_Block_0;
	CFont Font_Block_90;
	CFont Font_Gleis_Info_0;
	CFont Font_Gleis_Info_90;

	
	CDC* pDC = NULL;

	CRect LokButton;
	CDlg_Lok_Schuppen* pDlgSchuppen = NULL;
	CDlg_Block_Info* pDlgBlockInfo = NULL;

	//{{AFX_MSG(C3DMeterGleis)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void neueDaten();

	void ZeichenHintergrund( CDC * pDC_H);
	

	void ZeicheTaster(byte Nr);
	void ZeicheAchteck(CPoint P, byte Nr, BlockType Block);
	void ZeichenTest();
};
