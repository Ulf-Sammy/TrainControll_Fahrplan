#pragma once
#include "afxwin.h"
#include "MemDC.h"

using namespace std; 

class CGraphCtrl : 	public CStatic
{
// Construction
public:
	CGraphCtrl(void);
	CGraphCtrl(const int x, const int y);
// Destruction
public:
	~CGraphCtrl(void);

// Attributes
public:

// Operations
public:

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGraphCtrl)
public:
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:


	void Resize(const unsigned short width, const unsigned short height);
	void FullResize(const CRect newRect);
	void SetSize(const unsigned short width, const unsigned short height);
	void SetPlotDataSize(const int x, const int y);
	CRect GetSize();
	CRect GetPlotDataSize();

	void SetPlotData(vector<vector<COLORREF> > InputArray);
	void UpdatePlot();
	void PaintPixel(CMemDC* pDC, const int x, const int y, COLORREF c);
	void PlotToDC(CMemDC* pDC);

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
};

