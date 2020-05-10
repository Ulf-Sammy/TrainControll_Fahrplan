#pragma once
#include "pch.h"
#include "CBlockData.h"
#include "afxwin.h"

// CDlg_Test_Weiche-Dialogfeld

class CDlg_Test_Weiche : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Test_Weiche)

public:
	CDlg_Test_Weiche(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CDlg_Test_Weiche();

	void SetPointer(CDataBlock* PGData);
	afx_msg void OnBnClickedFunktion(UINT nID);

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WEICHE_TESTEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	
	CDataBlock      *Gleis_Data;

	DECLARE_MESSAGE_MAP()

public:
	CComboBox Setting_Test;
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedInfoLok();
};
