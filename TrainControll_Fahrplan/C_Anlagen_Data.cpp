#include "pch.h"
#include "C_Anlagen_Data.h"



void CAnlagen_Data::Start_Socket()
{
	if (Wifi_Client.Begin_Winsock())
	{
		LisentoServer = true;
		No_Socket = false;
	}
	else LisentoServer = false;
}

void CAnlagen_Data::Init()
{                                  // Desktop
	unsigned long timeout = 200;

	
	TRACE(_T("Verbinden zum Server\n"));
	if (LisentoServer)
	{
		
		Wifi_Client.connect_Server();
		
		while(!Server_da)
		{
			timeout--;
			Sleep(10);
			if(timeout == 0)
			{
				TRACE(_T("Timeout beim Verbinden mit Server\n"));
				LisentoServer = false;
				break;
			}
		}
		Wifi_Client.get_Wifi_RSSI(); // Feldstärke abfragen
		Connect_LZV200(); // LZV200 verbinden
	}
	Gleis_Data.Init();
	meineLoks.Init(&Wifi_Client);
	Besetze_AbstellGleise_mit_Loks();
}

void CAnlagen_Data::close_Comunikation()
{
		Wifi_Client.disconnect_Server();

		while(Server_da){ }
		
		
}


void CAnlagen_Data::Do_incoming_Data()
{
	if (Wifi_Client.Listen_to_Data())
	{
		Wifi_Client.Trace_Data();

		switch (Wifi_Client.get_Kennung())
		{
		case 0x00: // Anlagen Daten

			break;
		case 0x01: // Software Version Giga
			SoftVersion_Giga = Wifi_Client.Get_Data_String();
			DoUpdateGiga;
			break;
		case 0x02: // Software Version LZV 200
			SoftVersion_LZV = Wifi_Client.Get_Data_String();
			DoUpdateLZV;
			break;
		case 0x03: // Weichen Daten
			break;
		case 0x04: // Lok Funktionen Daten
			break;
		case 0x05: // Lok Funktionen Status Daten
			break;
		case 0x06: // Lok Funktionen Status Daten
			break;
		case 0x07: // Tor auf zu
			if (Wifi_Client.Get_Data_Byte1() == 0x01)
				Tor_offen = true;
			else
				Tor_offen = false;
			DoUpdatePower = true;
			break;
		case 0x08: //
			break;
		case 0x09: // Verbunden mit LZV200
			if (Wifi_Client.Get_Data_Byte1() == 0x01)
				LZV_da = true;
			else
				LZV_da = false;
			DoUpdateLZV = true;
			break;
		case 0x0A: //
			break;
		case 0x0B: //
			break;
		case 0x0C: // Feldstärke abfragen
			Signalstaerke_Wifi = Wifi_Client.Get_Data_String();
			DoUpdateWifi = true;
			break;
		case 0x0D: //
			break;
		case 0x0E: // an Server angemeldet
			Server_da = true;
			NR_IP_Client = Wifi_Client.Get_Data_String();
			DoUpdateGiga = true;
			break;
		case 0x0F: //
			LisentoServer = false;
		    Server_da = false;
			TRACE(_T("vom Server abgemeldet /n"));
			break;

		case 0xA0: // 

			break;
		case 0xB0: // Daten von Mobile

			break;
		case 0xC0:
		{
			Do_XpressNet_Data();			
		}
			break;
		case 0xD0: // 

			break;
		case 0xE0: // Daten von Desktop

			break;
		case 0xF0: // Daten von Mobile

			break;
		default:
			break;
		}
	}
}

void CAnlagen_Data::Do_XpressNet_Data()
{// Daten von XpressNet verarbeiten



	if (LZV_da)
	{
		byte A = Wifi_Client.Get_Data_Byte(4);
		byte B = Wifi_Client.Get_Data_Byte(5);
		byte C = Wifi_Client.Get_Data_Byte(6);

		switch (Wifi_Client.Get_Data_Byte(4))
		{
		case 0x61: // Software Version LZV200
			if (B == 0) { Gleispower = false; } // Gleis Aus
			if (B == 1) 
			{
				Gleispower = true;  
				ProgMode = false;
			}// Gleis An
			if (B == 2) { ProgMode = true; }
			DoUpdatePower = true;
			break;
		case 0x63: // Softwarw Version LZV200
			break;
		case 0x6E: // Lok Status Meldung
				break;
		}
	}


}

void CAnlagen_Data::Get_SoftVersion_GIGA()
{
	// Software Version GIGA
	Wifi_Client.make_Message((byte)0x01, (byte) 0x00, Send_Data);
	Wifi_Client.Send_Data(Send_Data);
}
void CAnlagen_Data::Get_SoftVersion_LZV()
{
	// Software Version LZV200
	// Softwareversion anfordern 0xFF 0xFE 0x21 0x21 0x00
	Wifi_Client.make_Message_LZV_FE((byte)0x02, (byte)0x21,(byte)0x21 ,Send_Data);
	Wifi_Client.Send_Data(Send_Data);
}
void CAnlagen_Data::Connect_LZV200()
{
	Wifi_Client.make_Message((byte)0x09, (byte)0x01, Send_Data);
	Wifi_Client.Send_Data(Send_Data);

}
void CAnlagen_Data::Get_Max_Weiche(size_t Weichen)
{
}
void CAnlagen_Data::Send_BlockPower(TrainCon_Paar Relais)
{
}
void CAnlagen_Data::Send_Door_open(bool Bit)
{
}
void CAnlagen_Data::Send_Door_Status()
{
}
void CAnlagen_Data::Send_WeichenData(TrainCon_Paar Weiche)
{
}
void CAnlagen_Data::Send_Mod_TC(ControlStatus Mod)
{
}
void CAnlagen_Data::Send_XpressNet_Status(bool on)
{
}
void CAnlagen_Data::Send_XpressNet_Power(bool Power)
{
	//  Alles An  0xFF 0xFE 0x21 0x81 0xA0
    //	Alles Aus 0xFF 0xFE 0x21 0x80 0xA1
	byte On_Off;
	if (Power) On_Off = 0x81;
	else On_Off = 0x80;

	Wifi_Client.make_Message_LZV_FE((byte)0xc0, (byte)0x21, On_Off, Send_Data);
	Wifi_Client.Send_Data(Send_Data);
}
void CAnlagen_Data::Set_LokFuntion(byte Lok_Nr, Zug_Status SetTo, byte Funktion, bool Status)
{

}
void CAnlagen_Data::Debug_Data()
{

}
void CAnlagen_Data::Setze_Lok_aufGleis(Start_Lok_Block Data)
{
	if (Data.Lok_Name.IsEmpty())
	{
		Gleis_Data.Set_Block_free(Data.Block);
	}
	else
	{
		CDataXpressNet* XpressNet_Lok;

		XpressNet_Lok = meineLoks.Get_Lok_Data(Data.Lok_Name);
		XpressNet_Lok->Set_BlickRichtung(Data.Blick);
		XpressNet_Lok->Set_auf_Gleis(Data.Block);

		Gleis_Data.Set_Block_Lok(Data.Block,XpressNet_Lok);
	}
}

void CAnlagen_Data::Besetze_AbstellGleise_mit_Loks()
{
	for (auto& LB : meineLoks.HomeZüge)
	{
		Setze_Lok_aufGleis(LB);
	}
}

void CAnlagen_Data::Update_Lok_Abstellgleis()
{
	Start_Lok_Block Data;
	meineLoks.HomeZüge.clear();
	for (byte i = 33; i < 41; i++)
	{
		Data = Gleis_Data.Get_StartLokInfo(i);
		if (!Data.Lok_Name.IsEmpty())
		{
			meineLoks.HomeZüge.push_back(Data);
		}
	}

}

void CAnlagen_Data::NewTimeZug(clock_t Zeit)
{
	static byte Lok_Nr = 0;
	byte		max_active_Loks = meineLoks.Get_max_Aktiv_Loks();;

	if (Lok_Nr >= max_active_Loks)
	{
		Lok_Nr = 0;
	}
	else
	{
		DoCheckIt(Lok_Nr, Zeit);
		Lok_Nr = (Lok_Nr + 1) % (max_active_Loks);
	}
}

	

void CAnlagen_Data::DoCheckIt(byte Lok_Nr, clock_t Zeit)
{
	if (meineLoks.MeineZüge[Lok_Nr].Plan_Warten)
	{
		if (meineLoks.MeineZüge[Lok_Nr].WarteZeit < Zeit)
		{
		}
	}
	if ((meineLoks.MeineZüge[Lok_Nr].Get_Status() == Zug_Status::Zug_haelt) && (meineLoks.MeineZüge[Lok_Nr].Betriebs_Modus == Zug_Steuerung::Hand_Betrieb))
	{ // Dies nur für den Handbetreib 
	}
	if ((meineLoks.MeineZüge[Lok_Nr].Get_Status() == Zug_Status::Zug_haelt) && (meineLoks.MeineZüge[Lok_Nr].Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb))
	{ // Dies nur für den Automaticbetreib 
	}
}