
// TrainControll_Fahrplan.cpp: Definiert das Klassenverhalten f�r die Anwendung.
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "DlgTrainRun.h"
#include "TrainControll_FahrplanDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;

// #define new DEBUG_NEW
#endif


// CTrainControll_FahrplanApp

BEGIN_MESSAGE_MAP(CTrainControll_FahrplanApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTrainControll_FahrplanApp-Erstellung

CTrainControll_FahrplanApp::CTrainControll_FahrplanApp()
{
	// TODO: Hier Code zur Konstruktion einf�gen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CTrainControll_FahrplanApp-Objekt

CTrainControll_FahrplanApp theApp;


// CTrainControll_FahrplanApp-Initialisierung

BOOL CTrainControll_FahrplanApp::InitInstance()
{
	CWinApp::InitInstance();


	// Shell-Manager erstellen, falls das Dialogfeld
	// Shellstrukturansicht- oder Shelllistenansicht-Steuerelemente enth�lt.
	//CShellManager *pShellManager = new CShellManager;

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Gr��e
	// der ausf�hrbaren Datei verringern m�chten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// �ndern Sie den Registrierungsschl�ssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: �ndern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CTrainControll_FahrplanDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: F�gen Sie hier Code ein, um das Schlie�en des
		//  Dialogfelds �ber "OK" zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: F�gen Sie hier Code ein, um das Schlie�en des
		//  Dialogfelds �ber "Abbrechen" zu steuern
	}

	// Den oben erstellten Shell-Manager l�schen.
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	// Da das Dialogfeld geschlossen wurde, FALSE zur�ckliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}

