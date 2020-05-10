#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CDlg_Fahrplan.h"
#include "afxdialogex.h"


// CDlg_Fahrplan-Dialog

IMPLEMENT_DYNAMIC(CDlg_Fahrplan, CDialogEx)

CDlg_Fahrplan::CDlg_Fahrplan(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FAHRPLAN, pParent)
{

}

CDlg_Fahrplan::~CDlg_Fahrplan()
{
}

void CDlg_Fahrplan::SetPointer(CDataBlock * BlDa)
{
	Block_Data = BlDa;
}

void CDlg_Fahrplan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Fahrplan, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_BL1, IDC_CHECK_BL4, &CDlg_Fahrplan::OnBnClickedCheckBlock)
END_MESSAGE_MAP()


// CDlg_Fahrplan-Meldungshandler


void CDlg_Fahrplan::OnBnClickedCheckBlock(UINT nID)
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}
