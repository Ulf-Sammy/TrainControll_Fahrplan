#include "pch.h"
#include "GraphCtrl.h"


CGraphCtrl::CGraphCtrl(void)
{
	// Set Number of Datamember to 0
}

CGraphCtrl::CGraphCtrl(const int x, const int y)
{
	SetPlotDataSize(x,y);
}

CGraphCtrl::~CGraphCtrl(void)
{
}


BEGIN_MESSAGE_MAP(CGraphCtrl, CStatic)
	//{{AFX_MSG_MAP(CGraphCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()	
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl message handlers

BOOL CGraphCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CStatic::OnNotify(wParam, lParam, pResult);
}


LRESULT CGraphCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CStatic::WindowProc(message, wParam, lParam);
}

BOOL CGraphCtrl::OnEraseBkgnd(CDC* pDC) 
{	
	return FALSE;	
}


void CGraphCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
	
	CRect (rc_dest);
	rc_dest=GetSize();

	CRect (rc_source);
	rc_source=GetPlotDataSize();

	//CMemDC pDC(&dc,&rc_source, &rc_dest); // Double Buffering
	//PlotToDC(& pDC);
	
}

void CGraphCtrl::PlotToDC(CMemDC* pDC)
{

}


void CGraphCtrl::Resize(const unsigned short width, const unsigned short height)
{	
	SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CGraphCtrl::FullResize(const CRect newRect)
{
    SetWindowPos(NULL, newRect.left, newRect.top, newRect.right, newRect.bottom, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CGraphCtrl::SetSize(const unsigned short width, const unsigned short height)
{
	Resize(width-1, height-1);
}

CRect CGraphCtrl::GetSize()
{
	CRect rc;
	GetClientRect(rc);		
	return rc;
}

CRect CGraphCtrl::GetPlotDataSize()
{
	CRect rc;
		
	return rc;
}


void CGraphCtrl::SetPlotDataSize(const int x, const int y)
{

}

void CGraphCtrl::SetPlotData(vector<vector<COLORREF> > InputArray)
{	

}

void CGraphCtrl::UpdatePlot()
{		
	Invalidate();
	UpdateWindow();	
}

