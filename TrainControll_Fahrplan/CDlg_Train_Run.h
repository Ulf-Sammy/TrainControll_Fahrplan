#pragma once
#include "pch.h"
#include "VC_TableInfo.h"
#include "CDatenBankLok.h"
#include "CGleisPlan.h"
#include "C_Anlagen_Data.h"
#include "afxwin.h"
#include "Resource.h"
#include "CaptureGraph.h"

#pragma comment (lib, "strmiids")
// CDlgTrainRun dialog


class CDlg_Run_Train : public CDialog
{
private:
	using CDialog::Create;

public:
	CDlg_Run_Train(CWnd* pParent, byte ID);
	virtual ~CDlg_Run_Train();
	BOOL Create();
	void Set_Daten(byte Block);
	void SetInfo(CString Text);
	BOOL isCamera() const;
	
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnBnClickedFunktion(UINT nID);
	afx_msg void OnBnClickedSpeed(UINT nID);
	afx_msg void OnPaint();

	enum { IDD = IDD_DLG_RUN_TRAIN0 ,
		   IDD1 = IDD_DLG_RUN_TRAIN1,
		   IDD2 = IDD_DLG_RUN_TRAIN2,
		   IDD3 = IDD_DLG_RUN_TRAIN3,
		   IDD4 = IDD_DLG_RUN_TRAIN4,
		   IDD5 = IDD_DLG_RUN_TRAIN5,
		   IDD6 = IDD_DLG_RUN_TRAIN6,
		   IDD7 = IDD_DLG_RUN_TRAIN7,
		   IDD8 = IDD_DLG_RUN_TRAIN8
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetTasten(void);
	void SetButtom(bool Bit, int Butt_ID, int ImgON_ID, int ImgOFF_ID);
	CWnd* m_pParent;
	int m_nID;
	CameraBild		LokFenster;
	CAnlagen_Data   *Anlagen_Daten;
	CDecoderInfo	*CV_Table;
	CString			Lok_Name;
	byte			Lok_Park_Block;
	byte			Lok_Nr_act;
	HBITMAP			Lok_Bild;
	HBITMAP			Lok_Tacho;
	HBITMAP			Lok_Pfeil_vor;
	HBITMAP			Lok_Pfeil_zur;
	bool			Dlg_Richtung;
	byte			Dlg_Geschwindigkeit;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
