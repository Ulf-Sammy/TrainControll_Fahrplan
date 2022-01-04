#pragma once


// CDlg_Block_Info-Dialog

class CDlg_Block_Info : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Block_Info)

public:
	CDlg_Block_Info(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Block_Info();
	virtual BOOL Create();

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_TESTBLOCK };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	void Zeige_Daten();
	CWnd* m_pParent;
	int m_nID;
	CDC* pDC = NULL;
	CRect m_rectCtrl;
	byte Block_Nr;
	bool BlickLok;
	bool Lok_fahren_L = false;
	bool Lok_fahren_R = false;
	BlockDebugData BlockData;
	byte Weichen_Anzahl;
	DECLARE_MESSAGE_MAP()
	CString BlockN;
	CString BlockV;
	CString LokName;
	CString Beschriftung;
public:
	CSpinButtonCtrl Spin_BlockNr;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void Zeichen_BlockInfo(CRect R);
	virtual BOOL OnInitDialog();
	void NeueDaten();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton4();
};
