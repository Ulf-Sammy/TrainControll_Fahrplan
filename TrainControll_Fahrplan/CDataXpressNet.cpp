#include "pch.h"
#include "CDataXpressNet.h"
#include "TrainControll_FahrplanDlg.h"

CDataXpressNet::CDataXpressNet() 
    : ProgMode(false), 
      Zug_active(false), 
      Betriebs_Modus(Zug_Steuerung::nicht_Betriebs_bereit), 
      FahrRicht(true), 
      Blick(true), 
      FahrGesch(0), 
      Block_ist(0), 
      Block_soll(0), 
      Block_best(0), 
      Melder_next(0xFF)
{
}

CDataXpressNet::CDataXpressNet(CString LokName) 
	:ProgMode(false),
	Zug_active(false), 
	Betriebs_Modus(Zug_Steuerung::nicht_Betriebs_bereit),
	FahrRicht(true), 
	Blick(true),
	FahrGesch(0),
	Block_ist(0), 
	Block_soll(0),
	Block_best(0), 
	Melder_next(0xFF)
{
	CString DateiName;
	Name = LokName;
	Name.Trim();
	DateiName = _T(FILE_ALLE_ZUEGE_BILDER)+(Name)+L".BMP"; //_T("Images\\Spreewald.bmp")
	Bild = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), DateiName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);	
}


CDataXpressNet::~CDataXpressNet()
{
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

byte CDataXpressNet::Get_Soll_Block() const
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

void CDataXpressNet::Set_Adresse()
{
	Adresse = Lok_Adresse(Decoder_Data.Get_Adresse());
}

CString CDataXpressNet::Text_Block() const
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
}

void CDataXpressNet::Set_Status(Zug_Status UserSet_Status)
{
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
}

void CDataXpressNet::Set_BlickRichtung(bool LokBlick)
{
	Blick = LokBlick;
}

void CDataXpressNet::Set_auf_Gleis(byte BlockNr)
{
	Block_ist = BlockNr;
	Block_soll = BlockNr;
}

void CDataXpressNet::Set_Lok_Nr(byte Nr_EDV)
{
	
}

void CDataXpressNet::Set_Funktion(byte Nr, bool bit)
{
	byte G_Nr = FunNr[Nr];

	bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], bit);
	//XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]); 
}

void CDataXpressNet::Set_Funktion(FahrplanPos Befehl)
{
	if (Befehl.mache == FahrPlanDo::schalten_Funk)
	{ 
		byte G_Nr = FunNr[Befehl.Get_Funtion().GetWert()];
		
		bitWrite(FunktionsGruppe[G_Nr], BitNr[Befehl.Get_Funtion().GetWert()], Befehl.Get_Funtion().GetBit());
		//XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
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
		//XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
}

void CDataXpressNet::Set_Funktion_Rangieren(bool SW)
{
	byte Nr = Decoder_Data.Get_Rangier_FunktionTaste();
	if (Nr > 0)
	{
		byte G_Nr = FunNr[Nr];

		bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], SW);
		//XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
}

void CDataXpressNet::Set_Funktion_VerzögerungsZeit(bool SW)
{
	byte Nr = Decoder_Data.Get_VerzögZ_FunktionTaste();
	if (Nr > 0)
	{
		byte G_Nr = FunNr[Nr];

		bitWrite(FunktionsGruppe[G_Nr], BitNr[Nr], SW);
		//XpressNet->SendeZugDaten((XpNSendwas)G_Nr, Adresse, FunktionsGruppe[G_Nr]);
	}
}

bool CDataXpressNet::Get_Funktion(byte Nr)
{
	byte G_Nr = 0;
	if (Nr > 28) Nr = 0;
	
	G_Nr = FunNr[Nr];
	return bitRead(FunktionsGruppe[G_Nr], BitNr[Nr]);
}

bool CDataXpressNet::isNext_Melder(byte Melder) const
{
	return (Melder_next == Melder);
}

bool CDataXpressNet::isHalt() const
{
	return (Status == Zug_Status::Zug_haelt);
}

bool CDataXpressNet::isVorwärtz() const
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

bool CDataXpressNet::isActive() const
{
	return (!(Betriebs_Modus == Zug_Steuerung::nicht_Betriebs_bereit));
}

bool CDataXpressNet::isAutomaticOn() const
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

void CDataXpressNet::ASK_LokData(byte Info, byte* Data)
{
	Data[0] = WifiKennung; // Kennung
	Data[1] = 0x09; // Länge des Befehls (9 Bytes)
	Data[2] = 0xFF;
	Data[3] = 0xFE;
	Data[4] = 0xE3;
	Data[6] = Adresse.HighAdr(); // AH
	Data[7] = Adresse.LowAdr();  // AL
	Data[8] = 0x00; // XOR Checksumme wird später berechnet im GIGA

	switch (Info)
	{
	case 0: // Lokdaten anfordern // Antwort unter 3.1.9
		Data[5] = 0x00; // F0 .. F4 & F5 .. F12
		break;
	case 1: // Funktions status I  anfordern {F0 bis F12 }
		Data[5] = 0x07; // F0 .. F4 & F5 .. F12
		break;
	case 2: //3.2.25.1 Funktionsstatus II  {F13 bis F28 } 
		Data[5] = 0x08; // F13 ... F28
		break;
	case 3: //3.2.25.2 Funktionsstatus II {F13 bis F28 }
		Data[5] = 0x09; // F13 ... F28
		break;
	default:
		break;
	}
	Decoder_Data.Fill_Tasten_DlgRun();
}

void CDataXpressNet::Set_Stop(byte* Data) // Zug wird gestoppt. 
{
	Data[0] = WifiKennung; // Kennung
	Data[1] = 0x09; // Länge des Befehls (9 Bytes)
	Data[2] = 0xFF;
	Data[3] = 0xFE;
	Data[4] = 0x92;
	Data[5] = Adresse.HighAdr(); // AH
	Data[6] = Adresse.LowAdr();  // AL
	Data[7] = 0x00; 	// Speed Byte 7 = |R00V VVVV|
	Data[8] = 0x00; // XOR Checksumme wird später berechnet im GIGA
	Status = Zug_Status::Zug_Stopped;
	FahrGesch = 0; // Geschwindigkeit auf 0 setzen
	//Wifi_Client->Send_Data(); // Sende den Befehl über das Wifi Client Objekt
}

void CDataXpressNet::Set_Halt(byte* Data) // Zug wird angehalten
{
	Set_Geschwindigkeit(0, FahrRicht,Data); // Geschwindigkeit auf 0 setzen
}

void CDataXpressNet::Set_Halt(FahrplanPos Befehl, byte* Data)
{
	Set_Halt(Data);
}
void CDataXpressNet::Set_Geschwindigkeit(byte Geschwindigkeit, bool FahrtRichtung, byte* Data)
{  
	FahrRicht = FahrtRichtung;
	FahrGesch = Geschwindigkeit;

	Data[0] = WifiKennung; // Kennung
	Data[2] = 0xFF;
	Data[1] = 0x09; // Länge des Befehls (9 Bytes)
	Data[3] = 0xFE;
	Data[4] = 0x12; 
	Data[5] = Adresse.HighAdr(); // AH
	Data[6] = Adresse.LowAdr();  // AL
	Data[7] = DCC_SPEED_coded[FahrGesch]; 	// Speed Byte 7 = |R00V VVVV|
	Data[8] = 0x00; // XOR Checksumme wird später berechnet im GIGA
	if (FahrRicht)
	{
		Data[7] = bitSet(FahrGesch, 7);
		Status = Zug_Status::Zug_faehrt_vor;
	}
	else
		Status = Zug_Status::Zug_faehrt_rueck;
	if (Geschwindigkeit == 0)		Status = Zug_Status::Zug_Stopped;
}
void CDataXpressNet::Set_Geschwindigkeit(FahrplanPos F_Befehl, byte* Data)
{
	bool Fahr_Richt = true; // Vorwärts
	byte Fahr_Gesch = 0; // Geschwindigkeit

	if (F_Befehl.isGeschwindigkeit())
	{
		if (F_Befehl.mache == FahrPlanDo::stoppen)
		{
			Fahr_Gesch = 0;
		}
		else
		{
			Fahr_Gesch = F_Befehl.GetGeschwindigkeit();
			if (F_Befehl.mache == FahrPlanDo::vorwaerz_fahren) 	Fahr_Richt = true;
			if (F_Befehl.mache == FahrPlanDo::rueckwaerz_fahren) 	Fahr_Richt = false;
		}
		Set_Geschwindigkeit(Fahr_Gesch, Fahr_Richt, Data);
	}
	else
	{
		TRACE(_T(" ERROR falscher Befehl \n\n"));
	}
}

byte CDataXpressNet::Progmmiere_RW_CV(bool RW, byte CV, byte Wert, byte* Data)
{
	if (!ProgMode)
	{
		ProgMode = true;
		CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
		APP->InfoModus.Set_Status(2);
	}
	//if (RW)  // true dann schreiben
	//	return	XpressNet->Sende_Write_CV(CV, Wert);
	//else			// false weil nur lesen
	//	return  XpressNet->Sende_Read_CV(CV, Wert);
	return 0; // Dummy Rückgabe, da XpressNet nicht implementiert ist
}

