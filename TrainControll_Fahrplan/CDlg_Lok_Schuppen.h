#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "MemDC.h"

constexpr int  MaxWeiche = 7;

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
	CDatenBankLok* Loks = NULL;
	CGleisPlan* Gleise;

	CPen StiftTor;
	CPen StiftHaus;

	CBrush PinselHaus;
	CBrush PinselTor;
	CRect m_rectCtrl;

	CDC* pDC = NULL;
	CBitmap* m_pBitmapOldBackground = NULL;
	CBitmap m_bitmapBackground;

	virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV-Unterstützung
	void ChangeCombox(byte Nr);
	void ZeichenHintergrund();
	void ZeicheWeiche(CDC* pDC ,CRect Weiche, bool Bit);
	void ZeicheGleis(CDC* pDC, byte I, Line2Point Gleis);
	void WeichenWeg_schalten(byte I);

	CWnd* m_pParent;
	int m_nID;
	const CRect SchuppenTor   = { 550, 280, 557, 435 };
	const CRect SchuppenTor_L = { 550, 270, 557, 300 };
	const CRect SchuppenTor_R = { 550, 420, 557, 450 };

	const CRect SchuppenTor_G0 ={ 530, 300, 550, 340 };
	const CRect SchuppenTor_G1 ={ 530, 341, 550, 380 };
	const CRect SchuppenTor_G2 ={ 530, 381, 550, 420 };

	CRect Lok_Platz[MaxAbstellGleise];
	CRect Gleis_Platz[MaxAbstellGleise];
	CRect Weiche_Platz[MaxWeiche] = {{660, 370, 700, 410},{620, 330, 660, 370},{470, 290, 510, 330},{430, 250, 470, 290},{390, 210, 430, 250},{350, 170, 390, 210},{310, 130, 350, 170}};

	TrainCon_Paar  Weiche_Nummer[MaxWeiche]		= { {18, false}, {19, false}, {20, false}, {21, false}, {22, false},{23, false},{24, false} };
	byte Weiche_Block[MaxWeiche]				= {24,24,24,24,24,24};

	Line2Point Gleis_Schuppen[MaxAbstellGleise] = {{20,655,410},{20,615,370},{20,465,330},{20,425,290},{20,385,250},{20,345,210},{20,305,170},{20,305,130}};
	Line2Point Gleis_Strecke[2]                 = {{510,620,330},{700,740,410} };
	byte Strecke_Block[2]                       = {24,24};
	byte Gleis_Block[MaxAbstellGleise]          = {33,34,35,36,37,38,39,40};
	bool Gleis_Block_On[MaxAbstellGleise];
	byte last_On_Block;
	bool Zeige_Comobox;
	byte Tor_pos = 0;
	DECLARE_MESSAGE_MAP()
public:
	CComboBox Lok_Gleis[MaxAbstellGleise];
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSelchangeCombos1();
	afx_msg void OnSelchangeCombos2();
	afx_msg void OnSelchangeCombos3();
	afx_msg void OnSelchangeCombos4();
	afx_msg void OnSelchangeCombos5();
	afx_msg void OnSelchangeCombos6();
	afx_msg void OnSelchangeCombos7();
	afx_msg void OnSelchangeCombos8();
};

