#pragma once
#include "CGleisPlan.h"
#include "CDatenBankLok.h"
#include "C_UDP_Client.h"


class CAnlagen_Data
{
public:
	// Relais  für Tür, Power,  uns sow.
	// Block
	// Weiche
	// Signale
	CGleisPlan			Gleis_Data; // Gleise
	CDatenBankLok		meineLoks; // Lok Datenbank
	C_UDP_Client        Wifi_Client; // UDP Client für Wifi
	
	CString			Text; 
	CString 		SoftVersion_Giga; // Software Version
	CString			SoftVersion_LZV; // Software Version der Anlage
	CString 		Signalstaerke_Wifi; // Software Version des XpressNet
	CString			NR_IP_Client; // IP Adresse des Wifi Moduls

	bool Server_da = false;
	bool LZV_da = false;
	bool No_Socket = true;
	bool LisentoServer = false;
	bool DoUpdateWifi = false;
	bool DoUpdatePower = false;
	bool DoUpdateLZV = false;
	bool DoUpdateGiga = false;
	bool Gleispower = false;
	bool ProgMode = false; // Programmiermodus an oder aus
	bool Tor_offen = false; 

	unsigned int Max_Weichen = 0;// Maximale Anzahl Weichen


	CAnlagen_Data() : Send_Data{0} 	{	}
	~CAnlagen_Data() = default;
	void Start_Socket();
	void Init();
	void close_Comunikation();
	void Do_incoming_Data();
	void Do_XpressNet_Data();

	void Get_SoftVersion_GIGA();  // nach der Software Version GIGA R1fragen
	void Get_SoftVersion_LZV();  // nach der Software Version LZV 200 fragen
	void Connect_LZV200();     
	void Get_Max_Weiche(size_t Weichen);
	void Send_BlockPower(TrainCon_Paar Relais);
	void Send_Door_open(bool Bit);
	void Send_Door_Status();
	void Send_WeichenData(TrainCon_Paar Weiche);
	void Send_Mod_TC(ControlStatus Mod);
	void Send_XpressNet_Status(bool on);
	void Send_XpressNet_Power(bool Power);
	
	void DoCheckIt(byte Lok_Nr, clock_t Zeit);
	void NewTimeZug(clock_t Zeit);
	void Setze_Lok_aufGleis(Start_Lok_Block Data);
	void Besetze_AbstellGleise_mit_Loks();
	void Update_Lok_Abstellgleis();
	void Set_LokFuntion(byte Lok_Nr, Zug_Status SetTo, byte Funktion, bool Status);
protected:
	byte Send_Data[255]; // Puffer für die Daten die an den XpressNet gesendet werden
	void Debug_Data();
};
