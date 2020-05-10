#pragma once
#include "pch.h"
#include "CBlockData.h"
#include <vector>
// CDlg_Plan_Liste_Train-Dialog

class CDlg_Plan_Liste_Train : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Plan_Liste_Train)

public:
	CDlg_Plan_Liste_Train(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Plan_Liste_Train();
	void SetData(byte Anzahl, CDataBlock *Block);
	void Fill_Tabelle(byte Plan_Nr);
	void Get_FahrPlan_Befehl();
	void Set_Befehl_Dlg(FahrPlanDo Was);
// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlg_Plan_Liste_Train };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	int alt_Item;
	DECLARE_MESSAGE_MAP()

	std::vector <Weichen_Gruppe>W_Gruppe;
	CListCtrl FahrplanTabelle;
	CDataFahrplan FahrplanData;
	CComboBox Combo_Plan_Nr;
	CComboBox Combo_Was;
	byte Plan_Nr;
	CDataBlock *Block;
	byte Fahrplan_Anzahl;
	int Z_Nr;
	FahrplanPos Befehl;
	void SucheWegStrecke();

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboWas();
	afx_msg void OnCbnSelchangeComboNr();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	afx_msg void OnEnKillfocusEditName();
};
