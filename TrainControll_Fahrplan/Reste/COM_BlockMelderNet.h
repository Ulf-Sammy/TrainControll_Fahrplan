#pragma once
#include "pch.h"
#include "C_UDP_Client.h"
#include "CStatic_DrawBMP.h"
//#include "TrainControll_FahrplanDlg.h"

class CCom_BlockMelderNet
{
public:
	CCom_BlockMelderNet(void);
	~CCom_BlockMelderNet(void);
	void Init(CStatic_DrawBMP* LED, CStatic_DrawBMP* MOD, size_t Weichen);
	bool NoComToBlockNet();
	void WarteDaten();
	void verarbeite_Meldung_Zentrale(byte * Data);

	void Send_SoftVersion();  // nnach der Software Version fragen
	void Send_Max_Weiche(size_t Weichen);
	void Send_BlockPower(TrainCon_Paar Relais);
	void Send_Relais(byte RelaisNr, bool bit);
	void Send_Door_open(bool Bit);
	void Send_Door_Status();
	void Send_WeichenData(TrainCon_Paar Weiche);
	void Send_Mod_TC(ControlStatus Mod);
	void Send_XpressNet_Status(bool on);
	void Send_XpressNet_Power( bool Power);
	void Get_VersionInfo(CString *Text);
	void Set_SoftwareVersion(byte* Data);

	void Set_Melder_Adr(byte* Data);
	void Set_Relais_Adr(byte* Data);
	void Set_Positi_Adr(byte* Data);

protected:
	//CTrainControll_FahrplanDlg* APP = NULL;
	C_UDP_Client* Com_wifi = NULL;
	CFont Font_Info_Melder;
	CStatic_DrawBMP* p_StaticCom;
	CStatic_DrawBMP* p_StaticMod;
	ControlStatus ModeControl;
	COMSTAT COM_status;
	DWORD Error_Com;

	CString Softwareversion;
	bool Neue_Daten = false;
	bool conneced_LZV200 = false;
	bool conneced_Server = false;
	size_t AnzahlWeichen;
	size_t AnzahlMelder;
	byte Befehl_Send[8] = { 0,0,0,0,0,0,0,0 };


	
	std::vector<byte> MelderGruppe_Adr;
	std::vector<byte> GleisPowerGruppe_Adr;
	std::vector<byte> GleisPosGruppe_Adr;
};

