#pragma once


//#ifndef __AFXWIN_H__
//	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
//#endif
#include "pch.h"
#include "resource.h"		// Hauptsymbole


// CTrainControll_FahrplanApp:
// Siehe TrainControll_Fahrplan.cpp für die Implementierung dieser Klasse
//

class CTrainControll_FahrplanApp : public CWinApp
{
public:
	CTrainControll_FahrplanApp();
// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CTrainControll_FahrplanApp theApp;