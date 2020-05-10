#pragma once
#include "pch.h"
#include "afxcmn.h"
#include "CDatenBankLok.h"


// CDlg_Liste_Train-Dialogfeld

class CDlg_Liste_Train : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Liste_Train)

public:
	CDlg_Liste_Train(CWnd* pParent = NULL);   
	virtual ~CDlg_Liste_Train();
	void Set_TrainData(CDatenBankLok* Data);
// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LISTE_TRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	CListCtrl Zug_Liste;
	CDatenBankLok* Zug_Data;
	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
};
