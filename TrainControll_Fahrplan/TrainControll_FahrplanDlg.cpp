
// TrainControll_FahrplanDlg.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "Dlg_Setup_Train.h"
#include "Dlg_Liste_Train.h"
#include "Dlg_Test_Weiche.h"
#include "Dlg_Plan_Liste_Train.h"
#include "CDlg_Fahrplan.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg-Dialogfeld f¸r Anwendungsbefehl "Info"
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst¸tzung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTrainControll_FahrplanDlg-Dialogfeld
CTrainControll_FahrplanDlg::CTrainControll_FahrplanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrainControll_FahrplanDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Anzahl_Dlg_Run = 0;
}

void CTrainControll_FahrplanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GLEISE, m_3DGleis);
	DDX_Control(pDX, IDC_STATUS, m_3DStatus);
}

BEGIN_MESSAGE_MAP(CTrainControll_FahrplanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_COMMAND(ID_SETUP_PROGRAMMIEREZUG, &CTrainControll_FahrplanDlg::OnSetupProgrammierezug)
	ON_COMMAND(ID_SETUP_MEINEZUGLISTE, &CTrainControll_FahrplanDlg::OnSetupMeinezugliste)
	ON_COMMAND(ID_SETUP_TESTEDIEWEICHE, &CTrainControll_FahrplanDlg::OnSetupTestedieweiche)
	ON_COMMAND(ID_SETUP_FAHRPLAN_EDIT, &CTrainControll_FahrplanDlg::OnSetupFahrplanEdit)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_ZUG0, IDC_BUTTON_ZUG4, &CTrainControll_FahrplanDlg::OnBnClickedButtonZug)

	ON_BN_CLICKED(IDC_BUTTON_RESET, &CTrainControll_FahrplanDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, &CTrainControll_FahrplanDlg::OnBnClickedButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_POWER, &CTrainControll_FahrplanDlg::OnBnClickedButtonPower)

	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTrainControll_FahrplanDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_WEICHE, &CTrainControll_FahrplanDlg::OnBnClickedButtonWeiche)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CTrainControll_FahrplanDlg::DlgTrainRunDone(int Dlg_Nr)
{
	if (Dlg_Nr == CDlg_Run_Train::IDD)
	{
		{
			pDlgTrainRun[0]->ShowWindow(SW_HIDE);
			GetDlgItem(ButtonTrainRun[0])->EnableWindow(true);
		}
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD1)
	{
		{
			pDlgTrainRun[1]->ShowWindow(SW_HIDE);
			GetDlgItem(ButtonTrainRun[1])->EnableWindow(true);
		}
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD2)
	{
	
		{
			pDlgTrainRun[2]->ShowWindow(SW_HIDE);
			GetDlgItem(ButtonTrainRun[2])->EnableWindow(true);
		}
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD3)
	{

		{
			pDlgTrainRun[3]->ShowWindow(SW_HIDE);
			GetDlgItem(ButtonTrainRun[3])->EnableWindow(true);
		}
	}
}

BOOL CTrainControll_FahrplanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// Groﬂes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

//##########################################################################
	DoStartDialog(false);
	XpressNet.OpenCom(4);
	Gleis_Data.Init();
	m_3DStatus.OnInitDialog(&Gleis_Data);
	m_3DGleis.OnInitDialog(&Gleis_Data);

	InitDlg();
	Gleis_Data.Set_Startup(true);

	return TRUE; 
}
void CTrainControll_FahrplanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void CTrainControll_FahrplanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Ger‰tekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	Gleis_Data.Set_Startup(true);
}
HCURSOR CTrainControll_FahrplanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTrainControll_FahrplanDlg::InitDlg()
{
	CAtlString TextB, Text0, Text1, Text2;

	
	for (byte I = 0; I < 5; I++)
	{
		TextB = "kein Zug";
		Text0.Empty();
		Text1.Empty();
		Text2.Empty();

		if (meineLoks.is_Zug_Pointer(I))
		{
			if (meineLoks.Get_aktiveLok_Pointer(I).isAutomaticOn())
			{
				Text0.Format(_T("Zug Nr%i :  %s ........ (auto)"), I, (LPCTSTR)meineLoks.Get_aktiveLok_Pointer(I).Name);
			}
			else
			{
				Text0.Format(_T("Zug Nr%i :  %s ........ (hand)"), I, (LPCTSTR)meineLoks.Get_aktiveLok_Pointer(I).Name);
			
				if (meineLoks.Set_Zug_Dlg_Nr(I))
					GetDlgItem(IDC_BUTTON_ZUG0 + I)->ShowWindow(SW_SHOW);
			}
			TextB.Format(_T("Lok  Nr.%i"), I);
			// ============= die Lokomotive acktiv ==============================================
			if (meineLoks.Get_aktiveLok_Pointer(I).isActive())
			{
				if (meineLoks.Get_aktiveLok_Pointer(I).isOnGleis())
				{
					Text1.Format(_T("Sie ist auf dem Block: %i "), meineLoks.Get_aktiveLok_Pointer(I).Block_ist);
				}
				else
				{
					Text1.Format(_T("Sie ist nicht auf dem Gleis ! "));
					GetDlgItem(IDC_BUTTON_ZUG0 + I)->ShowWindow(SW_HIDE);
				}
			}
			else
			{ // Nicht acktive Lokomotive 
				GetDlgItem(IDC_BUTTON_ZUG0 + I)->ShowWindow(SW_HIDE);
				Text1.Format(_T("nicht aktive !"));
				Text2.Format(_T("."));
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_ZUG0 + I)->ShowWindow(SW_HIDE);
		}
		SetDlgItemTextW(IDC_BUTTON_ZUG0   + I, TextB);
		SetDlgItemTextW(IDC_TITEL_ZUG0    + I, Text0);
		SetDlgItemTextW(IDC_STATIC_ZUG0_0 + I, Text1);
		SetDlgItemTextW(IDC_STATIC_ZUG0_1 + I, Text2);

	}
}

void CTrainControll_FahrplanDlg::DoStartDialog(bool Zeige)
{
	for (int DlgNr = 0; DlgNr < 5; DlgNr++)
	{
		GetDlgItem(IDC_BUTTON_ZUG0 + DlgNr)->EnableWindow(Zeige);
		GetDlgItem(IDC_BUTTON_ZUG0 + DlgNr)->EnableWindow(Zeige);
	}
}

void CTrainControll_FahrplanDlg::OnSetupProgrammierezug()
{
	CDlg_Setup_Train Dlg;
	XpressNet.Sende_Setto_Prog(true); //ChangeMode(Program);
	Dlg.SetPointer(&meineLoks);
	Dlg.DoModal();
	XpressNet.Sende_Setto_Prog(false); //ChangeMode(Testen);
	meineLoks.Set_aktiveLok_FuntiontoRun();
}
void CTrainControll_FahrplanDlg::OnSetupMeinezugliste()
{
	CDlg_Liste_Train Dlg;
	Dlg.Set_TrainData(&meineLoks);
	Dlg.DoModal();
}
void CTrainControll_FahrplanDlg::OnSetupTestedieweiche()
{
	CDlg_Test_Weiche Dlg;

	Dlg.SetPointer(&Gleis_Data.Block_Data);
	Dlg.DoModal();
}
void CTrainControll_FahrplanDlg::OnSetupFahrplanEdit()
{
	CDlg_Plan_Liste_Train Dlg;
	Dlg.SetData(Gleis_Data.Block_Data.FahrplanAnzahl, &Gleis_Data.Block_Data);
	Dlg.DoModal();
}

void CTrainControll_FahrplanDlg::OnBnClickedButtonZug(UINT nID)
{
	int index = nID - IDC_BUTTON_ZUG0;
	byte Lok_Nr = index;
	if (meineLoks.Get_aktiveLok_Pointer(Lok_Nr).isAutomaticOn())
	{
		if (Gleis_Data.isPower_onGleis())
		{
			if(Gleis_Data.Block_Data.Set_FahrPlan(Lok_Nr, 0))
			{
				Gleis_Data.Block_Data.Do_Start_Plan_Zug(Lok_Nr);
				// Starte die Lok
				GetDlgItem(IDC_BUTTON_ZUG0 + index)->EnableWindow(FALSE);
				SetDlgItemTextW(IDC_STATIC_ZUG0_1+ index, _T("f‰hrt jetzt nach Fahrplan Nr.:0"));
			}
			else
				SetDlgItemTextW(IDC_STATIC_ZUG0_1 +index, _T("konnte nicht gestartet werden"));

		}
	}
	else
	{
		byte Dlg_Nr = 0xFF;
		if (meineLoks.Get_aktiveLok_Pointer(Lok_Nr).Dlg_Nr != 0xFF)
		{
			Dlg_Nr = meineLoks.Get_aktiveLok_Pointer(Lok_Nr).Dlg_Nr;

			if (pDlgTrainRun[Dlg_Nr] == NULL)
			{
				ButtonTrainRun[Dlg_Nr] = IDC_BUTTON_ZUG0 + index;
				pDlgTrainRun[Dlg_Nr] = new CDlg_Run_Train(this, Dlg_Nr);
				pDlgTrainRun[Dlg_Nr]->Set_aktive_Lok(Lok_Nr);
				if (pDlgTrainRun[Dlg_Nr]->Create())
				{
					GetDlgItem(IDC_BUTTON_ZUG0 + index)->EnableWindow(FALSE);
				}
			}
			else
			{
				pDlgTrainRun[Dlg_Nr]->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_ZUG0 + index)->EnableWindow(FALSE);
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_ZUG0 + index)->EnableWindow(FALSE);
		}
	}
}

void CTrainControll_FahrplanDlg::OnBnClickedButtonSound()
{
	// TODO: F¸gen Sie hier Ihren Handlercode f¸r Benachrichtigungen des Steuerelements ein.
}
void CTrainControll_FahrplanDlg::OnBnClickedButtonPower()
{
	Gleis_Data.Schalte_Power_LVZ();
}
void CTrainControll_FahrplanDlg::OnBnClickedButtonReset()
{
	// TODO: Reset gedr¸ckt
}

void CTrainControll_FahrplanDlg::OnBnClickedButtonTest()
{

	static bool Bit = true;
	Gleis_Data.Block_Data.setze_Block_Test(10, Bit);
	Bit = !Bit;
}
void CTrainControll_FahrplanDlg::OnBnClickedButtonWeiche()
{
	CDlg_RunCam_Train Dlg;
	//Dlg.SetPointer(&Gleis_Data.Train_Data.Block_Data);
	Dlg.DoModal();
	//meineLoks.Test_Data();
}


void CTrainControll_FahrplanDlg::OnClose()
{
	if (pDlgTrainRun[0] != NULL) 
	{
		pDlgTrainRun[0]->DestroyWindow();
		delete pDlgTrainRun[0];	
	}
	if (pDlgTrainRun[1] != NULL)
	{
		pDlgTrainRun[1]->DestroyWindow();
		delete pDlgTrainRun[1];
	}
	if (pDlgTrainRun[2] != NULL) 
	{
		pDlgTrainRun[2]->DestroyWindow();
		delete pDlgTrainRun[2];
	}
	if (pDlgTrainRunCam != NULL)
	{
		pDlgTrainRunCam->DestroyWindow();
		delete pDlgTrainRunCam;
	}
	Gleis_Data.Stop_Com_Thread();
	CDialogEx::OnClose();
}








