#include "pch.h"
#include "CGleisMelderNet.h"

CGleisMelderNetCom::CGleisMelderNetCom(void)
{
	PortNr = 0;
	m_hCom = NULL;
	bytesSend = 0;
}

CGleisMelderNetCom::~CGleisMelderNetCom(void)
{
}

bool CGleisMelderNetCom::OpenCom(int Port)
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
			dcbSerialParams.BaudRate = 112500;
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
	return(true);

}

void CGleisMelderNetCom::CloseCom()
{
	::CloseHandle(m_hCom);
}

bool CGleisMelderNetCom::GetTC_Message()
{
	byte  c;
	DWORD bytesRead;
	byte Nr = 0;
	byte Len;

	for (;;)
	{
		::ReadFile(m_hCom, &c, 1, &bytesRead, NULL);
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
