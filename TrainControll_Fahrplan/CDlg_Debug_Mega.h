#pragma once


// CDlg_Debug_Mega-Dialog

class CDlg_Debug_Mega : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Debug_Mega)
private:
	using CDialog::Create;

public:
	CDlg_Debug_Mega(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_Debug_Mega();
	BOOL Create();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	bool OpenCom(int Port);
	void CloseCom();
	CString Get_Message();
	void UpdateNewData(CString Textin);
// Dialogfelddaten
	enum { IDD = IDD_CDlg_Debug_Mega };
	bool ListentoCom;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	CWnd* m_pParent;
	int m_nID;
	CString Debug_Text[50];
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	CFont Font_Info;
	const int maxZeile = 50;


	DECLARE_MESSAGE_MAP()
public:
};
