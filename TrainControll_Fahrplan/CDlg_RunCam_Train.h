#pragma once
#include "CaptureGraph.h"
#include "pch.h"

#pragma comment (lib, "strmiids")

// CDlg_RunCam_Train-Dialog

class CDlg_RunCam_Train : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_RunCam_Train)

public:
	CDlg_RunCam_Train(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~CDlg_RunCam_Train();
	virtual BOOL OnInitDialog();
	void SetLokPointer(CDatenBankLok* PGData, CString LokName);
	void SetInfo(CString Text);
// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	CameraBild		LokFenster;
	CDatenBankLok   *Zug_Data = NULL;
	CString			Lok_Name;
	byte			Lok_Nr = 0;
	DECLARE_MESSAGE_MAP()
};
