#include "pch.h"
#include "Com_BlockMelderNet.h"
#include "TrainControll_Fahrplan.h"

CCom_BlockMelderNet::CCom_BlockMelderNet(void)
{
	PortNr = 0;
	m_hCom = NULL;
	bytesSend = 0;
	ComInfo = _T("Kein Uno");
	ComInfo_Nr = 0;
	
}

CCom_BlockMelderNet::~CCom_BlockMelderNet(void)
{
}

bool CCom_BlockMelderNet::OpenCom(int Port)
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
		DCB dcbSerialParams = { 0 };

		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (!::GetCommState(m_hCom, &dcbSerialParams))
		{
			TRACE(_T(" 1.ERROR COM \n"));
		}
		else
		{
			dcbSerialParams.BaudRate = 115200;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.StopBits = ONESTOPBIT;
			if (!SetCommState(m_hCom, &dcbSerialParams))
			{
				TRACE(_T(" 2.ERROR COM \n"));
			}
			COMMTIMEOUTS timeouts = { 0 };
			GetCommTimeouts(m_hCom, &timeouts);
			//*

			timeouts.ReadIntervalTimeout = 50;
			timeouts.ReadTotalTimeoutConstant = 200;
			timeouts.ReadTotalTimeoutMultiplier = 10;

			timeouts.WriteTotalTimeoutConstant = 50;
			timeouts.WriteTotalTimeoutMultiplier = 10;
			//*/
			if (!SetCommTimeouts(m_hCom, &timeouts))
			{
				TRACE(_T(" 3.ERROR COM \n"));
			}
		}
	}
	EscapeCommFunction(m_hCom,SETDTR); // Reset Arduino 
	EscapeCommFunction(m_hCom,CLRDTR);
	byte  c;
	DWORD bytesRead;
	for (;;)
	{
		if (::ReadFile(m_hCom, &c, 1, &bytesRead, NULL) == 0)
		{
			TRACE(_T("Lesefehler Com"));
			ComInfo_Nr = 2;
		}
		if (c == '\r')
		{
			ComInfo_Nr = 1;
			TRACE2(" - Serieller Com Port %i geöffnet von Uno [ %s ] \n", PortNr, ComInfo);
			return(true);
		}
		ComInfo.AppendChar(c);
		if (c =='\n')
		{
			ComInfo.Empty();
		}
	}
	return(true);

}

void CCom_BlockMelderNet::CloseCom()
{
	::CloseHandle(m_hCom);
}

bool CCom_BlockMelderNet::GetTC_Message()
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
		if (bytesRead == 1)
		{
			if (Nr == 0) Len = (c & 0x07);
			Read_Melder[Nr] = c;
			Nr++;
			if (Nr == (Len))
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

void CCom_BlockMelderNet::SetTC_Message()
{
}

void CCom_BlockMelderNet::Set_Debug_Text(byte Block, bool Bit)
{
	CString Text;
	static byte Pos = 0;
	if (Bit)
	{ 
		Text.Format(_T("Block %2i : I"), Block);
	}
	else
	{
		Text.Format(_T("Block %2i : o"), Block);
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

byte CCom_BlockMelderNet::GetNextMessage()
{
	/*static bool BlBit = true;
	static byte BlNr = 0;
	if (BlBit) BlNr++;

	if (BlNr == 31) BlNr = 1;
	Read_Melder[0] = COM_SEND_BLOCK;
	Read_Melder[1] = BlNr;
	Read_Melder[2] = (byte)BlBit;
	Sleep(500);
	return Read_Melder[0];
	*/
	if (GetTC_Message())
	{
		TRACE3(" Neue Meldung | %x | %x | %x  \n", Read_Melder[0], Read_Melder[1], Read_Melder[2]);
		return Read_Melder[0];
	}
	else
		return false;
}

void CCom_BlockMelderNet::StartKomunikation()
{
	DWORD bytesSend;
	Befehl_Send[0] = COM_WRITE_MOD;
	if (!WriteFile(this->m_hCom, &Befehl_Send, 1, &bytesSend, 0))
	{
		ClearCommError(this->m_hCom, &this->Error_Com, &this->COM_status);
		TRACE(_T("Fehler beim Senden !!!  StartKomunikation-CComBlockMelderNet \n"));
	}
}

bool CCom_BlockMelderNet::NoComToBlockNet()
{
	if (m_hCom == NULL) return (true);
	return (m_hCom == INVALID_HANDLE_VALUE);
}

byte CCom_BlockMelderNet::Get_VersionInfo(CString* Text)
{
	*Text = ComInfo;
	return ComInfo_Nr;
}

void CCom_BlockMelderNet::ZeichneBlockMeldung(CDC* pDC)
{
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&theApp.Font_Info_small);

	for (int i = 0; i < 11; i++)
	{
		pDC->TextOutW(300, ((i * 8) + 725), Debug_Text[i]);
	}
	pDC->SelectObject(pOldFont);

}

void CCom_BlockMelderNet::SetMelder_Zeit(byte Nr)
{
	//  der Wert mit 10 msec mal genommen 
// in der Ardunino Controller
	Befehl_Send[0] = COM_WRITE_MELDER_TI;
	Befehl_Send[1] = Nr;
	SetTC_Message();
	Sleep(300);
}

void CCom_BlockMelderNet::SetBlockPower(TrainCon_Paar Relais)
{
	TRACE(_T("schalte Relais Nr [%i] = %i \n"), Relais.GetWert(), Relais.GetBit());
	Befehl_Send[0] = COM_WRITE_WEICHE_ST;
	Befehl_Send[1] = Relais.GetWert();
	Befehl_Send[2] = (byte)Relais.GetBit();
	SetTC_Message();
	//Sleep(300);
}


TrainCon_Paar CCom_BlockMelderNet::HoleBlockData()
{
	Set_Debug_Text(Read_Melder[1], (bool)Read_Melder[2]);
	return (TrainCon_Paar(Read_Melder[1], (bool)Read_Melder[2]));
}

TrainCon_Paar CCom_BlockMelderNet::HoleRelayData()
{
	return (TrainCon_Paar(Read_Melder[1], (bool)Read_Melder[2]));
}
