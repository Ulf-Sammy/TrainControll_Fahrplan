#pragma once
#include "pch.h"
#include "CStatic_DrawBMP.h"

class CCom_BlockMelderNet
{
public:
	CCom_BlockMelderNet(void);
	~CCom_BlockMelderNet(void);
	void Set_Com(COM_Info* Port);
	void Init(CStatic_DrawBMP* LED, CStatic_DrawBMP* MOD, size_t Weichen);
	void CloseComunikation();
	bool NoComToBlockNet();
	void StartProcess(); // in Task;
	void WarteDaten();
	void verarbeite_Meldung_Zentrale();
	void verarbeite_Mod(byte* Data);

	void Send_SoftVersion();  // nnach der Software Version fragen
	void Send_Max_Weiche(size_t Weichen);
	void Send_BlockPower(TrainCon_Paar Relais);
	void Send_Door_open(bool Bit);
	void Send_Door_Status();
	void Send_WeichenData(TrainCon_Paar Weiche);
	void Send_Mod_TC(ControlStatus Mod);
	
	void Get_VersionInfo(CString *Text);
	void Set_SoftwareVersion(byte* Data);

	void Set_Melder_Adr(byte* Data);
	void Set_Relais_Adr(byte* Data);
	void Set_Positi_Adr(byte* Data);

protected:
	CFont Font_Info_Melder;
	HANDLE* p_hCom;
	bool* p_aCom;
	CStatic_DrawBMP* p_StaticCom;
	CStatic_DrawBMP* p_StaticMod;
	COMSTAT COM_status;
	DWORD Error_Com;

	CString Softwareversion;
	bool Neue_Daten;
	ControlStatus ModeControl;
	size_t AnzahlWeichen;
	size_t AnzahlMelder;
	byte Read_Antwort[8] = { 0,0,0,0,0,0,0,0 };
	byte Befehl_Send[8] = { 0,0,0,0,0,0,0,0 };


	
	std::vector<byte> MelderGruppe_Adr;
	std::vector<byte> GleisPowerGruppe_Adr;
	std::vector<byte> GleisPosGruppe_Adr;

	void Send_Message();
	bool Read_Message(byte* RD);
};

