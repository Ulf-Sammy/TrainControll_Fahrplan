// CDlg_Plan_Liste_Train.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "Dlg_Plan_Liste_Train.h"
#include "afxdialogex.h"


// CDlg_Plan_Liste_Train-Dialog

IMPLEMENT_DYNAMIC(CDlg_Plan_Liste_Train, CDialog)


BEGIN_MESSAGE_MAP(CDlg_Plan_Liste_Train, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_WAS, &CDlg_Plan_Liste_Train::OnCbnSelchangeComboWas)
	ON_CBN_SELCHANGE(IDC_COMBO_NR, &CDlg_Plan_Liste_Train::OnCbnSelchangeComboNr)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlg_Plan_Liste_Train::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlg_Plan_Liste_Train::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlg_Plan_Liste_Train::OnBnClickedButtonAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlg_Plan_Liste_Train::OnLvnItemchangedList1)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CDlg_Plan_Liste_Train::OnEnKillfocusEditName)
END_MESSAGE_MAP()



CDlg_Plan_Liste_Train::CDlg_Plan_Liste_Train(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDlg_Plan_Liste_Train, pParent)
{
	alt_Item = -1;
}

CDlg_Plan_Liste_Train::~CDlg_Plan_Liste_Train()
{
}

void CDlg_Plan_Liste_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, FahrplanTabelle);
	DDX_Control(pDX, IDC_COMBO_NR, Combo_Plan_Nr);
	DDX_Control(pDX, IDC_COMBO_WAS, Combo_Was);
}

void CDlg_Plan_Liste_Train::SetData( byte Anzahl)
{
	Fahrplan_Anzahl = Anzahl;
}
void CDlg_Plan_Liste_Train::SucheWegStrecke()
{
	for (int i = 0; i < MAX_WEICHEN_WEGE; i++)
	{
	}
}
BOOL CDlg_Plan_Liste_Train::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Combo_Was.AddString(_T("Zug stoppen"));
	Combo_Was.AddString(_T("Zug vorwärtz fahren"));
	Combo_Was.AddString(_T("Zug rückwärtz fahren"));
	Combo_Was.AddString(_T("Zug warten dann fahren"));
	Combo_Was.AddString(_T("Zug warten dann stoppen"));
	Combo_Was.AddString(_T("Zug Funktion schalten"));
	Combo_Was.AddString(_T("Weiche schalten"));
	

	CString Text;
	
	for (int i = 0; i < Fahrplan_Anzahl; i++)
	{
		Text.Format(_T("Nr.: %2i"), i);
		Combo_Plan_Nr.AddString(Text);
	}
	
	FahrplanTabelle.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP);//LVS_EX_FULLROWSELECT);
	FahrplanTabelle.InsertColumn(0, _T("Pos."), LVCFMT_CENTER, 40, 0);
	FahrplanTabelle.InsertColumn(1, _T("Mel."), LVCFMT_CENTER, 50, 0);
	FahrplanTabelle.InsertColumn(2, _T("Was machen:"), LVCFMT_LEFT, 230, 0);
	FahrplanTabelle.InsertColumn(3, _T("W.-A"), LVCFMT_CENTER, 60, 0);
	FahrplanTabelle.InsertColumn(4, _T("W.-B"), LVCFMT_CENTER, 50, 0);

	Combo_Plan_Nr.SetCurSel(0);
	Plan_Nr = Combo_Plan_Nr.GetCurSel();

	Fill_Tabelle(Plan_Nr);
	Z_Nr = FahrplanTabelle.GetItemCount() -1;
	FahrplanTabelle.SetHotItem(Z_Nr);
	return TRUE;
}

void CDlg_Plan_Liste_Train::Fill_Tabelle(byte Plan_Nr)
{
}

void CDlg_Plan_Liste_Train::Get_FahrPlan_Befehl()
{
	byte Melder = GetDlgItemInt(IDC_EDIT_MELDER);
	int WertA = GetDlgItemInt(IDC_EDIT_WERTA);
	int WertB = GetDlgItemInt(IDC_EDIT_WERTB);
	FahrPlanDo Do = (FahrPlanDo)(Combo_Was.GetCurSel()+1);
	//Befehl = FahrplanPos(Melder,Do , WertA, WertB);
}

void CDlg_Plan_Liste_Train::Set_Befehl_Dlg(FahrPlanDo Was)
{
	CString Text_WertA;
	CString Text_WertB;
	bool bWertA;
	bool bWertB;
	bool bWas;
	bool bMelder;
	switch (Was)
	{
	case FahrPlanDo::begin_Block:
		Text_WertA = _T("Start Block Nr.:");
		Text_WertB = _T("Blickrichtung  :");
		bWertA = true;
		bWertB = true;
		bWas = false;
		bMelder =false;
		break;
	case FahrPlanDo::stoppen:
		Text_WertA = _T("");
		Text_WertB = _T("");
		bWertA = false;
		bWertB = false;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::vorwaerz_fahren:
		Text_WertA = _T("Die Geschwindigkeit : ");
		Text_WertB = _T("");
		bWertA = true;
		bWertB = false;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::rueckwaerz_fahren:
		Text_WertA = _T("Die Geschwindigkeit:");
		Text_WertB = _T("");
		bWertA = true;
		bWertB = false;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::warten_fahren:
		Text_WertA = _T("Zeit Wert in msec:");
		Text_WertB = _T("");
		bWertA = true;
		bWertB = false;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::warten_stoppen:
		Text_WertA = _T("Zeit Wert in msec:");
		Text_WertB = _T("");
		bWertA = true;
		bWertB = false;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::schalten_Funk:
		Text_WertA = _T("Die Funktions Nummer:");
		Text_WertB = _T("schalten:   ein / aus");
		bWertA = true;
		bWertB = true;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::schalten_Weiche:
		Text_WertA = _T("Den Weg von Block:");
		Text_WertB = _T("        zum Block: ");
		bWertA = true;
		bWertB = true;
		bWas = true;
		bMelder = true;
		break;
	case FahrPlanDo::letzte_Zeile:
		Text_WertA = _T("");
		Text_WertB = _T("");
		bWertA = false;
		bWertB = false;
		bWas = false;
		bMelder = false;
		break;
	default:
		break;
	}
	SetDlgItemTextW(IDC_TEXT_WERTA, Text_WertA);
	SetDlgItemTextW(IDC_TEXT_WERTB, Text_WertB);
	GetDlgItem(IDC_EDIT_MELDER)->EnableWindow(bMelder);
	GetDlgItem(IDC_EDIT_WERTA)->EnableWindow(bWertA);
	GetDlgItem(IDC_EDIT_WERTB)->EnableWindow(bWertB);
	GetDlgItem(IDC_COMBO_WAS)->EnableWindow(bWas);
}

void CDlg_Plan_Liste_Train::OnCbnSelchangeComboWas()
{
	FahrPlanDo PlanDo;
	PlanDo = (FahrPlanDo)(Combo_Was.GetCurSel()+1);
	Set_Befehl_Dlg(PlanDo);
}

void CDlg_Plan_Liste_Train::OnCbnSelchangeComboNr()
{
	Plan_Nr = Combo_Plan_Nr.GetCurSel();
	FahrplanTabelle.DeleteAllItems();
	Fill_Tabelle(Plan_Nr);
	Z_Nr = -1;
}


void CDlg_Plan_Liste_Train::OnBnClickedButtonDel()
{
	
}
void CDlg_Plan_Liste_Train::OnBnClickedButtonEdit()
{
}
void CDlg_Plan_Liste_Train::OnBnClickedButtonAdd()
{
}

void CDlg_Plan_Liste_Train::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int neu_Item = pNMLV->iItem;
	CString Text;
	//FahrplanPos Befehl;
	*pResult = 0;
}

void CDlg_Plan_Liste_Train::OnEnKillfocusEditName()
{
	CString Neu_Name;
	GetDlgItemText(IDC_EDIT_NAME,Neu_Name);
}

void CDlg_Plan_Liste_Train::OnCancel()
{
	CDialog::OnCancel();
}
