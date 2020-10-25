// CDlg_Debug_Mega.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CDlg_Debug_Mega.h"
#include "afxdialogex.h"
UINT Thread_Update_Debug(LPVOID pParam)
{
	CDlg_Debug_Mega* Info = (CDlg_Debug_Mega*)pParam;
	CString Text;
	do
	{
		Text = Info->Get_Message();
		if (!Text.IsEmpty())
		{// Daten empfangen
			Info->UpdateNewData(Text);
		}
	} while (Info->ListentoCom);
	return 0;
}

// CDlg_Debug_Mega-Dialog

IMPLEMENT_DYNAMIC(CDlg_Debug_Mega, CDialog)

CDlg_Debug_Mega::CDlg_Debug_Mega(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDlg_Debug_Mega, pParent)
{
	m_pParent = pParent;
	m_nID = CDlg_Debug_Mega::IDD;

	Font_Info.CreateFontIndirectW(&FontType_Ar_15_0);
	
}
CDlg_Debug_Mega::~CDlg_Debug_Mega()
{
	ListentoCom = false;
	CloseCom();

}

BOOL CDlg_Debug_Mega::Create()
{
	ListentoCom = true;
	OpenCom(4);
	AfxBeginThread(Thread_Update_Debug, this);
	return CDialog::Create(m_nID, m_pParent);;
}

BOOL CDlg_Debug_Mega::OnInitDialog()
{
	SetWindowPos(NULL, 2000, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	
	CDialog::OnInitDialog();
	return 0;
}

void CDlg_Debug_Mega::OnPaint()
{
	CFont* pOldFont;
	CPaintDC dc(this);
	pOldFont = dc.SelectObject(&theApp.Font_Info_s);

	for (int i = 0; i < maxZeile; i++)
	{
		dc.TextOutW(10, ((i*15) +8), Debug_Text[i]);
	}
	dc.SelectObject(pOldFont);
}

bool CDlg_Debug_Mega::OpenCom(int Port)
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
	return true;
}

void CDlg_Debug_Mega::CloseCom()
{
	::CloseHandle(m_hCom);
}

CString CDlg_Debug_Mega::Get_Message()
{
	byte  c;
	DWORD bytesRead;
	bool Error;
	CString InText;
	for (;;)
	{
		Error = ::ReadFile(m_hCom, &c, 1, &bytesRead, NULL);
		if (bytesRead == 1)
		{
			if (c == '\r') return(InText);

			InText.AppendChar(c);
			
		}
		else
		{
			return CString();
		}
	}
	return CString();
}

void CDlg_Debug_Mega::UpdateNewData(CString Textin)
{
	static int Zeile = 0;
	
	if (!Textin.IsEmpty())
	{
		if(Zeile == maxZeile)
		{
			for (int i = 0; i < maxZeile; i++)
			{
				Debug_Text[i] = Debug_Text[i+1];

			}
			Debug_Text[maxZeile] = Textin;
		
		}
		else
		{
			Debug_Text[Zeile] = Textin;
			Zeile++;
		}
		Invalidate();
	}
}

void CDlg_Debug_Mega::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Debug_Mega, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlg_Debug_Mega-Meldungshandler
