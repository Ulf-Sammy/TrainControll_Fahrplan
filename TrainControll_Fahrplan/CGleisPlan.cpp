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

UINT Thread_Update(LPVOID pParam)
{
	CGleisPlan * Info = (CGleisPlan*)pParam;
	TRACE(_T("starte update Thread .....\n"));
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
		switch (Info->GetNextMessage())
			{// alles was vom Train Control Kommt
			case false:
				break;
			case COM_SEND_MELDER: // neue Melder Daten
				Info->NewDataMelder();
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
	TRACE(_T("ende Update Thread .....\n"));
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
	max_active_Loks = Züge->Init();
	Block_Data.Init();
	Züge->Set_aktiveLoks_aufsGleis();
}


byte CGleisPlan::GetNextMessage()
{
	return XpressNet->GetNextMessage();
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
		AfxBeginThread(Thread_Update_Time, this);
	}
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

void CGleisPlan::DoMelder_I(byte Melder_Nr, byte Lok_Nr)
{
	static FahrplanPos Befehl;
	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).isAutomaticOn())
	{ // hier wird der Fahrplan abgearbeitet.
		Befehl = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl();

		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).isNext_Melder(Melder_Nr))
		{ // das war der nächst erwartete Melder
			if (Block_Data.besetzte_bis_Next_Block_Auto(Lok_Nr) != BlockRueckmeldung::Frei_Fahrt)
			{
				Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Halt();
			}
		}

		while (Befehl.Get_Melder() == Melder_Nr)
		{
			switch (Befehl.Get_Was())
			{
			case FahrPlanDo::stoppen:
				Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Stop();
				showPlan_Dlg(Lok_Nr, _T("Lok gestopped !"));
				break;
			case FahrPlanDo::vorwaerz_fahren:
				// Lok Steht und wird durch eine andere Lok gestartet :-)
				if (!Züge->Get_aktiveLok_Pointer(Lok_Nr).isHalt())
				{
					Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Geschwindigkeit(Befehl);
				}
				break;
			case FahrPlanDo::rueckwaerz_fahren:
				if (!Züge->Get_aktiveLok_Pointer(Lok_Nr).isHalt())
				{
					Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Geschwindigkeit(Befehl);
				}
				break;
			case FahrPlanDo::warten_fahren:
				//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Warten(Befehl);
				break;
			case FahrPlanDo::warten_stoppen:
				//Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Warten(Befehl);
				break;
			case FahrPlanDo::schalten_Weiche:
				// Weiche wird in der Prüfung geschaltet;
				break;
			case FahrPlanDo::schalten_Funk:
				// Funktion
				Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Funktion(Befehl);
				break;
			case FahrPlanDo::letzte_Zeile:
				break;
			default:
				break;
			}
			Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Done_Plan_Befehl();
			Befehl = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl();
		}
		return;
	}
	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).isHand())
	{
		TRACE3(" Zug %i = Melder %i  =  %i\n", Lok_Nr, Melder_Nr, Züge->Get_aktiveLok_Pointer(Lok_Nr).Melder_next);
		if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Melder_next == Melder_Nr) || (Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt))
		{ // in dem Zweig fährt die Lok
			TRACE(_T("Zug %i # Melder %i\n"), Lok_Nr, Melder_Nr);
			PruefeZugWeg = Block_Data.besetzte_bis_Next_Block_Hand(Lok_Nr);
			switch (PruefeZugWeg)
			{
			case BlockRueckmeldung::Frei_Fahrt:
				//Set_Lok_Geschwindigkeit(LokNr, ZugA, G);
				break;
			case BlockRueckmeldung::Weichenweg_nichtfrei:
				showInfo_Dlg(Lok_Nr, _T("!m!!Stopp !!! Weichenweg ist nicht frei"));
				Set_Lok_Geschwindigkeit(Lok_Nr, Zug_Status::Zug_haelt, 0);
				break;
			case BlockRueckmeldung::Block_besetzt:
				showInfo_Dlg(Lok_Nr, _T("!m!!Stopp !!! Der Block ist besetzt !!"));
				Set_Lok_Geschwindigkeit(Lok_Nr, Zug_Status::Zug_haelt, 0);
				break;
			default:
				break;
			}
		}
		return;
	}
}

void CGleisPlan::DoMelder_O(byte Melder_Nr, byte Lok_Nr)
{	
	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.isnextMelder(Melder_Nr))
		Block_Data.ClearBlockbyLok(Lok_Nr, Melder_Nr);

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
		if ((Block_Data.ist_frei(Züge->Get_aktiveLok_Pointer(Lok_Nr).Block_best)) || (Züge->Get_aktiveLok_Pointer(Lok_Nr).Block_best == 0))
		{
			PruefeZugWeg = Block_Data.besetzte_bis_Next_Block_Hand(Lok_Nr);
			switch (PruefeZugWeg)
			{
			case BlockRueckmeldung::Frei_Fahrt:
				Züge->Get_aktiveLok_Pointer(Lok_Nr).ReSet_Geschwindigkeit();
				TRACE(_T(" Starte Lok Nr.: %i\n"), Lok_Nr);
				break;
			case BlockRueckmeldung::Weichenweg_nichtfrei:
				showInfo_Dlg(Lok_Nr, _T("!z!!Stopp !!! Weichenweg ist nicht frei"));
				break;
			case BlockRueckmeldung::Block_besetzt:
				showInfo_Dlg(Lok_Nr, _T("!z!!Stopp !!! Der Block ist besetzt !!"));
				break;
			default:
				break;
			}
		}
	}
	if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt) && (Züge->Get_aktiveLok_Pointer(Lok_Nr).Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb))
	{ // Dies nur für den Automaticbetreib 
		if (Block_Data.ist_frei(Züge->Get_aktiveLok_Pointer(Lok_Nr).Block_best))
		{
			if (Block_Data.besetzte_bis_Next_Block_Auto(Lok_Nr) == BlockRueckmeldung::Frei_Fahrt)
			{
				Züge->Get_aktiveLok_Pointer(Lok_Nr).ReSet_Geschwindigkeit();
				TRACE(_T(" Starte Lok Nr.: %i\n"), Lok_Nr);
			}
			else
				showInfo_Dlg(Lok_Nr, _T("!z!!Stopp !!! Der Block ist noch besetzt !!"));
		}
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
void CGleisPlan::NewDataMelder()
{
	Block_Data.MelderRing.SetMelder(XpressNet->HoleMelderData());
}
void CGleisPlan::NewDataWeiche()
{
	TrainCon_Paar Weiche ;

	Weiche = XpressNet->HoleWeicheData();
	Block_Data.Set_Stellung_Weiche(Weiche);
}
void CGleisPlan::NewDataZug()
{
	Züge->New_Lok_Data();
}
void CGleisPlan::NewTimeZug(clock_t Zeit)
{
	static byte Lok_Nr = 0;
	static TrainCon_Paar neuerMelder;

	if (Block_Data.MelderRing.isNeueData())
	{
		neuerMelder = Block_Data.MelderRing.GetMelder();
		neuerMelder.TraceInfo();
		Block_Data.Set_Melder(neuerMelder);

		for (byte Nr = 0; Nr < max_active_Loks; Nr++)
		{
			if (Züge->Get_aktiveLok_Pointer(Nr).isActive())
			{
				if (neuerMelder.GetBit())
					DoMelder_I(neuerMelder.GetWert(), Nr);
				else
					DoMelder_O(neuerMelder.GetWert(), Nr);
			}
		}
	}
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
		Block_Data.doBlocks_byZug_Stop(Lok_Nr);
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
			Block_Data.doBlocks_byZug_Halt(Lok_Nr);
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
	byte AnzahlgemeldeteMelder;
	TrainCon_Paar Melder;

	byte WeichenAnzahl = 0;
	for (CInfoBlock I : *Block_Data.GetBlockPointer())
	{
		if (I.Block_Type == BlockType::isWeiche) WeichenAnzahl++;
	}
	byte MelderAnzahl = Block_Data.GetAnzahl_Melder();
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
		XpressNet->SetMelder_Zeit(Block_Data.GetMelder_Zeit()); 	// sende Melder Nachlaufzeit
		AnzahlgemeldeteMelder = XpressNet->GetMelderAnzahl();
		if (AnzahlgemeldeteMelder != 0)
		{
			if (MelderAnzahl != AnzahlgemeldeteMelder)
				TRACE(_T(" ERROR Melder anzahl stimmt micht %i != %i"), MelderAnzahl, AnzahlgemeldeteMelder);
			for (int i = 0; i < AnzahlgemeldeteMelder; i++)
			{
				Melder = XpressNet->ReadMelder();                     // Hole Zustand
				if (Melder.GetWert() != i) TRACE(_T(" Fehler bei der Übertragung  Melder %i # %i\n"), i, Melder.GetWert());
				Block_Data.Set_Melder(Melder);
			}
		}
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
