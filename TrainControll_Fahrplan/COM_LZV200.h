#pragma once
#include "framework.h"
#include "CStatic_DrawBMP.h"

class CCom_LZV200
{
public:
	CCom_LZV200();
	~CCom_LZV200();
	void Set_Com(COM_Info* Port);
	void Init(CStatic_DrawBMP* LED);
	bool NoComtoLZV200();
	void WarteDaten();
	void StartProcess();
	BC_Meldungen GetStatusLZV200();
	void Get_VersionInfo(CString* Text);
	bool Get_Power_onGleis();
	void Read_Version();
	void Interface_Status();
	void SendeLVZ_Power(bool Bit); // BC Alles an oder aus
	void Alle_Loks_Anhalten();
	void die_Lok_anhalten(Lok_Adresse ADR);
	void SendeAsknachLokDaten(Lok_Adresse ADR);
	void HoleZugData(byte* Data, Lok_Adresse Adr);
	void Sende_Write_CV(byte CV, byte Wert);
	void Sende_Read_CV(byte CV, byte Wert);
	byte Hole_CV_Wert();
	void SendeZugDaten(XpNSendwas was, Lok_Adresse Adr, byte Daten);
	void verarbeite_Meldung_Zentrale();

protected:
	HANDLE* p_hCom;
	bool* p_aCom;
	CStatic_DrawBMP* p_StaticCom;
	
	CString Softwareversion;
	CString Vesrionsnummer;
	CString Letzte_Antwort;
	BC_Meldungen Letzte_BC_Meldung;
	
	bool Neue_Daten;
	CString Debug_Text[11];
	byte Fahrstufenzahl;
	byte Fahrgeschwindigkeit;

	bool LokNothalt;
	bool LokFree;
	bool LokRichtung;	
	WORD LokAdresse;
	byte Lok_FGruppe0; // R00v vvvv 
	byte Lok_FGruppe1; // 000 F0 F4 F3 F2 F1 
	byte Lok_FGruppe2; // 0000 F8 F7 F6 F5 
	byte Lok_FGruppe3; // 0000 F12 F11 F10 F9 
	byte Lok_FGruppe4; // F20 F19 F18 F17 F16 F15 F14 F13 
	byte Lok_FGruppe5; // F28 F27 F26 F25 F24 F23 F22 F21 
	bool Read_Message(byte *RD);
	void Send_Message(byte *SD, byte lSD);
	void Meldung_Header_x01(byte *Data);
	void Meldung_Header_x02(byte* Data);
	void Meldung_Header_x61(byte* Data);
	void Meldung_Header_x63(byte* Data);
	void Meldung_Header_x81(byte* Data);
	void Meldung_Header_xE1(byte* Data);
	void Meldung_Header_xE2(byte* Data);
	void Meldung_Header_xE3(byte* Data);
	void Meldung_Header_xE4(byte* Data);
	void Meldung_Header_xE5(byte* Data);
	void Meldung_Header_xE6(byte* Data);
	void Meldung_Header_xF2(byte* Data);

	bool check_XOR_Data(byte *Data, byte lenData);
	void get_XOR(byte* Data);
};

