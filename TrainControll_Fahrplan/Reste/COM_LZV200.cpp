#include "pch.h"
#include "COM_LZV200.h"
#include "TrainControll_FahrplanDlg.h"

CCom_LZV200::CCom_LZV200()
{
	p_hCom = NULL;
	p_aCom = NULL;
	p_StaticCom = NULL;
	Neue_Daten = false;
	Letzte_BC_Meldung = BC_Meldungen::BC_Alles_Aus;
	LokNothalt = false;
	LokFree = false;
	LokRichtung = false;
	Fahrstufenzahl = 0;
	Fahrgeschwindigkeit = 0;
	LokAdresse =  0;
	Lok_FGruppe0 = 0;
	Lok_FGruppe1 = 0;
	Lok_FGruppe2 = 0;
	Lok_FGruppe3 = 0;
	Lok_FGruppe4 = 0;
	Lok_FGruppe5 = 0;
	CV_Adr = 0;
	CV_Wert = 0;
	CV_Err_Data = 0;
}

CCom_LZV200::~CCom_LZV200()
{
}

void CCom_LZV200::Set_Com(COM_Info* Port)
{
	p_hCom = &Port->COM_Handel;
	p_aCom = &Port->COM_Active;
}

void CCom_LZV200::Init(CStatic_DrawBMP* LED) 
{
	p_StaticCom = LED;
	if (*p_aCom)
	{
		p_StaticCom->Set_Status(1, L" Com on LZV200");
	}
	else
	{
		p_StaticCom->Set_Status(0, L" no LZV200");
	}
}


bool CCom_LZV200::NoComtoLZV200()
{
	return !(*p_aCom);
}

void CCom_LZV200::Send_Message(byte* SD, byte l)
{
	get_XOR(SD);
	LPDWORD Error_Com = NULL;
	LPCOMSTAT Status_Com = NULL;
	DWORD bytesSend;

	if (!WriteFile(*p_hCom, SD, l, &bytesSend, 0))
	{
		ClearCommError(*p_hCom, Error_Com, Status_Com);
		p_StaticCom->Set_Status(3, L" Error Senden !");
	}
}

bool CCom_LZV200::Read_Message(byte* RD)
{
	byte  c = 0;
	DWORD bytesRead;
	byte Nr = 0;
	byte Len = 0;
	BOOL Error;
	for (;;)
	{
		Error = ::ReadFile(*p_hCom, &c, 1, &bytesRead, NULL);
		if (!Error)
		{
			p_StaticCom->Set_Status(3, L" Error Read !");
		}
		if (bytesRead == 1)
		{
			if (c == 0xFF)			Nr = 0;
			if (Nr == 2) // Länge der Meldung bestimmen
			{
				Len = (c & 0x0F) + 4;
			}
			*(RD + Nr) = c;
			Nr++;
			if (Nr == Len) 	return(true);
		}
		else
		{
			return(false);
		}
	}
}

bool CCom_LZV200::check_XOR_Data(byte* data, byte lenData)
{
	byte XORByte_ist;
	byte XORByte_neu;
	int n = (data[2] & 0x0F) + 1; // ersten zwei bytes = Frame 1 und Frame 2
	XORByte_ist = data[n];
	get_XOR(data);
	XORByte_neu = data[n];
	if (XORByte_ist == XORByte_neu)
		return true;
	else
		return false;
}

void CCom_LZV200::get_XOR(byte* data)
{
	data = data + 2;
	byte len = (*data & 0x0F) + 1;
	byte XOR = 0x00;

	for (byte i = 0; i < len; i++)
	{
		XOR = XOR ^ *data;
		data++;
	}
	*data = XOR;
}

void CCom_LZV200::WarteDaten()
{
	unsigned int Timer = MAXUINT;
	Neue_Daten = false;

	do
	{
		Timer--;
		if (Timer == 0)
		{
			Neue_Daten = true;
			p_StaticCom->Set_Status(2, L"Time Out !!");
		}
	} while (!Neue_Daten);
}

void CCom_LZV200::StartProcess()
{
	Read_Version();

	SendeLVZ_Power(true);

}


void CCom_LZV200::Get_VersionInfo(CString* Text)
{
	Softwareversion = L"LZV200 V3.4";

	*Text = Softwareversion;
}

bool CCom_LZV200::Get_Power_onGleis()
{
	return (Letzte_BC_Meldung == BC_Meldungen::BC_Alles_An);
}

void CCom_LZV200::Read_Version()
{
	Befehl_Text.Format(_T("lese Version LZV200 "));
	byte GetVersion[5] = { 0xFF, 0xFE, 0x21, 0x21, 0x00 };
	Send_Message(GetVersion,sizeof(GetVersion) );
	
}

void CCom_LZV200::Interface_Status()
{//2.1 Interface Status Befehl
	byte Befehl[5] = { 0xFF, 0xFE, 0xF1, 0x01, 0xF0 };
	Send_Message(Befehl, sizeof(Befehl));
	Befehl_Text.Format(_T("frage nach Interface Status "));
}

void CCom_LZV200::SendeLVZ_Power(bool Bit)
{
	if (Bit)
	{// 3.2.1. Alles An
		byte Befehl[5] = { 0xFF, 0xFE, 0x21, 0x81, 0xA0 };
		Send_Message(Befehl, sizeof(Befehl));
		Befehl_Text.Format(_T("schalte Power on Gleis an "));
	}
	else
	{// 3.2.2. Alles Aus
		byte Befehl[5] = { 0xFF, 0xFE, 0x21, 0x80, 0xA1 };
		Send_Message(Befehl, sizeof(Befehl));
		Befehl_Text.Format(_T("schalte Power on Gleis aus"));
	}
}



void CCom_LZV200::Alle_Loks_Anhalten()
{
	Befehl_Text.Format(_T("alle Loks anhalten"));

}

void CCom_LZV200::die_Lok_anhalten(Lok_Adresse ADR)
{
	Befehl_Text.Format(_T("Lok mit Adr: %i anhalten"));

}

void CCom_LZV200::SendeAsknachLokDaten(Lok_Adresse Adresse)
{
	
	byte Befehl_A[7] = { 0xFF, 0xFE, 0xE3, 0x00, 0x00, 0x00, 0x00 };
	// 3.2.25 Lokinformationen anfordern
	// Antwort unter 3.1.9
	// F0 .. F4 & F5 .. F12
	

	byte Befehl_B[7] = { 0xFF, 0xFE, 0xE3, 0x07, 0x00, 0x00, 0x00 };
	//3.2.25.1 Funktionsstatus I  {F0 bis F12 } anfordern Tastend
	// Antwort unter 3.1.11 
	
	byte Befehl_C[7] = { 0xFF, 0xFE, 0xE8, 0x07, 0x00, 0x00, 0x00 };
	//3.2.25.2 Funktionsstatus II {F13 bis F28 } anfordern Tastend
	// Antwort unter 3.1.12
	
	byte Befehl_D[7] = { 0xFF, 0xFE, 0xE8, 0x07, 0x00, 0x00, 0x00 };
	//3.2.25.3 Funktionsstatus II {F13 bis F28 } anfordern 
	// Antwort unter 3.1.9.2
	Befehl_Text.Format(_T("Lok Daten mit Adr: %i abfragen F0 .. F12"),Adresse);
	Befehl_A[4] = Befehl_B[4] = Befehl_C[4] = Befehl_D[4] = Adresse.HighAdr(); // AH
	Befehl_A[5] = Befehl_B[5] = Befehl_C[5] = Befehl_D[5] = Adresse.LowAdr();  // AL
	
	Send_Message(Befehl_A, sizeof(Befehl_A));
	WarteDaten();
	
	//Send_Message(Befehl_D, sizeof(Befehl_D));
	//WarteDaten();

}

void CCom_LZV200::HoleZugData(byte* Data, Lok_Adresse Adr)
{
	Data[0]	= Lok_FGruppe0;
	Data[1] = Lok_FGruppe1;
	Data[2] = Lok_FGruppe2;
	Data[3] = Lok_FGruppe3;
	Data[4] = Lok_FGruppe4;
	Data[5] = Lok_FGruppe5;
}

void CCom_LZV200::Sende_Write_CV(byte CV, byte Wert)
{
	// 3.2.15 Schreibe Programmieren CV 1 - 2555
	byte Befehl[7] = { 0xFF,0xFE,0x23,0x16,0x00,0x00,0x00};
	Befehl[4] = CV;
	Befehl[5] = Wert;
	Send_Message(Befehl, sizeof(Befehl));
	Befehl_Text.Format(_T("schreibe CV %i = %i "), CV, Wert);
}

void CCom_LZV200::Sende_Read_CV(byte CV, byte Wert)
{
	// 3.2.7. Leseanfrage CV 1 - 255
	byte Befehl[6] = { 0xFF,0xFE,0x22,0x18,0x00,0x00 };
	Befehl[4] = CV;
	Send_Message(Befehl, sizeof(Befehl));
	Befehl_Text.Format(_T("lese  CV %i  "), CV);

}

byte CCom_LZV200::Hole_Zug_CV(WORD CV, byte* Wert)
{
	// 
	byte Befehl[5] = { 0xFF,0xFE,0x21,0x10,0x00 };
	Send_Message(Befehl, sizeof(Befehl));
	WarteDaten();
	if (CV == CV_Adr)
	{
		*Wert = CV_Wert;
	}
	Befehl_Text.Format(_T("lese von CV %i = %i "), CV, CV_Wert);

	return CV_Err_Data;
}



void CCom_LZV200::SendeZugDaten(XpNSendwas was, Lok_Adresse Adr, byte Daten)
{//                      0    1    2    3ken 4    5    6    7xor
	byte Befehl[8] = { 0xFF,0xFE,0xE4,0x00,0x00,0x00,0x00,0x00 };
	Befehl[4] = Adr.HighAdr(); // AH
	Befehl[5] = Adr.LowAdr();  // AL
	// Daten sind schon codiert
	switch (was)
	{
	case XpNSendwas::FGruppe0: // Befewhl fahren absetzen
		Befehl[3] = 0x12 ; // Kennung
		Befehl[6] = Daten; // R00v vvvv 
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 0 "), Adr);
		break;
	case XpNSendwas::FGruppe1:
		Befehl[3] = 0x20; // Kennung
		Befehl[6] = Daten; // 000 F0 F4 F3 F2 F1 
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 1 "), Adr);
		break;
	case XpNSendwas::FGruppe2:
		Befehl[3] = 0x21; // Kennung
		Befehl[6] = Daten; // 0000 F8 F7 F6 F5 
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 2 "), Adr);
		break;
	case XpNSendwas::FGruppe3:
		Befehl[3] = 0x22; // Kennung
		Befehl[6] = Daten; // 0000 F12 F11 F10 F9 
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 3 "), Adr);
		break;
	case XpNSendwas::FGruppe4:
		Befehl[3] = 0x23; // Kennung
		Befehl[6] = Daten; // F20 F19 F18 F17 F16 F15 F14 F13 
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 4 "), Adr);
		break;
	case XpNSendwas::FGruppe5:
		Befehl[3] = 0x28; // Kennung
		Befehl[6] = Daten; // F28 F27 F26 F25 F24 F23 F22 F21  
		Befehl_Text.Format(_T("schreibe Lok %i = FuntionGruppe  Nr 5 "), Adr);
		break;
	default:
		break;
	}
	Send_Message(Befehl, sizeof(Befehl));
}
void CCom_LZV200::verarbeite_Meldung_Zentrale()
{
	static byte Data[11];
	if (Read_Message(Data))
	{
		switch (Data[2])
		{
		case 0x01:
			Meldung_Header_x01(Data);
			break;
		case 0x02:
			Meldung_Header_x02(Data);
			break;
		case 0x61:
			Meldung_Header_x61(Data);
			break;
		case 0x63:
			Meldung_Header_x63(Data);
			break;
		case 0x81:
			Meldung_Header_x81(Data);
			break;
		case 0xE1:
			Meldung_Header_xE1(Data);
			break;
		case 0xE2:
			Meldung_Header_xE2(Data);
			break;
		case 0xE3:
			Meldung_Header_xE3(Data);
			break;
		case 0xE4:
			Meldung_Header_xE4(Data);
			break;
		case 0xE5:
			Meldung_Header_xE5(Data);
			break;
		case 0xE6:
			Meldung_Header_xE6(Data);
			break;
		case 0xF2: // Geräte Adresse
			Meldung_Header_xF2(Data);
			break;
		default:
			break;
		}
	}
}

void CCom_LZV200::updateInfo()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	APP->updateInfoLZV2000(Letzte_Antwort,Letzte_BC_Meldung);
}

void CCom_LZV200::Meldung_Header_x01(byte* Data)
{
	switch (Data[3])
	{
	case 0x01:
		Letzte_Antwort = " Header angebenene Anzahl Bytes stimmt nicht ";
		break;
	case 0x02:
		Letzte_Antwort = " Fehler zwischen Interface und Zentrale ";
		break;
	case 0x03:
		Letzte_Antwort = " unbekanter Fehler ";
		break;
	case 0x04:
		Letzte_Antwort = " Befehl ist an Zentrale geschickt";
		break;
	case 0x05:
		Letzte_Antwort = " Zentrale adressiert das Interface nicht mehr ";
		break;
	case 0x06:
		Letzte_Antwort = " Puffer - Überlauf im Interface ";
		break;
	case 0x07:
		Letzte_Antwort = " Zentrale adressiert das Interface wieder";
		break;
	case 0x08:
		Letzte_Antwort = " Derzeit können keine Befehle an die Zentrale gesendet werden ";
		break;
	case 0x09:
		Letzte_Antwort = " Fehler in den Befehlsparametern";
		break;
	case 0x10:
		Letzte_Antwort = " unbekannter Fehler ";
		break;
	default:
		break;
	}
	TRACE2("--x01- %s | %s \n", Befehl_Text, Letzte_Antwort);
	Neue_Daten = false;
	updateInfo();
}

void CCom_LZV200::Meldung_Header_x02(byte* Data)
{
}

void CCom_LZV200::Meldung_Header_x61(byte* Data)
{
	bool no_BC = true;
	switch (Data[3])
	{
	case 0x00:
		Letzte_BC_Meldung = BC_Meldungen::BC_Alles_Aus;
		//Letzte_Antwort = "BroadCast  = alles aus";
		TRACE(_T("---- BroadCast = alles aus         ---- \n"));
		no_BC = false;
		break;
	case 0x01:
		Letzte_BC_Meldung = BC_Meldungen::BC_Alles_An;
		//Letzte_Antwort = "BroadCast = alles an";
		TRACE(_T("---- BroadCast = alles an          ---- \n"));
		no_BC = false;
		break;
	case 0x02:
		Letzte_BC_Meldung = BC_Meldungen::BC_Programmiermode;
		//Letzte_Antwort = "BroadCast =   Programmier Modus";
		TRACE(_T("---- BroadCast = Programmier Modus ---- \n"));
		no_BC = false;
		break;
	case 0x11:
		// 0x11 P-Info bereit
		Letzte_Antwort = "Zentrale Bereit";
		CV_Err_Data = 2;
		break;
	case 0x12:
		// 0x12 P-Info Kurzschuß
		Letzte_Antwort = "Kurzschluß erkannt ";
		CV_Err_Data = 2;
		break;
	case 0x13:
		// 0x13 P-Info Keine Daten
		Letzte_Antwort = "Kein Decoder erkannt ";
		CV_Err_Data = 1;
		break;
	case 0x1f:
		// 0x1f P-Info Busy
		Letzte_Antwort = "Zentrale beschäftig ";
		CV_Err_Data = 3;
		break;
	case 0x80:
		// 0x80 3.1.5 Übertragungsfehler
		Letzte_Antwort = "Übertragungsfehler ";
		break;
	case 0x81:
		// 0x81 3.1.6 Zentrale Busy
		Letzte_Antwort = " Zentrale ist Busy"; 
		break;
	case 0x82:
		// 0x82 3.1.7 Befehl in Zentrale nicht vorhanden
		Letzte_Antwort = " Zentrale kennnt den Befehl nicht"; 
		break;
	default:
		// 0x80+F DTR Fehler
		Letzte_Antwort = " DTR Fehler"; 
		
		break;
	}
	updateInfo();
	if (no_BC)
	{
		TRACE2("--x61- %s | %s \n", Befehl_Text, Letzte_Antwort);
	}
}

void CCom_LZV200::Meldung_Header_x63(byte* Data)
{
	CString Text1;
	CString Text2;
	byte V1 = 0;
	byte V2 = 0;

	switch (Data[3])
	{
	case 0x10: // P-InfoDaten 3 Byte
		break;
	case 0x14: // CV 1.. 255 
			   // 3.1.2.6  Daten 4 byte Format
		CV_Adr = Data[4];
		if (CV_Adr == 0) CV_Adr = 1024;
		CV_Wert = Data[5];
		CV_Err_Data = 0;
		Letzte_Antwort.Format(_T("Data - CV  %i = %i"), CV_Adr, CV_Wert);
		break;
	case 0x15: // CV 256 ... 511
		CV_Adr = Data[4]+ 0x10;
		CV_Wert = Data[5] ;
		break;
	case 0x16:// CV 512 ... 767
		CV_Adr = Data[4] + 0x20;
		CV_Wert = Data[5] ;
		break;
	case 0x17:// CV 768 ... 1023
		CV_Adr = Data[4] + 0x30;
		CV_Wert = Data[5];
		break;
	case 0x21: // SoftwareVersion
		if (Data[5] == 0x00) { Text2 = L"/ LZ 100"; }
		if (Data[5] == 0x01) { Text2 = L"/ LH 200"; }
		if (Data[5] == 0x02) { Text2 = L"/ DPC   "; }
		V1 = Data[4] >> 4; 
		V2 = Data[4] & 0x0F;
		Text1.Format(L"V%i.%i   ",V1,V2);
		Softwareversion = L" Lenz LZV 200 " + Text1 + Text2;
		p_StaticCom->Set_Status(1, Softwareversion);
		Letzte_Antwort.Format(_T("Hardware = %s"), Softwareversion);
		break;
	default:
		break;
	}
	Neue_Daten = true;
	TRACE2("--x63- %s | %s \n", Befehl_Text, Letzte_Antwort);

}

void CCom_LZV200::Meldung_Header_x81(byte* Data)
{// 3.1.1.3 BC "Alle Loks Aus" (Nothalt)
	if (Data[3]== 0x00)
	{

	}
	TRACE0("--x81- \n");
}

void CCom_LZV200::Meldung_Header_xE1(byte* Data)
{
	TRACE0("--xE1- \n");
}

void CCom_LZV200::Meldung_Header_xE2(byte* Data)
{
	// 3.1.9.4 Lokinformation Lockadresse ist die Basisadresse einer Mehrfachtraktion
	TRACE0("--xE2- \n");
}

void CCom_LZV200::Meldung_Header_xE3(byte* Data)
{ // Antwort auf Lockanfrage 3.2.25
	switch (Data[3])
	{
	case 0x30:
		// 3.1.13 Lokinformation bei Adress-Suchanfragen
		// Antwort aus 3.2.30
		// Normale Lok Daten in Data[4] und [5] Ah + AL
		break;
	case 0x40:
	   //3.1.10 Lok besetzt
		LokAdresse = Data[5]; // nur für Lok Adresse bis 100
		break;
	case 0x50:
		//3.1.11 Funktionsstatus F0 bis F12 der angefragten Lok
		// Data[4] S0 = 0 0 0 S0 S4 S3 S2 S1
		// Data[5] S1 = S12 S11 S10 S9 S8 S7 S6 S5
		break;
	case 0x51:
		//3.1.12 Funktionsstatus F13 bis F28 der angefragten Lok
		break;
	case 0x52:
		// Beschreinbung  3.1.9.2 Funktionszustand F13 bis F28
		// Data[4] = F20 F19 F18 F17 F16 F15 F14 F13
		Lok_FGruppe2 = Data[4];
		// Data[5] = F28 F27 F26 F25 F24 F23 F22 F21
		Lok_FGruppe3 = Data[5];
		break;
	default:
		break;
	}
	TRACE2("--xE3- %s | %s \n", Befehl_Text, Letzte_Antwort);
	Neue_Daten = true;
}

void CCom_LZV200::Meldung_Header_xE4(byte* Data)
{ // Antwort auf Lockanfrage 3.2.25 
  // Beschreinbung  3.1.9.1 Lockinformation normale Lok
	// F0 = R00v vvvv 
	Lok_FGruppe0 = Data[4];
	// F1 = 0 0 0 F0 F4 F3 F2 F1 
	Lok_FGruppe1 = Data[5];
	// F2 = F12 F11 F10 F9 F8 F7 F6 F5
	Lok_FGruppe2 = Data[6];
	Neue_Daten = true;
	TRACE1("--xE4- %s  | Daten von Gruppe 0 ... 2 \n",Befehl_Text);
}

void CCom_LZV200::Meldung_Header_xE5(byte* Data)
{
	// 3.1.9.3 Lokinformation Lok befindet sich in einer Mehrfachtraction
	TRACE0("--xE5- Daten  \n");
}
void CCom_LZV200::Meldung_Header_xE6(byte* Data)
{
	// 3.1.9.5 Lokinformation Lok befindet sich in einer Doppeltraktion
	TRACE0("--xE6- Daten  \n");
}
void CCom_LZV200::Meldung_Header_xF2(byte* Data)
{
	bool empfängt;
	int  V1;
	int  V2;

	switch (Data[3])
	{
	case 0x01:
		// 2.1 Staus Interface
		empfängt = Data[4] & 0x01;
		break;
	case 0x02:
		// 2.2 XpressNet Version
		V1 = Data[4] > 4;
		V2 = Data[4] & 0x0f;
		break;
	default:
		break;
	}
	TRACE0("--xF2- Daten \n");
}
