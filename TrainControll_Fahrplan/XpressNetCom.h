#pragma once
#include "pch.h"

#define SENDBUFFER 8

class CXpressNetCom
{
public:
	CXpressNetCom(void);
	~CXpressNetCom(void);
	bool OpenCom(int Port);
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

	byte Hole_Acknolage_Mode();
	void HoleZugData(byte *Data, byte Adr);
	TrainCon_Paar HoleMelderData();
	TrainCon_Paar HoleWeicheData();
	byte HoleStatus_LZV();
	byte Hole_CV_Wert();
	// Nur für Setup ;
	void Set_In_Mode(ControlStatus Mode, byte SubMode, byte *LZVMode);
	bool Get_Acknolage_Mode(ControlStatus Mode, byte SubMode, byte *LZVMode);
	int GetMelderAnzahl();
	TrainCon_Paar ReadMelder();
	void SetWeichenAnzahl(byte Nr);
	void SetMelder_Zeit(byte Nr);
	byte GetStatus_Setup_LZV();

	byte DoWorkonCV(bool RW, byte CV, byte Wert);


protected:
	const byte SendAdr[6] = { COM_WRITE_ZUG_D0 ,COM_WRITE_ZUG_D1,COM_WRITE_ZUG_D2,COM_WRITE_ZUG_D3,COM_WRITE_ZUG_D4,COM_WRITE_ZUG_D5 };
	DWORD bytesSend;
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	bool in_Setup_Setup;
	int HeadSend_Liste;
	int TailSend_Liste;
	byte Send_Liste[SENDBUFFER][8];
	byte Befehl_Read[8];
	byte Befehl_Send[8];
	void Neu_Send_Befehl();
	bool GetTC_Message();
	void SetTC_Message();
	

};
