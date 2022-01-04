#pragma once



// CDlg_Com_Liste-Dialog

class CDlg_Com_Liste : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Com_Liste)

public:
	CDlg_Com_Liste(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Com_Liste();

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_COM_LISTE };

protected:
	int m_nID;
	CWnd* m_pParent;
	COM_Info* pCom_Liste;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(COM_Info* Data);
	virtual BOOL OnInitDialog();
	CListCtrl View_Com_Liste;
};
