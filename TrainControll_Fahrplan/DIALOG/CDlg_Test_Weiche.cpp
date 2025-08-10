// Dlg_Test_Weiche.cpp: Implementierungsdatei
//
#include "pch.h"
#include "TrainControll_FahrplanDlg.h"
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
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	CCom_BlockMelderNet* BlockMelder = &APP->BlockMelder;
	TrainCon_Paar Antrieb;
	CString Text;
	byte xMal;

	bool bit;
	UpdateData(true);
	
	Antrieb.SetWert( nID - IDC_CHECK_W1 + 1);
	bit = APP->Gleis_Data.Get_Weiche(Antrieb);
	Antrieb.SetBit(bit);
	if (Setting_Test.GetCurSel() == 0)
	{
		Antrieb.InvBit();
		BlockMelder->Send_WeichenData(Antrieb);
		static_cast<CButton*>(GetDlgItem(Antrieb.GetWert() + IDC_CHECK_W1 - 1))->SetCheck(Antrieb.GetBit());
		Text.Format(_T("Schalte Weiche: %2i"), Antrieb.GetWert());
		SetDlgItemTextW(IDC_COUNTER, Text);
	}
	else
	{
		xMal = Setting_Test.GetCurSel() * 10;
		for (int i = 0; i < xMal; i++)
		{
			Antrieb.InvBit();
			BlockMelder->Send_WeichenData(Antrieb);
			static_cast<CButton*>(GetDlgItem(Antrieb.GetWert() + IDC_CHECK_W1 - 1))->SetCheck(Antrieb.GetBit());
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
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	
	TrainCon_Paar Antrieb;

	bool bit;
	
	int Item_Nr = IDC_CHECK_W1-1;

	// 1..32
	for (byte i = 1; i < APP->Gleis_Data.Get_Weichen_Anzahl(); i++)
	{
		Antrieb.SetWert(i);
		bit = APP->Gleis_Data.Get_Weiche(Antrieb);
		AktiveWeichenSetting[i-1] = bit;
		static_cast<CButton*>(GetDlgItem(IDC_CHECK_W1 + i-1))->SetCheck(bit);
	}
	Setting_Test.SetCurSel(0);
	return TRUE; 
}


void CDlg_Test_Weiche::OnStnClickedInfoLok()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
}
