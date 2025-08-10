// CDlg_Com_Liste.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CDlg_Com_Liste.h"
#include "afxdialogex.h"


// CDlg_Com_Liste-Dialog

IMPLEMENT_DYNAMIC(CDlg_Com_Liste, CDialogEx)

CDlg_Com_Liste::CDlg_Com_Liste(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COM_LISTE, pParent)
{
	m_pParent = pParent;
	m_nID = CDlg_Com_Liste::IDD;
	pCom_Liste = NULL;
}

CDlg_Com_Liste::~CDlg_Com_Liste()
{
}

void CDlg_Com_Liste::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, View_Com_Liste);
}


BEGIN_MESSAGE_MAP(CDlg_Com_Liste, CDialogEx)
END_MESSAGE_MAP()


// CDlg_Com_Liste-Meldungshandler


BOOL CDlg_Com_Liste::Create(COM_Info* Data)
{
	pCom_Liste = Data;
	return CDialogEx::Create(m_nID, m_pParentWnd);
}


BOOL CDlg_Com_Liste::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString		strText;

	View_Com_Liste.InsertColumn(0, _T("Nr"), LVCFMT_RIGHT, 40, 0);
	View_Com_Liste.InsertColumn(1, _T(" Com Port"), LVCFMT_CENTER, 80, 0);
	View_Com_Liste.InsertColumn(2, _T(" Com Info"), LVCFMT_LEFT, 200, 0);
	View_Com_Liste.InsertColumn(3, _T(" Com Status"), LVCFMT_LEFT, 120, 0);
	View_Com_Liste.InsertColumn(4, _T(" Com Connect"), LVCFMT_LEFT, 80, 0);


	for (byte i = 0; i < COM_MAX_LISTE; i++)
	{
		strText.Format(_T(" %2i "), i); //= Zug_Data->Name;

		View_Com_Liste.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText, 0, LVIS_SELECTED, 0, 0);
		View_Com_Liste.SetItemText(i, 1, pCom_Liste->COM_PORT);
		View_Com_Liste.SetItemText(i, 2, pCom_Liste->COM_FriendlyName);
		View_Com_Liste.SetItemText(i, 3, pCom_Liste->COM_Status);
		View_Com_Liste.SetItemText(i, 4, pCom_Liste->COM_Conect);
		pCom_Liste++;

	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}