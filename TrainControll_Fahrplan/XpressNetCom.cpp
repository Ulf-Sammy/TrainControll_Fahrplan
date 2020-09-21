#include "pch.h"
#include "XpressNetCom.h"

CXpressNetCom::CXpressNetCom(void)
{
	PortNr = 0;
	m_hCom = NULL;
    HeadSend_Liste = 0;
	TailSend_Liste = 0;
	bytesSend = 0;

}
CXpressNetCom::~CXpressNetCom(void)
{

}

bool CXpressNetCom::OpenCom(int Port)
{
	CString strPortName;
	PortNr = Port;
	strPortName.Format(_T("COM%d"), PortNr);
	
	m_hCom = ::CreateFile(strPortName,
		GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
		if (m_hCom == INVALID_HANDLE_VALUE) 
	{
		strPortName.Format(_T(" Serieller Port Com%d: kann nicht geöffnet werden "), Port);
		TRACE(_T("ERROR %s !!\n"), strPortName);
		//AfxMessageBox(strPortName,MB_OK | MB_ICONSTOP   );
		return (false);
	}
	else
	{
		DCB dcbSerialParams = {0};

		dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
		
		if(!::GetCommState(m_hCom, &dcbSerialParams))
		{
			TRACE(_T(" 1.ERROR COM \n"));
		}
		else
		{
			dcbSerialParams.BaudRate = 112500; 
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.Parity   = NOPARITY;
			dcbSerialParams.StopBits = ONESTOPBIT;
			if(!SetCommState(m_hCom, &dcbSerialParams))
			{
				TRACE(_T(" 2.ERROR COM \n"));
			}
			COMMTIMEOUTS timeouts = { 0 };
			GetCommTimeouts(m_hCom,&timeouts);
			//*

			timeouts.ReadIntervalTimeout=50;
			timeouts.ReadTotalTimeoutConstant=200;
			timeouts.ReadTotalTimeoutMultiplier=10;
			
			timeouts.WriteTotalTimeoutConstant=50;
			timeouts.WriteTotalTimeoutMultiplier=10;
			//*/
			if(!SetCommTimeouts(m_hCom, &timeouts))
			{
				TRACE(_T(" 3.ERROR COM \n"));
			}
		}
	}
	return(true);

}
void CXpressNetCom::CloseCom()
{
	if (!NoComToXpressNet())
	{
		Befehl_Send[0] = COM_WRITE_MOD;
		Befehl_Send[1] = (byte)ControlStatus::Ende_COM;
		Befehl_Send[2] = 0;
		SetTC_Message();
	}
	::CloseHandle(m_hCom);
}
bool CXpressNetCom::NoComToXpressNet()
{
	if (m_hCom == NULL) return (true);
	return (m_hCom == INVALID_HANDLE_VALUE);
}

void CXpressNetCom::SendeNeueDaten()
{
	if (HeadSend_Liste != TailSend_Liste)
	{		
		byte l = COM_LEN(Send_Liste[TailSend_Liste][0]);
		if (!WriteFile(this->m_hCom, &Send_Liste[TailSend_Liste], l, &bytesSend, 0))
		{
			ClearCommError(this->m_hCom, &this->Error_Com, &this->COM_status);
			TRACE(_T("Fehler beim Senden !!! \n"));
		}
		else
		{
			TailSend_Liste = (TailSend_Liste + 1) % SENDBUFFER;
			Sleep(300);
		}
	}
}

void CXpressNetCom::SendeAsknachLokDaten(byte Adr)
{
	Send_Liste[HeadSend_Liste][2] = 0;// nicht mehr benötigt
	Send_Liste[HeadSend_Liste][1] = Adr;
	Send_Liste[HeadSend_Liste][0] = COM_READ_ZUG_DA;
	Neu_Send_Befehl();
}
void CXpressNetCom::SendeZugDaten(XpNSendwas was, byte Adresse, byte Daten)
{
	Send_Liste[HeadSend_Liste][2] = Daten ;
	Send_Liste[HeadSend_Liste][1] = Adresse;
	Send_Liste[HeadSend_Liste][0] = SendAdr[(byte)was];
	Neu_Send_Befehl();
}
void CXpressNetCom::SendeWeicheDaten(TrainCon_Paar Wl)
{
	TRACE(_T("schalte Weiche Nr [%i] = %i \n"), Wl.GetWert(), Wl.GetBit());
	Send_Liste[HeadSend_Liste][2] = (byte)Wl.GetBit();
	Send_Liste[HeadSend_Liste][1] = Wl.GetWert();
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_WEICHE;
	Neu_Send_Befehl();
}
void CXpressNetCom::SendeLVZ_Power(bool Bit)
{
	Send_Liste[HeadSend_Liste][1] = (byte)Bit;
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_LVZ_POWER;
	Neu_Send_Befehl();
}
void CXpressNetCom::Sende_Write_CV(byte CV, byte Wert)
{
	Send_Liste[HeadSend_Liste][2] = Wert;
	Send_Liste[HeadSend_Liste][1] = CV;
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_CV;
	Neu_Send_Befehl();

}
void CXpressNetCom::Sende_Read_CV(byte CV, byte Wert)
{
	Send_Liste[HeadSend_Liste][2] = Wert;
	Send_Liste[HeadSend_Liste][1] = CV;
	Send_Liste[HeadSend_Liste][0] = COM_READ_CV;
	Neu_Send_Befehl();
}
void CXpressNetCom::Sende_Setto_Prog(bool onoff)
{
	Send_Liste[HeadSend_Liste][2] = 0;
	if( onoff)
		Send_Liste[HeadSend_Liste][1] = (byte)ControlStatus::Program;
	else
		Send_Liste[HeadSend_Liste][1] = (byte)ControlStatus::Testen;

	Send_Liste[HeadSend_Liste][0] = COM_WRITE_MOD; // Befehl Schreiben
	Neu_Send_Befehl();
}
void CXpressNetCom::Neu_Send_Befehl()
{
	HeadSend_Liste = (HeadSend_Liste + 1) % SENDBUFFER;
}
// Nur im Setup 
void CXpressNetCom::Set_In_Mode(ControlStatus Mode, byte SubMode, byte *LZVMode)
{
	bool NoCom = true;
	Befehl_Send[0] = COM_WRITE_MOD; // Befehl Schreiben
	Befehl_Send[1] = (byte)Mode;
	Befehl_Send[2] = SubMode;
	SetTC_Message();
	do
	{
	} while (!GetTC_Message());
	if ((Befehl_Read[0] == COM_ACKN_MOD) && (Befehl_Read[1] == (byte)Mode) && (Befehl_Read[2] == SubMode))
	{
		*LZVMode = Befehl_Read[3];
	}
	else
	{
		TRACE(_T(" Mode ist changed NOT OK !!!!! \n"));
	}
}
bool CXpressNetCom::Get_Acknolage_Mode(ControlStatus Mode, byte SubMode, byte * LZVMode)
{
	do
	{
	} while (!GetTC_Message());
	if ((Befehl_Read[0] == COM_ACKN_MOD) && (Befehl_Read[1] == (byte)Mode) && (Befehl_Read[2] == SubMode))
	{
		*LZVMode = Befehl_Read[3];
		return true;
	}
	return false;
}
void CXpressNetCom::SetMelder_Zeit(byte Nr)
{
	//  der Wert mit 10 msec mal genommen 
	// in der Ardunino Controller
	Befehl_Send[0] = COM_WRITE_MELDER_TI;
	Befehl_Send[1] = Nr;
	SetTC_Message();
	Sleep(300);
}
int CXpressNetCom::GetMelderAnzahl()
{ // wird nur im Setup abgefragt
	do
	{
	} while (!GetTC_Message());

	if (Befehl_Read[0] == COM_WRITE_MELDER_ST)
	{
		TRACE1("Melder Anzahl : %i \n", Befehl_Read[1]);
		return (Befehl_Read[1]); // MelderAnzahl
	}
	return 0;
}
TrainCon_Paar CXpressNetCom::ReadMelder()
{
	do
	{
	} while (!GetTC_Message());
	if (Befehl_Read[0] == COM_SEND_MELDER)
	{

		return (TrainCon_Paar(Befehl_Read[1], (bool)Befehl_Read[2]));
	}
	else
		TRACE(_T("Fehler beim Melder Setup  \n "));
	return (TrainCon_Paar(0xFF, false));
}
void CXpressNetCom::SetWeichenAnzahl(byte Nr)
{
	// Block Nr = 0 gibt es nicht hier werden die Anzahl der Weichen geschickt
	// sonst ob Block Frei oder bezetzt ist.
	Befehl_Send[0] = COM_WRITE_WEICHE_ST;
	Befehl_Send[1] = Nr;
	SetTC_Message();
	Sleep(300);
}
byte CXpressNetCom::GetStatus_Setup_LZV()
{
	do
	{
	} while (!GetTC_Message());

	if (Befehl_Read[0] == COM_SEND_LVZ_STA)
	{
		return (Befehl_Read[1]);    // Status
	}
	else
		TRACE(_T("Fehler beim melden LZV Status  \n "));
	return (0xFF);
}




bool CXpressNetCom::GetTC_Message()
{
	byte  c;
	DWORD bytesRead;
	byte Nr = 0;
	byte Len;

	for (;;)
	{
		::ReadFile(m_hCom, &c, 1, &bytesRead, NULL);
		if( bytesRead == 1)
		{
			if (Nr == 0) Len = (c & 0x07) ;
			Befehl_Read[Nr] = c;
			Nr++;
			if( Nr == (Len))
			{
				Nr = 0;
				return(true);
			}
		}
		else
		{
			return(false);
		}
	}
}
void CXpressNetCom::SetTC_Message()
{
	DWORD bytesSend;
	byte l = COM_LEN(Befehl_Send[0]);
	if(!WriteFile(this->m_hCom, &Befehl_Send, l , &bytesSend, 0))
	{
		ClearCommError(this->m_hCom, &this->Error_Com, &this->COM_status);
		TRACE(_T("Fehler beim Senden !!! \n"));
	}
}

byte CXpressNetCom::GetNextMessage()
{
	if (GetTC_Message())
	{
		TRACE3(" Neue Meldung | %x | %x | %x  \n",Befehl_Read[0], Befehl_Read[1], Befehl_Read[2]);
		return Befehl_Read[0];
	}
	else
		return false;
}

byte CXpressNetCom::Hole_Acknolage_Mode()
{
	//(Befehl_Read[0] == COM_ACKN_MOD) 
	return(Befehl_Read[1]);
	//	(Befehl_Read[2] == SubMode))
}

void CXpressNetCom::HoleZugData(byte *Data, byte Adr)
{
	if (Befehl_Read[6] == Adr)
	{
		TRACE(_T(" Daten geholt von Lok adr i%\n"), Befehl_Read[6]);
		*Data = Befehl_Read[1];
		*(Data + 1) = Befehl_Read[2];
		*(Data + 2) = Befehl_Read[3];
		*(Data + 3) = Befehl_Read[4];
		*(Data + 4) = Befehl_Read[5];
	}
}
TrainCon_Paar CXpressNetCom::HoleMelderData()
{
	return (TrainCon_Paar(Befehl_Read[1], (bool)Befehl_Read[2]));
}
TrainCon_Paar CXpressNetCom::HoleWeicheData()
{
	return (TrainCon_Paar(Befehl_Read[1], (bool)Befehl_Read[2]));
}
byte CXpressNetCom::HoleStatus_LZV()
{
	if (Befehl_Read[0] == COM_SEND_LVZ_STA)
	{
		return (Befehl_Read[1]);    // Status
	}
	else
		TRACE(_T("Fehler beim melden LZV Status  \n "));
	return (0xFF);
}

byte CXpressNetCom::Hole_CV_Wert()
{
	return(Befehl_Read[2]);;
}


byte CXpressNetCom::DoWorkonCV(bool RW, byte CV, byte Wert)
{
	bool Info;
	if (RW) // schreibe CV Wert;
	{
		Befehl_Send[0] = COM_WRITE_CV; // Befehl Schreiben
		TRACE2("write ->  CV Nr.: %i = %i \n", CV, Wert);
	}
	else
	{
		Befehl_Send[0] = COM_READ_CV; // Befehl Lesen
		TRACE2("Read <- CV N.: %i = %i \n", CV, Wert);
	}
	Befehl_Send[1] = CV;
	Befehl_Send[2] = Wert;
	SetTC_Message();
	Sleep(1000);
	Info = true;
	do
	{
		do
		{
		} while (!GetTC_Message());
		if (Befehl_Read[0] == COM_SEND_CV)
		{
			if (Befehl_Read[1] == CV)
			{
				//*Mode = Befehl_Read[3]; mode von LVZ;
				return (Befehl_Read[2]);
				Info = false;
			}
		}
		if (Befehl_Read[0] == COM_SEND_LVZ_STA)
		{
			TRACE(_T(" Brodcastmeldung \n"));
		}
	} while (Info);
    return(Wert);
}