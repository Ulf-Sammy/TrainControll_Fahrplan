#include "pch.h"
#include "CGleisPlan.h"
#include "TrainControll_FahrplanDlg.h"

UINT Thread_Update_Time(LPVOID pParam)
{
	CGleisPlan * Info = (CGleisPlan*)pParam;
	TRACE(_T("starte Time Thread .....\n"));

	do
	{
		Info->NewTimeZug(clock());
	} while (Info->ListentoCom);

	TRACE(_T("ende Time Thread .....\n"));
	return 0;
}

UINT Thread_Update_Block(LPVOID pParam)
{
	CGleisPlan* Info = (CGleisPlan*)pParam;
	TRACE(_T("starte update Thread Uno.....\n"));
	do
	{
		switch (Info->GetNextMessage_Uno())
		{
			case false:
				break;
			case COM_SEND_BLOCK:
				Info->NewDataMelder();
				break;
		    case COM_SEND_RELAIS:
				Info->NewDataRelais();
				break;
		}
	} while (Info->ListentoCom);
	TRACE(_T("ende Update Thread Uno.....\n"));
	return 0;
}

UINT Thread_Update(LPVOID pParam)
{
	CGleisPlan * Info = (CGleisPlan*)pParam;
	TRACE(_T("starte update Thread Mega.....\n"));
	do
	{
	} while (Info->isreadyforStartup());
	TRACE(_T("starte Setup .....\n"));
	 Info->Setup_TrainControl();
	TRACE(_T("ende   Setup .....\n"));
	Info->Thread_Run = true;
	do
	{
		// Daten Empfangen
		switch (Info->GetNextMessage_Mega())
			{// alles was vom Train Control Kommt
			case false:
				break;
			case COM_ACKN_MOD:
				Info->New_Controll_Mode();
				break;
			case COM_SEND_LVZ_STA:
				// Status der LVZ Zentrale
				Info->New_Daten_LVZ();
				break;
			case COM_SEND_WEICHE:
				Info->NewDataWeiche();
				break;
			case COM_SEND_ZUG_DA:// Zug antwortet
				Info -> NewDataZug();
				break;
			case COM_SEND_CV:
				Info->New_CV_Daten();
				break;
			}
		// Daten Senden
		Info->New_Daten_Senden();
	} while (Info->ListentoCom);
	Info->Thread_Run = false;
	TRACE(_T("ende Update Thread Mega.....\n"));
	return 0;
}

CGleisPlan::CGleisPlan()
{
	
}


CGleisPlan::~CGleisPlan()
{
}

void CGleisPlan::Init()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	Züge = &APP->meineLoks;
	XpressNet = &APP->XpressNet;
	BlockMelder = &APP->BlockMelder;
	max_active_Loks = Züge->Init();
	Lade_Daten();
}


byte CGleisPlan::GetNextMessage_Mega()
{
	return XpressNet->GetNextMessage();
}

byte CGleisPlan::GetNextMessage_Uno()
{
	return BlockMelder->GetNextMessage();
}

void CGleisPlan::Start_Com_Thread()
{
	 // damit hört der Task zu
	if (XpressNet->NoComToXpressNet())
	{
		ListentoCom = false;
		TRACE(_T(" Thread nicht gestart !!\n"));
	}
	else
	{
		ListentoCom = true;
		AfxBeginThread(Thread_Update, this);
		AfxBeginThread(Thread_Update_Block, this);
		AfxBeginThread(Thread_Update_Time, this);
		TRACE(_T(" Thread's gestart !!\n"));
	}
	Set_Weiche(TrainCon_Paar(17, true));

}

void CGleisPlan::Stop_Com_Thread()
{
	if (ListentoCom)
	{
		ListentoCom = false; // damit hört der Task zu
		{
		} while (Thread_Run);
		//Sleep(1000);
		XpressNet->CloseCom();
		TRACE(_T("So bin fertig mit dem Thread \n"));
	}
}

void CGleisPlan::ChangeSetupMode(ControlStatus Mode, byte Sub )
{
//	CString TextControlStatus[5] = { _T("starte Komunikation: "),_T("Mache Setup arbeite an "),_T("Programmiere Zug "),_T("Fahren auf Strecke"),_T("stoppe Com:") };
	ModeControl = Mode;
	ModeSub     = Sub;
	
	XpressNet->Set_In_Mode(Mode, Sub, &StatusZentrale);
	Neu_DataStatus.Putin();
}

void CGleisPlan::New_Controll_Mode()
{
	ModeControl  = (ControlStatus) XpressNet->Hole_Acknolage_Mode();
	Neu_DataStatus.Putin();
}


void CGleisPlan::DoCheckIt(byte Lok_Nr, clock_t Zeit)
{
	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Plan_Warten)
	{
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).WarteZeit < Zeit)
		{
		}
	}
	if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt) && (Züge->Get_aktiveLok_Pointer(Lok_Nr).Betriebs_Modus == Zug_Steuerung::Hand_Betrieb))
	{ // Dies nur für den Handbetreib 
	}
	if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt) && (Züge->Get_aktiveLok_Pointer(Lok_Nr).Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb))
	{ // Dies nur für den Automaticbetreib 
	}
}

void CGleisPlan::showInfo_Dlg(byte LokNr, CString Info)
{
}

void CGleisPlan::showPlan_Dlg(byte LokNr, CString Info)
{
}

bool CGleisPlan::isNewUpdate_Status()
{
	return Neu_DataStatus.Get_Out();
}
bool CGleisPlan::isNewUpdate_Gleis()
{
	return Neu_DataGleis.Get_Out();
}
bool CGleisPlan::isNewUpdate_Taster()
{
	return false;
}
void CGleisPlan::NewDataMelder()
{
	TrainCon_Paar upBlock;
	// CString strName;
	// byte BlockNr;
	upBlock = BlockMelder->HoleBlockData();
	Block[upBlock.GetWert()].bestetzen(upBlock.GetBit());
    //Block[upBlock.GetWert()-1].bestetzen(!upBlock.GetBit());

	Neu_DataGleis.Putin();
	//BlockNr = upBlock.GetWert();
	// strName.Format(_T(" Block Nr. %d: gezeigt "), BlockNr);
	//AfxMessageBox(strName, MB_OK );
}
void CGleisPlan::NewDataWeiche()
{
	TrainCon_Paar Weiche ;

	Weiche = XpressNet->HoleWeicheData();
	Set_Weiche(Weiche);

}
void CGleisPlan::NewDataRelais()
{
	TrainCon_Paar Relais;

	Relais = BlockMelder->HoleRelayData();
	Set_Relais(Relais);
}
void CGleisPlan::NewDataZug()
{
	Züge->New_Lok_Data();
}
void CGleisPlan::NewTimeZug(clock_t Zeit)
{
	static byte Lok_Nr = 0;
	static TrainCon_Paar neuerMelder;

	DoCheckIt(Lok_Nr, Zeit);
	Lok_Nr = (Lok_Nr + 1) % (max_active_Loks);
}
void CGleisPlan::New_Daten_LVZ()
{
	StatusZentrale = XpressNet->HoleStatus_LZV();
	SetStatusPower();
	Neu_DataStatus.Putin();
}

void CGleisPlan::New_CV_Daten()
{
	CTrainControll_FahrplanDlg* P;
	P = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	P->meineLoks.PRG_Set_CV();
	//Train_Data.DoGetNewCV(XpressNet.Hole_CV_Wert());
}

void CGleisPlan::New_Daten_Senden()
{
	XpressNet->SendeNeueDaten();
}



void CGleisPlan::Set_Lok_Geschwindigkeit(byte Lok_Nr, Zug_Status SetTo, byte Geschwindigkeit)
{
	switch (SetTo)
	{
	case Zug_Status::Zug_Stopped:
		//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Stop();
		Züge->Set_aktiveLok_Geschwindigkeit(Lok_Nr, 0, false, true);
		break;
	case Zug_Status::Zug_faehrt_vor:
		//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Geschwindigkeit(Geschwindigkeit, true);
		Züge->Set_aktiveLok_Geschwindigkeit(Lok_Nr, Geschwindigkeit, true, false);
		break;
	case Zug_Status::Zug_faehrt_rueck:
		//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Geschwindigkeit(Geschwindigkeit, false);
		Züge->Set_aktiveLok_Geschwindigkeit(Lok_Nr, Geschwindigkeit, false, false);
		break;
	case Zug_Status::Zug_haelt:
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() != Zug_Status::Zug_haelt)
		{
			//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Halt();
			Züge->Set_aktiveLok_Geschwindigkeit(Lok_Nr, 0, false, false);
		}
		break;
	default:
		break;
	}
}

void CGleisPlan::Reeady_ToRun()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	APP->DoStartDialog(true);
}

void CGleisPlan::Set_Startup(bool B)
{
	if ((B) && (StartUpCount < 3))
	{
		StartUpCount++;
	}
	if (StartUpCount == 2)
	{
		Start_Com_Thread();
	}
}

byte CGleisPlan::GetStatusZentrale()
{
	return StatusZentrale;
}
InfoConStat CGleisPlan::GetStatusMode()
{
	return InfoConStat(ModeControl, ModeSub);
}
void CGleisPlan::SetStatusPower()
{
	CTrainControll_FahrplanDlg* P;
	P = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	//TRACE(_T("Status Zentrale %i \n"), StatusZentrale);
	if (StatusZentrale == 1)
	{
		P->SetDlgItemText(IDC_BUTTON_POWER, _T("Power OFF"));
	}
	if (StatusZentrale == 2)
	{
		P->SetDlgItemText(IDC_BUTTON_POWER, _T("Power ON !"));
	}
	if (StatusZentrale == 3)
	{
		P->SetDlgItemText(IDC_BUTTON_POWER, _T("Programmiern"));
	}
	if (StatusZentrale == 4)
	{    // Zentrale Meldet programiern
		// das geht nicht P->SetDlgItemText(IDC_BUTTON_POWER, _T("Power (4)! "));
	}
	if (StatusZentrale == 5)
	{
		P->SetDlgItemText(IDC_BUTTON_POWER, _T("Power (5)! "));
	}

}
bool CGleisPlan::Schalte_Power_LVZ()
{
	if (StatusZentrale == 1)
	{// Power is on Power -> ausschalten
		XpressNet->SendeLVZ_Power(false);
		return true;
	}
	if (StatusZentrale == 2)
	{// Power is off Power -> einschalten
		XpressNet->SendeLVZ_Power(true);

	}
	// was mache ich sonst ?? 
	return false;
}
bool CGleisPlan::isPower_onGleis()
{
	return (StatusZentrale == 1);
}
bool CGleisPlan::isreadyforStartup()
{
	return (StartUpCount < 1);
}
void CGleisPlan::Setup_TrainControl()
{
	byte WeichenAnzahl = 32;
	if (XpressNet->NoComToXpressNet())
	{
		TRACE(_T(" Kein Setup weil keine Com Schnitstelle \n"));
		return;
	}
	else
	{
		ChangeSetupMode(ControlStatus::Begin_COM, 0);
		ChangeSetupMode(ControlStatus::Setup_Controller, 0);
		// Melder Setup
		ChangeSetupMode(ControlStatus::Setup_Controller, 1);
		// Weichen Setup
		ChangeSetupMode(ControlStatus::Setup_Controller, 2);
		XpressNet->SetWeichenAnzahl(WeichenAnzahl);
		//Block  Setup
		ChangeSetupMode(ControlStatus::Setup_Controller, 3);

		// XpressNet Setup
		ChangeSetupMode(ControlStatus::Setup_Controller, 4);
		StatusZentrale = XpressNet->GetStatus_Setup_LZV();
		SetStatusPower();
		ChangeSetupMode(ControlStatus::Testen, 0);
		
		// Setup Datenaustausch  ferig 
		// Starte den Thread
	}	
}

void CGleisPlan::ZeicheStrecke(CDC* pDC)
{
		for (auto&  B : Block)
	{
		B.ZeicheBlock(pDC);
	}
}

void CGleisPlan::ZeicheBlock(byte Nr, CDC* pDC)
{
	Block[Nr].ZeicheBlock(pDC);
}

void CGleisPlan::Kick_Block(CPoint Klick)
{
	TrainCon_Paar Data;
	byte Ergebnis;

	for (auto& B : Block)
	{
		Ergebnis = B.OnKlick(Klick, &Data);
		if(Ergebnis== 1)
		{
			if (XpressNet->NoComToXpressNet())			
				Set_Weiche(Data);
			else 
				XpressNet->SendeWeicheDaten(Data);
			return;
		}
		if (Ergebnis == 2)
		{
			if (XpressNet->NoComToXpressNet())
				Set_Relais(Data);
			else 
				BlockMelder->SetBlockPower(Data);
			return;
		}
	}
}

void CGleisPlan::Set_Weiche(TrainCon_Paar WeichenData)
{
	for (auto& B : Block)
	{
		auto i = &B - &Block[0];
		if (Block[i].set_Weiche(WeichenData))
		{
			Neu_DataGleis.Putin();
			return;
		}
	}
}

void CGleisPlan::Set_Relais(TrainCon_Paar RelayData)
{
	for (auto& B : Block)
	{
		auto i = &B - &Block[0];
		if (Block[i].set_Relais(RelayData))
		{
			Neu_DataGleis.Putin();
			return;
		}
	}
}

void CGleisPlan::Lade_Daten()
{
	LPCTSTR			pszPathName = _T(FILE_ANLAGE);
	CStdioFile		file;
	CFileException	ex;
	CString			strIn, Text;
	CPoint			Step;
	int				Nr;

	const int Rect_X = 1600;
	const int Rect_Y = 660;

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	file.ReadString(strIn); // Titel
	////// Allgemeine Daten lesen
	for (int I = 0; I < 4; I++)
	{
		file.ReadString(strIn); // Bildgröße
		if (strIn.Mid(0, 4) == "BILD")
		{
			Step.x = Rect_X / _ttoi(strIn.Mid(6, 3));
			Step.y = Rect_Y / _ttoi(strIn.Mid(10, 3));
		}
		if (strIn.Mid(0, 4) == "ZEIT")
		{
			int Z = _ttoi(strIn.Mid(6, 3));
		}
		if (strIn.Mid(0, 4) == "PLAN")
		{
			int P = _ttoi(strIn.Mid(6, 3));
		}
	}
	///// Den BLOCK einlesen
	if (strIn.Mid(0, 7) == "##Block")
	{
		file.ReadString(strIn);
		do
		{
			Block.push_back(CBlock(strIn));
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	///// die Weichen einlesen
	if (strIn.Mid(0, 7) == "##Bl Nr")
	{
		file.ReadString(strIn);
		do
		{
			Nr = _ttoi(strIn.Mid(0, 3));
			if (Nr < Block.size())
			{
				Block[Nr].AddWeiche(strIn, Step);
			}
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	///// die Strecken einlesen
	if (strIn.Mid(0, 7) == "## Stre")
	{
		file.ReadString(strIn);
		do
		{
			Nr = _ttoi(strIn.Mid(0, 3));
			if (Nr < Block.size())
			{
				Block[Nr].AddStrecke(strIn, Step);
			}
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	/// die Taster einlesen
	if (strIn.Mid(0, 7) == "## Tast")
	{
		file.ReadString(strIn);
		do
		{
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	/// Weg über Weichen einlesen
	if (strIn.Mid(0, 7) == "## Weg ")
	{
		file.ReadString(strIn);
		do
		{
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}

	file.Close();
	///
	/// 
	for (byte i = 0; i < Züge->Get_max_Aktiv_Loks(); i++)
	{
		CDataXpressNet* DieLok = &Züge->Get_aktiveLok_Pointer(i);
		Nr = DieLok->Block_ist;
		if (Nr < Block.size())
		{
			Block[Nr].bestetzen(&Züge->Get_aktiveLok_Pointer(i));
		}
	}
}
