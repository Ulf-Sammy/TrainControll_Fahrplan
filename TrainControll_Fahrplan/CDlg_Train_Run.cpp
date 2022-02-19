#include "pch.h"
#include "TrainControll_Fahrplan.h"
#include "TrainControll_FahrplanDlg.h"
#include "CDlg_Train_Run.h"



// CDlgTrainRun dialog

//IMPLEMENT_DYNAMIC(CDlgTrainRun, CDialog)
BEGIN_MESSAGE_MAP(CDlg_Run_Train, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0,   IDC_FUNK_16,   &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_1, IDC_FUNK_16_1, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_2, IDC_FUNK_16_2, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_3, IDC_FUNK_16_3, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_4, IDC_FUNK_16_4, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_5, IDC_FUNK_16_5, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_6, IDC_FUNK_16_6, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_7, IDC_FUNK_16_7, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FUNK_0_8, IDC_FUNK_16_8, &CDlg_Run_Train::OnBnClickedFunktion)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_VOR4, IDC_BUTTON_ZUR4, &CDlg_Run_Train::OnBnClickedSpeed)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CDlg_Run_Train::CDlg_Run_Train(CWnd* pParent, byte ID)
	: CDialog(CDlg_Run_Train::IDD, pParent)
{
	ASSERT(pParent != NULL);
	
	Lok_Park_Block = 0;
	Lok_Pfeil_vor  = 0;
	m_pParent= pParent;
	Dlg_Richtung = true;
	Dlg_Geschwindigkeit = 0;
	if (ID == 0)		m_nID = CDlg_Run_Train::IDD;
	if (ID == 1)		m_nID = CDlg_Run_Train::IDD1;
	if (ID == 2)		m_nID = CDlg_Run_Train::IDD2;
	if (ID == 3)		m_nID = CDlg_Run_Train::IDD3;
	if (ID == 4)		m_nID = CDlg_Run_Train::IDD4;
	if (ID == 5)		m_nID = CDlg_Run_Train::IDD5;
	if (ID == 6)		m_nID = CDlg_Run_Train::IDD6;
	if (ID == 7)		m_nID = CDlg_Run_Train::IDD7;
	if (ID == 8)		m_nID = CDlg_Run_Train::IDD8;
}
CDlg_Run_Train::~CDlg_Run_Train()
{
}

void CDlg_Run_Train::SetInfo(CString Text)
{
	if (m_nID == CDlg_Run_Train::IDD)  { SetDlgItemTextW(IDC_ZUG_ADRESSE, Text); }
	if (m_nID == CDlg_Run_Train::IDD1) { SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text); }
	if (m_nID == CDlg_Run_Train::IDD2) { SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text); }
	if (m_nID == CDlg_Run_Train::IDD3) { SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text); }
	if (m_nID == CDlg_Run_Train::IDD4) { SetDlgItemTextW(IDC_ZUG_ADRESSE_4, Text); }
	if (m_nID == CDlg_Run_Train::IDD5) { SetDlgItemTextW(IDC_ZUG_ADRESSE_5, Text); }
	if (m_nID == CDlg_Run_Train::IDD6) { SetDlgItemTextW(IDC_ZUG_ADRESSE_6, Text); }
	if (m_nID == CDlg_Run_Train::IDD7) { SetDlgItemTextW(IDC_ZUG_ADRESSE_7, Text); }
	if (m_nID == CDlg_Run_Train::IDD8) { SetDlgItemTextW(IDC_ZUG_ADRESSE_8, Text); }
}

BOOL CDlg_Run_Train::isCamera()
{
	return Lok_Name == L"grüne Stainz";
}

void CDlg_Run_Train::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDlg_Run_Train::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CDlg_Run_Train::Set_Daten(byte Block)
{
	CTrainControll_FahrplanDlg* APP;
	APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	CString Text;

	Zug_Data = &APP->meineLoks;
	Track_Data = &APP->Gleis_Data;
	Track_Data->GetStatus_Block(Block, &Lok_Name);
	Lok_Nr_act = Zug_Data->Get_Pos_LokName(Lok_Name); // ist die Nummer aus der aktive List von Zügen
	Lok_Bild = Zug_Data->Get_Lok_Image(Lok_Nr_act);

	Zug_Data->Verbinde_Zug_zu_XpNet(Lok_Nr_act);

	Zug_Data->Ask_for_Lok_Data(Lok_Nr_act);
	SetTasten();
	Text.Format(L"bediene Lok : %s", (LPCTSTR)Lok_Name);
	this->SetWindowTextW(Text);

}

BOOL CDlg_Run_Train::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_nID == CDlg_Run_Train::IDD)  { SetWindowPos(NULL,  10, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD1) { SetWindowPos(NULL, 300, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD2) { SetWindowPos(NULL, 490, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD3) { SetWindowPos(NULL, 500, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD4) { SetWindowPos(NULL, 600, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD5) { SetWindowPos(NULL, 700, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD6) { SetWindowPos(NULL, 800, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD7) { SetWindowPos(NULL, 900, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); }
	if (m_nID == CDlg_Run_Train::IDD8)
	{
		SetWindowPos(NULL, 490, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); 
		Lok_Bild = NULL;
		if (LokFenster.SucheKamera(_T("USB2.0 Grabber")))   //"Integrated Webcam")))
		{
			LokFenster.StarteKamera();
			LokFenster.Set_Bild_inDlg(*GetDlgItem(IDC_FENSTER_8));
			LokFenster.StartBild(true);
		}
	}
	else
	{
		Lok_Bild = NULL;
		Lok_Tacho     = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAPTACHO));
		Lok_Pfeil_vor = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PFEIL_L));
		Lok_Pfeil_zur = (HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PFEIL_R));
	}

	return TRUE;
}

void CDlg_Run_Train::OnCancel()
{
	((CTrainControll_FahrplanDlg*)m_pParent)->DlgTrainRunDone(this);
}

void CDlg_Run_Train::SetTasten(void)
{
	//	m_pParent->ShowWindow();
	bool bit;
	CString Text;
	int Item_Nr = 0;
	int i;
	static_cast<CButton*>(GetDlgItem(IDC_BUTTON_STOP))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_STOP)));

	if (m_nID == CDlg_Run_Train::IDD)  { Item_Nr = IDC_FUNK_0;  }
	if (m_nID == CDlg_Run_Train::IDD1) { Item_Nr = IDC_FUNK_0_1; }
	if (m_nID == CDlg_Run_Train::IDD2) { Item_Nr = IDC_FUNK_0_2; }
	if (m_nID == CDlg_Run_Train::IDD3) { Item_Nr = IDC_FUNK_0_3; }
	if (m_nID == CDlg_Run_Train::IDD4) { Item_Nr = IDC_FUNK_0_4; }
	if (m_nID == CDlg_Run_Train::IDD5) { Item_Nr = IDC_FUNK_0_5; }
	if (m_nID == CDlg_Run_Train::IDD6) { Item_Nr = IDC_FUNK_0_6; }
	if (m_nID == CDlg_Run_Train::IDD7) { Item_Nr = IDC_FUNK_0_7; }
	if (m_nID == CDlg_Run_Train::IDD8) { Item_Nr = IDC_FUNK_0_8; }
	if (Zug_Data->is_Lok_Sound(Lok_Nr_act))
	{
		bit = Zug_Data->Get_Lok_FunktionSound(Lok_Nr_act);
		static_cast<CButton*>(GetDlgItem(Item_Nr + 14))->ShowWindow(SW_SHOW);
		SetButtom(bit, (Item_Nr + 14), IDB_F14_I, IDB_F14_O);
	}
	else
	{
		static_cast<CButton*>(GetDlgItem(Item_Nr + 14))->ShowWindow(SW_HIDE);
	}
	bit = Zug_Data->Get_Lok_FunktionRangier(Lok_Nr_act);
	SetButtom(bit, (Item_Nr + 15), IDB_F15_I, IDB_F15_O);

	bit = Zug_Data->Get_Lok_FunktionZeit(Lok_Nr_act);
	SetButtom(bit, (Item_Nr + 16), IDB_F16_I, IDB_F16_O);

	i = 0;
	for(Function_A Funktion : Zug_Data->Get_Zug_ActivFunktion_Pointer(Lok_Nr_act))
	{
		// Hole Funktions Bit und Name vom der aktiven Lok
		if (i < (MAX_MASSOTH_FUNKTION - 3))
		{
			bit = Zug_Data->Get_Lok_FunktionBit(Lok_Nr_act,Funktion.Get_Taste());
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
	if (Zug_Data->Get_Lok_Status(Lok_Nr_act) == Zug_Status::Zug_Stopped) Text.Format(_T(" Zug stopped "));
	if (m_nID == CDlg_Run_Train::IDD)  { SetDlgItemTextW(IDC_ZUG_ADRESSE ,  Text);}
	if (m_nID == CDlg_Run_Train::IDD1) { SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text);}
	if (m_nID == CDlg_Run_Train::IDD2) { SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text);}
	if (m_nID == CDlg_Run_Train::IDD3) { SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text);}
	if (m_nID == CDlg_Run_Train::IDD4) { SetDlgItemTextW(IDC_ZUG_ADRESSE_4, Text);}
	if (m_nID == CDlg_Run_Train::IDD5) { SetDlgItemTextW(IDC_ZUG_ADRESSE_5, Text);}
	if (m_nID == CDlg_Run_Train::IDD6) { SetDlgItemTextW(IDC_ZUG_ADRESSE_6, Text);}
	if (m_nID == CDlg_Run_Train::IDD7) { SetDlgItemTextW(IDC_ZUG_ADRESSE_7, Text); }
	if (m_nID == CDlg_Run_Train::IDD8) { SetDlgItemTextW(IDC_ZUG_ADRESSE_8, Text); }
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
	if (m_nID == CDlg_Run_Train::IDD)  { index = nID - IDC_FUNK_0;	 }
	if (m_nID == CDlg_Run_Train::IDD1) { index = nID - IDC_FUNK_0_1; }
	if (m_nID == CDlg_Run_Train::IDD2) { index = nID - IDC_FUNK_0_2; }
	if (m_nID == CDlg_Run_Train::IDD3) { index = nID - IDC_FUNK_0_3; }
	if (m_nID == CDlg_Run_Train::IDD4) { index = nID - IDC_FUNK_0_4; }
	if (m_nID == CDlg_Run_Train::IDD5) { index = nID - IDC_FUNK_0_5; }
	if (m_nID == CDlg_Run_Train::IDD6) { index = nID - IDC_FUNK_0_6; }
	if (m_nID == CDlg_Run_Train::IDD7) { index = nID - IDC_FUNK_0_7; }
	if (m_nID == CDlg_Run_Train::IDD8) { index = nID - IDC_FUNK_0_8; }
	switch (index)
	{
	case 14:
		bit = !Zug_Data->Get_Lok_FunktionSound(Lok_Nr_act);
		Zug_Data->Set_Lok_FunktionSound(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F14_I, IDB_F14_O);
		break;
	case 15:
		bit = !Zug_Data->Get_Lok_FunktionRangier(Lok_Nr_act);
		Zug_Data->Set_Lok_FunktionRangier(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F15_I, IDB_F15_O);
		break;
	case 16:
		bit = !Zug_Data->Get_Lok_FunktionZeit(Lok_Nr_act);
		Zug_Data->Set_Lok_FunktionZeit(Lok_Nr_act, bit);
		SetButtom(bit, (nID), IDB_F16_I, IDB_F16_O);
		break;
	default:
		FunkNr = Zug_Data->Get_Zug_ActivFunktion_Pointer(Lok_Nr_act)[index].Get_Taste();
		bit = !Zug_Data->Get_Lok_FunktionBit(Lok_Nr_act, FunkNr);
		Zug_Data->Set_Funktion(Lok_Nr_act, FahrplanPos(0, FahrPlanDo::schalten_Funk, FunkNr, bit));

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

	switch (Zug_Data->Get_Lok_Status(Lok_Nr_act))
	{ 
	case Zug_Status::Zug_Stopped:
		Zug_Data->Set_Lok_Startbedingung(Lok_Nr_act, NeuerZug_Status); 
		[[fallthrough]];
	case Zug_Status::Zug_haelt:
		PrZug = BlockRueckmeldung::Frei_Fahrt;
		switch (PrZug)
		{
		case BlockRueckmeldung::Frei_Fahrt:
			Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
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
				Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
				Dlg_Geschwindigkeit = 0;
			}
			break;
		case BlockRueckmeldung::Block_besetzt:
			Text.Format(_T("Der Block ist besetzt !"));
			if (NeuerZug_Status == Zug_Status::Zug_Stopped)
			{
				Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
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
			Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug fährt vorwärtz mit : %2i"), G);
			Dlg_Geschwindigkeit = G;
			Dlg_Richtung = true;
		}
		if (NeuerZug_Status == Zug_Status::Zug_Stopped)
		{
			Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug hat gestopped !         "));
			Dlg_Geschwindigkeit = 0;
		}
		break;
	case Zug_Status::Zug_faehrt_rueck:
		if (NeuerZug_Status == Zug_Status::Zug_faehrt_rueck)
		{
			Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug fährt rückwärtz mit : %2i"), G);
			Dlg_Geschwindigkeit = G;
			Dlg_Richtung = false;
		}
		if (NeuerZug_Status == Zug_Status::Zug_Stopped)
		{
			Track_Data->Set_Lok_Geschwindigkeit(Lok_Nr_act, NeuerZug_Status, G);
			Text.Format(_T("Zug hat gestopped !         "));
			Dlg_Geschwindigkeit = 0;
		}
		break;
	}
	if (m_nID == CDlg_Run_Train::IDD)  { SetDlgItemTextW(IDC_ZUG_ADRESSE, Text);   }
	if (m_nID == CDlg_Run_Train::IDD1) { SetDlgItemTextW(IDC_ZUG_ADRESSE_1, Text); }
	if (m_nID == CDlg_Run_Train::IDD2) { SetDlgItemTextW(IDC_ZUG_ADRESSE_2, Text); }
	if (m_nID == CDlg_Run_Train::IDD3) { SetDlgItemTextW(IDC_ZUG_ADRESSE_3, Text); }
	if (m_nID == CDlg_Run_Train::IDD4) { SetDlgItemTextW(IDC_ZUG_ADRESSE_4, Text); }
	if (m_nID == CDlg_Run_Train::IDD5) { SetDlgItemTextW(IDC_ZUG_ADRESSE_5, Text); }
	if (m_nID == CDlg_Run_Train::IDD6) { SetDlgItemTextW(IDC_ZUG_ADRESSE_6, Text); }
	if (m_nID == CDlg_Run_Train::IDD7) { SetDlgItemTextW(IDC_ZUG_ADRESSE_7, Text); }
	if (m_nID == CDlg_Run_Train::IDD8) { SetDlgItemTextW(IDC_ZUG_ADRESSE_8, Text); }
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
