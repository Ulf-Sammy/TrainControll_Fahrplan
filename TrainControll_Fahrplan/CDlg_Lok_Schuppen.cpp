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

	Weiche_Platz[4] = CRect(300, 100, 340, 140);
	Weiche_Platz[3] = CRect(340, 140, 380, 180);
	Weiche_Platz[2] = CRect(380, 180, 420, 220);
	Weiche_Platz[1] = CRect(540, 220, 580, 260);
	Weiche_Platz[0] = CRect(580, 260, 620, 300);

	Gleis_Schuppen[7] = Line2Point(620, 800, 300);
	Gleis_Schuppen[6] = Line2Point(420, 540, 220);
	Gleis_Schuppen[5] = Line2Point(20, 300, 100);
	Gleis_Schuppen[4] = Line2Point(20, 300, 140);
	Gleis_Schuppen[3] = Line2Point(20, 380, 180);
	Gleis_Schuppen[2] = Line2Point(20, 420, 220);
	Gleis_Schuppen[1] = Line2Point(20, 550, 260);
	Gleis_Schuppen[0] = Line2Point(20, 580, 300);

	for (int i = 0; i < 6; i++)
	{
		Gleis_Platz[i] = CRect(50, -11, 200, 11);
		Gleis_Platz[i].OffsetRect(Gleis_Schuppen[i].A_Punkt);
		Gleis_Block_On[i] = false;
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
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	BlockNet = &APP->BlockMelder;
	Gleise = &APP->Gleis_Data;

	SetWindowPos(NULL, 10, 300, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	Weiche_Nummer[0] = TrainCon_Paar(18, false);
	Weiche_Nummer[1] = TrainCon_Paar(19, false);
	Weiche_Nummer[2] = TrainCon_Paar(20, false);
	Weiche_Nummer[3] = TrainCon_Paar(21, false);
	Weiche_Nummer[4] = TrainCon_Paar(22, false);
	Weiche_Nummer[5] = TrainCon_Paar(23, false);

	Gleis_Block[0] = 25;
	Gleis_Block[1] = 26;
	Gleis_Block[2] = 27;
	Gleis_Block[3] = 28;
	Gleis_Block[4] = 29;
	Gleis_Block[5] = 30;
	Gleis_Block[6] = 24;
	Gleis_Block[7] = 24;

	Weiche_Block[0] = 24;
	Weiche_Block[1] = 24;
	Weiche_Block[2] = 24;
	Weiche_Block[3] = 24;
	Weiche_Block[4] = 24;
	Weiche_Block[5] = 24;

	SchuppenTor = CRect(455,180,462,340);
	SchuppenTor_L = CRect(455, 180, 462, 200);
	SchuppenTor_R = CRect(455, 320, 462, 340);

	SchuppenTor_G2 = CRect(455, 200, 462, 240);
	SchuppenTor_G1 = CRect(455, 241, 462, 280);
	SchuppenTor_G0 = CRect(455, 281, 462, 320);
	return 0;
}

void CDlg_Lok_Schuppen::OnPaint()
{
	CPoint TextPos, TextBes, LokPos;
	CString GleisName;
	CString LocName;
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


		byte Tor_pos = 1;
		f_Tor.CreateSolidBrush(colorgruen);
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

	for (int i = 0; i < 8; i++)
	{
		switch (Gleise->GetStatus_Block(Gleis_Block[i],&LocName))		
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
	}

	for (int i = 0; i < 5; i++)
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
	pDC->SelectObject(theApp.Brush_White);
	for (int i = 0; i < 6; i++)
	{
		TextPos = Gleis_Platz[i].TopLeft() + CPoint(+10, +2);
		TextBes = Gleis_Platz[i].TopLeft() + CPoint(+5, -16);
		LokPos  = Gleis_Platz[i].TopLeft() + CPoint(+160, -28);

		GleisName.Format(_T(" Abstellgleis Nr.: %i"), i);
		byte BlockNr = Gleis_Block[i];
		if (Gleise->isPower_onBlock(BlockNr)) 
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
			theApp.Lok_ge_Uhr.DrawTransparent(pDC, LokPos.x, LokPos.y, RGB(255, 255, 255));
		}

	}
	CDialog::OnPaint();
}

void CDlg_Lok_Schuppen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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

	pDC->Rectangle(2, 45, 455, 350);

	pDC->SelectObject(StiftHaus);

	pDC->MoveTo(7, 50);
	pDC->LineTo(450, 50);
	pDC->LineTo(450, 200);
	pDC->LineTo(455, 200);
	pDC->LineTo(455, 45);
	pDC->LineTo(2, 45);
	pDC->LineTo(2, 350);
	pDC->LineTo(455, 350);
	pDC->LineTo(455, 320);
	pDC->LineTo(450, 320);
	pDC->LineTo(450, 345);
	pDC->LineTo(7, 345);
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
	case 0:
		Weiche_Nummer[0].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		break;
	case 1:
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		break;
	case 2:
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		break;
	case 3:
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(false);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		break;
	case 4:
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
	case 5:
		Weiche_Nummer[0].SetBit(true);
		Weiche_Nummer[1].SetBit(true);
		Weiche_Nummer[2].SetBit(true);
		Weiche_Nummer[3].SetBit(true);
		Weiche_Nummer[4].SetBit(true);
		Gleise->Set_Weiche(Weiche_Nummer[0]);
		Gleise->Set_Weiche(Weiche_Nummer[1]);
		Gleise->Set_Weiche(Weiche_Nummer[2]);
		Gleise->Set_Weiche(Weiche_Nummer[3]);
		Gleise->Set_Weiche(Weiche_Nummer[4]);
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
END_MESSAGE_MAP()

// CDlg_Lok_Schuppen-Meldungshandler

void CDlg_Lok_Schuppen::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (SchuppenTor.PtInRect(point))
	{
		bool SchuppenTor_auf = !Gleise->Get_Door_open();
		BlockNet->Send_Door_open(SchuppenTor_auf);
		Invalidate();
		return;
	}
	if (Gleise->Get_Door_open())
	{
		for (int i = 0; i < 6; i++)
		{
			Gleis_Block_On[i] = false;
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
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlg_Lok_Schuppen::Draw_LOK(CDC* pDC)

{
	CBrush *oldBrusch;
	CDC dcMemory;

	dcMemory.CreateCompatibleDC(pDC);

	dcMemory.SelectObject(&theApp.Lok_ge_Uhr);

	oldBrusch = pDC->SelectObject(&PinselHaus);

	pDC->BitBlt(100, 300, 54, 96, &dcMemory, 0, 0, SRCCOPY);

	pDC->SelectObject(oldBrusch);

}