#pragma once
#include "CGleisPlan.h"
#include "pch.h"

// CDlg_CV_Action dialog

class CDlg_CV_Action : public CDialog
{
	DECLARE_DYNAMIC(CDlg_CV_Action)

public:
	CDlg_CV_Action(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_CV_Action();

// Dialog Data
	enum { IDD = IDD_DLG_CV_ACTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadAnhang(void);
	DECLARE_MESSAGE_MAP()

	CString			   Anhang[23][11];
	CString			   m_Info[13];
	CDatenBankLok	   *Zug_Data;
	CDecoder_All_Infos *DecoderInfo;
	byte			   CV_Nr;
	byte			   altCV_Wert;
	byte			   newCV_Wert;

public:
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonFuntionsBeschreibung();
	afx_msg void OnBnClickedOk();
	void SetPointer(CDatenBankLok* pTData, CDecoder_All_Infos* pDInfo);
	void Set_Lok( byte CV,byte CVWt);
	void SetAnhang();
	byte GetCVValue(void);
	virtual BOOL OnInitDialog();

	CString m_CV_Info;
	CString m_Lok_Info;

	bool m_NewData;
};
