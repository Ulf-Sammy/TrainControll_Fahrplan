#pragma once


//#ifndef __AFXWIN_H__
//	#error "'stdafx.h' vor dieser Datei f�r PCH einschlie�en"
//#endif
#include "pch.h"
#include "resource.h"		// Hauptsymbole


// CTrainControll_FahrplanApp:
// Siehe TrainControll_Fahrplan.cpp f�r die Implementierung dieser Klasse
//

class CTrainControll_FahrplanApp : public CWinApp
{
public:
	CTrainControll_FahrplanApp();
// �berschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CTrainControll_FahrplanApp theApp;