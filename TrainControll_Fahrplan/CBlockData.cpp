#include "pch.h"
#include "CBlockData.h"
#include "TrainControll_FahrplanDlg.h"

CDataBlock::CDataBlock()
{
	 
}

CDataBlock::~CDataBlock()
{}

void CDataBlock::Init()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;

	Züge = &APP->meineLoks;
	XpressNet = &APP->XpressNet;
	Load_Data_Block();
}


void CDataBlock::Load_Data_Block()
{
	LPCTSTR			pszPathName = _T(FILE_ANLAGE);
	CStdioFile		file;
	CFileException	ex;
	CString			strIn, Text;
	int				Nr;
	CPoint			P;
	CString			Name;

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	file.ReadString(strIn); // Titel
	//////
	file.ReadString(strIn); // Bildgröße
	Text = strIn.Mid(0, 4);
	if (Text != "BILD") TRACE(_T(" Fehler beim Einlesen "));

	Step.x = Rect_X / _ttoi(strIn.Mid(6, 3));
	Step.y = Rect_Y / _ttoi(strIn.Mid(10, 3));
	/////
	file.ReadString(strIn); // Melder Zeit einlesen
	Text = strIn.Mid(0, 4);
	if (Text != "ZEIT") TRACE(_T(" Fehler beim Einlesen "));
	Melder_Zeit = _ttoi(strIn.Mid(6, 3));
	file.ReadString(strIn); // Melder Zeit einlesen
	Text = strIn.Mid(0, 4);
	if (Text != "PLAN") TRACE(_T(" Fehler beim Einlesen "));
	FahrplanAnzahl = _ttoi(strIn.Mid(6, 3));
	file.ReadString(strIn); // Überschrift

	Block.push_back(strIn); //Block[0] anlegen istr wichtig :-);
	file.ReadString(strIn); // Erste Zeile
	while((strIn.Mid(4, 6) == "Weiche") ||
		  (strIn.Mid(4, 6) == "Block ") ||
		  (strIn.Mid(4, 6) == "Gleis "))
	{
		Nr = _ttoi(strIn.Mid(0, 3));
		Block.push_back(strIn);
		Block[Block.size()-1].SkaliereDaten(Step);
		
		file.ReadString(strIn); // Block einlesen
	} 

	file.ReadString(strIn);
	while (strIn.Mid(4, 6) == "Taster")
	{
		Block[_ttoi(strIn.Mid(0, 3))].SetTaster(strIn);
		file.ReadString(strIn);
	}

	file.ReadString(strIn); 
	while(strIn.Mid(4, 6) == "Melder")
	{
		Melder.push_back(MelderBlock(strIn));
		Melder[Melder.size() - 1].SkaliereDaten(Step);
		file.ReadString(strIn); 
	}
	// ###############################################################################
	file.ReadString(strIn);
	while (strIn.Mid(4, 6) == "Weg to")
	{
		W_Gruppe.push_back(strIn);
		file.ReadString(strIn); // die Weg Streke
	}
	// ###############################################################################
	file.Close();

	for (byte i = 0; i < Züge->Get_max_Aktiv_Loks(); i++)
	{
		_Block_besetzen(Züge->Get_aktiveLok_Pointer(i).Block_ist, &Züge->Get_aktiveLok_Pointer(i));
	}

}


byte CDataBlock::GetAnzahl_Block()
{
	return (byte)Block.size();
}
CInfoBlock CDataBlock::GetWerte_Block(byte Nr)
{
	return Block[Nr];
}
BlockInfo CDataBlock::GetWerte_BlockInfo(byte Nr)
{
	return Block[Nr].Get_BlockInfo();
}
bool CDataBlock::ist_besetzt(byte Nr)
{
	return (Block[Nr].Besetzt_Zug != NULL);
}
bool CDataBlock::ist_frei(byte Nr)
{
	return (Block[Nr].Besetzt_Zug == NULL);
}
CString CDataBlock::GetZugName_in_Block(byte Nr)
{
	return (Block[Nr].Besetzt_Zug->Name);
}

BlockType CDataBlock::Get_BlockType(byte Nr)
{
	return Block[Nr].Block_Type;
}

byte CDataBlock::ClearBlockbyLok(byte LokNr, byte Melder)
{
	byte CBlock;
	while (Züge->Get_aktiveLok_Pointer(LokNr).ClearList.isnextMelder(Melder))
	{
		CBlock = Züge->Get_aktiveLok_Pointer(LokNr).ClearList.GetBlock();
		Block[CBlock].Besetzt_Zug = NULL;
	}
	Neu_DataGleis.Putin();
	return byte();
}

bool CDataBlock::Get_Stellung_Weiche(byte Nr)
{
	return Block[Nr].WeichenStellung;
}

void CDataBlock::Set_Stellung_Weiche(TrainCon_Paar Wl)
{
	Block[Wl.GetWert()].WeichenStellung = Wl.GetBit();
	Neu_DataGleis.Putin();
}

byte CDataBlock::KlickWeicheNummer(CPoint KlickP)
{
	int C = 0;
	for (CInfoBlock i : Block)
	{
		if (i.KlickRechteck.PtInRect(KlickP))	
			return(C);	
		C++;
	}
	return (0);
}

bool CDataBlock::Set_FahrPlan( byte Lok_Nr, byte Plan_Nr)
{
	Züge->Get_aktiveLok_Pointer(Lok_Nr).Lade_Fahrplan();

	Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Load_Daten(Plan_Nr);
	return (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfe_Plan_im_Block());
}


void CDataBlock::Do_Start_Plan_Zug(byte Lok_Nr)
{
	FahrplanPos	Befehl;
	BlockRueckmeldung Ergenis;

	Züge->Ask_aktivLok_Data(Lok_Nr); // Erstmal Daten holen

	while (Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl().mache == FahrPlanDo::schalten_Funk)
	{
		Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Funktion(Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl());
		Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Done_Plan_Befehl();
	}
	Befehl = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl(); // hole Schalt
	Ergenis = besetzte_bis_Next_Block_Auto(Lok_Nr);
	Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Done_Plan_Befehl();
	Befehl = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_Befehl();
	if (Befehl.mache == FahrPlanDo::vorwaerz_fahren)
	{
		if (Ergenis == BlockRueckmeldung::Frei_Fahrt)
		{
			TRACE(_T(" Starte Jetzt den Zug \n"));
			Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Geschwindigkeit(Befehl);
		}
		else
		{
			Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Halt(Befehl);
			TRACE(_T(" Hält bis zur freien Fahrt \n"));
		}
		Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Done_Plan_Befehl();
		Beep(523, 500);
	}
	else
	{
		TRACE(_T("ERROR hier hätte ein Fahrbefehl kommen müssen \n"));
	}
}

CString CDataBlock::Get_Gleis_Name(byte Nr)
{
	return Block[Nr].Name;
}

CRect CDataBlock::Get_Weichen_Feld(byte Nr)
{
	return Block[Nr].KlickRechteck;
}

byte CDataBlock::Get_Next_Block(byte Nr, bool PRichtung)
{
	byte N;
	if (PRichtung)	N = Block[Nr].AusgangBlock[0];
	else			N = Block[Nr].EingangBlock[0];

	while ((Block[N].Block_Type == BlockType::isWeiche))
	{ 
		if (PRichtung) N = Block[N].AusgangBlock[(byte)Block[N].WeichenStellung];
		else		   N = Block[N].EingangBlock[(byte)Block[N].WeichenStellung];
	};
	return (N) ;
}

byte CDataBlock::Get_Next_MelderzuBlock(byte Lok_Nr, byte Block_Nr) // in Fahrtrichtung
{
	byte Be = 0;
	if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
	{
		if ((Block[Block_Nr].Block_Type == BlockType::isWeiche) && (Block[Block_Nr].AusgangBlock[0] != Block[Block_Nr].AusgangBlock[1]))
		{
			Be = Block[Block_Nr].AusgangBlock[(byte)Block[Block_Nr].WeichenStellung];
		}
	}
	else
	{
		if ((Block[Block_Nr].Block_Type == BlockType::isWeiche) && (Block[Block_Nr].EingangBlock[0] != Block[Block_Nr].EingangBlock[1]))
		{
			Be = Block[Block_Nr].EingangBlock[(byte)Block[Block_Nr].WeichenStellung];
		}
		else
		{
			Be = Block[Block_Nr].EingangBlock[0];
		}
	}
	for (byte i = 0; i < Melder.size(); i++)
	{
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
		{
			if (Block[Block_Nr].Block_Type == BlockType::isWeiche)
			{
				if ((Block_Nr == Melder[i].Eingang_Block) && (Be == Melder[i].Ausgang_Block)) return (i);
			}
			else
			{
				if (Block_Nr == Melder[i].Eingang_Block) return(i);
			}
		}
		else
		{
			if (Block[Block_Nr].Block_Type == BlockType::isWeiche)
			{
				if ((Block_Nr == Melder[i].Ausgang_Block) && (Be == Melder[i].Eingang_Block)) return (i);
			}
			else
			{
				if (Block_Nr == Melder[i].Ausgang_Block) return(i);
			}
		}
	}
	return(255);
}

byte CDataBlock::Get_Soll_MelderzuBlock(byte Lok_Nr, byte Block_Nr)
{
	//if (Block[Block_Nr].Weiche_Type == isWeiche) TRACE(_T(" ERROR Prüfung auf Weiche\n"));

	for (byte i = 0; i < Melder.size(); i++)
	{
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
		{
			if(Block[Block_Nr].Block_Type == BlockType::isWeiche)
			{
				bool st = Block[Block_Nr].WeichenStellung;
				if(i == Block[Block_Nr].AusgangMelder[st]) return(i);
			}
			else
				if (i == Block[Block_Nr].AusgangMelder[0]) return(i);
		}
		else
		{
			if (Block[Block_Nr].Block_Type == BlockType::isWeiche)
			{
				bool st = Block[Block_Nr].WeichenStellung;
				if (i == Block[Block_Nr].EingangMelder[st]) return(i);
			}
			else
				if (i == Block[Block_Nr].EingangMelder[0])	return(i);
		}
	}
	return(255); // 255 ist Fehler
}

byte CDataBlock::Get_Soll_MelderzuBlock(byte Lok_Nr, TrainCon_Paar BlockT)
{
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
		{
			if (Block[BlockT.GetWert()].Block_Type == BlockType::isWeiche)
				return (Block[BlockT.GetWert()].AusgangMelder[BlockT.GetBit()]);
			else
				return (Block[BlockT.GetWert()].AusgangMelder[0]);
		}
		else
		{
			if (Block[BlockT.GetWert()].Block_Type == BlockType::isWeiche)
				return (Block[BlockT.GetWert()].EingangMelder[BlockT.GetBit()]);
			else
				return (Block[BlockT.GetWert()].EingangMelder[0]);
		}
	return(255); // 255 ist Fehler
}

void CDataBlock::besetzte_Block(byte Nr, CDataXpressNet* Lok)
{
	_Block_besetzen(Nr, Lok);
}

BlockRueckmeldung CDataBlock::besetzte_bis_Next_Block_Hand(byte Lok_Nr)
{ // von Ist --- bis -- Soll
	byte i = 1;
	unsigned int Frei = 0;
	byte LockBlock[6];
	byte N,L;
	LockBlock[0] = Züge->Get_aktiveLok_Pointer(Lok_Nr).Block_soll;
	{ 
		L = LockBlock[0];
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
		{
			N = Block[LockBlock[0]].AusgangBlock[0];
			if ((Block[N].Block_Type == BlockType::isWeiche) && (Block[N].EingangBlock[0] != Block[N].EingangBlock[1]))
			{
				if (Block[N].EingangBlock[(byte)Block[N].WeichenStellung] != L) return( BlockRueckmeldung::Weichenweg_nichtfrei);
			}
		}
		else
		{
			N = Block[LockBlock[0]].EingangBlock[0];
			if ((Block[N].Block_Type == BlockType::isWeiche) && (Block[N].AusgangBlock[0] != Block[N].AusgangBlock[1]))
			{
				if (Block[N].AusgangBlock[(byte)Block[N].WeichenStellung] != L) return(BlockRueckmeldung::Weichenweg_nichtfrei);
			}

		}
		LockBlock[1] = N;

		i++;
		if (Block[N].Besetzt_Zug != NULL)  Frei++;
		while ((Block[N].Block_Type == BlockType::isWeiche))
		{
			L = N;
			if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
			{
				N = Block[N].AusgangBlock[(byte)Block[N].WeichenStellung];
				if ((Block[N].Block_Type == BlockType::isWeiche) && (Block[N].EingangBlock[0] != Block[N].EingangBlock[1]))
				{
					if (Block[N].EingangBlock[(byte)Block[N].WeichenStellung] != L) return(BlockRueckmeldung::Weichenweg_nichtfrei);
				}
			}
			else
			{
				N = Block[N].EingangBlock[(byte)Block[N].WeichenStellung];
				if ((Block[N].Block_Type == BlockType::isWeiche) && (Block[N].AusgangBlock[0] != Block[N].AusgangBlock[1]))
				{
					if (Block[N].AusgangBlock[(byte)Block[N].WeichenStellung] != L) return(BlockRueckmeldung::Weichenweg_nichtfrei);
				}
			}
			LockBlock[i] = N; i++;
			if (Block[N].Besetzt_Zug != NULL) Frei++;
		};
		if (Frei == 0) // Die Strecke ist frei und wird jetzt besetzt.
		{
			byte Mi;
			Mi = Get_Soll_MelderzuBlock(Lok_Nr, LockBlock[0]);
			Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.SetBlock(Mi, LockBlock[0]);
			//TRACE(_T("=>a Melder %i löscht Block[ %i]  \n"), Mi, LockBlock[0]);
			if (Block[LockBlock[1]].Block_Type == BlockType::isWeiche)
			{
				Mi = Get_Soll_MelderzuBlock(Lok_Nr, LockBlock[i - 2]);
				for (byte a = 1; a < (i - 1);a++)
				{
					Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.SetBlock(Mi, LockBlock[a]);
					// TRACE(_T("=>b Melder %i löscht Block[ %i]  \n"), Mi, LockBlock[a]);
				}
			}
			// TRACE(_T("=> nächter Melder der auslöst ist %i \n"), Mi);
			Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Block(LockBlock[0], LockBlock[i - 1], LockBlock[i - 1], Mi);

			for (byte b = 1; b < (i); b++)
			{
				_Block_besetzen(LockBlock[b], &Züge->Get_aktiveLok_Pointer(Lok_Nr));
			}
			//TRACE(_T("check Melder = %i - %i \n", Get_Soll_MelderzuBlock(Lok, LokonTrack[Lok].Block_ClearList[i - 1])));
			Neu_DataGleis.Putin();
			return(BlockRueckmeldung::Frei_Fahrt);
		}

		Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Besetzt_Block(LockBlock[i-1]);
	}	
	return (BlockRueckmeldung::Block_besetzt); 
}

BlockRueckmeldung CDataBlock::besetzte_bis_Next_Block_Auto(byte Lok_Nr)
{ // von Ist --- bis -- Soll
	byte i = 0;
	unsigned int Frei = 0;
	TrainCon_Paar LockBlock[6];
	byte N;
	LockBlock[0].Set_Paar(Züge->Get_aktiveLok_Pointer(Lok_Nr).Get_Soll_Block(), false);
	{
		if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
		{
			N = Block[LockBlock[0].GetWert()].AusgangBlock[0];
			if (Block[N].Block_Type == BlockType::isWeiche)
			{
				i++;
				LockBlock[i] = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_WeichenWeg(i-1);
				N = Block[N].AusgangBlock[LockBlock[i].GetBit()];
			}
		}
		else
		{
			N = Block[LockBlock[0].GetWert()].EingangBlock[0];
			if (Block[N].Block_Type == BlockType::isWeiche)
			{
				i++;
				LockBlock[i] = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_WeichenWeg(i - 1);
				N = Block[N].EingangBlock[LockBlock[i].GetBit()];
			}
		}
		i++;
		LockBlock[i].Set_Paar(N , false);
		if (Block[N].Besetzt_Zug != NULL)  Frei++;
		while ((Block[N].Block_Type == BlockType::isWeiche))
		{
			LockBlock[i] = Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.Get_Plan_WeichenWeg(i-1);
			if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Prüfrichtung())
			{
				N = Block[LockBlock[i].GetWert()].AusgangBlock[LockBlock[i].GetBit()];
			}
			else
			{
				N = Block[LockBlock[i].GetWert()].EingangBlock[LockBlock[i].GetBit()];
			}
			i++;
			LockBlock[i].Set_Paar(N, false);
			if (Block[N].Besetzt_Zug != NULL) Frei++;
		};
		if (Frei == 0) // Die Strecke ist frei und wird jetzt besetzt.
		{
			byte Mi;
			Mi = Get_Soll_MelderzuBlock(Lok_Nr, LockBlock[0]);
			Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.SetBlock(Mi, LockBlock[0].GetWert());
			if (Block[LockBlock[1].GetWert()].Block_Type == BlockType::isWeiche)
			{
				Mi = Get_Soll_MelderzuBlock(Lok_Nr, LockBlock[i - 1]);
				for (byte a = 1; a < (i); a++)
				{
					Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.SetBlock(Mi, LockBlock[a].GetWert());
				}
			}
			Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Block(
			LockBlock[0].GetWert(),
			LockBlock[i].GetWert(),
			LockBlock[i].GetWert(),
			 Mi);
			for (byte b = 1; b <= i; b++)
			{
				if (Züge->Get_aktiveLok_Pointer(Lok_Nr).Fahr_Plan.isnot_PlanStop())
				{
					_Block_besetzen(LockBlock[b].GetWert(), &Züge->Get_aktiveLok_Pointer(Lok_Nr));
				}
				if (Block[LockBlock[b].GetWert()].Block_Type == BlockType::isWeiche)
				{
					SchalteWeiche(LockBlock[b]);
				}
			}
			Neu_DataGleis.Putin();
			return(BlockRueckmeldung::Frei_Fahrt);
		}
		Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Besetzt_Block(LockBlock[i].GetWert());
	}
	return (BlockRueckmeldung::Block_besetzt);
}
CDataXpressNet* CDataBlock::Get_Block_Lok(byte Block_Nr)
{
	//if (Block[Block_Nr].Besetzt_Zug == NULL) return nullptr;
	return (Block[Block_Nr].Besetzt_Zug);
}
void CDataBlock::setze_Block_Test(byte Nr, bool setze)
{
	if(setze)
		Block[Nr].Besetzt_Zug = &Züge->Get_aktiveLok_Pointer(0);
	else
		Block[Nr].Besetzt_Zug = NULL;
	Neu_DataGleis.Putin();
}


void CDataBlock::Set_Melder(TrainCon_Paar Ml)
{
	if (Ml.GetWert() < Melder.size())
	{
		Melder[Ml.GetWert()].Bit = Ml.GetBit();
		Neu_DataGleis.Putin();
	}
	else TRACE(_T(" ERROR Konnte Melder %i den Wert nicht eintragen \n"));
}

byte CDataBlock::GetAnzahl_Melder()
{
	return (byte) Melder.size();
}

byte CDataBlock::GetMelder_Zeit()
{
	return Melder_Zeit;
}

bool CDataBlock::Show_Melder(byte Nr)
{
	return Melder[Nr].Showit;
}

bool CDataBlock::Get_Melder(byte Nr)
{
	return Melder[Nr].Bit;
}



CPoint CDataBlock::Get_Melder_Position(byte Nr)
{
	return Melder[Nr].Pos;
}

CPoint CDataBlock::Get_Melder_PositionText(byte Nr)
{
	return Melder[Nr].TextPos;
}


bool CDataBlock::Show_Taster(byte Nr, bool Ort)
{
	if (Ort) // Eingang
		return Block[Nr].EingangTaster.Showit;
	else
		return Block[Nr].AusgangTaster.Showit;
}

char CDataBlock::GetCol_Taster(byte Nr, bool Ort)
{
	if (Ort) // Eingang
		return Block[Nr].EingangTaster.Color;
	else
		return Block[Nr].AusgangTaster.Color;
}

CRect CDataBlock::Get_Taster_Position(byte Nr, bool Ort)
{
	if (Ort) // Eingang
		return Block[Nr].EingangTaster.KlickRechteck;
	else
		return Block[Nr].AusgangTaster.KlickRechteck;
}




void CDataBlock::KlickTasterSchalten(CPoint KPoint)
{
	static bool zweiterKlick = false;
	static std::vector<byte> TasterListe;
	static bool NextAus = false;
	static bool NextEin = false;
	static byte ersterTaster;
	static byte ZweiterTaster;
	byte i = 0;
	if (!zweiterKlick)
	{
		zweiterKlick = true;
		i = 0;
		for (CInfoBlock B : Block)
		{
			if (B.EingangTaster.KlickRechteck.PtInRect(KPoint))
			{
				Block[i].EingangTaster.Color = 'W';
				ersterTaster = i;
				NextAus = true;
				if (Wege_zuBlock(false, i, &TasterListe))  // true = ein
				{
					zweiterKlick = false;
					ZweiterTaster = TasterListe.at(0);
					SchalteWeichenStrasse(ersterTaster, ZweiterTaster);
					Wege_zuBlock(true, ZweiterTaster, &TasterListe);
					for (byte r : TasterListe)
					{
						Block[r].EingangTaster.Color = 'R';
						if (r == ersterTaster) Block[r].EingangTaster.Color = 'G';	
					}
				}
				else
				{
					for (byte t : TasterListe)
					{
						Block[t].AusgangTaster.Color = 'Y';
					}
				}
				Neu_DataTaster.Putin();
				return;
			}
			if (B.AusgangTaster.KlickRechteck.PtInRect(KPoint))
			{
				Block[i].AusgangTaster.Color = 'W';
				ersterTaster = i;
				NextEin = true;
				if(Wege_zuBlock(true, i, &TasterListe))
				{
					zweiterKlick = false;
					ZweiterTaster = TasterListe.at(0);
					SchalteWeichenStrasse(ersterTaster, ZweiterTaster);
					Wege_zuBlock(false, ZweiterTaster, &TasterListe);
					for (byte r : TasterListe)
					{
						Block[r].AusgangTaster.Color = 'R';
						if (r == ersterTaster) Block[r].AusgangTaster.Color = 'G';
					}

				}
				else
				{
					for (byte t : TasterListe)
					{
						Block[t].EingangTaster.Color = 'Y';
					}
				}
				Neu_DataTaster.Putin();
				return;
			}
			i++;
		}
	}
	if(zweiterKlick)
	{ 
		zweiterKlick = false;
		i = 0;
		if (NextEin)
		{
			NextEin = false;
			Block[ersterTaster].AusgangTaster.Color = 'G';
			for (byte t : TasterListe)
			{
				if (Block[t].EingangTaster.KlickRechteck.PtInRect(KPoint))
				{
					Block[t].EingangTaster.Color = 'G';
					SchalteWeichenStrasse(ersterTaster, t);
				}
				else
					Block[t].EingangTaster.Color = 'R';
			}
			Neu_DataTaster.Putin();
		}
		if (NextAus)
		{
			NextAus = false;
			Block[ersterTaster].EingangTaster.Color = 'G';
			for (byte t : TasterListe)
			{
				if (Block[t].AusgangTaster.KlickRechteck.PtInRect(KPoint))
				{
					Block[t].AusgangTaster.Color = 'G';
					SchalteWeichenStrasse(ersterTaster, t);
				}
				else
					Block[t].AusgangTaster.Color = 'R';
			}
			Neu_DataTaster.Putin();
		}

	}
}


void CDataBlock::SchalteWeichenStrasse(byte TasterA, byte TasterB)
{
	for (Weichen_Gruppe i : W_Gruppe)
	{
		if(i.isdiesderWeg(TasterA, TasterB))
		{ 
			for (TrainCon_Paar wa : i.WStellung)
			{
				SchalteWeiche(wa);
			}
		}
	}
}

bool CDataBlock::SchalteWeicheTest(byte WeichenNr)
{
	bool Bit = !Get_Stellung_Weiche(WeichenNr);
	SchalteWeiche(WeichenNr, Bit);
	return (Bit); 
}
void CDataBlock::SchalteWeiche(CPoint KPoint)
{
	TrainCon_Paar WL;
	WL.SetWert(KlickWeicheNummer(KPoint));
	WL.SetBit(!Block[WL.GetWert()].WeichenStellung);
	
	if (XpressNet->NoComToXpressNet())
	{
		Block[WL.GetWert()].WeichenStellung = WL.GetBit();
		Neu_DataGleis.Putin();
	}
	else
		XpressNet->SendeWeicheDaten(WL);
}
void CDataBlock::SchalteWeiche(byte Weiche_a, bool Weg_a)
{
	if (XpressNet->NoComToXpressNet())
	{
		Block[Weiche_a].WeichenStellung = Weg_a;
		Neu_DataGleis.Putin();
	}
	else
	{
		XpressNet->SendeWeicheDaten(TrainCon_Paar(Weiche_a, Weg_a));
	}
}
void CDataBlock::SchalteWeiche(TrainCon_Paar Setto)
{
	if (XpressNet->NoComToXpressNet())
	{
		Block[Setto.GetWert()].WeichenStellung = Setto.GetBit();
		Neu_DataGleis.Putin();
	}
	else
		XpressNet->SendeWeicheDaten(Setto);
}

CDataXpressNet *CDataBlock::Get_Block_besetzt_Wert(byte Bl_Nr)
{
	return Block[Bl_Nr].Besetzt_Zug;
}
void CDataBlock::doBlocks_byZug_Stop(byte Lok_Nr)
{
	//LokonTrack[LokNr].Melder_next = 0xFF; // so einen gibt es nicht
	//LokonTrack[LokNr].Block_soll = LokonTrack[LokNr].Block_ist;
	Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Block_Pause(true);
	for (int i = 1; i < GetAnzahl_Block(); i++)
	{
		if (Block[i].Besetzt_Zug == &Züge->Get_aktiveLok_Pointer(Lok_Nr))
		{
			if (i != Züge->Get_aktiveLok_Pointer(Lok_Nr).Block_ist) Block[i].Besetzt_Zug = NULL;
		}
	}
	Züge->Get_aktiveLok_Pointer(Lok_Nr).ClearList.clearBuffer();
	Neu_DataGleis.Putin();
}
void CDataBlock::doBlocks_byZug_Halt(byte Lok_Nr)
{
	// LokonTrack[LokNr].Block_ist = LokonTrack[LokNr].Block_soll;
	// LokonTrack[LokNr].Block_best = 0;
	Züge->Get_aktiveLok_Pointer(Lok_Nr).Set_Block_Pause(false);
	Neu_DataGleis.Putin();
}
bool CDataBlock::isNewUpdate_Gleis()
{
	return Neu_DataGleis.Get_Out();
}
bool CDataBlock::isNewUpdate_Taster()
{
	return Neu_DataTaster.Get_Out();
}
bool CDataBlock::isNewUpdate_Zug()
{
	return Neu_DataZug.Get_Out();;
}

std::vector<CInfoBlock>* CDataBlock::GetBlockPointer()
{
	return &Block;
}

void CDataBlock::copyWeg(std::vector<Weichen_Gruppe>* Tabelle)
{
	*Tabelle = W_Gruppe;
}

bool CDataBlock::Wege_zuBlock(bool R, byte Blockn, std::vector<byte>* Tabelle)
{
	Tabelle->clear();
	for (Weichen_Gruppe W : W_Gruppe)
	{
		if (R)
		{
			if (W.isBlockinEin(Blockn))	Tabelle->push_back(W.GetBlock(Blockn));
		}
		else
		{
			if (W.isBlockinAus(Blockn))	Tabelle->push_back(W.GetBlock(Blockn));
		}
	}
	if (Tabelle->size() == 1)
	{
		if (R) 
		{
			Block[Blockn].AusgangTaster.Color = 'G';
			Block[Tabelle->at(0)].EingangTaster.Color = 'G';
		}
		else
		{
			Block[Blockn].EingangTaster.Color = 'G';
			Block[Tabelle->at(0)].AusgangTaster.Color = 'G';
		}
		return true;
	}
	return false;
}

void CDataBlock::_Block_besetzen(byte Nr, CDataXpressNet *Lok)
{
	Block[Nr].Besetzt_Zug = Lok;
}
void CDataBlock::_Block_freimachen(byte Nr, byte Lok_Nr)
{
	Block[Nr].Besetzt_Zug = NULL;
}
/*
void Anzeige_Liste();
	if ((Block[Nr].Weiche_Type == isBlock) || (Block[Nr].Weiche_Type == isGleis))
	{
		byte E = Block[Nr].EingangBlock;
		byte A = Block[Nr].AusgangBlock;
		TRACE(_T(" -> %i [ %i ] %i <- \n"), E, Nr, A);
	}
	if ((Block[Nr].Weiche_Type == isWeiche))
	{
		byte G = Weiche[Nr].WeichenGruppe;
		byte E0 = Weiche[Nr].Eingang[0];
		byte A0 = Weiche[Nr].Ausgang[0];
		byte E1 = Weiche[Nr].Eingang[1];
		byte A1 = Weiche[Nr].Ausgang[1];
		TRACE(_T(" -> %i [G %i ] %i <- \n"), E0,  G, A0);
		TRACE(_T(" -> %i [  %i ] %i <- \n"), E1, Nr, A1);
	}
*/



bool CDataBlock::ist_Lok_onBlock(byte Block_Nr)
{
	if (Block[Block_Nr].Besetzt_Zug == NULL) return false;

	return(Block[Block_Nr].Besetzt_Zug->Block_ist == Block_Nr);
}



CPoint CDataBlock::Get_Weiche_EinPos(byte Nr)
{
	return Block[Nr].EinPos[0];
}
CPoint CDataBlock::Get_Weiche_AusPos(byte Nr, bool Bit)
{
	if (Bit)
		return Block[Nr].AusPos[1];
	else
		return Block[Nr].AusPos[0];
}
CPoint CDataBlock::Get_Weiche_MitPos(byte Nr)
{
	return Block[Nr].MitPos[0];
}
CPoint CDataBlock::Get_Weiche_TexPos(byte Nr)
{
	return Block[Nr].TextPos;// .PointtoDraw();
}