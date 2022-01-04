#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "MemDC.h"

// CDlg_Lok_Schuppen-Dialog
struct Line2Point
{
	Line2Point()
	{
		A_Punkt = { 0,0 };
		E_Punkt = { 0,0 };
	};
	Line2Point(int Ax, int Ex, int Ay)
	{
		A_Punkt.x = Ax;
		E_Punkt.x = Ex;
		A_Punkt.y = Ay;
		E_Punkt.y = Ay;
	}
	CPoint A_Punkt;
	CPoint E_Punkt;
};

class CDlg_Lok_Schuppen : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Lok_Schuppen)

public:
	CDlg_Lok_Schuppen(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Lok_Schuppen();
	BOOL Create();
	BOOL Init();
	afx_msg	void OnPaint();

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_LOKSCHUPPEN };

protected:
	CCom_BlockMelderNet* BlockNet = NULL;
	CPen StiftTor;
	CPen StiftHaus;

	CBrush PinselHaus;
	CBrush PinselTor;
	CRect m_rectCtrl;

	CDC* pDC = NULL;
	CBitmap* m_pBitmapOldBackground = NULL;
	CBitmap m_bitmapBackground;

	virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV-Unterstützung
	void ZeichenHintergrund();
	void ZeicheWeiche(CDC* pDC ,CRect Weiche, bool Bit);
	void ZeicheGleis(CDC* pDC, byte I, Line2Point Gleis);
	void WeichenWeg_schalten(byte I);
	void Draw_LOK(CDC* pDC);

	CWnd* m_pParent;
	int m_nID;
	CRect SchuppenTor;
	CRect SchuppenTor_L;
	CRect SchuppenTor_R;

	CRect SchuppenTor_G0;
	CRect SchuppenTor_G1;
	CRect SchuppenTor_G2;

	CRect Weiche_Platz[6];
	TrainCon_Paar  Weiche_Nummer[6];
	byte Weiche_Block[6];

	CRect Gleis_Platz[7];
	Line2Point Gleis_Schuppen[8];
	byte Gleis_Block[8];
	bool Gleis_Block_On[6];
	byte last_On_Block;
	CGleisPlan* Gleise;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
