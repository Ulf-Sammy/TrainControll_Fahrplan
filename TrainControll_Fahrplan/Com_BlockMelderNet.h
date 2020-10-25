#pragma once
#include "pch.h"

#define SENDBUFFER 8

class CCom_BlockMelderNet
{
public:
	CCom_BlockMelderNet(void);
	~CCom_BlockMelderNet(void);
	bool OpenCom(int Port);
	void CloseCom();
	void SetMelder_Zeit(byte Nr);
	void SetBlockPower(TrainCon_Paar Relais);
	TrainCon_Paar HoleBlockData();
	TrainCon_Paar HoleRelayData();
	byte GetNextMessage(); // nur in Task
	void StartKomunikation(); // in Task;
	bool NoComToBlockNet();
	byte Get_VersionInfo(CString *Text);
	void ZeichneBlockMeldung(CDC* pDC);
protected:
	CFont Font_Info_Melder;
	DWORD bytesSend;
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	CString ComInfo;
	byte ComInfo_Nr;
	byte Read_Melder[8] = { 0,0,0,0,0,0,0,0 };
	byte Befehl_Send[8] = { 0,0,0,0,0,0,0,0 };
	CString Debug_Text[11];
	bool in_Setup_Setup;
	bool GetTC_Message();
	void SetTC_Message();
	void Set_Debug_Text(byte Block, bool Bit);
	
};

