#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "DlgTrainRun.h"



// CDlgTrainRun dialog

//IMPLEMENT_DYNAMIC(CDlgTrainRun, CDialog)
BEGIN_MESSAGE_MAP(CDlg_Run_Train, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0, IDC_FUNK_16, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_1, IDC_FUNK_16_1, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_2, IDC_FUNK_16_2, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_3, IDC_FUNK_16_3, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_VOR4, IDC_BUTTON_ZUR4, &CDlg_Run_Train::OnBnClickedSpeed)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CDlg_Run_Train::CDlg_Run_Train(CWnd* pParent, byte ID)
	: CDialog(CDlg_Run_Train::IDD, pParent)
{
	ASSERT(pParent != NULL);

	m_pParent= pParent;
	Dlg_Richtung = true;
	Dlg_Geschwindigkeit = 0;
	if (ID == 0)		m_nID = CDlg_Run_Train::IDD;
	if (ID == 1)		m_nID = CDlg_Run_Train::IDD1;
	if (ID == 2)		m_nID = CDlg_Run_Train::IDD2;
	if (ID == 3)		m_nID = CDlg_Run_Train::IDD3;
}
CDlg_Run_Train::~CDlg_Run_Train()
{
}

void  CDlg_Run_Train::Set_aktive_Lok(byte Nr)
{
	CTrainControll_FahrplanDlg* P;
	P = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	Zug_Data = &P->meineLoks;
	Train_Data = &P->Gleis_Data;
	Lok_Nr_act = Nr; // ist die Nummer aus der aktive List von Zügen
}

void CDlg_Run_Train::SetInfo(CString Text)
{
	if (m_nID == CDlg_Run_Train::IDD)  { SetDlgItemTextW(IDC_ZUG_ADRESSE, Text); }
	if (m_nID == CDlg_Run_Train::IDD1) { SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text); }
	if (m_nID == CDlg_Run_Train::IDD2) { SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text); }
	if (m_nID == CDlg_Run_Train::IDD3) { SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text); }
}

void CDlg_Run_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDlg_Run_Train::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

BOOL CDlg_Run_Train::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	
	if (m_nID == CDlg_Run_Train::IDD)  { SetWindowPos(NULL,  10, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD1) { SetWindowPos(NULL, 300, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD2) { SetWindowPos(NULL, 490, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD3) 
	{
		SetWindowPos(NULL, 490, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); 
		Lok_Bild = NULL;
		if (LokFenster.SucheKamera(_T("USB2.0 Grabber")))   //"Integrated Webcam")))
		{
			LokFenster.StarteKamera();
			LokFenster.Set_Bild_inDlg(*GetDlgItem(IDC_FENSTER_3));
			LokFenster.StartBild(true);
		}
	}
	else
	{
		Lok_Bild = Zug_Data->Get_aktiveLok_Image(Lok_Nr_act);
		Lok_Tacho = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAPTACHO));
		Lok_Pfeil_vor = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PFEIL_L));
		Lok_Pfeil_zur = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PFEIL_R));
	}
	Zug_Data->Ask_aktivLok_Data(Lok_Nr_act);
	SetTasten();
	return TRUE;
}

void CDlg_Run_Train::OnCancel()
{
	
	((CTrainControll_FahrplanDlg*)m_pParent)->DlgTrainRunDone(m_nID);
}

void CDlg_Run_Train::SetTasten(void)
{
	bool bit;
	CString Text;
	int Item_Nr;
	int i;
	
	if (m_nID == CDlg_Run_Train::IDD)	{	Item_Nr = IDC_FUNK_0;   }
	if (m_nID == CDlg_Run_Train::IDD1)	{	Item_Nr = IDC_FUNK_0_1;	}
	if (m_nID == CDlg_Run_Train::IDD2)	{	Item_Nr = IDC_FUNK_0_2; }
	if (m_nID == CDlg_Run_Train::IDD3)  {   Item_Nr = IDC_FUNK_0_3; }
	if (Zug_Data->is_aktiveLok_Sound(Lok_Nr_act))
	{
		bit = Zug_Data->Get_aktiveLok_FunktionSound(Lok_Nr_act);
		static_cast<CButton*>(GetDlgItem(Item_Nr + 14))->ShowWindow(SW_SHOW);
		SetButtom(bit, (Item_Nr + 14), IDB_F14_I, IDB_F14_O);
	}
	else
	{
		static_cast<CButton*>(GetDlgItem(Item_Nr + 14))->ShowWindow(SW_HIDE);
	}
	bit = Zug_Data->Get_aktiveLok_FunktionRangier(Lok_Nr_act);
	SetButtom(bit, (Item_Nr + 15), IDB_F15_I, IDB_F15_O);

	bit = Zug_Data->Get_aktiveLok_FunktionZeit(Lok_Nr_act);
	SetButtom(bit, (Item_Nr + 16), IDB_F16_I, IDB_F16_O);

	i = 0;
	for(Function_A Funktion : Zug_Data->Get_Zug_ActivFunktion_Pointer(Lok_Nr_act))
	{
		// Hole Funktions Bit und Name vom der aktiven Lok
		if (i < (MAX_MASSOTH_FUNKTION - 3))
		{
			bit = Zug_Data->Get_aktiveLok_FunktionBit(Lok_Nr_act,Funktion.Get_Taste());
			Text = Funktion.Text_Taste() + " |  " + Funktion.Text_Beschreibung();
			static_cast<CButton*>(GetDlgItem(Item_Nr + i))->ShowWindow(SW_SHOW);
			static_cast<CButton*>(GetDlgItem(Item_Nr + i))->SetCheck(bit);
			static_cast<CButton*>(GetDlgItem(Item_Nr + i))->SetWindowTextW(Text);
		}
		i++;
	}
	for ( ; i < (MAX_MASSOTH_FUNKTION - 3 ); i++)
	{
		static_cast<CButton*>(GetDlgItem(Item_Nr + i))->ShowWindow(SW_HIDE);
	}
	if (Zug_Data->Get_aktiveLok_Pointer(Lok_Nr_act).Get_Status() == Zug_Status::Zug_Stopped) Text.Format(_T(" Zug stopped "));
	if (m_nID == CDlg_Run_Train::IDD) { SetDlgItemTextW(IDC_ZUG_ADRESSE, Text); }
	if (m_nID == CDlg_Run_Train::IDD1){	SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text);}
	if (m_nID == CDlg_Run_Train::IDD2){ SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text); }
	if (m_nID == CDlg_Run_Train::IDD3){ SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text); }
}

void CDlg_Run_Train::SetButtom(bool Bit, int Button_ID, int ImgON_ID, int ImgOFF_ID)
{
	static_cast<CButton*>(GetDlgItem(Button_ID))->SetCheck(Bit);
	if ( Bit )
	{
		static_cast<CButton*>(GetDlgItem(Button_ID))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(ImgON_ID)));
	}
	else
	{
		static_cast<CButton*>(GetDlgItem(Button_ID))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(ImgOFF_ID)));
	}
}

void CDlg_Run_Train::OnBnClickedFunktion(UINT nID)
{
	byte bit;
	byte FunkNr;
	int index = 0;
	UpdateData(true);

	index = 0;
	if (m_nID == CDlg_Run_Train::IDD)	{ index = nID - IDC_FUNK_0;	  }
	if (m_nID == CDlg_Run_Train::IDD1)	{ index = nID - IDC_FUNK_0_1; }
	if (m_nID == CDlg_Run_Train::IDD2)	{ index = nID - IDC_FUNK_0_2; }
	if (m_nID == CDlg_Run_Train::IDD3)  { index = nID - IDC_FUNK_0_3; }
	switch (index)	
	{
	case 14:
		bit = !Zug_Data->Get_aktiveLok_FunktionSound(Lok_Nr_act);
		Zug_Data->Set_aktiveLok_FunktionSound(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F14_I, IDB_F14_O);
		break;
	case 15:
		bit = !Zug_Data->Get_aktiveLok_FunktionRangier(Lok_Nr_act);
		Zug_Data->Set_aktiveLok_FunktionRangier(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F15_I, IDB_F15_O);
		break;
	case 16:
		bit = !Zug_Data->Get_aktiveLok_FunktionZeit(Lok_Nr_act);
		Zug_Data->Set_aktiveLok_FunktionZeit(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F16_I, IDB_F16_O);
		break;
	default:
		FunkNr = Zug_Data->Get_Zug_ActivFunktion_Pointer(Lok_Nr_act)[index].Get_Taste();
		bit = !Zug_Data->Get_aktiveLok_FunktionBit(Lok_Nr_act, FunkNr);
		Zug_Data->Get_aktiveLok_Pointer(Lok_Nr_act).Set_Funktion(FahrplanPos(0, FahrPlanDo::schalten_Funk, FunkNr, bit));

		break;
	}

	static_cast<CButton*>(GetDlgItem(nID))->SetCheck(bit); 
}

void CDlg_Run_Train::OnBnClickedSpeed(UINT nID)
{
	#define Speed_N 8;
	#define Speed_M 12;
	#define Speed_F 18;
	#define Speed_T 21;
	static byte G;
	Zug_Status NeuerZug_Status;
	BlockRueckmeldung PrZug;
	CString Text;

	switch (nID)
	{
	case IDC_BUTTON_VOR4:
		NeuerZug_Status = Zug_Status::Zug_faehrt_vor;
		G = Speed_T;
		break;
	case IDC_BUTTON_VOR3:
		NeuerZug_Status = Zug_Status::Zug_faehrt_vor;
		G = Speed_F;
		break;
	case IDC_BUTTON_VOR2:
		NeuerZug_Status = Zug_Status::Zug_faehrt_vor;
		G = Speed_M;
		break;
	case IDC_BUTTON_VOR1:
		NeuerZug_Status = Zug_Status::Zug_faehrt_vor;
		G = Speed_N;
		break;
	case IDC_BUTTON_STOP:
		NeuerZug_Status = Zug_Status::Zug_Stopped;
		G = 0;
		break;
	case IDC_BUTTON_ZUR1:
		NeuerZug_Status = Zug_Status::Zug_faehrt_rueck;
		G = Speed_N;
		break;
	case IDC_BUTTON_ZUR2:
		NeuerZug_Status = Zug_Status::Zug_faehrt_rueck;
		G = Speed_M;
		break;
	case IDC_BUTTON_ZUR3:
		NeuerZug_Status = Zug_Status::Zug_faehrt_rueck;
		G = Speed_F;
		break;
	case IDC_BUTTON_ZUR4:
		NeuerZug_Status = Zug_Status::Zug_faehrt_rueck;
		G = Speed_T;
		break;

	}

	switch (Zug_Data->Get_aktiveLok_Status(Lok_Nr_act))
	{ 
	case Zug_Status::Zug_Stopped:
		Zug_Data->Set_aktiveLok_Startbedingung(Lok_Nr_act, NeuerZug_Status); 
	case Zug_Status::Zug_haelt:
		PrZug = BlockRueckmeldung::Frei_Fahrt;
		switch (PrZug)
		{
		case BlockRueckmeldung::Frei_Fahrt:
			Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			if (NeuerZug_Status == Zug_Status::Zug_faehrt_vor)
			{
				Text.Format(_T("Zug fährt vorwärtz mit : %2i"), G);
				Dlg_Richtung = true;
			}
			if (NeuerZug_Status == Zug_Status::Zug_faehrt_rueck)
			{
				Text.Format(_T("Zug fährt rückwärtz mit : %2i"), G);
				Dlg_Richtung = false;
			}
			Dlg_Geschwindigkeit = G;
			break;
		case BlockRueckmeldung::Weichenweg_nichtfrei:
			Text.Format(_T("Der Weichenweg ist nicht frei !"));
			if (NeuerZug_Status == Zug_Status::Zug_Stopped)
			{
				Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
				Dlg_Geschwindigkeit = 0;
			}
			break;
		case BlockRueckmeldung::Block_besetzt:
			Text.Format(_T("Der Block ist besetzt !"));
			if (NeuerZug_Status == Zug_Status::Zug_Stopped)
			{
				Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
				Dlg_Geschwindigkeit = 0;
			}
			break;
		default:
			break;
		}
		break;
	case Zug_Status::Zug_faehrt_vor:
		if (NeuerZug_Status == Zug_Status::Zug_faehrt_vor)
		{
			Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug fährt vorwärtz mit : %2i"), G);
			Dlg_Geschwindigkeit = G;
			Dlg_Richtung = true;
		}
		if (NeuerZug_Status == Zug_Status::Zug_Stopped)
		{
			Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug hat gestopped !         "));
			Dlg_Geschwindigkeit = 0;
		}
		break;
	case Zug_Status::Zug_faehrt_rueck:
		if (NeuerZug_Status == Zug_Status::Zug_faehrt_rueck)
		{
			Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug fährt rückwärtz mit : %2i"), G);
			Dlg_Geschwindigkeit = G;
			Dlg_Richtung = false;
		}
		if (NeuerZug_Status == Zug_Status::Zug_Stopped)
		{
			Train_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug hat gestopped !         "));
			Dlg_Geschwindigkeit = 0;
		}
		break;
	}
	if (m_nID == CDlg_Run_Train::IDD) { SetDlgItemTextW(IDC_ZUG_ADRESSE, Text); }
	if (m_nID == CDlg_Run_Train::IDD1){	SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text);}
	if (m_nID == CDlg_Run_Train::IDD2){ SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text); }
	if (m_nID == CDlg_Run_Train::IDD3){ SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text); }
	Invalidate();
}




void CDlg_Run_Train::OnPaint()
{
	CPoint Zeiger[29] = { {	218 ,	94 },{	213 ,	88 },{	209 ,	82 },{	207 ,	74 },{	205 ,	67 },{	205 ,	59 },{	206 ,	52 },{	208 ,	45 },{	212 ,	38 },{	216 ,	32 },{	222 ,	27 },{	228 ,	23 },{	235 ,	20 },{	242 ,	18 },{	250 ,	17 },
                          {	258 ,	18 },{	265 ,	20 },{	272 ,	23 },{	278 ,	27 },{	284 ,	32 },{	288 ,	38 },{	292 ,	45 },{	294 ,	52 },{	295 ,	59 },{	295 ,	67 },{	293 ,	74 },{	291 ,	82 },{	287 ,	88 },{	282 ,	94 } };
	const CPoint ZeigerSt = { 250,62 };
	const LPCRECT ZeigerKr = CRect(240,52,260,72);
	
	CPaintDC dc(this); 
	CDC memDC;
	CPen Pen_Ro;
	Pen_Ro.CreatePen(PS_SOLID, 3, RGB(225, 24, 32));
	CBrush Bru_Gr;
	Bru_Gr.CreateSolidBrush(RGB(153, 153, 153));
	
	if (Lok_Bild != NULL)
	{

		memDC.CreateCompatibleDC(&dc);
		// Bitmap auswählen und vorherige Auswahl merken
		CBitmap *pOldBitmap = memDC.SelectObject(CBitmap::FromHandle(Lok_Bild));
		dc.BitBlt(8, 4, 225, 150, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(CBitmap::FromHandle(Lok_Tacho));
		dc.BitBlt(191,4, 225, 150, &memDC, 0, 0, SRCCOPY);
		if (Dlg_Richtung) memDC.SelectObject(CBitmap::FromHandle(Lok_Pfeil_vor));
		else              memDC.SelectObject(CBitmap::FromHandle(Lok_Pfeil_zur));
		dc.BitBlt(232, 90, 38, 22, &memDC, 0, 0, SRCCOPY);
		dc.SelectObject(&Bru_Gr);
		dc.Ellipse(ZeigerKr);

		dc.SelectObject(&Pen_Ro);
		dc.MoveTo(ZeigerSt);
		dc.LineTo(Zeiger[Dlg_Geschwindigkeit]);
		memDC.SelectObject(pOldBitmap);
	}
}
