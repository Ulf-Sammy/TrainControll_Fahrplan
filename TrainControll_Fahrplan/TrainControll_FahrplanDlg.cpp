
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

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

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
	CString DateiName;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Anzahl_Dlg_Run = 0;
	StaticImage_Mega = new CStatic;
	Image_Mega.OffsetRect(CPoint(10, 725));
	StaticImage_Uno = new CStatic;
	Image_Uno.OffsetRect(CPoint(10, 770));
	StaticImage_Modus = new CStatic;
	Image_Modus.OffsetRect(CPoint(190, 724));
	StaticImage_LVZ = new CStatic;
	Image_LVZ.OffsetRect(CPoint(1400, 730));
	LED_rot.Load(_T("Images\\traffic_red.png"));
	LED_gruen.Load(_T("Images\\traffic_green.png"));
	LED_gelb.Load(_T("Images\\traffic_yellow.png"));
	Bild_Fahren.Load(_T("Images\\Train_RUN.png"));
	Bild_Progam.Load(_T("Images\\Train_Prog.png"));
	LVZ_ON.Load(_T("Images\\Power_ON.png"));
	LVZ_OFF.Load(_T("Images\\Power_OFF.png"));
	LVZ_PROG.Load(_T("Images\\Power_PROG.png"));
}

void CTrainControll_FahrplanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GLEISE, m_3DGleis);
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

	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTrainControll_FahrplanDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_WEICHE, &CTrainControll_FahrplanDlg::OnBnClickedButtonWeiche)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
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
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

//##########################################################################
	pDlgDebugInfo = new CDlg_Debug_Mega(this);
	pDlgDebugInfo->Create();
	pDlgDebugInfo->ShowWindow(SW_SHOW);

	StaticImage_Mega->Create(_T("A bitmap Mega"), WS_CHILD  | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, Image_Mega, this);
	StaticImage_Uno->Create(_T("A bitmap Uno")  , WS_CHILD  | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, Image_Uno, this);
	StaticImage_Modus->Create(_T("A bitmap Mode"), WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, Image_Modus, this);
	StaticImage_LVZ->Create(_T("A bitmap LVZ"), WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, Image_LVZ, this);

	DoStartDialog(false);
	XpressNet.OpenCom(3);
	BlockMelder.OpenCom(5);
	Gleis_Data.Init();

	m_3DGleis.OnInitDialog(&Gleis_Data);

	InitDlg();
	Gleis_Data.Start_Com_Thread();
	Sleep(200);
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
	CString Text;
	CPaintDC dc(this);

	switch (XpressNet.GetStatus_LZV())
	{ 
		case 0: // Status nicht bekannt
			StaticImage_LVZ->SetBitmap(HBITMAP(LVZ_OFF));
			break;
		case 1: // Status power on
			StaticImage_LVZ->SetBitmap(HBITMAP(LVZ_ON));
			break;
		case 2: // Status power off / NotAus
			StaticImage_LVZ->SetBitmap(HBITMAP(LVZ_OFF));
			break;
		case 4: // Status Programming
			StaticImage_LVZ->SetBitmap(HBITMAP(LVZ_PROG));
			break;
		default:
		break;
	}

	if (XpressNet.Get_Mode_Run())
	{
		StaticImage_Modus->SetBitmap(HBITMAP(Bild_Fahren));
	}
	else
	{
		StaticImage_Modus->SetBitmap(HBITMAP(Bild_Progam));
	}
	switch (XpressNet.Get_VersionInfo(&Text))
	{
	case 0:
		StaticImage_Mega->SetBitmap(HBITMAP(LED_rot));
		break;
	case 1:
		StaticImage_Mega->SetBitmap(HBITMAP(LED_gruen));
		break;
	case 2:
		StaticImage_Mega->SetBitmap(HBITMAP(LED_gelb));
		break;
	default:
		break;
	}
	dc.TextOutW(60, 725, Text);
	switch (BlockMelder.Get_VersionInfo(&Text))
	{
	case 0:
		StaticImage_Uno->SetBitmap(HBITMAP(LED_rot));
		break;
	case 1:
		StaticImage_Uno->SetBitmap(HBITMAP(LED_gruen));
		break;
	case 2:
		StaticImage_Uno->SetBitmap(HBITMAP(LED_gelb));
		break;
	default:
		break;
	}
	dc.TextOutW(60, 770, Text);
	BlockMelder.ZeichneBlockMeldung(&dc);
	XpressNet.ZeichneWeichenMeldung(&dc);
	dc.MoveTo(180, 700);
	dc.LineTo(180, 900);

	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

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
	Invalidate();
	Dlg.SetPointer(&meineLoks);
	Dlg.DoModal();
	XpressNet.Sende_Setto_Prog(false); //ChangeMode(Testen);
	Invalidate();
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
	Dlg.DoModal();
}
void CTrainControll_FahrplanDlg::OnSetupFahrplanEdit()
{
	CDlg_Plan_Liste_Train Dlg;
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
			//if (Gleis_Data.Block_Data.Set_FahrPlan(Lok_Nr, 0))
			//{
			//	//Gleis_Data.Block_Data.Do_Start_Plan_Zug(Lok_Nr);
			//	// Starte die Lok
			//	GetDlgItem(IDC_BUTTON_ZUG0 + index)->EnableWindow(FALSE);
			//	SetDlgItemTextW(IDC_STATIC_ZUG0_1 + index, _T("fährt jetzt nach Fahrplan Nr.:0"));
			//}
			//else
				SetDlgItemTextW(IDC_STATIC_ZUG0_1 + index, _T("konnte nicht gestartet werden"));

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


void CTrainControll_FahrplanDlg::OnBnClickedButtonTest()
{
}

void CTrainControll_FahrplanDlg::OnBnClickedButtonWeiche()
{
}


void CTrainControll_FahrplanDlg::OnClose()
{
	XpressNet.CloseCom();
	BlockMelder.CloseCom();

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
	if (pDlgDebugInfo != NULL)
	{
		pDlgDebugInfo->DestroyWindow();
		delete pDlgDebugInfo;
	}
	delete StaticImage_Mega;
	delete StaticImage_Uno;
	delete StaticImage_Modus;
	Gleis_Data.Stop_Com_Thread();
	CDialogEx::OnClose();
}

void CTrainControll_FahrplanDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.
	if (Image_LVZ.PtInRect(point))
	{
		Gleis_Data.Schalte_Power_LVZ();
		Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
