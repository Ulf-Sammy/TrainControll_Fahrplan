// MaskedBitmap.cpp: Implementierung der Klasse CMaskedBitmap.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "MaskedBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CMaskedBitmap::Draw(CDC *pDC, int x, int y)
{
    BITMAP bm;
    GetBitmap (&bm);
    CPoint size (bm.bmWidth, bm.bmHeight);
    pDC->DPtoLP (&size);

    CPoint org (0, 0);
    pDC->DPtoLP (&org);

    CDC dcMem;
    dcMem.CreateCompatibleDC (pDC);
    CBitmap* pOldBitmap = dcMem.SelectObject (this);
    dcMem.SetMapMode (pDC->GetMapMode ());

    pDC->BitBlt (x, y, size.x, size.y, &dcMem, org.x, org.y, SRCCOPY);

    dcMem.SelectObject (pOldBitmap);
}

void CMaskedBitmap::DrawTransparent(CDC *pDC, int x, int y,
	COLORREF clrTransparency)
{
    BITMAP bm;
    GetBitmap (&bm);
    CPoint size (bm.bmWidth, bm.bmHeight);
    pDC->DPtoLP (&size);

    CPoint org (0, 0);
    pDC->DPtoLP (&org);

	//
    // ersten Speicherkontext anlegen und Bitmap einsetzen
	//
    CDC dcImage;
    dcImage.CreateCompatibleDC (pDC);
    CBitmap* pOldBitmapImage = dcImage.SelectObject (this);
    dcImage.SetMapMode (pDC->GetMapMode ());

	//
    // zweiten Speicherkontext anlegen und eine AND-Maske erzeugen
	//
    CDC dcAnd;
    dcAnd.CreateCompatibleDC (pDC);
    dcAnd.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapAnd;
    bitmapAnd.CreateBitmap (bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    CBitmap* pOldBitmapAnd = dcAnd.SelectObject (&bitmapAnd);

    dcImage.SetBkColor (clrTransparency);
    dcAnd.BitBlt (org.x, org.y, size.x, size.y, &dcImage, org.x, org.y,
        SRCCOPY);

	//
    // dritten Speicherkontext anlegen und eine XOR-Maske erzeugen
	//
    CDC dcXor;
    dcXor.CreateCompatibleDC (pDC);
    dcXor.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapXor;
    bitmapXor.CreateCompatibleBitmap (&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapXor = dcXor.SelectObject (&bitmapXor);

    dcXor.BitBlt (org.x, org.y, size.x, size.y, &dcImage, org.x, org.y,
        SRCCOPY);

    dcXor.BitBlt (org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y,
        0x220326);

	//
    // Bildpunkte des Zielrechtecks in einen temporären Speicherkontext kopieren
	//
    CDC dcTemp;
    dcTemp.CreateCompatibleDC (pDC);
    dcTemp.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapTemp;
    bitmapTemp.CreateCompatibleBitmap (&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapTemp = dcTemp.SelectObject (&bitmapTemp);

    dcTemp.BitBlt (org.x, org.y, size.x, size.y, pDC, x, y, SRCCOPY);

	//
    // Das endgültige Bitmap entsteht über ein AND des Ziel-Rechtecks
	// (im temporären Speicherkontext), gefolgt von einem XOR
		//
    dcTemp.BitBlt (org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y,
        SRCAND);

    dcTemp.BitBlt (org.x, org.y, size.x, size.y, &dcXor, org.x, org.y,
        SRCINVERT);

	//
    // Ergebnis auf de Bildschirm
	//
    pDC->BitBlt (x, y, size.x, size.y, &dcTemp, org.x, org.y, SRCCOPY);

	//
    // Standard-Bitmaps wieder einsetzen (optional)
	//
    dcTemp.SelectObject (pOldBitmapTemp);
    dcXor.SelectObject (pOldBitmapXor);
    dcAnd.SelectObject (pOldBitmapAnd);
    dcImage.SelectObject (pOldBitmapImage);
}
