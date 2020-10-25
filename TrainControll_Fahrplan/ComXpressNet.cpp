#include "pch.h"
#include "ComXpressNet.h"
#include "TrainControll_Fahrplan.h"

CCom_XpressNet::CCom_XpressNet(void)
{
	PortNr = 0;
	m_hCom = NULL;
    HeadSend_Liste = 0;
	TailSend_Liste = 0;
	bytesSend = 0;
	//COM_status = *COMSTAT::cbInQue ;
	Error_Com = 0;
	ComInfo = _T("Kein Mega");
	ComInfo_Nr = 0;
	LZVInfo_Nr = 2;
	in_Run = true;
}
CCom_XpressNet::~CCom_XpressNet(void)
{

}

bool CCom_XpressNet::OpenCom(int Port)
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
			dcbSerialParams.BaudRate = 115200; 
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
	EscapeCommFunction(m_hCom, SETDTR); // Reset Arduino
	EscapeCommFunction(m_hCom, CLRDTR);
	//Sleep(5000);
	byte  c;
	DWORD bytesRead;
	for (;;)
	{
		if (::ReadFile(m_hCom, &c, 1, &bytesRead, NULL) == 0)
		{
			TRACE(_T("Lesefehler Com"));
		}
		if (c == '\r')
		{
			ComInfo_Nr = 1;
			TRACE2(" - Serieller Com Port %i geöffnet von Mega [ %s ] \n", PortNr, ComInfo);
			return(true);
		}
		if (bytesRead != 0)		ComInfo.AppendChar(c);
		if (c == '\n')
		{
			ComInfo.Empty();
		}
	}
	return(true);
}
void CCom_XpressNet::CloseCom()
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
bool CCom_XpressNet::NoComToXpressNet()
{
	if (m_hCom == NULL) return (true);
	return (m_hCom == INVALID_HANDLE_VALUE);
}

void CCom_XpressNet::SendeNeueDaten()
{
	if (HeadSend_Liste != TailSend_Liste)
	{		
		byte l = COM_LEN(Send_Liste[TailSend_Liste][0]);
		if (!WriteFile(this->m_hCom, &Send_Liste[TailSend_Liste], l, &bytesSend, 0))
		{
			ComInfo_Nr = 2;
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

void CCom_XpressNet::SendeAsknachLokDaten(byte Adr)
{
	Send_Liste[HeadSend_Liste][2] = 0;// nicht mehr benötigt
	Send_Liste[HeadSend_Liste][1] = Adr;
	Send_Liste[HeadSend_Liste][0] = COM_READ_ZUG_DA;
	Neu_Send_Befehl();
}
void CCom_XpressNet::SendeZugDaten(XpNSendwas was, byte Adresse, byte Daten)
{
	Send_Liste[HeadSend_Liste][2] = Daten ;
	Send_Liste[HeadSend_Liste][1] = Adresse;
	Send_Liste[HeadSend_Liste][0] = SendAdr[(byte)was];
	Neu_Send_Befehl();
}
void CCom_XpressNet::SendeWeicheDaten(TrainCon_Paar Wl)
{
	Set_Debug_Text(Wl.GetWert(), Wl.GetBit());
	Send_Liste[HeadSend_Liste][2] = (byte)Wl.GetBit();
	Send_Liste[HeadSend_Liste][1] = Wl.GetWert();
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_WEICHE;
	Neu_Send_Befehl();
}
void CCom_XpressNet::SendeLVZ_Power(bool Bit)
{
	Send_Liste[HeadSend_Liste][1] = (byte)Bit;
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_LVZ_POWER;
	Neu_Send_Befehl();
}
void CCom_XpressNet::Sende_Write_CV(byte CV, byte Wert)
{
	Send_Liste[HeadSend_Liste][2] = Wert;
	Send_Liste[HeadSend_Liste][1] = CV;
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_CV;
	Neu_Send_Befehl();

}
void CCom_XpressNet::Sende_Read_CV(byte CV, byte Wert)
{
	Send_Liste[HeadSend_Liste][2] = Wert;
	Send_Liste[HeadSend_Liste][1] = CV;
	Send_Liste[HeadSend_Liste][0] = COM_READ_CV;
	Neu_Send_Befehl();
}
void CCom_XpressNet::Sende_Setto_Prog(bool onoff)
{
	Send_Liste[HeadSend_Liste][2] = 0;
	if (onoff)
	{
		Send_Liste[HeadSend_Liste][1] = (byte)ControlStatus::Program;
		LZVInfo_Nr = 4;
		in_Run = false;
	}
	else
	{
		Send_Liste[HeadSend_Liste][1] = (byte)ControlStatus::Fahren;
		LZVInfo_Nr = 1;
		in_Run = true;
	}
	Send_Liste[HeadSend_Liste][0] = COM_WRITE_MOD; // Befehl Schreiben
	Neu_Send_Befehl();
}
void CCom_XpressNet::Neu_Send_Befehl()
{
	HeadSend_Liste = (HeadSend_Liste + 1) % SENDBUFFER;
}
// Nur im Setup 
void CCom_XpressNet::StartKomunikation()
{
	DWORD bytesSend;
	Befehl_Send[0] = COM_WRITE_MOD;
	if (!WriteFile(this->m_hCom, &Befehl_Send, 1, &bytesSend, 0))
	{
		ClearCommError(this->m_hCom, &this->Error_Com, &this->COM_status);
		TRACE(_T("Fehler beim Senden !!!  StartKomunikation-CComBlockMelderNet \n"));
	}
}

byte CCom_XpressNet::GetStatus_LZV()
{
	return (LZVInfo_Nr);
}
bool CCom_XpressNet::GetTC_Message()
{
	byte  c;
	DWORD bytesRead;
	byte Nr = 0;
	byte Len;
	BOOL Error;
	for (;;)
	{
		Error = ::ReadFile(m_hCom, &c, 1, &bytesRead, NULL);
		if (!Error)
		{
			ComInfo_Nr = 2;
		}
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
void CCom_XpressNet::SetTC_Message()
{
	DWORD bytesSend;
	byte l = COM_LEN(Befehl_Send[0]);
	if(!WriteFile(this->m_hCom, &Befehl_Send, l , &bytesSend, 0))
	{
		ComInfo_Nr = 2;
		ClearCommError(this->m_hCom, &this->Error_Com, &this->COM_status);
		TRACE(_T("Fehler beim Senden !!! \n"));
	}
}

void CCom_XpressNet::DebugMessage()
{
	if (Befehl_Read[0] == COM_WRITE_MOD)		TRACE2("=> MSG: COM_WRITE_MOD       | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_ACKN_MOD)			TRACE2("=> MSG: COM_ACKN_MOD        | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_LVZ_STA)		TRACE2("=> MSG: COM_SEND_LVZ_STA    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ERROR)		TRACE2("=> MSG: COM_SEND_ERROR      | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_DISPLAY)		TRACE2("=> MSG:	COM_SEND_DISPLAY    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D0)		TRACE2("=> MSG:	COM_WRITE_ZUG_D0    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D1)		TRACE2("=> MSG:	COM_WRITE_ZUG_D1    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D2)		TRACE2("=> MSG:	COM_WRITE_ZUG_D2    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D3)		TRACE2("=> MSG:	COM_WRITE_ZUG_D3    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D4)		TRACE2("=> MSG:	COM_WRITE_ZUG_D4    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_ZUG_D5)		TRACE2("=> MSG:	COM_WRITE_ZUG_D5    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_WEICHE)		TRACE2("=> MSG:	COM_WRITE_WEICHE    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_READ_ZUG_DA)		TRACE2("=> MSG:	COM_READ_ZUG_DA     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_READ_ZUG_D1)		TRACE2("=> MSG:	COM_READ_ZUG_D1     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_RELAIS)		TRACE2("=> MSG:	COM_SEND_RELAIS     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_RELAIS)		TRACE2("=> MSG:	COM_WRITE_RELAIS    | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_TIME)		TRACE2("=> MSG:	COM_SEND_TIME       | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_BLOCK)		TRACE2("=> MSG:	COM_SEND_BLOCK      | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_WEICHE)		TRACE2("=> MSG:	COM_SEND_WEICHE     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_DA)		TRACE2("=> MSG:	COM_SEND_ZUG_DA     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_D1)		TRACE2("=> MSG:	COM_SEND_ZUG_D1     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_D2)		TRACE2("=> MSG:	COM_SEND_ZUG_D2     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_D3)		TRACE2("=> MSG:	COM_SEND_ZUG_D3     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_D4)		TRACE2("=> MSG:	COM_SEND_ZUG_D4     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_ZUG_D5)		TRACE2("=> MSG:	COM_SEND_ZUG_D5     | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_MELDER_TI)  TRACE2("=> MSG:	COM_WRITE_MELDER_TI | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_MELDER_ST)  TRACE2("=> MSG:	COM_WRITE_MELDER_ST | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_WEICHE_ST)  TRACE2("=> MSG:	COM_WRITE_WEICHE_ST | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_LVZ_POWER)  TRACE2("=> MSG:	COM_WRITE_LVZ_POWER | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_SEND_CV)          TRACE2("=> MSG:	COM_SEND_CV         | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_WRITE_CV)         TRACE2("=> MSG:	COM_WRITE_CV        | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
	if (Befehl_Read[0] == COM_READ_CV)          TRACE2("=> MSG:	COM_READ_CV         | %x | %x  \n", Befehl_Read[1], Befehl_Read[2]);
}
void CCom_XpressNet::Set_Debug_Text(byte Block, bool Bit)
{
	CString Text;
	static byte Pos = 0;
	if (Bit)
	{
		Text.Format(_T("Weiche %2i : I"), Block);
	}
	else
	{
		Text.Format(_T("Weiche %2i : o"), Block);
	}
	Text = theApp.Get_Time(Text);
	if (Pos == 11)
	{
		for (int i = 0; i < 10; i++)
		{
			Debug_Text[i] = Debug_Text[i + 1];
		}
		Debug_Text[10] = Text;
	}
	else
	{
		Debug_Text[Pos] = Text;
		Pos++;
	}

}
void CCom_XpressNet::Set_Debug_Text()
{
	CString Text;
	static byte Pos = 0;
	Text.Format(_T("Zug Adr  %2i : %2i"), Befehl_Read[6],(Befehl_Read[1] & 0x1F));
	Text = theApp.Get_Time(Text);
	if (Pos == 11)
	{
		for (int i = 0; i < 10; i++)
		{
			Debug_Text[i] = Debug_Text[i + 1];
		}
		Debug_Text[10] = Text;
	}
	else
	{
		Debug_Text[Pos] = Text;
		Pos++;
	}


}
byte CCom_XpressNet::GetNextMessage()
{ 
	if (GetTC_Message())
	{
		DebugMessage();
		return Befehl_Read[0];
	}
	else
		return false;
}


void CCom_XpressNet::HoleZugData(byte *Data, byte Adr)
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
TrainCon_Paar CCom_XpressNet::HoleMelderData()
{
	return (TrainCon_Paar(Befehl_Read[1], (bool)Befehl_Read[2]));
}
TrainCon_Paar CCom_XpressNet::HoleWeicheData()
{
	return (TrainCon_Paar(Befehl_Read[1], (bool)Befehl_Read[2]));
}
byte CCom_XpressNet::HoleStatus_LZV()
{
	if (Befehl_Read[0] == COM_SEND_LVZ_STA)
	{
		LZVInfo_Nr = Befehl_Read[1];
		return (Befehl_Read[1]);    // Status
	}
	else
		TRACE(_T("Fehler beim melden LZV Status  \n "));
	return (0xFF);
}
byte CCom_XpressNet::Hole_CV_Wert()
{
	return(Befehl_Read[2]);;
}


byte CCom_XpressNet::DoWorkonCV(bool RW, byte CV, byte Wert)
{
	bool Info;
	if (RW) // schreibe CV Wert;
	{
		Befehl_Send[0] = COM_WRITE_CV; // Befehl Schreiben
		TRACE2("write ->  CV Nr.: %02i =  %i \n", CV, Wert);
	}
	else
	{
		Befehl_Send[0] = COM_READ_CV; // Befehl Lesen
		TRACE2("Read <- CV N.: %02i =  %i \n", CV, Wert);
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

byte CCom_XpressNet::Get_VersionInfo(CString *Text)
{
	*Text = ComInfo;
	return ComInfo_Nr;
}

bool CCom_XpressNet::Get_Mode_Run()
{
	return in_Run;
}

void CCom_XpressNet::ZeichneWeichenMeldung(CDC* pDC)
{
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&theApp.Font_Info_small);

	for (int i = 0; i < 11; i++)
	{
		pDC->TextOutW(460, ((i * 8) + 725), Debug_Text[i]);
	}
	pDC->SelectObject(pOldFont);

}
