// Dlg_Test_Weiche.cpp: Implementierungsdatei
//
#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "Dlg_Test_Weiche.h"
#include "afxdialogex.h"


// CDlg_Test_Weiche-Dialogfeld

IMPLEMENT_DYNAMIC(CDlg_Test_Weiche, CDialogEx)

CDlg_Test_Weiche::CDlg_Test_Weiche(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_WEICHE_TESTEN, pParent)
{

}

CDlg_Test_Weiche::~CDlg_Test_Weiche()
{
}

void CDlg_Test_Weiche::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Setting_Test);
}


BEGIN_MESSAGE_MAP(CDlg_Test_Weiche, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_W1, IDC_CHECK_W32, &CDlg_Test_Weiche::OnBnClickedFunktion)
	ON_STN_CLICKED(IDC_INFO_LOK, &CDlg_Test_Weiche::OnStnClickedInfoLok)
END_MESSAGE_MAP()


// CDlg_Test_Weiche-Meldungshandler
void CDlg_Test_Weiche::OnBnClickedFunktion(UINT nID)
{
	UpdateData(true);
	//byte bit;
	int index;
	byte xMal;
	index = nID - IDC_CHECK_W1 +1 ;
	CString Text;
	if (Setting_Test.GetCurSel() == 0)
	{
		//bit = Gleis_Data->SchalteWeicheTest(index);
		//static_cast<CButton*>(GetDlgItem(index + IDC_CHECK_W1 - 1))->SetCheck(bit);
		//Text.Format(_T("Schalte Weiche: %2i"), index);
		SetDlgItemTextW(IDC_COUNTER, Text);
	}
	else
	{
		xMal = Setting_Test.GetCurSel() * 10;
		for (int i = 0; i < xMal; i++)
		{
			//bit = Gleis_Data->SchalteWeicheTest(index);
			//static_cast<CButton*>(GetDlgItem(index+ IDC_CHECK_W1 - 1))->SetCheck(bit);
			if (xMal == 10) 
				Sleep(2000);
			else
				Sleep(1000);
			SetDlgItemTextW(IDC_COUNTER, Text);
		}
	}
}




BOOL CDlg_Test_Weiche::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//bool bit;
	//int Item_Nr = IDC_CHECK_W1-1;

	/*
	for (int i = 1; i < 33; i++)
	{
		//bit = Gleis_Data->Get_Stellung_Weiche(i);
		static_cast<CButton*>(GetDlgItem(Item_Nr + i))->SetCheck(bit);
	}
	Setting_Test.SetCurSel(0);
	*/
	return TRUE; 
}


void CDlg_Test_Weiche::OnStnClickedInfoLok()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}
