#pragma once
#include "pch.h"

// CDlg_Fahrplan-Dialog

class CDlg_Fahrplan : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Fahrplan)

public:
	CDlg_Fahrplan(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Fahrplan();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FAHRPLAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckBlock(UINT nID);
};
