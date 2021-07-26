
// TrainControll_Fahrplan.cpp: Definiert das Klassenverhalten für die Anwendung.
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "CDlg_Train_Run.h"
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
	Stift_SW_1.CreatePenIndirect((LPLOGPEN)&Stift_Schwarz);

	Gleis_Frei.CreatePenIndirect((LPLOGPEN)&StiftGleis_frei);
	Gleis_Besetzt.CreatePenIndirect((LPLOGPEN)&StiftGleis_besetzt);
	Gleis_PowerOff.CreatePenIndirect((LPLOGPEN)&StiftGleisRot);
	Gleis_Null.CreatePenIndirect((LPLOGPEN)&StiftGleisNull);
	Gleis_ErrorA.CreatePenIndirect((LPLOGPEN)&StiftGleis_ErrorA);
	Gleis_ErrorB.CreatePenIndirect((LPLOGPEN)&StiftGleis_ErrorB);

	Font_Info_s.CreateFontIndirectW(&FontType_Ar_15_0);
	Font_Info_small.CreateFontIndirectW(&FontType_Ar_9_0);

	Uberschrift_0.CreateFontIndirectW(&FontType_ArR_20_0);
	BeschriftungBlock_0.CreateFontIndirectW(&FontType_Ar_16_0);
	GleisBeschriftung_0.CreateFontIndirectW(&FontType_Ar_14_0);
	BeschriftungBlock_270.CreateFontIndirectW(&FontType_Ar_14_270);
	GleisBeschriftung_270.CreateFontIndirectW(&FontType_Ar_11_270);;

	Brush_White.CreateSolidBrush(colorWeiss);
	Brush_Yellow.CreateSolidBrush(RGB(255, 255, 0));
	Brush_Red.CreateSolidBrush(RGB(255, 0, 0));
	Brush_Green.CreateSolidBrush(RGB(0, 140, 0));
	Brush_Weiche.CreateSolidBrush(colorHinterGrund);



}


// Das einzige CTrainControll_FahrplanApp-Objekt

CTrainControll_FahrplanApp theApp;


// CTrainControll_FahrplanApp-Initialisierung

CString CTrainControll_FahrplanApp::Get_Time(CString Info)
{
	SYSTEMTIME  lt;
	CString	Text;

	GetLocalTime(&lt);

	Text.Format(_T(">  %02d:%02d:%02d :%03d  = %s"),lt.wHour, lt.wMinute,lt.wSecond, lt.wMilliseconds, Info);
	 return Text;
	 //GetTickCount64();
}

BOOL CTrainControll_FahrplanApp::InitInstance()
{
	CWinApp::InitInstance();

	// Shell-Manager erstellen, falls das Dialogfeld
	// Shellstrukturansicht- oder Shelllistenansicht-Steuerelemente enthält.
	//CShellManager *pShellManager = new CShellManager;

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	//SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CTrainControll_FahrplanDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "OK" zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Den oben erstellten Shell-Manager löschen.
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}

