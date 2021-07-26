#include "pch.h"
//#include "TrainControll_FahrplanDlg.h"
#include "Dlg_Lok_Funktionen.h"
//#include "afxdialogex.h"


// Dlg_Lok_Funktionen dialog

IMPLEMENT_DYNAMIC(CDlg_Lok_Funktionen, CDialog)

CDlg_Lok_Funktionen::CDlg_Lok_Funktionen(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Lok_Funktionen::IDD, pParent)
{
	
}

CDlg_Lok_Funktionen::~CDlg_Lok_Funktionen()
{
}

void CDlg_Lok_Funktionen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
/*	DDX_Text(pDX, IDC_ZUG_NAME, m_ZugName);
	DDX_Text(pDX, IDC_EDIT_A1, Ausgang[0]);
	DDX_Text(pDX, IDC_EDIT_A2, Ausgang[1]);
	DDX_Text(pDX, IDC_EDIT_A3, Ausgang[2]);
	DDX_Text(pDX, IDC_EDIT_A4, Ausgang[3]);
	DDX_Text(pDX, IDC_EDIT_A5, Ausgang[4]);
	DDX_Text(pDX, IDC_EDIT_A6, Ausgang[5]);
	DDX_Text(pDX, IDC_EDIT_A7, Ausgang[6]);
	DDX_Text(pDX, IDC_EDIT_A8, Ausgang[7]);
	DDX_Text(pDX, IDC_EDIT_A9, Ausgang[8]);
	DDX_Text(pDX, IDC_EDIT_S01, Sound[0]);
	DDX_Text(pDX, IDC_EDIT_S02, Sound[1]);
	DDX_Text(pDX, IDC_EDIT_S03, Sound[2]);
	DDX_Text(pDX, IDC_EDIT_S04, Sound[3]);
	DDX_Text(pDX, IDC_EDIT_S05, Sound[4]);
	DDX_Text(pDX, IDC_EDIT_S06, Sound[5]);
	DDX_Text(pDX, IDC_EDIT_S07, Sound[6]);
	DDX_Text(pDX, IDC_EDIT_S08, Sound[7]);
	DDX_Text(pDX, IDC_EDIT_S09, Sound[8]);
	DDX_Text(pDX, IDC_EDIT_S10, Sound[9]);
	DDX_Text(pDX, IDC_EDIT_S11, Sound[10]);
	DDX_Text(pDX, IDC_EDIT_S12, Sound[11]);
	*/
}


BEGIN_MESSAGE_MAP(CDlg_Lok_Funktionen, CDialog)
END_MESSAGE_MAP()


// Dlg_Lok_Funktionen message handlers


void CDlg_Lok_Funktionen::SetCV_TablePointer(CDecoderInfo * CV_T) //CVC_TableInfo * CV_T)
{
	CV_Table = CV_T;
}
void CDlg_Lok_Funktionen::Set_Type_and_Lok(byte Type, byte Lok)
{
	Sel_Lok = Lok;
	Sel_Type = Type;
}


BOOL CDlg_Lok_Funktionen::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString FunktionTxt;
	for(int i = 0; i < 35;i++)
	/*
	{
		if(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(0,7) =="Ausgang")
		{
			FunktionTxt = CV_Table->Zug_Data[Sel_Lok].FT[ i].Funktion;
			Nr = _ttoi(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(8,2));
			if (FunktionTxt=="                     ") Ausgang[Nr]="";
			else Ausgang[Nr] = FunktionTxt;
		}
		if(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(0,5) =="Sound")
		{
			FunktionTxt = CV_Table->Zug_Data[Sel_Lok].FT[ i].Funktion;
			Nr = _ttoi(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(9,2))-1;
			if (FunktionTxt=="                     ") Sound[Nr]="";
			else Sound[Nr] = FunktionTxt;
		}
	}
	m_ZugName = CV_Table->Zug_Data[Sel_Lok].Name;
	*/
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlg_Lok_Funktionen::OnOK()
{
	UpdateData(true);
	for(int i = 0; i < 35;i++)
	{
		/*
		if(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(0,7) =="Ausgang")
		{
			Nr = _ttoi(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(8,2));
			CV_Table->Zug_Data[Sel_Lok].FT[ i].Funktion = Ausgang[Nr];
		}
		if(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(0,5) =="Sound")
		{
			Nr = _ttoi(	CV_Table->Zug_Data[Sel_Lok].FT[ i].Ausgang.Mid(9,2));
			CV_Table->Zug_Data[Sel_Lok].FT[ i].Funktion = Sound[Nr-1];
		}
		*/
	}
	UpdateData(false);
	CDialog::OnOK();
}
