// CDlg_Debug_Mega.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CDlg_Debug_Mega.h"
#include "afxdialogex.h"


// CDlg_Debug_Mega-Dialog

IMPLEMENT_DYNAMIC(CDlg_Debug_Mega, CDialogEx)

CDlg_Debug_Mega::CDlg_Debug_Mega(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlg_Debug_Mega, pParent)
{
	m_pParent = pParent;
	m_nID = CDlg_Debug_Mega::IDD;
	p_hCom = NULL;
	ListentoCom = false;
	//COM_status = COMSTAT::cbInQue = true;	
}
CDlg_Debug_Mega::~CDlg_Debug_Mega()
{
	ListentoCom = false;
}

BOOL CDlg_Debug_Mega::Create(HANDLE* Data)
{
	p_hCom = Data;
	ListentoCom = true;
	
	return CDialogEx::Create(m_nID, m_pParent);
}

BOOL CDlg_Debug_Mega::OnInitDialog()
{	
	CDialogEx::OnInitDialog();

	//SetWindowPos(NULL, 2000, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	DebugTextListe.InsertColumn(0, _T("Nr"), LVCFMT_RIGHT, 40, 0);

	return 0;
}


void CDlg_Debug_Mega::Get_Message()
{
	byte  c;
	DWORD bytesRead;
	bool Error;
	CString InText;
	do
	{
		Error = ::ReadFile(*p_hCom, &c, 1, &bytesRead, NULL);
		if (bytesRead == 1)
		{
			InText.AppendChar(c);
			if (c == '\r')
			{
				DebugTextListe.InsertItem(LVIF_TEXT | LVIF_STATE, Zeile, InText, 0, LVIS_SELECTED, 0, 0);
				InText.Empty();
				Zeile++;
			}
		}
	
	} while (bytesRead == 1);
}


void CDlg_Debug_Mega::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DebugTextListe);
}


BEGIN_MESSAGE_MAP(CDlg_Debug_Mega, CDialogEx)
END_MESSAGE_MAP()


// CDlg_Debug_Mega-Meldungshandler
