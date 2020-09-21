#include "pch.h"
#include "CDataXpressNet.h"
#include "CBlockData.h"
#include "TrainControll_FahrplanDlg.h"

CDataXpressNet::CDataXpressNet()
{
}
CDataXpressNet::CDataXpressNet(CString LokName)
{
	CString DateiName;
	Name = LokName;
	Name.Trim();
	DateiName.Format(_T(FILE_ALLE_ZUEGE_BILDER), Name); //_T("Images\\Spreewald.bmp")
	Bild = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), DateiName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);	
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	XpressNet = &APP->XpressNet;
}


bool CDataXpressNet::FillData(CString InText)
{
	Zug_Steuerung BetreibsModus = Zug_Steuerung::nicht_Betriebs_bereit;
	bool Aktive;
	Name       = InText.Mid(0, 15); Name.Trim();
	
	byte PNr   = _ttoi(InText.Mid(40, 2));
	Block_ist  = _ttoi(InText.Mid(19, 3));
	Block_soll = _ttoi(InText.Mid(19, 3));
	Status = Zug_Status::Zug_Stopped;

	if (InText.Mid(23, 16) == "Zug in Uhrzeiger") 		Blick = true;
	else 		Blick = false;
	switch (InText[17])
	{
	case 'H':
		Aktive = true;
		BetreibsModus = Zug_Steuerung::Hand_Betrieb;
		break;
	case 'A':
		Aktive = true;
		BetreibsModus = Zug_Steuerung::Automatik_Betrieb;
		break;
	case '0':
		Aktive = false;
		BetreibsModus = Zug_Steuerung::nicht_Betriebs_bereit;
		break;
	default:
		break;
	}
	Betriebs_Modus = BetreibsModus;
	return(Block_ist > 0); // true weil Lok ist auf dem Gleis
}

CDataXpressNet::~CDataXpressNet()
{
}


void CDataXpressNet::Set_Dlg_Nr(byte Nr)
{
	Dlg_Nr = Nr;
}

byte CDataXpressNet::Get_Dlg_Nr()
{
	return byte(Dlg_Nr);
}

bool CDataXpressNet::Prüfe_Plan_im_Block()
{
	if (Fahr_Plan.Get_Plan_Befehl().mache == FahrPlanDo::begin_Block)
	{
		if (Block_ist == Fahr_Plan.Get_Plan_Befehl().WertA)
		{
			Fahr_Plan.Done_Plan_Befehl();
			return true;
		}
		else
		{
			TRACE(_T("ERROR Lok steht nicht im Richtigen Block"));
		}
	}
	return false;
}

void CDataXpressNet::Set_Block(byte Ist, byte Soll, byte Best, byte Melder)
{
	Block_ist = Ist;
	Block_soll = Soll;
	Block_best = Best;
	Melder_next = Melder;
}

void CDataXpressNet::Set_Besetzt_Block(byte Best)
{
	Block_best = Best;
}

void CDataXpressNet::Set_Block_Pause(bool Stop)
{
	if (Stop)
	{ 
		Melder_next = 0xFF;
	}
	else
	{ 
		Block_best = 0;
	}
	Block_soll = Block_ist;
}

byte CDataXpressNet::Get_Soll_Block()
{
	return Block_soll;
}



void CDataXpressNet::Lade_Fahrplan()
{
	if (Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb)
	{
		Fahr_Plan.Load_Daten(FahrPlan_Nr);
		Zug_active = true;
	}
}

void CDataXpressNet::ASK_LokData()
{
	if (XpressNet->NoComToXpressNet())
	{
		TRACE(_T("Fehler Com Port nicht geöffnet !!! \n   Kann Zug Daten nicht abfragen! \n"));
		return;
	}
	XpressNet->SendeAsknachLokDaten(Adresse);
	Zug_wartet_auf_Data = true;
	do
	{
	} while (Zug_wartet_auf_Data);
}

void CDataXpressNet::Get_LokData()
{
	XpressNet->HoleZugData(&FunktionsGruppe[0], Adresse);
	Zug_wartet_auf_Data = false;
}

void CDataXpressNet::Set_Adresse()
{
	Adresse = Decoder_Data.Get_Adresse();
}

CString CDataXpressNet::Text_Block()
{
	CString Tmp;
	if ((Block_ist > 200) || (Block_ist == 0))
		Tmp.Format(_T(" ... "));
	else
		Tmp.Format(_T(" %3i"), Block_ist);
	return Tmp;
}

CString CDataXpressNet::Text_Betrieb()
{
	CString Tmp;
	switch (Betriebs_Modus)
	{
	case Zug_Steuerung::nicht_Betriebs_bereit:
		Tmp.Format(L"....");
		break;
	case Zug_Steuerung::Hand_Betrieb:
		Tmp.Format(L"Hand");
		break;
	case Zug_Steuerung::Automatik_Betrieb:
		Tmp.Format(L"Auto");
		break;
	default:
		break;
	}
	return Tmp;
}

CString CDataXpressNet::Text_Adresse()
{
	return Decoder_Data.Text_Adresse();
}

CString CDataXpressNet::Text_Hersteller()
{
	return Decoder_Data.Text_Decoder_Hersteller();
}

CString CDataXpressNet::Text_Decoder()
{
	return Decoder_Data.Text_Decoder_Type();
}

CString CDataXpressNet::Text_Decoder_Sub()
{
	return Decoder_Data.Text_Decoder_Soft_Version();
}


byte CDataXpressNet::Get_Decoder_Nr()
{
	return 0x00;
}

void CDataXpressNet::Set_Startbedingungen(Zug_Status UserSet_Status)
{
	BlockInfo StartBlock;

	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	StartBlock = APP->Gleis_Data.Block_Data.GetWerte_BlockInfo(Block_ist);

	switch (UserSet_Status)
	{
	case Zug_Status::Zug_Stopped:
	case Zug_Status::Zug_faehrt_vor:
	case Zug_Status::Zug_haelt:
		FahrRicht = true;
		break;
	case Zug_Status::Zug_faehrt_rueck:
		FahrRicht = false;
		break;
	default:
		FahrRicht = true;
		break;

	}
	byte S = 0;
	if ((StartBlock.Block_Type == BlockType::isWeiche))
		S = StartBlock.WeichenStellung;
	// Weichentype ?
	if (Prüfrichtung())
	{
		Block_soll = StartBlock.AusgangBlock[S];
		Melder_next = StartBlock.AusgangMelder[S];
	}
	else
	{
		Block_soll = StartBlock.EingangBlock[S];
		Melder_next = StartBlock.EingangMelder[S];
	}
}

void CDataXpressNet::Set_Lok_Nr(byte Nr_EDV)
{
	
}

void CDataXpressNet::Set_Funktion(byte Nr, bool bit)
{
	byte G_Nr = FunNr[Nr];

	bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], bit);
	XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]); 
}

void CDataXpressNet::Set_Funktion(FahrplanPos Befehl)
{
	if (Befehl.mache == FahrPlanDo::schalten_Funk)
	{ 
		byte G_Nr = FunNr[Befehl.Get_Funtion().GetWert()];
		
		bitWrite(FunktionsGruppe[G_Nr], BitNr[Befehl.Get_Funtion().GetWert()], Befehl.Get_Funtion().GetBit());
		XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
	else
	{
		TRACE(_T(" ERROR falscher Befehl \n\n"));
	}
}

void CDataXpressNet::Set_Funktion_Sound(bool SW)
{
	byte Nr = Decoder_Data.Get_Sound_FuntionsTaste();
	if ((Nr > 0) &&(Nr < 255))
	{
		byte G_Nr = FunNr[Nr];

		bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], SW);
		XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
}

void CDataXpressNet::Set_Funktion_Rangieren(bool SW)
{
	byte Nr = Decoder_Data.Get_Rangier_FunktionTaste();
	if (Nr > 0)
	{
		byte G_Nr = FunNr[Nr];

		bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], SW);
		XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
}

void CDataXpressNet::Set_Funktion_VerzögerungsZeit(bool SW)
{
	byte Nr = Decoder_Data.Get_VerzögZ_FunktionTaste();
	if (Nr > 0)
	{
		byte G_Nr = FunNr[Nr];

		bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], SW);
		XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}

}

bool CDataXpressNet::Get_Funktion(byte Nr)
{
	byte G_Nr = 0;
	if (Nr > 28) Nr = 0;
	
	G_Nr = FunNr[Nr];
	return bitRead(FunktionsGruppe[G_Nr], BitNr[Nr]);
}

void CDataXpressNet::Set_Geschwindigkeit(byte Geschwindigkeit, bool FahrtRichtung)
{
 	FahrRicht = FahrtRichtung;
	FahrGesch = Geschwindigkeit;

	FunktionsGruppe[0] = FahrGesch;
	if (FahrRicht)
	{
		FunktionsGruppe[0] = bitSet(FahrGesch, 7);
		Status = Zug_Status::Zug_faehrt_vor;
	}
	else 
		Status = Zug_Status::Zug_faehrt_rueck;

	XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
}

void CDataXpressNet::Set_Geschwindigkeit(FahrplanPos Befehl)
{
	if (Befehl.isGeschwindigkeit())
	{
		if (Befehl.mache == FahrPlanDo::stoppen)
		{
			FahrGesch = 0;
			Status = Zug_Status::Zug_Stopped;
		}
		else
		{
			FahrGesch = Befehl.GetGeschwindigkeit();
			if (Befehl.mache == FahrPlanDo::vorwaerz_fahren) 	FahrRicht = true;
			if (Befehl.mache == FahrPlanDo::rueckwaerz_fahren) 	FahrRicht = false;
		}
		FunktionsGruppe[0] = FahrGesch;
		if (FahrRicht)
		{
			FunktionsGruppe[0] = bitSet(FahrGesch, 7);
			Status = Zug_Status::Zug_faehrt_vor;
		}
		else Status = Zug_Status::Zug_faehrt_rueck;

		XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
	}
	else
	{
		TRACE(_T(" ERROR falscher Befehl \n\n"));
	}
}

void CDataXpressNet::ReSet_Geschwindigkeit()
{
	FunktionsGruppe[0] = FahrGesch;
	if (FahrRicht) FunktionsGruppe[0] = bitSet(FahrGesch, 7);

	XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
	if (FahrRicht) 			Status = Zug_Status::Zug_faehrt_vor;
	else Status = Zug_Status::Zug_faehrt_rueck;
}

void CDataXpressNet::Set_Stop()
{
	byte Data = 0;
	if (FahrRicht) FunktionsGruppe[0] = bitSet(Data, 7);
	XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
	Status = Zug_Status::Zug_Stopped;
}

void CDataXpressNet::Set_Halt()
{
	byte Data = 0;
	if (FahrRicht) FunktionsGruppe[0] = bitSet(Data, 7);
	XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
	Status = Zug_Status::Zug_haelt;
}

void CDataXpressNet::Set_Halt(FahrplanPos Befehl)
{
	if (Befehl.isGeschwindigkeit())
	{
		if (Befehl.mache == FahrPlanDo::vorwaerz_fahren) 	FahrRicht = true;
		if (Befehl.mache == FahrPlanDo::rueckwaerz_fahren) 	FahrRicht = false;
		FahrGesch = Befehl.GetGeschwindigkeit();
		byte Data = 0;
		if (FahrRicht) FunktionsGruppe[0] = bitSet(Data, 7);
		XpressNet->SendeZugDaten(XpNSendwas::FGruppe0, Adresse, FunktionsGruppe[0]);
		Status = Zug_Status::Zug_haelt;
	}
	else
	{
		TRACE(_T(" ERROR falscher Befehl \n\n"));
	}
}


bool CDataXpressNet::isNext_Melder(byte Melder)
{
	return (Melder_next == Melder);
}

bool CDataXpressNet::isHalt()
{
	return (Status == Zug_Status::Zug_haelt);
}

bool CDataXpressNet::isVorwärtz()
{
	return FahrRicht;
}

bool CDataXpressNet::isOnGleis()
{
	if (Block_ist == 0x00)
	{
		Betriebs_Modus = Zug_Steuerung::nicht_Betriebs_bereit;
		return false;
	}
	return true;
}

bool CDataXpressNet::isActive()
{
	return (!(Betriebs_Modus == Zug_Steuerung::nicht_Betriebs_bereit));
}

bool CDataXpressNet::isAutomaticOn()
{
	return ((Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb)&&(Zug_active));
}

bool CDataXpressNet::isHand()
{
	return (Betriebs_Modus == Zug_Steuerung::Hand_Betrieb);
}

bool CDataXpressNet::Prüfrichtung()
{
	if (Blick)
		return(FahrRicht);
	else
		return(!FahrRicht);
}

Zug_Status CDataXpressNet::Get_Status()
{
	return Status;
}

