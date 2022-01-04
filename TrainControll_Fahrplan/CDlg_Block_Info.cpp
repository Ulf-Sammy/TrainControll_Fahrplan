// CDlg_Block_Info.cpp: Implementierungsdatei
//

#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "CDlg_Block_Info.h"
#include "afxdialogex.h"


// CDlg_Block_Info-Dialog

IMPLEMENT_DYNAMIC(CDlg_Block_Info, CDialogEx)

CDlg_Block_Info::CDlg_Block_Info(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TESTBLOCK, pParent)
{
	m_pParent = pParent;
	m_nID = CDlg_Block_Info::IDD;
	Block_Nr = 0;
	Weichen_Anzahl = 0;
	BlickLok = true;
	Lok_fahren_L = false;
	Lok_fahren_R = false;
}

CDlg_Block_Info::~CDlg_Block_Info()
{
}

void CDlg_Block_Info::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, Spin_BlockNr);
}

void CDlg_Block_Info::Zeige_Daten()
{
	CString str, Einstr, Ausstr;
	CGleisPlan *Gleise;

	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	
	Gleise = &APP->Gleis_Data;
	BlockData = Gleise->Get_DebugData(Block_Nr);

	str.Format(_T("%d"), Block_Nr);
	SetDlgItemText(IDC_EDIT_BLOCK, str);
	str.Format(_T("Block : %d"), BlockData.Block_Nr);
	SetDlgItemText(IDC_STATIC_NR, str);
	if (Gleise->Weg_nach_von_Block(Block_Nr,true, &BlockData.Block_Wege_nach))
	{ 
		if (BlockData.Block_Wege_nach.size() == 1)
			{	BlockN.Format(_T(" %3d <--"), BlockData.Block_Wege_nach[0]);	}
		if (BlockData.Block_Wege_nach.size() == 2)
		{
			BlockN.Format(_T("%2d <-- [%3d]"), BlockData.Block_Wege_nach[1], BlockData.Block_Wege_nach[0]);
		}
		if (BlockData.Block_Wege_nach.size() == 3)
		{
			BlockN.Format(_T("Ende : %d"), BlockData.Block_Wege_nach[2]);
		}
		Lok_fahren_R = 	Gleise->isWeg_frei(BlockData.Block_Wege_nach);
	}
	else
	{
		BlockN.Format(_T("Stop!"));
		Lok_fahren_R = false;
	}
	if (Gleise->Weg_nach_von_Block(Block_Nr, false, &BlockData.Block_Wege_von))
	{
		if (BlockData.Block_Wege_von.size() == 1)
		{
			BlockV.Format(_T("--> %3d"), BlockData.Block_Wege_von[0]);
		}
		if (BlockData.Block_Wege_von.size() == 2)
		{
			BlockV.Format(_T(" [%3d]--> %2d "), BlockData.Block_Wege_von[0], BlockData.Block_Wege_von[1]);
		}
		Lok_fahren_L = Gleise->isWeg_frei(BlockData.Block_Wege_von);
	}
	else
	{
		BlockV.Format(_T("Stop!"));
		Lok_fahren_L = false;
	}
	for (int i = 0; i < 6 ; i++)
	{
		if (i < BlockData.EinBlock.size())
			Einstr.Format(_T("<-: %2d "), BlockData.EinBlock[i]);
		else
			Einstr.Empty();
		if (i < BlockData.AusBlock.size())
			Ausstr.Format(_T(" %2d :->"), BlockData.AusBlock[i]);
		else
			Ausstr.Empty();

		SetDlgItemText(IDC_STATIC_IN1 + i, Einstr);
		SetDlgItemText(IDC_STATIC_OUT1 + i, Ausstr);
	}

	str.Format(_T("%d"), BlockData.Weichen_Anzahl);
	SetDlgItemText(IDC_STATIC_GLEIS_N, BlockData.Gleis_Name);
	SetDlgItemText(IDC_STATIC_LOK, BlockData.Lok_Name);
	if (BlockData.Block_gemeldet)
	{	SetDlgItemText(IDC_STATIC_BLOCK, _T("Melder ON"));	}
	else
	{	SetDlgItemText(IDC_STATIC_BLOCK, _T("Melder OFF"));	}

	str.Format(_T("%d"), BlockData.Relais_Nr);
	str.Format(_T("%d"), BlockData.Relais_Power);

	Beschriftung.Format(_T(" Block Nr. %2d - %s"),Block_Nr, BlockData.Gleis_Name) ;
	LokName = BlockData.Lok_Name;
}


BEGIN_MESSAGE_MAP(CDlg_Block_Info, CDialogEx)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_Block_Info::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_Block_Info::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg_Block_Info::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg_Block_Info::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlg_Block_Info-Meldungshandler



BOOL CDlg_Block_Info::Create()
{

	return CDialogEx::Create(m_nID, m_pParentWnd);
}


void CDlg_Block_Info::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Block_Nr = nPos;
	NeueDaten();

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CDlg_Block_Info::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Block_Nr = 1;
	Spin_BlockNr.SetRange(1, 30);
	Spin_BlockNr.SetPos(Block_Nr);
	return TRUE; 
}

void CDlg_Block_Info::NeueDaten()
{
	Zeige_Daten();
	Invalidate();
}


void CDlg_Block_Info::OnBnClickedButton1()
{ // Links
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	APP->Gleis_Data.TestBlock_mitZug(Block_Nr, true,false);
	NeueDaten();
	APP->Invalidate();
	BlickLok = true;
}


void CDlg_Block_Info::OnBnClickedButton2()
{ // Rechts
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	APP->Gleis_Data.TestBlock_mitZug(Block_Nr, false,false);
	NeueDaten();
	APP->Invalidate();
	BlickLok = false;
}


void CDlg_Block_Info::OnBnClickedButton3()
{ // Block Leeren
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	APP->Gleis_Data.TestBlock_mitZug(Block_Nr, false,true);
	NeueDaten();
	APP->Invalidate();
}

void CDlg_Block_Info::OnBnClickedButton4()
{// Block bezetzen
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	CDataXpressNet* Lok_Block;
	Lok_Block = APP->Gleis_Data.TestZug(BlickLok);
	if (BlickLok)
	{
		APP->Gleis_Data.Besetze_Weg_mit_Lok(Lok_Block, BlockData.Block_Wege_nach);
	}
	else
	{
		APP->Gleis_Data.Besetze_Weg_mit_Lok(Lok_Block, BlockData.Block_Wege_von);
	}
	APP->Invalidate();
}


void CDlg_Block_Info::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	GetClientRect(&m_rectCtrl);
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;
	Zeichen_BlockInfo(CRect(2,200,282,300));
}

void CDlg_Block_Info::Zeichen_BlockInfo(CRect R)
{
	CBrush Brush_Hinterg;
	CBrush* pOldBrush;
	CPen* pOldPen;
	CFont* pOldFont;

	Brush_Hinterg.CreateSolidBrush(colorHinterGrund);
	pOldBrush = pDC->SelectObject(&Brush_Hinterg);
	pOldPen = pDC->SelectObject(&theApp.Stift_SW_1);
	pOldFont = pDC->SelectObject(&theApp.GleisBeschriftung_0);
	pDC->SetBkMode(TRANSPARENT);
	pDC->Rectangle(R);

	pDC->TextOutW(6, 210, Beschriftung);


	if (BlockData.Block_gemeldet)
	{
		pDC->TextOutW(110, 250, LokName);

		if (BlockData.Lok_Richtung)
		{
			theApp.Lok_ge_Uhr.DrawTransparent(pDC, 110, 260, RGB(255, 255, 255));
			pDC->TextOutW(170, 269,BlockV);
			if (Lok_fahren_L)
			{	theApp.Signal_G.DrawTransparent(pDC, 230, 230, RGB(195, 195, 195));		}
			else
			{	theApp.Signal_R.DrawTransparent(pDC, 230, 230, RGB(195, 195, 195));		}

		}
		else
		{
			theApp.Lok_in_Uhr.DrawTransparent(pDC, 110, 260, RGB(255, 255, 255));
			pDC->TextOutW(56, 269, BlockN);
			if (Lok_fahren_R)
			{	theApp.Signal_G.DrawTransparent(pDC, 4, 230, RGB(195, 195, 195));	}
			else
			{	theApp.Signal_R.DrawTransparent(pDC, 4, 230, RGB(195, 195, 195));	}
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}