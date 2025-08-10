#pragma once


// CDlg_Debug_Mega-Dialog

class CDlg_Debug_Mega : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Debug_Mega)

public:
	CDlg_Debug_Mega(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Debug_Mega();

	virtual BOOL OnInitDialog();
	virtual BOOL Create(HANDLE* Data);
	bool OpenCom(HANDLE* Port);
	void Get_Message();
	// Dialogfelddaten
	enum { IDD = IDD_CDlg_Debug_Mega };
	bool ListentoCom;
	CListCtrl DebugTextListe;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	CWnd* m_pParent;
	int m_nID;
	HANDLE* p_hCom;
	const unsigned int maxZeile = 50;
	unsigned int Zeile = 0;

	DECLARE_MESSAGE_MAP()
};
