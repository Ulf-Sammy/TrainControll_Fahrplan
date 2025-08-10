// Dlg_Liste_Funktion.cpp: Implementierungsdatei
//
#include "pch.h"
#include "Dlg_Liste_Funktion.h"
//#include "afxdialogex.h"


// CDlg_Liste_Funktion-Dialogfeld

IMPLEMENT_DYNAMIC(CDlg_Liste_Funktion, CDialog)

BEGIN_MESSAGE_MAP(CDlg_Liste_Funktion, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlg_Liste_Funktion::OnClickList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CDlg_Liste_Funktion::OnKeydownList1)
	ON_MESSAGE(WM_NOTIFY_DESCRIPTION_EDITED, OnNotifyDescriptionEdited)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlg_Liste_Funktion::OnBnClickedButtonChange)
END_MESSAGE_MAP()

CDlg_Liste_Funktion::CDlg_Liste_Funktion(CWnd* pParent )
	: CDialog(CDlg_Liste_Funktion::IDD, pParent)
{
	View_1 = true;
	View_2 = false;
	Do_Edit = false;
}

CDlg_Liste_Funktion::~CDlg_Liste_Funktion()
{
	Zug_Data->Save_Zug_Data();
}

void CDlg_Liste_Funktion::SetPointer(CDatenBankLok * pZData)
{
	Zug_Data	= pZData;
}

BOOL CDlg_Liste_Funktion::OnInitDialog()
{
	CDialog::OnInitDialog();
	Zeile = 0;
	Fill_Liste_Anschluss();
	Liste_FunktionsTasten.SetExtendedStyle(Liste_FunktionsTasten.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
	Liste_FunktionsTasten.SetHotItem(Zeile);
	Liste_FunktionsTasten.SetFocus();
	return 0;
}

void CDlg_Liste_Funktion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Liste_FunktionsTasten);
}

void CDlg_Liste_Funktion::Clear_Tabelle()
{
	int nColumnCount = Liste_FunktionsTasten.GetHeaderCtrl()->GetItemCount();

	for (int i = 0; i < nColumnCount; i++)
	{
		Liste_FunktionsTasten.DeleteColumn(0);
	}
	Liste_FunktionsTasten.DeleteAllItems();
}


void CDlg_Liste_Funktion::Fill_Liste_Anschluss()
{
	CString Text;
	byte Zeile = 0;
	byte CV;

	Liste_FunktionsTasten.InsertColumn(0, _T("Nr"), LVCFMT_RIGHT, 40, 0);
	Liste_FunktionsTasten.InsertColumn(1, _T("Ausgang"), LVCFMT_RIGHT, 70, 0);
	Liste_FunktionsTasten.InsertColumn(2, _T("Beschreibung "), LVCFMT_CENTER, 300, 0);
	Liste_FunktionsTasten.InsertColumn(3, _T("ON"), LVCFMT_CENTER, 40, 0);
	Liste_FunktionsTasten.InsertColumn(4, _T("Taste"), LVCFMT_CENTER, 50, 0);
	Liste_FunktionsTasten.InsertColumn(5, _T("CV Adr"), LVCFMT_CENTER, 60, 0);
	Liste_FunktionsTasten.InsertColumn(6, _T("CV Wert"), LVCFMT_CENTER, 50, 0);

	for (byte I :Zug_Data->Get_Sel_Zug_Decoder().Pointer_Funktion())
	{
		Text.Format(_T("%2i.)"), Zeile);
		Liste_FunktionsTasten.InsertItem(LVIF_TEXT | LVIF_STATE, Zeile, Text, 0, LVIS_SELECTED, 0, 0);
		Liste_FunktionsTasten.SetItemText(Zeile, 1, Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).Text_AusgangInfo());
		Liste_FunktionsTasten.SetItemText(Zeile, 2, Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).Text_Name());
		Liste_FunktionsTasten.SetItemText(Zeile, 3, Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).Text_Zeigen());
		Liste_FunktionsTasten.SetItemText(Zeile, 4, Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).TextTaste());
		Liste_FunktionsTasten.SetItemText(Zeile, 5, Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).Text_CV());
		CV = Zug_Data->Get_Sel_Zug_Decoder().Get_Funktion().at(Zeile).TasteCV;
		Liste_FunktionsTasten.SetItemText(Zeile, 6, Zug_Data->Get_Sel_Zug_Decoder().Text_CVWert(CV));
		Zeile++;
	}
}

void CDlg_Liste_Funktion::Fill_Liste_FunktionTasten()
{
	CString Text;
	byte Zeile = 0;
	std::vector<Function_A> Tab;
	Tab = Zug_Data->Get_Sel_Zug_Decoder().Get_Aktive_Funktion();
	Liste_FunktionsTasten.InsertColumn(0, _T("Nr"), LVCFMT_RIGHT, 40, 0);
	Liste_FunktionsTasten.InsertColumn(1, _T("Beschreibung "), LVCFMT_CENTER, 300, 0);
	Liste_FunktionsTasten.InsertColumn(2, _T("Taste"), LVCFMT_CENTER, 50, 0);
	for (Function_A I : Zug_Data->Get_Sel_Zug_Decoder().Get_Aktive_Funktion())
	{
		Text.Format(_T("%2i.)"), Zeile);
		Liste_FunktionsTasten.InsertItem(LVIF_TEXT | LVIF_STATE, Zeile, Text, 0, LVIS_SELECTED, 0, 0);
		Liste_FunktionsTasten.SetItemText(Zeile, 1, I.Text_Beschreibung());
		Liste_FunktionsTasten.SetItemText(Zeile, 2, I.Text_Taste());
		Zeile++;
	}
}

LRESULT CDlg_Liste_Funktion::OnNotifyDescriptionEdited(WPARAM wParam, LPARAM lParam)
{
	LV_DISPINFO* dispinfo = reinterpret_cast<LV_DISPINFO*>(lParam);

	CString Nr;
	int I;
	bool B;
	
	I = _ttoi(Liste_FunktionsTasten.GetItemText(dispinfo->item.iItem, 0).Mid(0, 2));

	Liste_FunktionsTasten.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, dispinfo->item.pszText);
	if (dispinfo->item.iSubItem == 2)
	{
		Zug_Data->Set_Sel_Zug_Decoder_Funktion_Name(I,dispinfo->item.pszText);
	}
	if (dispinfo->item.iSubItem == 3)
	{
		Nr = dispinfo->item.pszText;
		if ((Nr.Compare(_T("O")) == 0) || Nr.Compare(_T("1")) == 0)
		{
			B = true;
			Liste_FunktionsTasten.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, _T("on"));
		}
		else
		{
			B = false;
			Liste_FunktionsTasten.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, _T("--"));
		}
		Zug_Data->Set_Sel_Zug_Decoder_Funktion_Eigen(I, B, true);
	}
	return 0;
}

void CDlg_Liste_Funktion::OnBnClickedButtonChange()
{
	View_1 = View_2;
	View_2 = !View_2;
	Clear_Tabelle();
	if (View_1)
	{
		Fill_Liste_Anschluss();
	}
	if (View_2)
	{
		Zug_Data->Sort_Sel_Zug_FunktionsTasten();
		Fill_Liste_FunktionTasten();
	}
}

void CDlg_Liste_Funktion::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rect;
	DWORD pos = GetMessagePos();
	CPoint pt(LOWORD(pos), HIWORD(pos));
	ScreenToClient(&pt);
	Do_Edit = true;
	CWnd *pWnd = GetDlgItem(IDC_LIST1);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	pt.x -= rect.left;
	pt.y -= rect.top;
	Liste_FunktionsTasten.OnLButtonDown(MK_LBUTTON, pt, &Zeile);
	*pResult = 0;
}

void CDlg_Liste_Funktion::OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString Nr;
	int I;

	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if ((pLVKeyDow->wVKey == 40) && (Do_Edit))
	{
		if (Zeile < (MAX_FUNKTION - 1)) Zeile++;
	}
	else Do_Edit = true;
	if ((pLVKeyDow->wVKey == 38)&&(Do_Edit))
	{
		if (Zeile != 0 ) Zeile--; 
	}
	I = _ttoi(Liste_FunktionsTasten.GetItemText(Zeile, 0).Mid(0, 2));
	if (pLVKeyDow->wVKey == 113) // Taste F2
	{
		Liste_FunktionsTasten.OnKeydown(Zeile, 2);
	}
	if (pLVKeyDow->wVKey == 114) // Taste F3
	{
		Liste_FunktionsTasten.SetItemText(Zeile, 3, _T("--"));
		Zug_Data->Set_Sel_Zug_Decoder_Funktion_Eigen(I, false, true);
	}
	if (pLVKeyDow->wVKey == 115) // Taste F4
	{
		Liste_FunktionsTasten.SetItemText(Zeile, 3, _T("on"));
		Zug_Data->Set_Sel_Zug_Decoder_Funktion_Eigen(I, true, true);
	}
	*pResult = 0;
}
