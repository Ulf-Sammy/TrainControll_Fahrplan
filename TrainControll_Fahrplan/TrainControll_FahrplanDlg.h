#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "3DMeterGleis.h"
#include "3DMeterStatus.h"
#include "DlgTrainRun.h"
#include "CDlg_RunCam_Train.h"
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
	

protected:
	
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void InitDlg();

public:
	byte		   Anzahl_Dlg_Run = 0;
	CGleisPlan	   Gleis_Data;
	C3DMeterGleis  m_3DGleis;
	C3DMeterStatus m_3DStatus;
	CDatenBankLok  meineLoks;
	CXpressNetCom  XpressNet;

	void DoStartDialog(bool Zeige);

	afx_msg void OnSetupProgrammierezug();
	afx_msg void OnSetupMeinezugliste();
	afx_msg void OnSetupTestedieweiche();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonSound();
	afx_msg void OnBnClickedButtonPower();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonWeiche();
	afx_msg void OnBnClickedButtonZug(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnSetupFahrplanEdit();
};
