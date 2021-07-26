// MaskedBitmap.h: Schnittstelle für die Klasse CMaskedBitmap.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMaskedBitmap : public CBitmap  
{
public:
	void DrawTransparent (CDC* pDC, int x, int y, COLORREF clrTransparency);
	void Draw (CDC* pDC, int x, int y);
};
