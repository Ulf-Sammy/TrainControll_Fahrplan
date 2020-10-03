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
protected:
	DWORD bytesSend;
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	byte Read_Melder[8];
	byte Befehl_Send[8]; 
	bool in_Setup_Setup;
	bool GetTC_Message();
	void SetTC_Message();
	
};

