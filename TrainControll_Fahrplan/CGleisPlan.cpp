#include "pch.h"
#include "CGleisPlan.h"
#include "TrainControll_FahrplanDlg.h"



CGleisPlan::CGleisPlan()
{
	Tor.Tor_frei = false;
	Tor.Tor_offen = false;

}
CGleisPlan::~CGleisPlan()
{
}

void CGleisPlan::Init()
{
	for (int i = 0; i <= MAX_WEICHEN_MOTOR; i++)
	{
		WeichenAntrieb.push_back(TrainCon_Paar(i, false));
	}
	Lade_Daten();
	Set_Taster_Farbe();
}

//void CGleisPlan::DoCheckIt(byte Lok_Nr, clock_t Zeit)
//{
//	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Plan_Warten)
//	{
//		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).WarteZeit < Zeit)
//		{
//		}
//	}
//	if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt) && (Züge->Get_aktiveLok_Pointer(Lok_Nr).Betriebs_Modus == Zug_Steuerung::Hand_Betrieb))
//	{ // Dies nur für den Handbetreib 
//	}
//	if ((Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Status() == Zug_Status::Zug_haelt) && (Züge->Get_aktiveLok_Pointer(Lok_Nr).Betriebs_Modus == Zug_Steuerung::Automatik_Betrieb))
//	{ // Dies nur für den Automaticbetreib 
//	}
//}

void CGleisPlan::showInfo_Dlg(byte LokNr, CString Info)
{
}

void CGleisPlan::showPlan_Dlg(byte LokNr, CString Info)
{
}

bool CGleisPlan::isNewUpdate_Taster()
{
	return false;
}

bool CGleisPlan::isPower_onGleis()
{
	return true; // (XpressNet->Get_Power_onGleis());
}

bool CGleisPlan::isPower_onBlock(byte Nr)
{
	return Block[Nr].Get_Relais_Data().GetBit();
}


void CGleisPlan::ZeicheStrecke(CDC* pDC)
	
{
	for (auto&  B : Block)
	{
		B.ZeicheBlock(pDC, &WeichenAntrieb);
	}
}

void CGleisPlan::ZeicheStreckenInfo(CDC* pDC)
{
	int Ib = 0;
	for (auto& B : Block)
	{
		B.ZeicheBlockInfo(pDC, Ib);
		Ib++;
	}
}

void CGleisPlan::ZeicheTaster(CDC* pDC)
{
	CBrush* pOldBrush = NULL;
	CPen * pOldPen = NULL;
	pOldPen = pDC->SelectObject(&theApp.Taster_Rand);
	pOldBrush = pDC-> SelectObject(&theApp.Farbe_Gelb_FL);
	byte i = 0;
	for (auto& B : Block)
	{
		B.ZeicheTaster(pDC);
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

}


bool CGleisPlan::Klick_Block(CPoint Klick)
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	TrainCon_Paar Data;
	byte Wert;
	byte Ergebnis;

	for (auto& B : Block)
	{
		Ergebnis = B.OnKlick(Klick, &Wert);
		if ((Ergebnis > 2)&&(Ergebnis < 5))
		{ 
			static bool firstKlick = true;
			static CBlock* vonBlock;
			static 	std::vector<byte> Blockliste;
		

			byte Nr = (byte)(&B - &Block[0]);
			if (firstKlick)
			{
				firstKlick = false;
				vonBlock = &B;
				if (Ergebnis == 3 ) // EingangsTaster
				{
					B.Get_Ein_nextBlock(&WeichenAntrieb,&Blockliste);
					Block[Nr].Set_EinTasterFarbe('O');
					for (auto& nb : Blockliste) { Block[nb].Set_AusTasterFarbe('Y'); }
				}
				if (Ergebnis == 4) // AusgangsTaster
				{
					B.Get_Aus_nextBlock(&WeichenAntrieb, &Blockliste);
					Block[Nr].Set_AusTasterFarbe('O');
					for (auto& nb : Blockliste) {Block[nb].Set_EinTasterFarbe('Y');	}
				}				
			}
			else
			{
				for (auto& TasterL : Blockliste)
				{
					if (TasterL == Nr)
					{
						std::vector<TrainCon_Paar>* Der_Weg = vonBlock->Get_zuschaltener_Weg(Nr);;
						//Der_Weg = vonBlock->Get_zuschaltener_Weg(Nr);
						if (Der_Weg != NULL)
						{
							for (auto& Antrieb : *Der_Weg) { Set_Weiche(Antrieb); }
						}
					}
				}
				firstKlick = true;
				Set_Taster_Farbe();
				Blockliste.clear();
			}
 			APP->InfoGleisBild.Invalidate();
			//AfxMessageBox(MsgText);
			return (false);
		}
		/// ###################################

		if (Ergebnis == 1)
		{
			Data = WeichenAntrieb[Wert].GetInvBit();
			//if (BlockMelder->NoComToBlockNet())
			//{
			//	Set_Weiche(Data);
			//	TRACE1(" Weiche Nr: %i \n",Data.GetWert());
			//	Set_Taster_Farbe();
			//	return(true);
			//}
			//else
			//	BlockMelder->Send_WeichenData(Data);
			Set_Taster_Farbe();
		    return false;
		}
		if (Ergebnis == 2)
		{
			Data = B.Get_Relais_Data();
			//if (BlockMelder->NoComToBlockNet())
			//	Set_Relais(Data);
			//else
			//	BlockMelder->Send_BlockPower(Data);
			//return false;
		}
	}
	return (false);
}

void CGleisPlan::Set_Taster_Farbe()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	std::vector<byte> Blockliste;
	byte nBlock;

	for (auto& B : Block)
	{
		B.Set_EinTasterFarbe('R');
		B.Set_AusTasterFarbe('R');
	}

	for (auto& B : Block)
	{
		nBlock = B.Get_Aus_nextBlock(&WeichenAntrieb, &Blockliste);
		if (nBlock != 0) 	B.Set_AusTasterFarbe('G');
		nBlock = B.Get_Ein_nextBlock(&WeichenAntrieb, &Blockliste);
		if (nBlock != 0) 	B.Set_EinTasterFarbe('G');
	}
	APP->InfoGleisBild.Invalidate();
}

bool CGleisPlan::Set_Block(byte* Data)
{
	if (Data[1] < Block.size())
	{
		byte Block_Nr = Data[1];
		bool Bit = (bool)Data[2];
		Block[Block_Nr].bestetzen(Bit);
		return true;
	}
	return false;
}

void CGleisPlan::Set_Block_free(byte BlockNr)
{
	Block[BlockNr].freimachen();
}

void CGleisPlan::Set_Block_Lok(byte BlockNr, CDataXpressNet *Lok)
{
	Block[BlockNr].bestetzen(Lok);
}



bool CGleisPlan::Set_Weiche(byte* Data)
{
	return Set_Weiche(TrainCon_Paar(Data));
}

bool CGleisPlan::Set_Weiche(TrainCon_Paar WeichenDaten)
{
	if (WeichenDaten.GetWert() < MAX_WEICHEN_MOTOR)
	{
		WeichenAntrieb[WeichenDaten.GetWert()].SetBit(WeichenDaten.GetBit());
		return true;
	}
	return false;
}

bool CGleisPlan::Get_Weiche(TrainCon_Paar WeichenDaten)
{
	return WeichenAntrieb[WeichenDaten.GetWert()].GetBit();
}

byte CGleisPlan::Get_Weichen_Anzahl()
{
	return (byte) WeichenAntrieb.size();
}

bool CGleisPlan::Set_Relais(byte* Data) 
{//
	return Set_Relais(TrainCon_Paar(Data));
}

bool CGleisPlan::Set_Relais(TrainCon_Paar RelayData)
{
	for (auto& B : Block)
	{
		if (Block[&B - &Block[0]].set_Relais(RelayData))
		{
			return true;
		}
	}
	return false;
}

void CGleisPlan::Set_Door( byte* Data)
{
		Tor.Tor_offen = (bool)Data[1];
		Tor.Tor_frei = Data[2];
}

void CGleisPlan::Ask_Door_Status()
{
	//BlockMelder->Send_Door_Status();
}


bool CGleisPlan::Get_Door_open()
{
	return Tor.Tor_offen;
}

byte CGleisPlan::Get_Door_free()
{
	return Tor.Tor_frei;
}

void CGleisPlan::Schalte_Relais(byte Nr, bool Bit)
{
	if (Nr > 0)
	{
		TrainCon_Paar Relais;
		Relais = Block[Nr].Get_Relais_Data();
		Relais.SetBit(Bit);
		//BlockMelder->Send_BlockPower(Relais);
		//if (BlockMelder->NoComToBlockNet())
		//{
		//	Block[Nr].set_Relais(Relais);
		//}
	}
}

Start_Lok_Block CGleisPlan::Get_StartLokInfo(byte Nr)
{
	return Block[Nr].Get_StartLokInfo();
}

void CGleisPlan::GetAnschlussBlocks(byte Nr, std::vector<byte>* EinBlocks, std::vector<byte>* AusBlocks)
{
	Block[Nr].GetAnschlussBlocks(EinBlocks,AusBlocks);
}

bool CGleisPlan::Weg_Von_bis_frei(byte Nr, byte BlockTo, std::vector<byte>*WegeBlocks)
{
	return Block[Nr].Weg_zuBlock(true, &WeichenAntrieb, WegeBlocks);
}

bool CGleisPlan::Weg_nach_von_Block(byte BlockNr, bool Richtung, std::vector<byte>* WegeBlocks)
{
	if (!WegeBlocks->empty())
	{
		WegeBlocks->clear();
	}
	return Block[BlockNr].Weg_zuBlock(Richtung, &WeichenAntrieb, WegeBlocks);
}

bool CGleisPlan::isWeg_frei(std::vector<byte> WegeBlocks)
{
	bool Result = true;
	for (auto Nr : WegeBlocks)
	{
		Result = Result && Block[Nr].ist_frei();
	}
	return Result;
}

bool CGleisPlan::Besetze_Weg_mit_Lok(CDataXpressNet* Lok, std::vector<byte> WegeBlocks)
{
	for (auto Nr : WegeBlocks)
	{
		Block[Nr].bestetzen(Lok);
	}
	return false;
}

bool CGleisPlan::Hole_Lok_Blick_vonGleis(byte Nr)
{
	return Block[Nr].Get_Lok_onBlock()->Blick;
}

CString CGleisPlan::Get_LokName_in_Block(byte Nr)
{
	return Block[Nr].GetLok_NameonBlock();
}

BlockStatus CGleisPlan::GetStatus_Block(byte Nr, CString* Lok_Name)
{
	bool Richtung;
	return Block[Nr].GetStatus_Block(Lok_Name,&Richtung);
}

BlockStatus CGleisPlan::GetStatus_Block(byte Nr, CString* Lok_Name, bool* Richtung)
{
	return Block[Nr].GetStatus_Block(Lok_Name, Richtung);
}

BlockDebugData CGleisPlan::Get_DebugData(byte Nr)
{
	if (Block.size() == 0) return BlockDebugData();
	if (Block.size() > Nr)
	{
		return Block[Nr].Get_Debug();
	}
	return Block[0].Get_Debug();;
}


CDataXpressNet* CGleisPlan::Get_Zug_Point(byte BlockNr)
{
	return Block[BlockNr].Get_Lok_onBlock();
}

void CGleisPlan::Lade_Daten()
{
	LPCTSTR			pszPathName = _T(FILE_ANLAGE);
	CStdioFile		file;
	CFileException	ex;
	CString			strIn, Text;
	CPoint			Step;
	int				Nr;
	std::vector<CBlock_Weg> alle_Wege;
	const int Rect_X = 1600;
	const int Rect_Y = 660;

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	file.ReadString(strIn); // Titel
	////// Allgemeine Daten lesen
	for (int I = 0; I < 4; I++)
	{
		file.ReadString(strIn); // Bildgröße
		if (strIn.Mid(0, 4) == "BILD")
		{
			Step.x = Rect_X / _ttoi(strIn.Mid(6, 3));
			Step.y = Rect_Y / _ttoi(strIn.Mid(10, 3));
		}
		if (strIn.Mid(0, 4) == "ZEIT")
		{
			int Z = _ttoi(strIn.Mid(6, 3));
		}
		if (strIn.Mid(0, 4) == "PLAN")
		{
			int P = _ttoi(strIn.Mid(6, 3));
		}
	}
	///// Den BLOCK einlesen
	if (strIn.Mid(0, 7) == "##Block")
	{
		file.ReadString(strIn);
		do
		{
			Block.push_back(CBlock(strIn,Step));
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	///// die Weichen einlesen
	if (strIn.Mid(0, 7) == "##Bl Nr")
	{
		file.ReadString(strIn);
		do
		{
			Nr = _ttoi(strIn.Mid(0, 3));
			if (Nr < Block.size())
			{
				Block[Nr].AddWeiche(strIn, Step);
			}
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	///// die Strecken einlesen
	if (strIn.Mid(0, 7) == "## Stre")
	{
		file.ReadString(strIn);
		do
		{
			Nr = _ttoi(strIn.Mid(0, 3));
			if (Nr < Block.size())
			{
				Block[Nr].AddStrecke(strIn, Step);

			}
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}
	/// Weg über Weichen einlesen
	if (strIn.Mid(0, 7) == "## Weg ")
	{
		file.ReadString(strIn);
		do
		{
			alle_Wege.push_back(CBlock_Weg(strIn));
			file.ReadString(strIn);
		} while (strIn.Mid(0, 2) != "##");
	}

	file.Close();
	///
	///
	/// 
	/* 
	for (byte i = 0; i < Züge->Get_max_Aktiv_Loks(); i++)
	{
		CDataXpressNet* DieLok = &Züge->Get_aktiveLok_Pointer(i);
		Nr = DieLok->Block_ist;
		if (Nr < Block.size())
		{
			Block[Nr].bestetzen(&Züge->Get_aktiveLok_Pointer(i));
		}
	}
	*/
	TRACE(_T("-------- Alle Wege --------------\n"));
	for (auto& Weg : alle_Wege)
	{
		TRACE(_T("Von %i --- %i ---- %i nach \n"), Weg.VonBlock(),Weg.ZwiBlock(),Weg.NachBlock());
		Block[Weg.VonBlock()].AddWeg(true,Weg);
		Block[Weg.NachBlock()].AddWeg(false,Weg);
	}
}
