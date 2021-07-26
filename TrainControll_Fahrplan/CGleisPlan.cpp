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

UINT Thread_Update_UNO(LPVOID pParam)
{
	CGleisPlan* Info = (CGleisPlan*)pParam;
	byte Data;
	Info->Setup_MelderControl();
	TRACE(_T("starte update Thread Uno.....\n"));
	do
	{
		Data = Info->GetNextMessage_Uno();
		if (Data)
		{
			Info->Verarbeite_Uno_Daten(Data);
		}
	} while (Info->ListentoCom);
	TRACE(_T("ende Update Thread Uno.....\n"));
	return 0;
}

UINT Thread_Update_MEGA(LPVOID pParam)
{
	CGleisPlan * Info = (CGleisPlan*)pParam;
	byte Data;
	TRACE(_T("starte update Thread Mega.....\n"));
	 Info->Setup_TrainControl();
	TRACE(_T("run Thread   .....\n"));
	Info->Thread_Run = true;
	do
	{
		// Daten Empfangen
		Data = Info->GetNextMessage_Mega();
		if(Data)
		{
			Info->Verarbeite_Mega_Daten(Data);
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
	for (int i = 0; i <= MAX_WEICHEN_MOTOR; i++)
	{
		WeichenAntrieb.push_back(TrainCon_Paar(i, false));
	}
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
	if (XpressNet->NoComToXpressNet() && BlockMelder->NoComToBlockNet())
	{
		ListentoCom = false;
		TRACE(_T(" Thread nicht gestart Kein Uno & Mega !!\n"));
	}
	else
	{
		ListentoCom = true;
		AfxBeginThread(Thread_Update_MEGA, this);
		AfxBeginThread(Thread_Update_UNO, this);
		AfxBeginThread(Thread_Update_Time, this);
		TRACE(_T(" Thread's gestart für Uno und Mega !!\n"));
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
		TRACE(_T("So bin fertig mit dem Thread \n"));
	}
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

bool CGleisPlan::isNewUpdate_Taster()
{
	return false;
}


void CGleisPlan::Verarbeite_Mega_Daten(byte neueInfo)
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	TrainCon_Paar DatenPaar;

	switch (neueInfo)
	{
	case COM_SEND_LVZ_STA:
		// Status der LVZ Zentrale
		StatusZentrale = XpressNet->HoleStatus_LZV();
		APP->Invalidate();
		break;
	case COM_SEND_WEICHE:
		DatenPaar = XpressNet->HoleWeicheData();
		Set_Weiche(DatenPaar);
		APP->Invalidate();
		break;
	case COM_SEND_ZUG_DA:// Zug antwortet
		Züge->New_Lok_Data();
		APP->Invalidate();
		break;
	case COM_SEND_CV:
		APP->meineLoks.PRG_Set_CV();
		APP->Invalidate();
		break;
	}
}

void CGleisPlan::Verarbeite_Uno_Daten(byte neueInfo)
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	TrainCon_Paar DatenPaar;
	switch (neueInfo)
	{
	case COM_SEND_BLOCK:
		DatenPaar = BlockMelder->HoleBlockData();
		Block[DatenPaar.GetWert()].bestetzen(DatenPaar.GetBit());
		APP->Invalidate();
		break;
	case COM_SEND_RELAIS:
		DatenPaar = BlockMelder->HoleRelayData();
		Set_Relais(DatenPaar);
		APP->Invalidate();
		break;
	}
}

void CGleisPlan::NewTimeZug(clock_t Zeit)
{
	static byte Lok_Nr = 0;

	DoCheckIt(Lok_Nr, Zeit);
	Lok_Nr = (Lok_Nr + 1) % (max_active_Loks);
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


byte CGleisPlan::GetStatusZentrale()
{
	return StatusZentrale;
}

void CGleisPlan::SetStatusPower()
{
	
}
bool CGleisPlan::Schalte_Power_LVZ()
{
	if (StatusZentrale == 1)
	{// Power is on Power -> ausschalten
		XpressNet->SendeLVZ_Power(false);
		return true;
	}
	if (StatusZentrale == 2||StatusZentrale == 0)
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

bool CGleisPlan::isPower_onBlock(byte Nr)
{
	return Block[Nr].Get_Relais_Data(false).GetBit();
}

void CGleisPlan::Setup_TrainControl()
{
	if (XpressNet->NoComToXpressNet())
	{
		TRACE(_T(" Kein Setup weil keine Com Schnitstelle \n"));
		return;
	}
	else
	{
		XpressNet->StartKomunikation();
	}	
}

void CGleisPlan::Setup_MelderControl()
{
	if (BlockMelder->NoComToBlockNet())
	{

		TRACE(_T(" Kein Setup weil keine Com UNO Schnittstelle \n"));
		return;
	}
	else
	{
		BlockMelder->StartKomunikation();
	}
}

void CGleisPlan::ZeicheStrecke(CDC* pDC)
	
{
	for (auto&  B : Block)
	{
		B.ZeicheBlock(pDC, &WeichenAntrieb);
	}
}


bool CGleisPlan::Kick_Block(CPoint Klick)
{
	TrainCon_Paar Data;
	byte Wert;
	byte Ergebnis;

	for (auto& B : Block)
	{
		Ergebnis = B.OnKlick(Klick, &Wert);
		if(Ergebnis== 1)
		{
			Data = WeichenAntrieb[Wert].GetInvBit();
			if (XpressNet->NoComToXpressNet())
				return (Set_Weiche(Data));
			else 
				XpressNet->SendeWeicheDaten(Data);
			return false;
		}
		if (Ergebnis == 2)
		{
			Data = B.Get_Relais_Data(true);
			if (XpressNet->NoComToXpressNet())
				Set_Relais(Data);
			else 
				BlockMelder->SetBlockPower(Data);
			return false;
		}
	}
	return (false);
}

bool CGleisPlan::Set_Weiche(TrainCon_Paar WeichenData)
{
	if (WeichenData.GetWert() < MAX_WEICHEN_MOTOR)
	{
		WeichenAntrieb[WeichenData.GetWert()].SetBit(WeichenData.GetBit());
		return true;
	}
	return false;
}

bool CGleisPlan::Get_Weiche(TrainCon_Paar WeichenDaten)
{
	return WeichenAntrieb[WeichenDaten.GetWert()].GetBit();
}

bool CGleisPlan::Set_Relais(TrainCon_Paar RelayData)
{
	for (auto& B : Block)
	{
		if (Block[&B - &Block[0]].set_Relais(RelayData))
		{
			return true;
		}
	}
	return false;
}

void CGleisPlan::Schalte_Relais(byte Nr, bool Bit)
{
	if (Nr > 0)
	{
		TrainCon_Paar Relais;
		Relais = Block[Nr].Get_Relais_Data(false);
		Relais.SetBit(Bit);
		BlockMelder->SetBlockPower(Relais);
		if (BlockMelder->NoComToBlockNet())
		{
			Block[Nr].set_Relais(Relais);
		}
	}
}

void CGleisPlan::GetAnschlussBlocks(byte Nr, std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks)
{
	Block[Nr].GetAnschlussBlocks(EinBlocks,AusBlocks);
}

bool CGleisPlan::Weg_Von_bis_frei(byte Nr, byte BlockTo, std::vector<byte>*WegeBlocks)
{
	return Block[Nr].Weg_zuBlock(true, &WeichenAntrieb, WegeBlocks);
}

bool CGleisPlan::Weg_nach_von_Block(byte BlockNr, bool Richtung, std::vector<byte>* WegeBlocks)
{
	if (!WegeBlocks->empty())
	{
		WegeBlocks->clear();
	}
	return Block[BlockNr].Weg_zuBlock(Richtung, &WeichenAntrieb, WegeBlocks);
}

bool CGleisPlan::isWeg_frei(std::vector<byte> WegeBlocks)
{
	bool Result = true;
	for (auto Nr : WegeBlocks)
	{
		Result = Result && Block[Nr].ist_frei();
	}
	return Result;
}

bool CGleisPlan::Besetze_Weg_mit_Lok(CDataXpressNet* Lok, std::vector<byte> WegeBlocks)
{
	for (auto Nr : WegeBlocks)
	{
		Block[Nr].bestetzen(Lok);
	}
	return false;
}

CString CGleisPlan::Get_LokName_in_Block(byte Nr)
{
	return Block[Nr].GetLok_NameonBlock();
}

BlockStatus CGleisPlan::GetStatus_Block(byte Nr, CString* Lok_Name)
{
	bool Richtung;
	return Block[Nr].GetStatus_Block(Lok_Name,&Richtung);
}

BlockDebugData CGleisPlan::Get_DebugData(byte Nr)
{
	if (Block.size() > Nr)
	{
		return Block[Nr].Get_Debug();
	}
	return Block[0].Get_Debug();;
}

void CGleisPlan::TestBlock_mitZug(byte Nr, bool Bit, bool leeren)
{
	CDataXpressNet *Zug_L, *Zug_R;
	Zug_L = &Züge->Get_aktiveLok_Pointer(1);
	Zug_R = &Züge->Get_aktiveLok_Pointer(2);
	if(leeren)
	{
		Block[Nr].freimachen();
		Block[Nr].bestetzen(false);
	}
	else
	{
		if (Bit)
		{
			Block[Nr].bestetzen(Zug_L);
			Block[Nr].bestetzen(true);
		}
		else
		{
			Block[Nr].bestetzen(Zug_R);
			Block[Nr].bestetzen(true);
		}
	}
}

CDataXpressNet* CGleisPlan::TestZug(bool Bit)
{
	if(Bit)
	{ 
		return &Züge->Get_aktiveLok_Pointer(1);
	}
	else
	{
		return &Züge->Get_aktiveLok_Pointer(2);
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
	std::vector<CBlock_Weg> alle_Wege;
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
			alle_Wege.push_back(CBlock_Weg(strIn));
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
	TRACE(_T("-------- Alle Wege --------------\n"));
	for (auto Weg : alle_Wege)
	{
		TRACE(_T("Von %i --- %i ---- %i nach \n"), Weg.VonBlock(),Weg.ZwiBlock(),Weg.NachBlock());
		Block[Weg.VonBlock()].AddWeg(true,Weg);
		Block[Weg.NachBlock()].AddWeg(false,Weg);
	}
}
