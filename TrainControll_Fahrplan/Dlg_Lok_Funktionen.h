#pragma once
#include "pch.h"
#include "VC_TableInfo.h"
#include "resource.h"	


// Dlg_Lok_Funktionen dialog

class CDlg_Lok_Funktionen : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Lok_Funktionen)

public:
	CDlg_Lok_Funktionen(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Lok_Funktionen();

// Dialog Data
	enum { IDD = IDD_DLG_LOK_FUNKTIONEN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CDecoderInfo* CV_Table;
	byte Sel_Type;
	byte Sel_Lok;
	DECLARE_MESSAGE_MAP()
public:
	void SetCV_TablePointer(CDecoderInfo* CV_T);

	void Set_Type_and_Lok(byte Type, byte Lok);
	virtual BOOL OnInitDialog();
	CString m_ZugName;
	CString Ausgang[9];
	CString Sound[13];
	virtual void OnOK();
};
