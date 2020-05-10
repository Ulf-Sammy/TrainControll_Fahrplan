// Dlg_CV_Action.cpp : implementation file
//
#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "Dlg_CV_Action.h"
#include "afxdialogex.h"


// CDlg_CV_Action dialog

IMPLEMENT_DYNAMIC(CDlg_CV_Action, CDialog)

CDlg_CV_Action::CDlg_CV_Action(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_CV_Action::IDD, pParent)
{
	m_NewData   = false;
	newCV_Wert  = 0;
	CV_Nr		= 0;
	m_Lok_Info  = _T("");
	m_CV_Info   = _T("");
	m_Lok_Info  = _T("");
	LoadAnhang();
}

CDlg_CV_Action::~CDlg_CV_Action()
{
}

void CDlg_CV_Action::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, newCV_Wert);
	DDX_Text(pDX, IDC_CV_INFO, m_CV_Info);
	DDX_Text(pDX, IDC_INFO_LOK, m_Lok_Info);
	DDX_Text(pDX, IDC_INFO_1,  m_Info[ 0]);
	DDX_Text(pDX, IDC_INFO_2,  m_Info[ 1]);
	DDX_Text(pDX, IDC_INFO_3,  m_Info[ 2]);
	DDX_Text(pDX, IDC_INFO_4,  m_Info[ 3]);
	DDX_Text(pDX, IDC_INFO_5,  m_Info[ 4]);
	DDX_Text(pDX, IDC_INFO_6,  m_Info[ 5]);
	DDX_Text(pDX, IDC_INFO_7,  m_Info[ 6]);
	DDX_Text(pDX, IDC_INFO_8,  m_Info[ 7]);
	DDX_Text(pDX, IDC_INFO_9,  m_Info[ 8]);
	DDX_Text(pDX, IDC_INFO_10, m_Info[ 9]);
	DDX_Text(pDX, IDC_INFO_11, m_Info[10]);
	DDX_Text(pDX, IDC_INFO_12, m_Info[11]);
}

void CDlg_CV_Action::LoadAnhang(void)
{
	LPCTSTR			pszPathName;
	CStdioFile		file;
	CFileException	ex;
	CString			strIn, Text;
	int				AnhangNr;
	pszPathName = _T("ANHANG.TXT");

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	for (int Z = 0; Z < 11; Z++) { Anhang[0][Z] = ""; }
	for (int A = 1; A < 23; A++)
	{
		file.ReadString(strIn);
		Text = strIn.Mid(2, 2);
		AnhangNr = _ttoi(strIn.Mid(2, 2));
		for (int Z = 0; Z < 11; Z++)
		{
			file.ReadString(strIn);
			Text.Empty();
			for (int i = 0; i < strIn.GetLength(); i++)
			{
				if (i > 0)
				{
					// char  A = strIn[i];
					if (strIn[i] == '\\')
					{
						Text.AppendChar(0x09);
						i++;
					}
					else
						Text.AppendChar(strIn.GetAt(i));
				}
			}
			Anhang[AnhangNr][Z] = Text;
		}
		file.ReadString(strIn);
	}
	file.Close();
}


BEGIN_MESSAGE_MAP(CDlg_CV_Action, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CDlg_CV_Action::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CDlg_CV_Action::OnBnClickedButtonFuntionsBeschreibung)
	ON_BN_CLICKED(IDOK, &CDlg_CV_Action::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_CV_Action message handlers


void CDlg_CV_Action::OnBnClickedButtonRead()
{ //Read Data
		UpdateData(true);
		Zug_Data->PRG_Lok_CV(false, CV_Nr, newCV_Wert);

		newCV_Wert = Zug_Data->PRG_Get_CV(CV_Nr); 
		if (newCV_Wert != altCV_Wert)
		{
			m_NewData = true;
		}
		m_CV_Info.Format(_T("gelesen CV Nr.: %3d   =  %3d"), CV_Nr, newCV_Wert);
		UpdateData(false);
}

void CDlg_CV_Action::OnBnClickedButtonFuntionsBeschreibung()
{ // Write Data
		UpdateData(true);
		Zug_Data->PRG_Lok_CV(true, CV_Nr, newCV_Wert);

		newCV_Wert = Zug_Data->PRG_Get_CV(CV_Nr); 
		if (newCV_Wert != altCV_Wert)
		{
			m_NewData = true;
		}
		m_CV_Info.Format(_T("geschreiben CV Nr.: %3d   =  %3d"), CV_Nr, newCV_Wert);
		UpdateData(false);
}

byte CDlg_CV_Action::GetCVValue(void)
{
	if (!m_NewData)
	{
		newCV_Wert = altCV_Wert;
	}
	return newCV_Wert;
}


void CDlg_CV_Action::SetPointer( CDatenBankLok* pTData, CDecoder_All_Infos* pDInfo)
{
	Zug_Data   = pTData;
	DecoderInfo = pDInfo;
}

void CDlg_CV_Action::Set_Lok(byte CVNr, byte CVWt)
{
	CV_Nr = CVNr;
	altCV_Wert = CVWt;
}

void CDlg_CV_Action::SetAnhang()
{
	byte AnhNr = DecoderInfo->Docu_Data.at(CV_Nr).Get_Anhang();
	for(int i=0; i<11; i++)
	{
		m_Info[i] = Anhang[AnhNr][i];
	}
	m_Info[11] = DecoderInfo->Docu_Data.at(CV_Nr).Text_Beschreibung();
}


void CDlg_CV_Action::OnBnClickedOk()
{
	CDialog::OnOK();
}


BOOL CDlg_CV_Action::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_NewData = false;
	newCV_Wert = 0;
	
	SetAnhang();
	SetDlgItemTextW(IDC_ZUG_NAME, m_Lok_Info);
	
	m_Lok_Info.Format(_T(" Werte der Lok : %s "), (LPCTSTR)Zug_Data->MeineZüge[Zug_Data->Get_Prog_Zug()].Name);
	m_CV_Info.Format(_T(" CV Nr.: %3d   =  %3d"), CV_Nr, altCV_Wert);
	SetDlgItemTextW(IDC_INFO_LOK, m_Lok_Info);
	SetDlgItemTextW(IDC_CV_INFO, m_CV_Info);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
}
