// Dlg_Liste_Train.cpp: Implementierungsdatei
//
#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "Dlg_Liste_Train.h"
#include "afxdialogex.h"


// CDlg_Liste_Train-Dialogfeld

IMPLEMENT_DYNAMIC(CDlg_Liste_Train, CDialog)

CDlg_Liste_Train::CDlg_Liste_Train(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_LISTE_TRAIN, pParent)
{

}

CDlg_Liste_Train::~CDlg_Liste_Train()
{
}

void CDlg_Liste_Train::Set_TrainData(CDatenBankLok * Data)
{
	Zug_Data = Data;
}
		


void CDlg_Liste_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Zug_Liste);
}


BEGIN_MESSAGE_MAP(CDlg_Liste_Train, CDialog)
END_MESSAGE_MAP()


// CDlg_Liste_Train-Meldungshandler




BOOL CDlg_Liste_Train::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i = 0;
	CString		strText;
	Zug_Liste.InsertColumn(0, _T("Pos."), LVCFMT_CENTER, 30, 0);
	Zug_Liste.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100, 0);
	Zug_Liste.InsertColumn(2, _T("Block"), LVCFMT_CENTER, 40, 0);
	Zug_Liste.InsertColumn(3, _T("Gleis"), LVCFMT_CENTER, 40, 0);
	Zug_Liste.InsertColumn(4, _T("DCC Adr"), LVCFMT_CENTER, 60, 0);
	Zug_Liste.InsertColumn(5, _T("Decoder-Type"), LVCFMT_LEFT, 130, 0);
	Zug_Liste.InsertColumn(6, _T("Dec. Herst."), LVCFMT_LEFT, 140, 0);
	Zug_Liste.InsertColumn(7, _T("DecNr"), LVCFMT_CENTER, 45, 0);
	Zug_Liste.InsertColumn(8, _T("Dec Herst."), LVCFMT_CENTER, 45, 0);
	for (CDataXpressNet Z : Zug_Data->MeineZüge)
	{
		strText.Format(_T(" %2i "), i); //= Zug_Data->Name;
		Zug_Liste.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText, 0, LVIS_SELECTED, 0, 0);
		Zug_Liste.SetItemText(i, 1, Z.Name);

		Zug_Liste.SetItemText(i, 2,Z.Text_Block());

		Zug_Liste.SetItemText(i, 3, Z.Text_Betrieb());

		Zug_Liste.SetItemText(i, 4, Z.Text_Adresse());

		Zug_Liste.SetItemText(i, 5, Z.Text_Decoder());

		Zug_Liste.SetItemText(i, 6, Z.Text_Hersteller());

		Zug_Liste.SetItemText(i, 7, Z.Text_Decoder_Sub());
		i++;
	}


	return TRUE;  
}




