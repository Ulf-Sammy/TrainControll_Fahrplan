// CDlg_Lok_Schuppen.cpp: Implementierungsdatei
//

#include "pch.h"
#include "framework.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "CDlg_Lok_Schuppen.h"
#include "afxdialogex.h"


// CDlg_Lok_Schuppen-Dialog

IMPLEMENT_DYNAMIC(CDlg_Lok_Schuppen, CDialogEx)

CDlg_Lok_Schuppen::CDlg_Lok_Schuppen(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOKSCHUPPEN, pParent)
{
	m_pParent = pParent;
	m_nID = CDlg_Lok_Schuppen::IDD;
	last_On_Block = 0;
	
	for (int i = 0; i < 6; i++)
	{
		Gleis_Platz[i] = CRect(50, -11, 200, 11);
		Gleis_Platz[i].OffsetRect(Gleis_Schuppen[i].A_Punkt);
	}

	PinselHaus.CreateSolidBrush(colorDunkelGrau);
	PinselTor.CreateSolidBrush(colorSchuppenTor);
	StiftHaus.CreatePenIndirect((LPLOGPEN)&StiftLokSchuppen);
	StiftTor .CreatePenIndirect((LPLOGPEN)&StiftTorSchuppen);
}

CDlg_Lok_Schuppen::~CDlg_Lok_Schuppen()
{
}

BOOL CDlg_Lok_Schuppen::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

BOOL CDlg_Lok_Schuppen::Init()
{
	const SIZE S = { 39,50 };
	POINT P;

	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	//BlockNet = &APP->BlockMelder;
	Gleise = &APP->Anlagen_Daten.Gleis_Data;
	Loks = &APP->Anlagen_Daten.meineLoks;

	SetWindowPos(NULL, 10, 300, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	Zeige_Comobox = false;
	CString LName[MaxAbstellGleise];
	bool   LBlick[MaxAbstellGleise] = {true,true,true,true,true,true,true,true };

	for (auto& Lok : APP->Anlagen_Daten.meineLoks.HomeZüge)
	{
		Gleis_Block_On[Lok.Block - 33] = true;
		LName[Lok.Block - 33] = Lok.Lok_Name;
		LBlick[Lok.Block - 33] = Lok.Blick;
	}
	for (int i = 0; i < MaxAbstellGleise; i++)
	{
		Gleis_Platz[i] = CRect(50, -11, 220, 11);
		Gleis_Platz[i].OffsetRect(Gleis_Schuppen[i].A_Punkt);
		Lok_Gleis[i].SetWindowPos(NULL, Gleis_Platz[i].left, Gleis_Platz[i].top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		Lok_Gleis[i].ShowWindow(SW_HIDE);
		
		APP->Anlagen_Daten.meineLoks.Fill_Liste_Zug(&Lok_Gleis[i],LName[i]);
		if(LBlick[i])
			P = POINT(Gleis_Platz[i].TopLeft() + CPoint(+180, -28));
		else 
			P = POINT(Gleis_Platz[i].TopLeft() + CPoint( -55, -28));

		Lok_Platz[i] = CRect(P, S);
	}
	return 0;
}

void CDlg_Lok_Schuppen::OnPaint()
{
	CPoint TextPos, TextBes;
	CString GleisName;
	CString LocName;
	bool Lok_Richtung;
	CFont* pOldFont;

	CPaintDC dc(this);
	GetClientRect(&m_rectCtrl);
	CMem_DC memDC(&dc, &m_rectCtrl);
	pDC = &memDC;
	ZeichenHintergrund();

	pOldFont = pDC->SelectObject(&theApp.Uberschrift_0);
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOutW(10, 20, _T("Lok-Schuppen"));

	if (Gleise->Get_Door_open())
	{
		CBrush f_Tor;
		CBrush b_Tor;
		Tor_pos = Gleise->Get_Door_free();

		f_Tor.CreateSolidBrush(colorGruen);
		b_Tor.CreateSolidBrush(colorRot);
		pDC->SelectObject(StiftTor);
		pDC->SelectObject(f_Tor);

		if (Tor_pos == 3) pDC->SelectObject(b_Tor);
		pDC->Rectangle(SchuppenTor_G2);

		if (Tor_pos == 2) pDC->SelectObject(b_Tor);
		pDC->Rectangle(SchuppenTor_G1);

		if (Tor_pos == 1) pDC->SelectObject(b_Tor);
		pDC->Rectangle(SchuppenTor_G0);
	}

	pDC->SelectObject(&theApp.Gleis_Frei);

	for (int i = 0; i < 2; i++)
	{
		switch (Gleise->GetStatus_Block(Strecke_Block[i], &LocName))
		{
		case BlockStatus::Frei:
			pDC->SelectObject(theApp.Gleis_Frei);
			break;
		case BlockStatus::Besetzt:
			pDC->SelectObject(theApp.Gleis_Besetzt);
			break;
		case BlockStatus::BesetztError_A:
			pDC->SelectObject(theApp.Gleis_ErrorA);
			break;
		case BlockStatus::BesetztError_B:
			pDC->SelectObject(theApp.Gleis_ErrorB);
			break;
		default:
			pDC->SelectObject(theApp.Gleis_Null);
			break;
		}
		pDC->MoveTo(Gleis_Strecke[i].A_Punkt);		pDC->LineTo(Gleis_Strecke[i].E_Punkt);
	}

	for (int i = 0; i < MaxWeiche; i++)
	{
		ZeicheWeiche(pDC, Weiche_Platz[i], Gleise->Get_Weiche(Weiche_Nummer[i]));
	}
	pDC->SelectObject(PinselTor);
	pDC->SelectObject(StiftTor);
	if (Gleise->Get_Door_open())
	{
		pDC->Rectangle(SchuppenTor_L);
		pDC->Rectangle(SchuppenTor_R);
	}
	else
	{
		pDC->Rectangle(SchuppenTor);
	}

	pDC->SelectObject(theApp.Farbe_Weiss_FL);

	for (int i = 0; i < MaxAbstellGleise; i++)
	{
		switch (Gleise->GetStatus_Block(Gleis_Block[i], &LocName, &Lok_Richtung))
		{
		case BlockStatus::Frei:
			pDC->SelectObject(theApp.Gleis_Frei);
			break;
		case BlockStatus::Besetzt:
			pDC->SelectObject(theApp.Gleis_Besetzt);
			break;
		case BlockStatus::BesetztError_A:
			pDC->SelectObject(theApp.Gleis_ErrorA);
			break;
		case BlockStatus::BesetztError_B:
			pDC->SelectObject(theApp.Gleis_ErrorB);
			break;
		default:
			pDC->SelectObject(theApp.Gleis_Null);
			break;
		}
		pDC->MoveTo(Gleis_Schuppen[i].A_Punkt);		pDC->LineTo(Gleis_Schuppen[i].E_Punkt);

		TextPos = Gleis_Platz[i].TopLeft() + CPoint(+10, +2);
		TextBes = Gleis_Platz[i].TopLeft() + CPoint(+5, -16);

		GleisName.Format(_T(" Abstellgleis Nr.: %i"), i+1);
		
		if (Gleise->isPower_onBlock(Gleis_Block[i]))
			pDC->SelectObject(theApp.Gleis_Frei);
		else
			pDC->SelectObject(theApp.Gleis_PowerOff);

		pDC->Rectangle(Gleis_Platz[i]);
		pDC->SelectObject(theApp.Stift_SW_1);
		pDC->Rectangle(Gleis_Platz[i]);

		pDC->SelectObject(&theApp.BeschriftungBlock_0);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(TextPos.x, TextPos.y, Gleise->Get_LokName_in_Block(Gleis_Block[i])); // Lok in Block text
		pDC->SelectObject(&theApp.GleisBeschriftung_0);
		pDC->TextOutW(TextBes.x, TextBes.y, GleisName); // Beschreibung Gleis
		if (!Gleise->Get_LokName_in_Block(Gleis_Block[i]).IsEmpty())
		{
			if (Lok_Richtung)
				theApp.Lok_ge_Uhr.DrawTransparent(pDC, Lok_Platz[i].left, Lok_Platz[i].top, RGB(255, 255, 255));
			else
				theApp.Lok_in_Uhr.DrawTransparent(pDC, Lok_Platz[i].left, Lok_Platz[i].top , RGB(255, 255, 255));
		}

	}
	CDialog::OnPaint();
}

void CDlg_Lok_Schuppen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Lok_Gleis[0]);
	DDX_Control(pDX, IDC_COMBO2, Lok_Gleis[1]);
	DDX_Control(pDX, IDC_COMBO3, Lok_Gleis[2]);
	DDX_Control(pDX, IDC_COMBO4, Lok_Gleis[3]);
	DDX_Control(pDX, IDC_COMBO5, Lok_Gleis[4]);
	DDX_Control(pDX, IDC_COMBO6, Lok_Gleis[5]);
	DDX_Control(pDX, IDC_COMBO7, Lok_Gleis[6]);
	DDX_Control(pDX, IDC_COMBO8, Lok_Gleis[7]);
}

void CDlg_Lok_Schuppen::ZeichenHintergrund()
{
	CBrush Brush_Hinterg;
	CBrush* pOldBrush;
	CPen* pOldPen;

	Brush_Hinterg.CreateSolidBrush(colorHinterGrund);
	pOldBrush = pDC->SelectObject(&Brush_Hinterg);
	pOldPen = pDC->SelectObject(&theApp.Stift_SW_1);

	pDC->Rectangle(m_rectCtrl);
	pDC->SelectObject(PinselHaus);

	pDC->Rectangle(2, 45, 550, 450);

	pDC->SelectObject(StiftHaus);

	pDC->MoveTo(7, 50);
	pDC->LineTo(545, 50);
	pDC->LineTo(545, 300);
	pDC->LineTo(550, 300);
	pDC->LineTo(550, 45);
	pDC->LineTo(2, 45);
	pDC->LineTo(2, 450);
	pDC->LineTo(550, 450);
	pDC->LineTo(550, 420);
	pDC->LineTo(545, 420);
	pDC->LineTo(545, 445);
	pDC->LineTo(7, 445);
	pDC->LineTo(7, 50);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CDlg_Lok_Schuppen::ZeicheWeiche(CDC* pDC, CRect Weiche, bool Bit)
{
	pDC->SelectObject(&theApp.Gleis_Null);

	if (Bit)
	{
		pDC->MoveTo(Weiche.left, Weiche.bottom);
		pDC->LineTo(Weiche.right, Weiche.bottom);
	}
	else
	{
		pDC->MoveTo(Weiche.left, Weiche.top);
		pDC->LineTo(Weiche.right, Weiche.bottom);
	}
	
	pDC->SelectObject(&theApp.Gleis_Frei);
	if (Bit)
	{
		pDC->MoveTo(Weiche.left, Weiche.top);
		pDC->LineTo(Weiche.right, Weiche.bottom);
	}
	else
	{
		pDC->MoveTo(Weiche.left, Weiche.bottom);
		pDC->LineTo(Weiche.right, Weiche.bottom);
	}
}

void CDlg_Lok_Schuppen::ZeicheGleis(CDC* pDC, byte I, Line2Point Gleis)
{
	//Gleise->
	pDC->MoveTo(Gleis_Schuppen[I].A_Punkt);		
	pDC->LineTo(Gleis_Schuppen[I].E_Punkt);
}

void CDlg_Lok_Schuppen::WeichenWeg_schalten(byte I)
{
	switch (I)
	{
	case 0: // Abstellgeleis Gleis No: 1 
		Weiche_Nummer[0].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		break;
	case 1: // Abstellgeleis Gleis No: 2 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		break;
	case 2: // Abstellgeleis Gleis No: 3 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		break;
	case 3: // Abstellgeleis Gleis No: 4 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		break;
	case 4: // Abstellgeleis Gleis No: 5 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(true);
		Weiche_Nummer[4].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		Gleise->Set_Weiche(Weiche_Nummer[4]);
		break;
	case 5: // Abstellgeleis Gleis No: 6 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(true);
		Weiche_Nummer[4].SetBit(true);
		Weiche_Nummer[5].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		Gleise->Set_Weiche(Weiche_Nummer[4]);
		Gleise->Set_Weiche(Weiche_Nummer[5]);
		break;
	case 6: // Abstellgeleis Gleis No: 7 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(true);
		Weiche_Nummer[4].SetBit(true);
		Weiche_Nummer[5].SetBit(true);
		Weiche_Nummer[6].SetBit(false);

		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		Gleise->Set_Weiche(Weiche_Nummer[4]);
		Gleise->Set_Weiche(Weiche_Nummer[5]);
		Gleise->Set_Weiche(Weiche_Nummer[6]);
		break;
	case 7: // Abstellgeleis Gleis No: 8 
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(true);
		Weiche_Nummer[4].SetBit(true);
		Weiche_Nummer[5].SetBit(true);
		Weiche_Nummer[6].SetBit(true);

		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		Gleise->Set_Weiche(Weiche_Nummer[4]);
		Gleise->Set_Weiche(Weiche_Nummer[5]);
		Gleise->Set_Weiche(Weiche_Nummer[6]);

		break;
	default:
		break;
	}
}

BEGIN_MESSAGE_MAP(CDlg_Lok_Schuppen, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_Lok_Schuppen::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Lok_Schuppen::OnSelchangeCombos1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_Lok_Schuppen::OnSelchangeCombos2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_Lok_Schuppen::OnSelchangeCombos3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_Lok_Schuppen::OnSelchangeCombos4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_Lok_Schuppen::OnSelchangeCombos5)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_Lok_Schuppen::OnSelchangeCombos6)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_Lok_Schuppen::OnSelchangeCombos7)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_Lok_Schuppen::OnSelchangeCombos8)
END_MESSAGE_MAP()

// CDlg_Lok_Schuppen-Meldungshandler

void CDlg_Lok_Schuppen::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (SchuppenTor.PtInRect(point))
	{
		bool SchuppenTor_auf = !Gleise->Get_Door_open();
		//BlockNet->Send_Door_open(SchuppenTor_auf);
		Invalidate();
		return;
	}
	if (Gleise->Get_Door_open())
	{
		for (int i = 0; i < MaxAbstellGleise; i++)
		{
			if (Gleis_Platz[i].PtInRect(point))
			{
				if (last_On_Block == Gleis_Block[i])
				{ 
					Gleise->Schalte_Relais(Gleis_Block[i], false);
					last_On_Block = 0;
				}
				else
				{
					WeichenWeg_schalten(i);
					Gleise->Schalte_Relais(last_On_Block, false);
					Gleise->Schalte_Relais(Gleis_Block[i], true);
					last_On_Block = Gleis_Block[i];
				}
				Invalidate();
			}
			if (Weiche_Platz[i].PtInRect(point))
			{
				Weiche_Nummer[i].SetBit(!Weiche_Nummer[i].GetBit());
				Invalidate();
			}
		}
	}

	if (Zeige_Comobox)
	{
		for (int i = 0; i < MaxAbstellGleise; i++)
		{
			if (Lok_Platz[i].PtInRect(point))
			{
				if (Gleise->Get_Zug_Point(i + 33) != nullptr)
				{
					Gleise->Get_Zug_Point(i + 33)->Blick = !Gleise->Get_Zug_Point(i + 33)->Blick;
					if (Gleise->Get_Zug_Point(i + 33)->Blick)
					{
						Lok_Platz[i].left = 250;
						Lok_Platz[i].right = (250 + 50);
					}
					else
					{
						Lok_Platz[i].left = 15;
						Lok_Platz[i].right = (15 + 50);
					}
					Invalidate();
				}
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlg_Lok_Schuppen::OnBnClickedButton1()
{
	Zeige_Comobox = !Zeige_Comobox;
	if (Zeige_Comobox)
	{
		for (byte i = 0; i < MaxAbstellGleise; i++)
		{
			Lok_Gleis[i].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
		for (byte i = 0; i < MaxAbstellGleise; i++)
		{
			Lok_Gleis[i].ShowWindow(SW_HIDE);
		}
		Invalidate();
		APP->Anlagen_Daten.Update_Lok_Abstellgleis();
		APP->Anlagen_Daten.meineLoks.Save_acktiv_Zug_Data();
		APP->Set_Train_Run_DLG();
	}
}

void CDlg_Lok_Schuppen::OnSelchangeCombos1()
{
	ChangeCombox(33);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos2()
{
	ChangeCombox(34);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos3()
{
	ChangeCombox(35);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos4()
{
	ChangeCombox(36);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos5()
{
	ChangeCombox(37);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos6()
{
	ChangeCombox(38);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos7()
{
	ChangeCombox(39);
}
void CDlg_Lok_Schuppen::OnSelchangeCombos8()
{
	ChangeCombox(40);
}

void CDlg_Lok_Schuppen::ChangeCombox(byte Nr)
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	Start_Lok_Block Data;
	Data.Block = Nr;
	Data.LokNr = Lok_Gleis[Data.Block - 33].GetCurSel();

	if (Data.LokNr == 0)
	{
		Data.Lok_Name.Empty();
	}
	else
	{
		Data.Lok_Name = APP->Anlagen_Daten.meineLoks.Get_Lok_Name(Data.LokNr - 1);
		Data.Blick = true;
	}

	APP->Anlagen_Daten.Setze_Lok_aufGleis(Data);
	Invalidate();
}