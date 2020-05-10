// CDlg_RunCam_Train.cpp: Implementierungsdatei
//
#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "CDlg_RunCam_Train.h"
#include "afxdialogex.h"
#include "resource.h"


// CDlg_RunCam_Train-Dialog

IMPLEMENT_DYNAMIC(CDlg_RunCam_Train, CDialogEx)

CDlg_RunCam_Train::CDlg_RunCam_Train(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CAM, pParent)
{
}

CDlg_RunCam_Train::~CDlg_RunCam_Train()
{
}

void CDlg_RunCam_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CDlg_RunCam_Train, CDialogEx)

END_MESSAGE_MAP()

// CDlg_RunCam_Train-Meldungshandler





BOOL CDlg_RunCam_Train::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	if (LokFenster.SucheKamera(_T("Integrated Webcam")))
	{
		LokFenster.StarteKamera();
		LokFenster.Set_Bild_inDlg(*GetDlgItem(IDC_FENSTER));
		LokFenster.StartBild(true);
	}

	return TRUE;  
}

void CDlg_RunCam_Train::SetLokPointer(CDatenBankLok * pGData, CString LokName)
{
	Zug_Data = pGData;
	Lok_Name = LokName;
	Lok_Nr = Zug_Data->Get_Pos_LokName(Lok_Name);
}

void CDlg_RunCam_Train::SetInfo(CString Text)
{
}
