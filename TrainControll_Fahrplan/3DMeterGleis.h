#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "MemDC.h"

UINT Thread_Update_Gleis(LPVOID pParam);

enum { Zeichne_Gitter, Zeichne_Block_Nr, Zeichne_Gleis_Nr, Zeichne_Weichen_Nr, Zeichne_Tasten_Nr, Zeichne_Melder_Nr, Zeichnen_Ende };

class C3DMeterGleis: public CStatic
{
public:
	C3DMeterGleis(void);
	~C3DMeterGleis(void);
	void OnInitDialog(CGleisPlan* pBlock);
	void ReconstructControl();
	bool RunThread = false;
	CGleisPlan *DataPlan = NULL;

protected:
	bool Zeige[Zeichnen_Ende];

	CRect m_rectCtrl ;
	int Bild_X = 0;
	int Bild_Y = 0;

	bool Setup = false;
	bool Error = false;
	CPen Pen_Gr; // Grün
	CPen Pen_Ro; // Rot
	CPen Pen_hR; // hell Rot
	CPen Pen_Or; // Orange
	CPen Pen_Ge; // Gelb
	CPen Pen_Ga; // Schwarz
	CPen Pen_Bl; // Blau
	CPen Pen_SW; // Schwarz
	CPen Pen_Melder;
	CPen Pen_Taster;
	CPen Pen_White;
	CPen* Pen_Old = NULL; 
	
	CBrush Brush_White;
	CBrush Brush_Yellow;
	CBrush Brush_Red;
	CBrush Brush_Green;

    CBrush Brush_Hinterg;
	CBrush Brush_Weiche;
	CBrush Brush_Block;
	CBrush Brush_Melder_I;
	CBrush Brush_Melder_O;
	CBrush* Brush_Old = NULL;
	
	CFont Font_Info;
	CFont Font_Block_0;
	CFont Font_Block_90;
	CFont Font_Gleis_Info_0;
	CFont Font_Gleis_Info_90;
	CFont* Font_Old = NULL;

	HBITMAP Lok_in_Uhr = NULL;
	HBITMAP Lok_ge_Uhr = NULL;
	HBITMAP Lok_Hoch   = NULL;
	HBITMAP Lok_Runter = NULL;

	CDC* pDC = NULL;
	CDC  m_dcBackground ;
	
	CBitmap *m_pBitmapOldBackground = NULL;
	CBitmap m_bitmapBackground ;




	//{{AFX_MSG(C3DMeterGleis)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ZeichenHintergrund(CDC * pDC_H, CRect & rect);
	

	void ZeicheTaster(byte Nr);
	void ZeicheAchteck(CPoint P, byte Nr, BlockType Block);
	void SetzeBlockFarbe(byte Nr);
	void ZeicheLok(CMem_DC LokBitmap,int  BlockNr);
	void ZeichenTest();
};
