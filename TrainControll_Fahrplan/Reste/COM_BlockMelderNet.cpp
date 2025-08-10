#include "pch.h"
#include "Com_BlockMelderNet.h"


CCom_BlockMelderNet::CCom_BlockMelderNet(void)
{
	Softwareversion = L"Kein Riga R1";
//	APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
//	Com_wifi = &APP->Com_Wifi;
}

CCom_BlockMelderNet::~CCom_BlockMelderNet(void)
{
}



void CCom_BlockMelderNet::Init(CStatic_DrawBMP* LED, CStatic_DrawBMP* MOD,  size_t Weichen)
{
	p_StaticCom = LED;
	p_StaticMod = MOD;
	AnzahlWeichen = Weichen;
	if (conneced_LZV200)
	{
		p_StaticCom->Set_Status(1, L" Com on Giga R1");
	}
	else
	{
		p_StaticCom->Set_Status(0, L" no Giga R1");
	}
}


bool CCom_BlockMelderNet::NoComToBlockNet()
{
	return false;
}


void CCom_BlockMelderNet::WarteDaten()
{
	unsigned int Timer = 990000000;
	Neue_Daten = false;

	do
	{
		Timer -- ;
		if (Timer == 0)
		{
			Neue_Daten = true;
			p_StaticCom->Set_Status(2, L"Time Out !!");
		}
	} while (!Neue_Daten);
}

void CCom_BlockMelderNet::verarbeite_Meldung_Zentrale(byte* Data)
{
	
		//switch (Data[0])
		//{
		//case COM_SEND_GLPOWER:
		//	APP->Gleis_Data.Set_Relais_GleisPower(Data);
		//	if (APP->pDlgSchuppen->IsWindowVisible())
		//		APP->pDlgSchuppen->Invalidate();
		//	break;
		//case COM_SEND_RELAIS:
		//	break;
		//case COM_SEND_DOOR:
		//	// Tür öffnen Rückmeldung
		//	APP->Gleis_Data.Set_Door(Data);
		//	APP->pDlgSchuppen->Invalidate();
		//	break;
		//case COM_SEND_DOOR_STAT:
		//	// Abfrag ist Tür Status Rückmeldung
		//	APP->Gleis_Data.Set_Door(Data);
		//	APP->pDlgSchuppen->Invalidate();
		//	break;
		//case COM_SEND_WEICHE:
		//	APP->Gleis_Data.Set_Weiche(Data);
		//	APP->InfoWeiche.Set_Debug_Text(Data);
		//	if (APP->pDlgSchuppen->IsWindowVisible())
		//		APP->pDlgSchuppen->Invalidate();
		//	APP->InfoGleisBild.Invalidate();
		//	break;
		//case COM_TC_WIRTE_BLOCK:
		//	if (APP->Gleis_Data.Set_Block(Data))
		//	{
		//		APP->InfoMelder.Set_Debug_Text(Data);
		//		APP->InfoGleisBild.Invalidate();
		//		if (APP->pDlgSchuppen->IsWindowVisible())
		//			APP->pDlgSchuppen->Invalidate();
		//	}
		//	break;
		//case COM_TC_WRITE_CUR1:
		//	APP->StromKurve.Set_Strom_Wert(Data);
		//	break;
		//case COM_PC_ASK_VERSION:
		//	Set_SoftwareVersion(Data);
		//	break;
		//}
		Neue_Daten = true;
}



void CCom_BlockMelderNet::Get_VersionInfo(CString* Text)
{
	*Text = Softwareversion;
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
	Befehl_Send[0] = 0x10;
	Befehl_Send[1] = 0; // Version 3.
	Befehl_Send[2] = 0; // .0
}

void CCom_BlockMelderNet::Send_Max_Weiche(size_t Weichen)
{
	Befehl_Send[0] = 0x10;
	if (Weichen < 256) 	Befehl_Send[1] = (byte)Weichen;
	else 		        Befehl_Send[1] = 255;
	Com_wifi->Send_Data(Befehl_Send);
}

void CCom_BlockMelderNet::Send_BlockPower(TrainCon_Paar Relais)
{
	TRACE(_T("schalte Gleis Relais Nr [%i] = %i \n"), Relais.GetWert(), Relais.GetBit());
	Befehl_Send[0] = 0x10;
	Befehl_Send[1] = Relais.GetWert();
	Befehl_Send[2] = (byte)Relais.GetBit();
	Com_wifi->Send_Data(Befehl_Send);
}

void CCom_BlockMelderNet::Send_Relais(byte RelaisNr, bool bit)
{
	Befehl_Send[0] = COM_SEND_RELAIS;
	Befehl_Send[1] = RelaisNr;
	Befehl_Send[2] = bit;
	Befehl_Send[3] = 0x00;

	Com_wifi->Send_Data(Befehl_Send);

}

void CCom_BlockMelderNet::Send_Door_open(bool Bit)
{
	Befehl_Send[0] = COM_SEND_DOOR;
	Befehl_Send[1] = Bit;
	Befehl_Send[2] = 0x00;
	Befehl_Send[3] = 0x00;

	Com_wifi->Send_Data(Befehl_Send);
	WarteDaten();
}

void CCom_BlockMelderNet::Send_Door_Status()
{
	Befehl_Send[0] = COM_SEND_DOOR_STAT;
	Befehl_Send[1] = 0x00;
	Befehl_Send[2] = 0x00;
	Befehl_Send[3] = 0x00;

	Com_wifi->Send_Data(Befehl_Send);
	WarteDaten();
}

void CCom_BlockMelderNet::Send_WeichenData(TrainCon_Paar Weiche)
{
	Befehl_Send[2] = (byte)Weiche.GetBit();
	Befehl_Send[1] = Weiche.GetWert();
	Befehl_Send[0] = COM_SEND_WEICHE;
	Com_wifi->Send_Data(Befehl_Send);
}

void CCom_BlockMelderNet::Send_Mod_TC(ControlStatus Mod)
{
	Befehl_Send[0] = COM_SEND_MOD;
	Befehl_Send[1] = (byte)Mod;
	Com_wifi->Send_Data(Befehl_Send);
}

void CCom_BlockMelderNet::Send_XpressNet_Status(bool on)
{
	Befehl_Send[0] = COM_XPNET_ON;
	Befehl_Send[1] = (byte) on;
	Com_wifi->Send_Data(Befehl_Send);
}

void CCom_BlockMelderNet::Send_XpressNet_Power(bool Power)
{
	Befehl_Send[0] = COM_XPNET_POWER;
	Befehl_Send[1] = (byte)!Power;
	Com_wifi->Send_Data(Befehl_Send);
}
