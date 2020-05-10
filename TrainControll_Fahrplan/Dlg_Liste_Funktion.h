#pragma once
#include "pch.h"
#include "CGleisPlan.h"
#include "EditableListCtrl.h"
#include "VC_TableInfo.h"
#include "resource.h"

// CDlg_Liste_Funktion-Dialogfeld

class CDlg_Liste_Funktion : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Liste_Funktion)

public:
	CDlg_Liste_Funktion(CWnd* pParent = NULL);  
	virtual ~CDlg_Liste_Funktion();
	
	void SetPointer(CDatenBankLok* pGData);
	virtual BOOL OnInitDialog();
	enum { IDD = IDD_DLG_LISTE_FUNKTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	void Clear_Tabelle();
	CWnd* m_pParent;
	int m_nID;
	bool View_1;
	bool View_2;
	bool Do_Edit;
	int Zeile;
	CDecoderInfo		*CV_Table;
	CDatenBankLok		*Zug_Data;
	void Fill_Liste_Anschluss();
	void Fill_Liste_FunktionTasten();
	DECLARE_MESSAGE_MAP()
public:
	CEditableListCtrl Liste_FunktionsTasten;
	afx_msg LRESULT OnNotifyDescriptionEdited(WPARAM, LPARAM);
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
};
