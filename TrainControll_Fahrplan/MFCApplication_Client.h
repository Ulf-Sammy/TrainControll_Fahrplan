
// MFCApplication_Client.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CMFCApplicationClientApp:
// Siehe MFCApplication_Client.cpp für die Implementierung dieser Klasse
//

class CMFCApplicationClientApp : public CWinApp
{
public:
	CMFCApplicationClientApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationClientApp theApp;
