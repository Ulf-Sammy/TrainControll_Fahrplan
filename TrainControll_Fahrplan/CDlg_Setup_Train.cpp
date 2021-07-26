// Dlg_Setup_Train.cpp : implementation file
//
#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "Dlg_Setup_Train.h"
#include "afxdialogex.h"


// CDlg_Setup_Train dialog

IMPLEMENT_DYNAMIC(CDlg_Setup_Train, CDialogEx)

BEGIN_MESSAGE_MAP(CDlg_Setup_Train, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlg_Setup_Train::OnDblclkList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CDlg_Setup_Train::OnColumnclickList1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Setup_Train::OnSelchangeComboAusgewaelteLok)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_Setup_Train::OnBnClickedButtonAlleCVsLesen)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_Setup_Train::OnBnClickedButtonFunktionListe)
END_MESSAGE_MAP()


CDlg_Setup_Train::CDlg_Setup_Train(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Setup_Train::IDD, pParent)
{
	selectet_DecoderType = DecoderTypen::NoDecoder;
	selectet_LokName     = "";
}

CDlg_Setup_Train::~CDlg_Setup_Train()
{
		Zug_Data->Save_Zug_Data();
}
void CDlg_Setup_Train::SetPointer(CDatenBankLok * pZugData)
{
	Zug_Data	= pZugData;
}

void CDlg_Setup_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_CVs);
	DDX_Control(pDX, IDC_COMBO1, Lok_Liste_Gesamt);
	DDX_Control(pDX, IDC_PROGRESS1, PositionAuslesen);
}

void CDlg_Setup_Train::NeueGruppeDaten()
{
	selectet_DecoderType = Zug_Data->Get_Sel_ZugDecoder_Type();
	selectet_LokGroup = Zug_Data->Get_LokGruppe_Decoder(selectet_DecoderType);

	DecoderInfo.Set_DecoderType(selectet_DecoderType);
	DecoderInfo.Load_Data();

	Fill_Zug_Liste_CVData();

}

BOOL CDlg_Setup_Train::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListImageList.Create(16, 16, ILC_COLOR, 2, 2);
	m_ListImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
    m_ListImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));


	Zug_Data->Fill_Liste_Zug(&Lok_Liste_Gesamt);

	NeueGruppeDaten();

	OnSelchangeComboAusgewaelteLok();

	return TRUE;
}

void CDlg_Setup_Train::ChangeHeader()
{
	HD_ITEM curItem;
	int selLok =  3;
	CHeaderCtrl* pHdrCtrl= NULL;
	pHdrCtrl= m_List_CVs.GetHeaderCtrl();
	pHdrCtrl->SetImageList(&m_ListImageList);
	memset(&curItem, 0, sizeof(curItem));
	
	for (CString Z : selectet_LokGroup)
	{
		pHdrCtrl->GetItem(selLok, &curItem);
		curItem.mask = HDI_IMAGE | HDI_FORMAT;
		curItem.cxy = 100;
		if (selectet_LokName == Z)
		{
			curItem.iImage = 1;
			selectet_Dlg_Lok = selLok;
		}
		else
		{
			curItem.iImage = 0;
		}
		curItem.fmt = HDF_LEFT | HDF_IMAGE | HDF_STRING;
		pHdrCtrl->SetItem(selLok, &curItem);
		selLok++;
	}
	SetDlgItemTextW(IDC_SEL_DECODER, Zug_Data->Get_Sel_Zug_Decoder().Text_Decoder_Type());
	SetDlgItemTextW(IDC_SEL_ZUG, selectet_LokName);
}

void CDlg_Setup_Train::Fill_Zug_Liste_CVData()
{
	CString		Text;
	int Zeile = 0;
	int Spalte = 3;

	m_List_CVs.DeleteAllItems();
	int colum = m_List_CVs.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < colum; i++)
	{
		m_List_CVs.DeleteColumn(0);
	}
	
	m_List_CVs.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_CVs.SetOutlineColor(0x000000FF);
	m_List_CVs.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_List_CVs.InsertColumn(0, _T("CV-Nr"), LVCFMT_CENTER, 60, 0);
	m_List_CVs.InsertColumn(1, _T("CV-Beschreibung"), LVCFMT_LEFT, 300, 1);
	m_List_CVs.InsertColumn(2, _T("CV Std."), LVCFMT_CENTER, 60, 2);
	for (byte D : DecoderInfo.Pointer_MaxCV())
	{
		m_List_CVs.InsertItem(LVIF_TEXT | LVIF_STATE, Zeile , DecoderInfo.Docu_Data.at(D).Text_CV(), 0, LVIS_SELECTED, 0, 0);
		m_List_CVs.SetItemText(Zeile, 1, DecoderInfo.Docu_Data.at(D).Text_Beschreibung());
		m_List_CVs.SetItemText(Zeile, 2, DecoderInfo.Docu_Data.at(D).Text_Wertebereich());
		Zeile++;
	}
	for (CDataXpressNet Z : Zug_Data->MeineZüge)
	{
		if (Z.Decoder_Data.isDecoderType(selectet_DecoderType))
		{
			m_List_CVs.InsertColumn(Spalte, Z.Name, LVCFMT_CENTER, 100, 2);
			Zeile = 0;
			for (byte D : DecoderInfo.Pointer_MaxCV())
			{
				m_List_CVs.SetItemText(Zeile, Spalte, Z.Decoder_Data.Text_CVWert(D));
				Zeile++;
			}
			Spalte++;
		}
	}
}


void CDlg_Setup_Train::OnSelchangeComboAusgewaelteLok()
{
	bool neueGruppe = true;
	Zug_Data->Set_Prog_Zug(Lok_Liste_Gesamt.GetCurSel());
	Lok_Liste_Gesamt.GetLBText(Zug_Data->Get_Prog_Zug(), selectet_LokName);
	for (CString F : selectet_LokGroup)
	{
		if (F == selectet_LokName) neueGruppe = false;
	}
	if (neueGruppe)
	{
		NeueGruppeDaten();
	}
	ChangeHeader();
}

void CDlg_Setup_Train::OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);
	byte Lok_Pos;
	if (pNMLV->iSubItem > 2)
	{
		selectet_LokName = selectet_LokGroup[pNMLV->iSubItem - 3];
		Lok_Pos = Zug_Data->Get_Pos_LokName(selectet_LokName);
		Lok_Liste_Gesamt.SetCurSel(Zug_Data->Get_Prog_Zug());
		ChangeHeader();
	}
	*pResult = 0;
}

void CDlg_Setup_Train::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{ // ändere CV Daten
	UNREFERENCED_PARAMETER(pResult);

	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pNMHDR;
	LVHITTESTINFO lvhti;
	CString TextCVWert, TextCV;
	byte CV, CV_Wert, neuCV_Wert;
	int CV_Pos;
	CRect rect;
	INT_PTR nRet = -1;


	lvhti.pt = pia->ptAction;
	m_List_CVs.SubItemHitTest(&lvhti);
	CV_Pos  = lvhti.iItem;
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		CV		= _ttoi(m_List_CVs.GetItemText(CV_Pos, 0));
		CV_Wert = _ttoi(m_List_CVs.GetItemText(CV_Pos, selectet_Dlg_Lok));
		m_List_CVs.SetItemText(CV_Pos, selectet_Dlg_Lok, 0);
		m_List_CVs.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		m_List_CVs.SetItemState(CV_Pos, LVIS_SELECTED, LVIS_SELECTED);
	
		Dlg_CV.SetPointer(Zug_Data,&DecoderInfo);
		Dlg_CV.Set_Lok(CV, CV_Wert);

		if (Dlg_CV.DoModal() ==IDOK)
		{
			neuCV_Wert = Dlg_CV.GetCVValue();
			if (CV_Wert != neuCV_Wert)
			{
				CV_Wert = neuCV_Wert;
				Zug_Data->Set_Zug_CV_Wert(CV, CV_Wert);
			}
		}
		TextCVWert.Format(_T("%3d"),CV_Wert);
		m_List_CVs.SetItemText(CV_Pos, selectet_Dlg_Lok, TextCVWert);
	}
}

void CDlg_Setup_Train::OnBnClickedButtonAlleCVsLesen()
{
	CString strText;
	CRect   rect;
	byte	CV, CV_Wert;
	byte	MaxCVs;
	DWORD   altStyle		= m_List_CVs.GetExtendedStyle();
	int maxScroll			= 0;
	int index				= m_List_CVs.GetTopIndex();
	int last_visible_index	= index + m_List_CVs.GetCountPerPage();

	m_List_CVs.GetWindowRect(&rect);
	m_List_CVs.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	MaxCVs = m_List_CVs.GetItemCount();

	PositionAuslesen.SetRange(0, MaxCVs-1);
	PositionAuslesen.SetPos(0);

	for (int i = 0; i < MaxCVs; i++)
	{
		m_List_CVs.SetItemText(i, selectet_Dlg_Lok, _T("."));
		m_List_CVs.Update(i);
	}
	CWaitCursor wait;
	wait.Restore();
	for (int CV_Pos = 0; CV_Pos < MaxCVs; CV_Pos++)
	{
		PositionAuslesen.SetPos(CV_Pos);
		CV = _ttoi(m_List_CVs.GetItemText(CV_Pos, 0));

		Zug_Data->PRG_Lok_CV(false, CV, 0); // false weil nur lesen
		CV_Wert = Zug_Data->PRG_Get_CV(CV);
		Zug_Data->Set_Zug_CV_Wert( CV, CV_Wert);

		strText.Format(_T("%3d"), CV_Wert);
		m_List_CVs.SetItemText(CV_Pos, selectet_Dlg_Lok, strText);
		m_List_CVs.SetHotItem(CV_Pos);
		m_List_CVs.Update(CV_Pos);
		
		if (CV_Pos == last_visible_index)
		{
			maxScroll++;
			m_List_CVs.Scroll(rect.Size());
			index = m_List_CVs.GetTopIndex();
			last_visible_index = index + m_List_CVs.GetCountPerPage();
		}
	}
	for (int i = 0; i < maxScroll; i++)		m_List_CVs.Scroll(-rect.Size());

	m_List_CVs.SetExtendedStyle(altStyle);
	m_List_CVs.SetHotItem(0);
	m_List_CVs.Update(0);
	m_List_CVs.UpdateWindow();
	Zug_Data->Update_Sel_Zug_FunktionsTasten();
}

void CDlg_Setup_Train::OnBnClickedButtonFunktionListe()
{
	CDlg_Liste_Funktion Dlg;
	Dlg.SetPointer(Zug_Data);
	Dlg.DoModal();
}
