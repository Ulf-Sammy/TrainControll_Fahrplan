#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "3DMeterGleis.h"
#include "DlgTrainRun.h"
#include "CDlg_RunCam_Train.h"
#include "CDlg_Debug_Mega.h"
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
	CDlg_Run_Train*    pDlgTrainRun[5]   = { NULL,NULL,NULL,NULL,NULL };
	int				   ButtonTrainRun[5] = { 0, 0, 0, 0, 0 };
	CDlg_RunCam_Train* pDlgTrainRunCam   = NULL;
	CDlg_Debug_Mega*   pDlgDebugInfo  = NULL;

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
	byte				Anzahl_Dlg_Run = 0;
	CGleisPlan			Gleis_Data;
	C3DMeterGleis		m_3DGleis;
	//C3DMeterStatus		m_3DStatus;
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
