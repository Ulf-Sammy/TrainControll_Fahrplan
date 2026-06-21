#pragma once


//#ifndef __AFXWIN_H__
//	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
//#endif
#include "pch.h"
#include "resource.h"		// Hauptsymbole
#include "MaskedBitmap.h"
#include <setupapi.h>
#pragma comment (lib, "Setupapi.lib")

#define NUI32_TEXTLAENGE_SERPORTENUM_FRIENDLYNAME  255


// CTrainControll_FahrplanApp:
// Siehe TrainControll_Fahrplan.cpp für die Implementierung dieser Klasse
//


class CTrainControll_FahrplanApp : public CWinApp
{
public:
	CTrainControll_FahrplanApp();
// Überschreibungen
	CFont Uberschrift_0;
	CFont Font_Info_s;
	CFont Font_Info_small;
	CFont BeschriftungBlock_0;
	CFont GleisBeschriftung_0;
	CFont BeschriftungBlock_270;
	CFont GleisBeschriftung_270;

	CBrush Farbe_Weiss_FL;
	CBrush Farbe_Gelb_FL;
	CBrush Farbe_Orange_FL;
	CBrush Farbe_Rot_FL;
	CBrush Farbe_Gruen_FL;
	CBrush Brush_Weiche;
	CBrush Farbe_Dialog;

	CPen Stift_SW_1;

	CPen  Gleis_Frei;
	CPen  Gleis_Besetzt;
	CPen  Gleis_ErrorA;
	CPen  Gleis_ErrorB;
	CPen  Gleis_PowerOff;
	CPen  Gleis_Null;
	CPen  Taster_Rand;

	CMaskedBitmap Lok_in_Uhr;
	CMaskedBitmap Lok_ge_Uhr;
	CMaskedBitmap Lok_Hoch;
	CMaskedBitmap Lok_Runter;
	CMaskedBitmap Taster_Bitmap;

	CMaskedBitmap Signal_R;
	CMaskedBitmap Signal_G;

	CMaskedBitmap WarnungNotAus;
	CMaskedBitmap LokSchuppen;

	CString Get_Time(CString Info);
	virtual BOOL InitInstance();
	
// Implementierung
	DECLARE_MESSAGE_MAP()

};


extern CTrainControll_FahrplanApp theApp;