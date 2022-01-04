#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "CStatic_GleisBild.h"
#include "CStatic_DrawBMP.h"
#include "CStaticDraw.h"
#include "CStaticText.h"
#include "CDlg_Train_Run.h"
#include "CDlg_RunCam_Train.h"
#include "CDlg_Debug_Mega.h"
#include "CDlg_Lok_Schuppen.h"
#include "CDlg_Block_Info.h"
#include "CDlg_Com_Liste.h"
#include "afxwin.h"
#include "Resource.h"


UINT Thread_Update_LZV(LPVOID pParam);
UINT Thread_Update_MEGA(LPVOID pParam);
UINT Thread_Update_Time(LPVOID pParam);


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
	CDlg_Lok_Schuppen* pDlgSchuppen   =	NULL;
	CDlg_Block_Info*   pDlgBlockInfo  = NULL;
	CDlg_Com_Liste*    pDlgComListe   = NULL;

	CStaticDraw		   StromKurve;
	CStaticText        InfoMelder;
	CStaticText        InfoWeiche;
	CStatic_GleisBild  InfoGleisBild;
	CStatic_DrawBMP	   InfoMega;
	CStatic_DrawBMP	   InfoLVZ200;
	CStatic_DrawBMP	   InfoModus;
	CStatic_DrawBMP	   InfoPower;

protected:
	
	HICON m_hIcon;
	
	COM_Info ComListe[COM_MAX_LISTE];
	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void InitDlg();

public:
	
	
	CDatenBankLok		meineLoks;
	CCom_LZV200			XpressNet;
	CCom_BlockMelderNet	BlockMelder;
	CGleisPlan			Gleis_Data;


	COM_Info COM_LZV_Data;
	COM_Info COM_MEGA_Data;
	COM_Info COM_DEBUG_Data;

	void EnumSerialPortFriendlyName();
	void Get_Com_Handel();
	bool Open_Setup_Com(COM_Info* p_COM_I);
	void Start_Com_Thread();
	void Close_Com_Handel();
	void DoStartDialog(bool Zeige);
	void updatePowerOn(bool Bit);
	

	afx_msg void OnSetupProgrammierezug();
	afx_msg void OnSetupMeinezugliste();
	afx_msg void OnSetupTestedieweiche();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonWeiche();
	afx_msg void OnBnClickedButtonZug(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnSetupFahrplanEdit();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetupMegadebugdata();
	afx_msg void OnSetupComlisteInfo();
};
