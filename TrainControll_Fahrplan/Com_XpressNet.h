#pragma once
#include "pch.h"

#define SENDBUFFER 8

class CCom_XpressNet
{
public:
	CCom_XpressNet(void);
	~CCom_XpressNet(void);
	bool OpenCom(int Port);
	void StartKomunikation();
	void CloseCom();
	bool NoComToXpressNet();
	void SendeNeueDaten();
	void SendeZugDaten(XpNSendwas was, byte Adresse, byte Daten);
	void SendeWeicheDaten(TrainCon_Paar Wl);
	void SendeAsknachLokDaten(byte Adr);
	void SendeLVZ_Power(bool Bit);
	void Sende_Write_CV(byte CV, byte Wert);
	void Sende_Read_CV(byte CV, byte Wert);
	void Sende_Setto_Prog(bool onoff);
	byte GetNextMessage(); // nur in Task

	void HoleZugData(byte *Data, byte Adr);
	TrainCon_Paar HoleMelderData();
	TrainCon_Paar HoleWeicheData();
	byte HoleStatus_LZV();
	byte Hole_CV_Wert();
	// Nur für Setup ;
	byte GetStatus_LZV();

	byte DoWorkonCV(bool RW, byte CV, byte Wert);
	byte Get_VersionInfo(CString *text);
	bool Get_Mode_Run();
	void ZeichneWeichenMeldung(CDC* pDC);
	void ZeichneZugInfoMeldung(CDC* pDC);

protected:
	const byte SendAdr[6] = { COM_WRITE_ZUG_D0 ,COM_WRITE_ZUG_D1,COM_WRITE_ZUG_D2,COM_WRITE_ZUG_D3,COM_WRITE_ZUG_D4,COM_WRITE_ZUG_D5 };
	DWORD bytesSend;
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	CString ComInfo;
	byte ComInfo_Nr;
	byte LZVInfo_Nr;
	bool in_Run;
	int HeadSend_Liste;
	int TailSend_Liste;
	byte Send_Liste[SENDBUFFER][8];
	byte Befehl_Read[8] = {0,0,0,0,0,0,0,0};
	byte Befehl_Send[8] = { 0,0,0,0,0,0,0,0 };
	CString Debug_Text[11];
	CString Debug_Text2[11];
	void Neu_Send_Befehl();
	bool GetTC_Message();
	void SetTC_Message();
	void DebugMessage();
	void Set_Debug_Text(byte Block, bool Bit);
	void Set_Debug_Text();
};
