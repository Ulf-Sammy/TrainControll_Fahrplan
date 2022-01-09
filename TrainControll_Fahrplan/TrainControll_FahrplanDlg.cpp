
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



UINT Thread_Update_MEGA(LPVOID pParam)
{
	CTrainControll_FahrplanDlg* Info = (CTrainControll_FahrplanDlg*)pParam;
	TRACE(_T("starte update Thread MEGA.....\n"));
	while (Info->COM_MEGA_Data.COM_Active)
	{
		Info->Gleis_Data.GetNextMessage_Mega();
	} 
	TRACE(_T("ende Update Thread MEGA.....\n"));
	return 0;
}

UINT Thread_Update_Time(LPVOID pParam)
{
	CTrainControll_FahrplanDlg* Info = (CTrainControll_FahrplanDlg*)pParam;
	TRACE(_T("starte Time Thread .....\n"));

	while (Info->COM_MEGA_Data.COM_Active)
	{
		Info->Gleis_Data.NewTimeZug(clock());
	} 
	TRACE(_T("ende Time Thread .....\n"));
	return 0;
}

UINT Thread_Update_LZV(LPVOID pParam)
{
	CTrainControll_FahrplanDlg* Info = (CTrainControll_FahrplanDlg*)pParam;
	TRACE(_T("starte update Thread LZV200.....\n"));
	
	while (Info->COM_LZV_Data.COM_Active)
	{
		Info->Gleis_Data.GetNextMessage_LZV();
	} 
	TRACE(_T("ende Update Thread LZV200.....\n"));
	return 0;
}



UINT Thread_Update_Debug(LPVOID pParam)
{
	// CDlg_Debug_Mega* Info = (CDlg_Debug_Mega*)pParam;
	CTrainControll_FahrplanDlg* Info = (CTrainControll_FahrplanDlg*)pParam;

	TRACE(_T("starte update Thread Debug.....\n"));

	while (Info->COM_DEBUG_Data.COM_Active)
	{
		Info->pDlgDebugInfo->Get_Message();
	} 
	TRACE(_T("ende update Thread Debug.....\n"));
	return 0;
}


UINT Thread_Start_Prozess(LPVOID pParam)
{
	CTrainControll_FahrplanDlg* Info = (CTrainControll_FahrplanDlg*)pParam;
	Info->InfoPower.Set_Status(255);
	Sleep(5000); // 5 sec
	Info->BlockMelder.StartProcess();
	Info->XpressNet.StartProcess();
	Info->BlockMelder.Send_XpressNet_Status(Info->COM_LZV_Data.COM_Active);
	return 0;
}


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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	theApp.Lok_in_Uhr.LoadBitmap(IDB_BITLOK_MUHR);
	theApp.Lok_ge_Uhr.LoadBitmap(IDB_BITLOK_GUHR);
	theApp.Lok_Hoch.LoadBitmap(IDB_BITLOK_HOCH);
	theApp.Lok_Runter.LoadBitmap(IDB_BITLOK_RUNTER);
	theApp.Signal_G.LoadBitmap(IDB_BITSIGNAL_GRU);
	theApp.Signal_R.LoadBitmap(IDB_BITSIGNAL_ROT);
	theApp.WarnungNotAus.LoadBitmap(IDB_BIT_WARNUNG);
	theApp.LokSchuppen.LoadBitmap(IDB_BITMAP_SCHUPPEN);

	pDlgDebugInfo = new CDlg_Debug_Mega(this);
	pDlgSchuppen = new CDlg_Lok_Schuppen(this);
	pDlgBlockInfo = new CDlg_Block_Info(this);
	pDlgComListe = new CDlg_Com_Liste(this);
	
	pDlgDebugInfo->Create(&COM_DEBUG_Data.COM_Handel);
	pDlgSchuppen->Create();
	pDlgBlockInfo->Create();
	pDlgComListe->Create(ComListe);

	XpressNet.Set_Com(&COM_LZV_Data);

	BlockMelder.Set_Com(&COM_MEGA_Data);
}

void CTrainControll_FahrplanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GLEISE, InfoGleisBild);
	DDX_Control(pDX, IDC_STATIC_MEGA, InfoMega);
	DDX_Control(pDX, IDC_STATIC_LVZ2, InfoLVZ200);
    DDX_Control(pDX, IDC_STATIC_MODUS, InfoModus);
    DDX_Control(pDX, IDC_STATIC_ZUGI, StromKurve);
	DDX_Control(pDX, IDC_STATIC_MELD, InfoMelder);
	DDX_Control(pDX, IDC_STATIC_WEIC, InfoWeiche);
	DDX_Control(pDX, IDC_STATIC_POWER, InfoPower);
	//DDX_Control(pDX, IDC_STATIC_MODUS, Info_BC);
}

BEGIN_MESSAGE_MAP(CTrainControll_FahrplanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_COMMAND(ID_SETUP_PROGRAMMIEREZUG, &CTrainControll_FahrplanDlg::OnSetupProgrammierezug)
	ON_COMMAND(ID_SETUP_MEINEZUGLISTE, &CTrainControll_FahrplanDlg::OnSetupMeinezugliste)
	ON_COMMAND(ID_SETUP_TESTEDIEWEICHE, &CTrainControll_FahrplanDlg::OnSetupTestedieweiche)
	ON_COMMAND(ID_SETUP_FAHRPLAN_EDIT, &CTrainControll_FahrplanDlg::OnSetupFahrplanEdit)
	ON_COMMAND(ID_SETUP_MEGADEBUGDATA, &CTrainControll_FahrplanDlg::OnSetupMegadebugdata)
	ON_COMMAND(ID_SETUP_COMLISTEINFO, &CTrainControll_FahrplanDlg::OnSetupComlisteInfo)

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
		pDlgTrainRun[0]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[0])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD1)
	{
		pDlgTrainRun[1]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[1])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD2)
	{
		pDlgTrainRun[2]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[2])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD3)
	{
		pDlgTrainRun[3]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[3])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD4)
	{
		pDlgTrainRun[4]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[4])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD5)
	{
		pDlgTrainRun[5]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[5])->EnableWindow(true);
	}
	if (Dlg_Nr == CDlg_Run_Train::IDD6)
	{
		pDlgTrainRun[6]->ShowWindow(SW_HIDE);
		GetDlgItem(ButtonTrainRun[6])->EnableWindow(true);
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
	ShowWindow(SW_SHOWMAXIMIZED);
//##########################################################################
	meineLoks.Init();
	Gleis_Data.Init();
	InfoGleisBild.Init();

	StromKurve.Init();
	InfoMega.Init(sizeof(LED_Images), LED_Images);
	InfoLVZ200.Init(sizeof(LED_Images), LED_Images);
	InfoPower.Init(sizeof(LVZ_Images), LVZ_Images);
	InfoModus.Init(sizeof(MOD_Images), MOD_Images);
	InfoMelder.Init(L"Melder");
	InfoWeiche.Init(L"Weiche");

	XpressNet.Init(&InfoMega);
	BlockMelder.Init(&InfoLVZ200,&InfoModus,Gleis_Data.Get_Weichen_Anzahl());

	pDlgSchuppen->Init();

	InitDlg();
//##########################################################################
	// los gehts !
	Start_Com_Thread();
	
	//pDlgDebugInfo->ShowWindow(SW_SHOW);

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
	CPaintDC dc(this);

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

void CTrainControll_FahrplanDlg::EnumSerialPortFriendlyName()
{
	DWORD bufsize;
	DWORD dword_regtype;
	size_t sizet_string;
	TCHAR tcsz_string[256];
	TCHAR tcsz_portname_friendly[NUI32_TEXTLAENGE_SERPORTENUM_FRIENDLYNAME + 1]{}; //+1
	HDEVINFO hdevinfoset;
	HKEY hkey_device;
	GUID guid_DevInterfaceComport;
	SP_DEVINFO_DATA s_spdevinfodata{};
	SP_DEVICE_INTERFACE_DATA s_spdeviceinterfacedata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA ps_spdeviceinterfacedetaildata;
	const GUID S_GUID_DEVINTERFACE_COMPORT = { 0x86E0D1E0L, 0x8089, 0x11D0, 0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73 };


	guid_DevInterfaceComport = S_GUID_DEVINTERFACE_COMPORT;
	hdevinfoset = SetupDiGetClassDevsW(&guid_DevInterfaceComport, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hdevinfoset != INVALID_HANDLE_VALUE)
	{
		byte device_idx = 0;
		bool b_result;
		do
		{
			s_spdeviceinterfacedata.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			b_result = SetupDiEnumDeviceInterfaces(hdevinfoset, NULL, &guid_DevInterfaceComport, device_idx, &s_spdeviceinterfacedata);
			//Enumerates the device interfaces that
			//are contained in a device information set.
			if (b_result != FALSE)
			{
				SetupDiGetDeviceInterfaceDetail(hdevinfoset, &s_spdeviceinterfacedata, NULL, 0, &bufsize, NULL);

				if (bufsize >= sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA))
				{
					ps_spdeviceinterfacedetaildata = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(bufsize);
					if (ps_spdeviceinterfacedetaildata != NULL)
					{
						ps_spdeviceinterfacedetaildata->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
						s_spdevinfodata.cbSize = sizeof(SP_DEVINFO_DATA);
						if (SetupDiGetDeviceInterfaceDetail(hdevinfoset, &s_spdeviceinterfacedata, ps_spdeviceinterfacedetaildata, bufsize, NULL, &s_spdevinfodata) == TRUE)
						{
							if (SetupDiGetDeviceRegistryProperty(hdevinfoset, &s_spdevinfodata, SPDRP_FRIENDLYNAME, NULL, (PBYTE)tcsz_portname_friendly, NUI32_TEXTLAENGE_SERPORTENUM_FRIENDLYNAME * sizeof(TCHAR), NULL))
							{
								hkey_device = SetupDiOpenDevRegKey(hdevinfoset, &s_spdevinfodata, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
								if (hkey_device != INVALID_HANDLE_VALUE)
								{
									sizet_string = sizeof(tcsz_string);
									dword_regtype = 0;
									if (RegQueryValueEx(hkey_device, TEXT("PortName"), NULL, &dword_regtype, (LPBYTE)tcsz_string, (LPDWORD)&sizet_string) == ERROR_SUCCESS)
									{
										if ((dword_regtype == REG_SZ)&&(device_idx < COM_MAX_LISTE))
										{
											ComListe[device_idx].COM_PORT = CString(tcsz_string);
											ComListe[device_idx].COM_FriendlyName = CString(tcsz_portname_friendly);
										}
									}
									RegCloseKey(hkey_device);
								}//pdlg->Set_Com_Info(tcsz_portname_friendly,SerPortName_GetPortNr(hdevinfoset, &s_spdevinfodata));
							}
						}
						free(ps_spdeviceinterfacedetaildata);
					}
				}
			}
			device_idx++;
		} while (b_result != FALSE);
		SetupDiDestroyDeviceInfoList(hdevinfoset);          //Device information set aus Speicher entfernen.
	}
}

void CTrainControll_FahrplanDlg::Get_Com_Handel()
{
	for (byte i = 0; i < COM_MAX_LISTE; i++)
	{
		ComListe[i].COM_Handel = NULL;
		ComListe[i].COM_Conect = "!!!";
		if(ComListe[i].COM_PORT =="COM20")
		{// Komunikation LZV2000 
			if(ComListe[i].COM_FriendlyName == L"USB Serial Port (COM20)") // pürfen auf richtigen Namen
			{ //{COM_FriendlyName = L"USB Serial Port (COM20)" COM_PORT = L"COM20" COM_Status = L"" ...}
				
				ComListe[i].COM_Status = "OK mit LZV200";
				COM_LZV_Data = ComListe[i];
				COM_LZV_Data.BaudRate = 57600; 
				Open_Setup_Com(&COM_LZV_Data);
			}
			else
			{
				ComListe[i].COM_Status = " No LZV200 found";
			}
		}
		if (ComListe[i].COM_PORT == "COM3")
		{// Komunikation zu MEGA   
			if (ComListe[i].COM_FriendlyName == L"USB-SERIAL CH340 (COM3)") // pürfen auf richtigen Namen
			{
				ComListe[i].COM_Status = "OK mit MEGA";
				COM_MEGA_Data = ComListe[i];
				COM_MEGA_Data.BaudRate = 115200;
				Open_Setup_Com(&COM_MEGA_Data);
			}
			else			
			{
				ComListe[i].COM_Status = "No MEGA found";
			}
		}
		if (ComListe[i].COM_PORT == "COM4")
		{// Komunikation Debug vom MEGA
			if (ComListe[i].COM_FriendlyName == L"USB-SERIAL CH340 (COM4)") // pürfen auf richtigen Namen
			{
				ComListe[i].COM_Status = "OK mit Debug";
				COM_DEBUG_Data = ComListe[i];
				COM_DEBUG_Data.BaudRate = 115200;
				Open_Setup_Com(&COM_DEBUG_Data);
			}
			else
			{
				ComListe[i].COM_Status = "No Debug";
			}
		}
	}
}

bool CTrainControll_FahrplanDlg::Open_Setup_Com(COM_Info* p_COM_I)
{
	CString TxT = (L"\\\\.\\" + p_COM_I->COM_PORT);
	p_COM_I->COM_Handel = ::CreateFile((L"\\\\.\\"+p_COM_I->COM_PORT), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (p_COM_I->COM_Handel != INVALID_HANDLE_VALUE)
	{
		DCB dcbSerialParams = { 0 };

		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (!::GetCommState(p_COM_I->COM_Handel, &dcbSerialParams))
		{
			p_COM_I->COM_Conect.Format(_T("not Open (%i) "), GetLastError());
			TRACE(_T(" 1.ERROR COM \n"));
		}
		else
		{
			dcbSerialParams.BaudRate = p_COM_I->BaudRate;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.StopBits = ONESTOPBIT;
			if (!SetCommState(p_COM_I->COM_Handel, &dcbSerialParams))
			{
				p_COM_I->COM_Error = true;
				TRACE(_T(" 2. ERROR COM Set Com Parameter \n"));
			}
			COMMTIMEOUTS timeouts = { 0 };
			GetCommTimeouts(p_COM_I->COM_Handel, &timeouts);
			//

			timeouts.ReadIntervalTimeout = 50;
			timeouts.ReadTotalTimeoutConstant = 200;
			timeouts.ReadTotalTimeoutMultiplier = 10;

			timeouts.WriteTotalTimeoutConstant = 50;
			timeouts.WriteTotalTimeoutMultiplier = 10;
			//
			if (!SetCommTimeouts(p_COM_I->COM_Handel, &timeouts))
			{
				p_COM_I->COM_Error = true;
				TRACE(_T(" 3.ERROR COM \n"));
			}
		}
		p_COM_I->COM_Active = true;
		p_COM_I->COM_Conect = "Open";
		if (p_COM_I->COM_Error == true)
		{
			p_COM_I->COM_Conect = "Open & Error";
		}
		return true;
	}

	return false;
}

void CTrainControll_FahrplanDlg::Start_Com_Thread()
{
	AfxBeginThread(Thread_Update_LZV, this);

	AfxBeginThread(Thread_Update_MEGA, this);
	//AfxBeginThread(Thread_Update_Time, this);

	AfxBeginThread(Thread_Update_Debug, this);
	
	AfxBeginThread(Thread_Start_Prozess, this);
}

void CTrainControll_FahrplanDlg::Close_Com_Handel()
{
	if (COM_LZV_Data.COM_Active)
	{
		COM_LZV_Data.COM_Active = false;
		COM_LZV_Data.COM_Conect = "closed";
		::CloseHandle(COM_LZV_Data.COM_Handel);
	}
	if (COM_MEGA_Data.COM_Active)
	{
		COM_MEGA_Data.COM_Active = false;
		COM_MEGA_Data.COM_Conect = "closed";
		::CloseHandle(COM_MEGA_Data.COM_Handel);
	}
	if (COM_DEBUG_Data.COM_Active)
	{
		COM_DEBUG_Data.COM_Active = false;
		COM_DEBUG_Data.COM_Conect = "closed";
		::CloseHandle(COM_DEBUG_Data.COM_Handel);

	}


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
		byte Dlg_Nr = meineLoks.Get_Dialog_Nr(Lok_Nr);
		if (Dlg_Nr != 0xFF)
		{
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
	pDlgBlockInfo->NeueDaten();
	if (pDlgBlockInfo->IsWindowVisible())
		pDlgBlockInfo->ShowWindow(SW_HIDE);
	else
		pDlgBlockInfo->ShowWindow(SW_SHOW);
}

void CTrainControll_FahrplanDlg::OnBnClickedButtonWeiche()
{

}

void CTrainControll_FahrplanDlg::OnClose()
{
	XpressNet.SendeLVZ_Power(false);
	BlockMelder.Send_XpressNet_Status(false);
	Sleep(500);
	BlockMelder.CloseComunikation();
	Sleep(500);
	for (int i = 0; i < 7; i++)
	{
		if (pDlgTrainRun[i] != NULL)
		{
			pDlgTrainRun[i]->DestroyWindow();
			delete pDlgTrainRun[i];
		}
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
	if (pDlgSchuppen != NULL)
	{
		pDlgSchuppen->DestroyWindow();
		delete pDlgSchuppen;
	}
	if (pDlgBlockInfo != NULL)
	{
		pDlgBlockInfo->DestroyWindow();
		delete pDlgBlockInfo;
	}
	if (pDlgComListe != NULL)
	{
		pDlgComListe->DestroyWindow();
		delete pDlgComListe;
	}
	Close_Com_Handel();
	CDialogEx::OnClose();
}

void CTrainControll_FahrplanDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.
	//if (Image_LVZ.PtInRect(point))
	{
		if (XpressNet.Get_Power_onGleis())
		{// Power is on Power -> ausschalten
			XpressNet.SendeLVZ_Power(false);
		}
		else
		{// Power is off Power -> einschalten
			XpressNet.SendeLVZ_Power(true);
		}
		XpressNet.WarteDaten();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CTrainControll_FahrplanDlg::DoStartDialog(bool Zeige)
{
	for (int DlgNr = 0; DlgNr < 5; DlgNr++)
	{
		GetDlgItem(IDC_BUTTON_ZUG0 + DlgNr)->EnableWindow(Zeige);
		GetDlgItem(IDC_BUTTON_ZUG0 + DlgNr)->EnableWindow(Zeige);
	}
}

void CTrainControll_FahrplanDlg::updatePowerOn(bool Bit)
{
	BlockMelder.Send_XpressNet_Power(Bit);
	InfoPower.Set_Status((byte)Bit);
	InfoGleisBild.Invalidate();	
}


void CTrainControll_FahrplanDlg::OnSetupProgrammierezug()
{ // wie schalte ich zu progrmmodus um 
	CDlg_Setup_Train Dlg;
	//XpressNet.Sende_Setto_Prog(true); //ChangeMode(Program);
	Invalidate();
	Dlg.SetPointer(&meineLoks);
	Dlg.DoModal();
	//XpressNet.Sende_Setto_Prog(false); //ChangeMode(Fahren);
	Invalidate();
	meineLoks.Set_aktiveLok_FuntiontoRun();
}

void CTrainControll_FahrplanDlg::OnSetupMegadebugdata()
{
	if (pDlgDebugInfo->IsWindowVisible())
		pDlgDebugInfo->ShowWindow(SW_HIDE);
	else
		pDlgDebugInfo->ShowWindow(SW_SHOW);
}

void CTrainControll_FahrplanDlg::OnSetupComlisteInfo()
{
	if (pDlgComListe->IsWindowVisible())
		pDlgComListe->ShowWindow(SW_HIDE);
	else
		pDlgComListe->ShowWindow(SW_SHOW);
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


