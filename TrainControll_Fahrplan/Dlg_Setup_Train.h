#pragma once

#include "pch.h"
#include "CDatenBankLok.h"
#include "Dlg_CV_Action.h"
#include "Dlg_Liste_Funktion.h"
#include "afxcmn.h"

// CDlg_Setup_Train dialog


class CDlg_Setup_Train : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Setup_Train)

public:
	CDlg_Setup_Train(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Setup_Train();
	void SetPointer(CDatenBankLok* pZugData);
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DLG_SETUP_TRAIN };

protected:
	CDlg_CV_Action			Dlg_CV;
	CDlg_Liste_Funktion*	pDlgListFunktion;
	CComboBox				Lok_Liste_Gesamt;
	CDatenBankLok			*Zug_Data;
	CDecoder_All_Infos      DecoderInfo;
	CListCtrl				m_List_CVs;
	CImageList				m_ListImageList;
	byte					selectet_Dlg_Lok;      //Spalte der ausgewählten Lok
	DecoderTypen			selectet_DecoderType;
	CString					selectet_LokName;
	std::vector<CString>	selectet_LokGroup;

	virtual void   DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void NeueGruppeDaten();
	void ChangeHeader();
	void Fill_Zug_Liste_CVData();
	DECLARE_MESSAGE_MAP()
public:

	CProgressCtrl PositionAuslesen;

	afx_msg void OnSelchangeComboAusgewaelteLok();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAlleCVsLesen();
	afx_msg void OnBnClickedButtonFunktionListe();
};
