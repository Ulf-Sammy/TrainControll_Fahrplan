#pragma once
#include "pch.h"

#define SENDBUFFER 8

class CGleisMelderNetCom
{
public:
	CGleisMelderNetCom(void);
	~CGleisMelderNetCom(void);
	bool OpenCom(int Port);
	void CloseCom();
protected:
	DWORD bytesSend;
	HANDLE m_hCom;
	COMSTAT COM_status;
	DWORD Error_Com;
	int PortNr;
	byte Read_Melder[8];
	bool in_Setup_Setup;
	bool GetTC_Message();
	void SetTC_Message();
	
};

