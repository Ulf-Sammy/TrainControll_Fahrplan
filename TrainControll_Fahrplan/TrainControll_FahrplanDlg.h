#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "3DMeterGleis.h"
#include "CDlg_Train_Run.h"
#include "CDlg_RunCam_Train.h"
#include "CDlg_Debug_Mega.h"
#include "CDlg_Lok_Schuppen.h"
#include "CDlg_Block_Info.h"
#include "afxwin.h"
#include "Resource.h"


// CTrainControll_FahrplanDlg-Dialogfeld
class CTrainControll_FahrplanDlg : public CDialogEx
{
// Konstruktion
public:
	CTrainControll_FahrplanDlg(CWnd* pParent = NULL);	// Standardkonstruktor
	void DlgTrainRunDone(int Dlg_Nr);

// Dialogfelddaten
	enum { IDD = IDD_TRAINCONTROLL_FAHRPLAN_DIALOG };
	CDlg_Run_Train*    pDlgTrainRun[7]   = { NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	int				   ButtonTrainRun[7] = { 0, 0, 0, 0, 0, 0, 0 };
	CDlg_RunCam_Train* pDlgTrainRunCam   = NULL;
	CDlg_Debug_Mega*   pDlgDebugInfo  = NULL;
	CDlg_Lok_Schuppen* pDlgSchuppen =	NULL;
	CDlg_Block_Info*   pDlgBlockInfo = NULL;

protected:
	
	HICON m_hIcon;

	CStatic* StaticImage_Modus;
	CStatic* StaticImage_Uno;
	CStatic* StaticImage_Mega;
	CStatic* StaticImage_LVZ;

	CRect Image_Uno   = { 0,0,40,40 };
	CRect Image_Mega  = { 0,0,40,40 };
	CRect Image_Modus = { 0,0,90,90 };
	CRect Image_LVZ   = { 0,0,175,70 };

	CImage Bild_Fahren;
	CImage Bild_Progam;
	CImage LED_rot;
	CImage LED_gelb;
	CImage LED_gruen;
	CImage LVZ_ON;
	CImage LVZ_OFF;
	CImage LVZ_PROG;
	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void InitDlg();

public:
	CGleisPlan			Gleis_Data;
	C3DMeterGleis		m_3DGleis;
	CDatenBankLok		meineLoks;
	CCom_XpressNet		XpressNet;
	CCom_BlockMelderNet	BlockMelder;

	void DoStartDialog(bool Zeige);

	afx_msg void OnSetupProgrammierezug();
	afx_msg void OnSetupMeinezugliste();
	afx_msg void OnSetupTestedieweiche();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonWeiche();
	afx_msg void OnBnClickedButtonZug(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnSetupFahrplanEdit();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
