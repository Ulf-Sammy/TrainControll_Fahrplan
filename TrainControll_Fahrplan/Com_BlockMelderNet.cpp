#include "pch.h"
#include "Com_BlockMelderNet.h"
#include "TrainControll_FahrplanDlg.h"


CCom_BlockMelderNet::CCom_BlockMelderNet(void)
{
	p_hCom = NULL;
	p_aCom = NULL;

	Softwareversion = L"Kein Mega";
}

CCom_BlockMelderNet::~CCom_BlockMelderNet(void)
{
}

void CCom_BlockMelderNet::Set_Com(COM_Info* Port)
{
	p_hCom = &Port->COM_Handel;
	p_aCom = &Port->COM_Active;
}

void CCom_BlockMelderNet::Init(CStatic_DrawBMP* LED, CStatic_DrawBMP* MOD,  size_t Weichen)
{
	p_StaticCom = LED;
	p_StaticMod = MOD;
	AnzahlWeichen = Weichen;
	if (*p_aCom)
	{
		p_StaticCom->Set_Status(1, L" Com on Mega");
	}
	else
	{
		p_StaticCom->Set_Status(0, L" no Mega");
	}
}

void CCom_BlockMelderNet::CloseComunikation()
{
	Send_Mod_TC(ControlStatus::Ende_COM);
	Sleep(500);
}

bool CCom_BlockMelderNet::NoComToBlockNet()
{
	return !(*p_aCom);
}

void CCom_BlockMelderNet::Send_Message()
{
	DWORD bytesSend;
	byte l = COM_LEN(Befehl_Send[0]);
	if (!WriteFile(*p_hCom, &Befehl_Send, l, &bytesSend, 0))
	{
		ClearCommError(*p_hCom, &Error_Com, &COM_status);
		p_StaticCom->Set_Status(3, L" Error Senden !");
	}
}

bool CCom_BlockMelderNet::Read_Message(byte* RD)
{
	byte  c = 0;
	DWORD bytesRead;
	byte Nr = 0;
	byte Len;
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
			if (Nr == 0) Len = (c & 0x07);
			*(RD +  Nr) = c;
			Nr++;
			if (Nr == Len)	return(true);
		}
		else
		{
			return(false);
		}
	}
	return false;
}

void CCom_BlockMelderNet::WarteDaten()
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

void CCom_BlockMelderNet::verarbeite_Meldung_Zentrale()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	
	static byte Data[11];
	if (Read_Message(Data))
	{
		switch (Data[0])
		{
		case COM_SEND_MOD:
			verarbeite_Mod(Data);
			break;
		case COM_SEND_GLPOWER:
			APP->Gleis_Data.Set_Relais(Data);
			APP->pDlgSchuppen->Invalidate();
			break;
		case COM_SEND_RELAIS:
			break;
		case COM_SEND_DOOR:
			// Tür öffnen Rückmeldung
			APP->Gleis_Data.Set_Door(Data);
			APP->pDlgSchuppen->Invalidate();
			break;
		case COM_SEND_DOOR_STAT:
			// Abfrag ist Tür Status Rückmeldung
			APP->Gleis_Data.Set_Door(Data);
			APP->pDlgSchuppen->Invalidate();
			break;
		case COM_SEND_WEICHE:
			APP->Gleis_Data.Set_Weiche(Data);
			APP->InfoWeiche.Set_Debug_Text(Data);
			APP->InfoGleisBild.Invalidate();
			break;
		case COM_TC_WIRTE_BLOCK:
			if (APP->Gleis_Data.Set_Block(Data))
			{
				APP->InfoMelder.Set_Debug_Text(Data);
				APP->InfoGleisBild.Invalidate();
			}
			break;
		case COM_I2C_DEV1:
			Set_Melder_Adr(Data);
			break;
		case COM_I2C_DEV2:
			Set_Relais_Adr(Data);
			break;
		case COM_I2C_DEV3:
			Set_Positi_Adr(Data);
			break;
		case COM_TC_WRITE_CUR1:
			APP->StromKurve.Set_Strom_Wert(Data);
			break;
		case COM_PC_ASK_VERSION:
			Set_SoftwareVersion(Data);
			break;
		}
		Neue_Daten = true;
	}
}

void CCom_BlockMelderNet::verarbeite_Mod(byte* Data)
{
	switch ((ControlStatus)Data[1])
	{
	case ControlStatus::Begin_COM:
		// BeginCom abgeschlossen
		Send_Mod_TC(ControlStatus::Setup);
		Send_Max_Weiche(AnzahlWeichen);
		Send_SoftVersion();

		break;
	case ControlStatus::Setup:
		// Sleep(300);
		p_StaticMod->Set_Status(0);
		Send_Mod_TC(ControlStatus::Fahren);
		break;
	case ControlStatus::Fahren:
		p_StaticMod->Set_Status(1);

		break;
	case ControlStatus::Program:
		p_StaticMod->Set_Status(2);
		break;
	case ControlStatus::Ende_COM:

		break;
	}
}

void CCom_BlockMelderNet::Get_VersionInfo(CString* Text)
{
	*Text = Softwareversion;
}

void CCom_BlockMelderNet::StartProcess()
{
	Send_Mod_TC(ControlStatus::Begin_COM);
	Send_SoftVersion();
}

void CCom_BlockMelderNet::Set_SoftwareVersion(byte* Data)
{
	Softwareversion.Format(L"Train Controll V%d.%2d",Data[1],Data[2]);
	p_StaticCom->Set_Status(1, Softwareversion);
}

void CCom_BlockMelderNet::Set_Melder_Adr(byte* Data)
{
	byte Adr = Data[1];
	byte IOData = Data[2];
	bool notIN = true;
	for (auto inAdr : MelderGruppe_Adr)
	{
		if (Data[1] == inAdr)
		{
			notIN = false;
		}
	}
	if (notIN)
	{
		MelderGruppe_Adr.push_back(Data[1]);
	}
}

void CCom_BlockMelderNet::Set_Relais_Adr(byte* Data)
{
	byte Adr = Data[1];
	byte IOData = Data[2];
	bool notIN = true;
	for (auto inAdr : GleisPowerGruppe_Adr)
	{
		if (Data[1] == inAdr)
		{
			notIN = false;
		}
	}
	if (notIN)
	{
		GleisPowerGruppe_Adr.push_back(Data[1]);
	}
}

void CCom_BlockMelderNet::Set_Positi_Adr(byte* Data)
{
	byte Adr = Data[1];
	byte IOData = Data[2];
	bool notIN = true;
	for (auto inAdr : GleisPosGruppe_Adr)
	{
		if (Data[1] == inAdr)
		{
			notIN = false;
		}
	}
	if (notIN)
	{
		GleisPosGruppe_Adr.push_back(Data[1]);
	}
}

void CCom_BlockMelderNet::Send_SoftVersion()
{
	Befehl_Send[0] = COM_PC_ASK_VERSION;
	Befehl_Send[1] = 0; // Version 3.
	Befehl_Send[2] = 0; // .0
	Send_Message();

}

void CCom_BlockMelderNet::Send_Max_Weiche(size_t Weichen)
{
	Befehl_Send[0] = COM_PC_WRITE_WEICHE;
	if (Weichen < 256) 	Befehl_Send[1] = (byte)Weichen;
	else 		        Befehl_Send[1] = 255;
	Send_Message();
}

void CCom_BlockMelderNet::Send_BlockPower(TrainCon_Paar Relais)
{
	TRACE(_T("schalte Gleis Relais Nr [%i] = %i \n"), Relais.GetWert(), Relais.GetBit());
	Befehl_Send[0] = COM_SEND_GLPOWER;
	Befehl_Send[1] = Relais.GetWert();
	Befehl_Send[2] = (byte)Relais.GetBit();
	Send_Message();
	//Sleep(300);
}

void CCom_BlockMelderNet::Send_Door_open(bool Bit)
{
	Befehl_Send[0] = COM_SEND_DOOR;
	Befehl_Send[1] = Bit;
	Befehl_Send[2] = 0x00;
	Befehl_Send[3] = 0x00;

	Send_Message();
	WarteDaten();
}

void CCom_BlockMelderNet::Send_Door_Status()
{
	Befehl_Send[0] = COM_SEND_DOOR_STAT;
	Befehl_Send[1] = 0x00;
	Befehl_Send[2] = 0x00;
	Befehl_Send[3] = 0x00;

	Send_Message();
	WarteDaten();
}

void CCom_BlockMelderNet::Send_WeichenData(TrainCon_Paar Weiche)
{
	Befehl_Send[2] = (byte)Weiche.GetBit();
	Befehl_Send[1] = Weiche.GetWert();
	Befehl_Send[0] = COM_SEND_WEICHE;
	Send_Message();
}

void CCom_BlockMelderNet::Send_Mod_TC(ControlStatus Mod)
{
	Befehl_Send[0] = COM_SEND_MOD;
	Befehl_Send[1] = (byte)Mod;
	Send_Message();
}
