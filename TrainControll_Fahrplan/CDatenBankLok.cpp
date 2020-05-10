#include "pch.h"
#include "CDatenBankLok.h"
#include "TrainControll_FahrplanDlg.h"

CDatenBankLok::CDatenBankLok()
{
}

CDatenBankLok::~CDatenBankLok()
{
}

byte CDatenBankLok::Init()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	XpressNet = &APP->XpressNet;

	Load_Zug_Data();
	Load_acktiv_Zug_Data();
	Set_aktiveLok_FuntiontoRun();
	return (byte)aktiveLoks.size();
}


CXpressNetCom * CDatenBankLok::Get_XpressNet()
{
	return XpressNet;
}

void CDatenBankLok::Test_Data()
{
}

void CDatenBankLok::Load_Zug_Data()
{
	LPCTSTR			pszPathName;
	CStdioFile		file;
	CFileException	ex;
	CString Text;
	CString strIn;
	
	byte Max_Zug;
	byte altDecoder = 0;
	byte nDecoderNr = 0;
	Text.Format(_T(FILE_ALLE_ZUEGE), 1);
	pszPathName = Text;
	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
		try
	{
		file.ReadString(strIn); // Anzahl an Loks einlesen
		Max_Zug = _ttoi(strIn.Mid(0, 3));
		file.ReadString(strIn); // �berschrift 1
		file.ReadString(strIn); // �berschrift 2
		file.ReadString(strIn); // ### Die Daten der Lok Nr :
		int Z = 0;
		do
		{
			file.ReadString(strIn); //===========
			file.ReadString(strIn); // Lok Na
			MeineZ�ge.push_back(CDataXpressNet(strIn.Mid(8, 15)));
			file.ReadString(strIn); 
			MeineZ�ge.at(Z).LokNr = strIn.Mid(8, 15);
			file.ReadString(strIn);
			MeineZ�ge.at(Z).Decoder_Data.Set_Decoder(&file);
			MeineZ�ge.at(Z).Decoder_Data.Fill_Tasten_Info();
			MeineZ�ge.at(Z).Set_Adresse();
			Z++;
			file.ReadString(strIn); // ##########################
			file.ReadString(strIn); // ### Die Daten der Lok Nr :
		} while (strIn.Mid(0, 4) != "ENDE");
		file.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		file.Abort();
		return;
	}
}

void CDatenBankLok::Save_Zug_Data()
{
	if (Do_Save_Data)
	{
		LPCTSTR			pszPathName;
		CStdioFile		file;
		CFileException	ex;
		CString			Text, strOut;

		Text.Format(_T(FILE_ALLE_ZUEGE), 1);
		pszPathName = Text;
		if (!file.Open(pszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeText, &ex))
		{
			ex.ReportError();
			return;
		}

		try
		{
			file.Seek(0, CFile::end);
			strOut.Format(_T("%03d\n"), int(MeineZ�ge.size()));
			file.WriteString(strOut);
			file.WriteString(_T("### Meine Zug Data for TrainSetup  ###\n"));
			file.WriteString(_T("######################################\n"));
			for (int Z = 0; Z < MeineZ�ge.size(); Z++)
			{
				strOut.Format(_T("### Die Daten der Lok Nr : %02d ###\n"), Z);
				file.WriteString(strOut);
				file.WriteString(_T("===============================================\n"));
				file.WriteString(_T("Lok Na: ") + MeineZ�ge[Z].Name + "\n");
				file.WriteString(_T("Lok Nr: ") + MeineZ�ge[Z].LokNr + "\n");
				file.WriteString(_T("===============================================\n"));
				MeineZ�ge[Z].Decoder_Data.Save_Decoder(&file);
				file.WriteString(_T("######################################\n"));
			}
			file.WriteString(_T("ENDE##################################\n"));
			file.Close();
		}
		catch (CFileException* e)
		{
			e->ReportError();
			e->Delete();
			file.Abort();
		}
	}
}

void CDatenBankLok::Load_acktiv_Zug_Data()
{
	LPCTSTR			pszPathName;
	CStdioFile		file;
	CFileException	ex;
	CString			strIn,Text, Name;
 
	Text.Format(_T(FILE_AKTIVE_ZUEGE), 1);
	pszPathName  = Text;
	Do_Save_Data = false;

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	file.ReadString(strIn); // Titel
	file.ReadString(strIn); // Tbeschreibung
	do
	{
		file.ReadString(strIn); // ZugListe

		if ((strIn[17] == 'H') || (strIn[17] == 'A'))
		{
			Name = strIn.Mid(0, 15); Name.Trim();
			for (int i = 0; i < MeineZ�ge.size(); i++)
			{
				if (MeineZ�ge[i].Name == Name)
				{
					if (MeineZ�ge[i].FillData(strIn)) aktiveLoks.push_back(i);
				}
			}
			if ((aktiveLoks.size() == MAX_ZUEGE_AKT))
			{
				TRACE(_T(" ERROR ! Zuviele aktive Z�ge mehr als 5 \n"));
				break;
			}
		}

	} while (strIn.Mid(0, 12) != "## Ende ####");

	file.Close();
}

void CDatenBankLok::Save_acktiv_Zug_Data()
{
}

void CDatenBankLok::Sound_acktive_Zug(bool SW)
{
	for (byte Zug : aktiveLoks)
	{
		MeineZ�ge[Zug].Set_Funktion_Sound(SW);
	}
}

void CDatenBankLok::SetAktiv_Lok(CString InText)
{
	
}

byte CDatenBankLok::Get_max_Aktiv_Loks()
{
	return (byte) aktiveLoks.size();
}



void CDatenBankLok::Ask_aktivLok_Data(byte Lok_Nr)
{
	Ask_Lok_Nr = aktiveLoks[Lok_Nr];
	MeineZ�ge[Ask_Lok_Nr].ASK_LokData();
}
void CDatenBankLok::New_Lok_Data()
{
	MeineZ�ge.at(Ask_Lok_Nr).Get_LokData();
}

void CDatenBankLok::Set_Zug_CV_Wert(byte CV, byte CV_Wert)
{
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Set_CV_Wert(CV, CV_Wert);
	Do_Save_Data = true;
}

CDataXpressNet & CDatenBankLok::Get_aktiveLok_Pointer(byte Lok_Nr)
{
	return MeineZ�ge[aktiveLoks[Lok_Nr]];
}

bool CDatenBankLok::is_Zug_Pointer(byte Nr)
{
	return (aktiveLoks.size()> Nr);
}

void CDatenBankLok::Fill_Liste_Zug(CComboBox * Data, DecoderTypen Decoder_Type)
{
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	for (CDataXpressNet D : MeineZ�ge)
	{
		if (D.Decoder_Data.isDecoderType(Decoder_Type))
		{
			Data->AddString(D.Name);
		}
	}
	
}
void CDatenBankLok::Fill_Liste_Zug(CComboBox * Data)
{
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	for (CDataXpressNet D : MeineZ�ge)
	{
		Data->AddString(D.Name);
	}
	Data->SetCurSel(0);
	Selected_Prog_Lok = 0;
}
void CDatenBankLok::Fill_Act_Liste_Zug(CComboBox * Data)
{
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	for (byte D : aktiveLoks)
	{
		Data->AddString(MeineZ�ge[D].Name);
	}
	Data->SetCurSel(0);
}

std::vector<CString> CDatenBankLok::Get_LokGruppe_Decoder(DecoderTypen Decoder)
{
	std::vector<CString> LGruppe;
	for (CDataXpressNet Z : MeineZ�ge)
	{
		if (Z.Decoder_Data.isDecoderType(Decoder))
		{
			LGruppe.push_back(Z.Name);
		}
	}
	return LGruppe;
}

byte CDatenBankLok::Get_Pos_LokName(CString LName)
{
	byte Pos = 0;
	for (CDataXpressNet Z : MeineZ�ge)
	{
		if (Z.Name == LName)
		{
			Selected_Prog_Lok = Pos;
			return Pos;
		}
		Pos++;
	}
	return Pos;
}

std::vector<Function_A> CDatenBankLok::Get_Zug_ActivFunktion_Pointer(byte Nr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Decoder_Data.Get_Aktive_Funktion();
}


void CDatenBankLok::Set_aktiveLok_Startbedingung(byte Nr, Zug_Status Status)
{
	MeineZ�ge[aktiveLoks[Nr]].Set_Startbedingungen(Status);
}

Zug_Status CDatenBankLok::Get_aktiveLok_Status(byte Nr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Get_Status();
}

void CDatenBankLok::Set_aktiveLoks_aufsGleis()
{
	CTrainControll_FahrplanDlg* APP = (CTrainControll_FahrplanDlg*)AfxGetApp()->m_pMainWnd;
	CDataBlock* Block = &APP->Gleis_Data.Block_Data;
	for (byte Nr = 0; Nr < (byte)aktiveLoks.size(); Nr++)
	{
		Block->besetzte_Block(MeineZ�ge[aktiveLoks[Nr]].Block_ist, &MeineZ�ge[aktiveLoks[Nr]]);
	}
}

void CDatenBankLok::Set_aktiveLok_FuntiontoRun()
{
	for (byte Zug : aktiveLoks)
	{
		MeineZ�ge[Zug].Decoder_Data.Fill_Tasten_DlgRun();
	}
}

bool CDatenBankLok::Get_aktiveLok_FunktionBit(byte Nr, byte FunktionNr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Get_Funktion(FunktionNr);
}

bool CDatenBankLok::Get_aktiveLok_FunktionSound(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZ�ge[aktiveLoks[Nr]].Decoder_Data.Get_Sound_FuntionsTaste();
	return MeineZ�ge[aktiveLoks[Nr]].Get_Funktion(Funktion_Nr);
}

void CDatenBankLok::Set_aktiveLok_FunktionSound(byte Nr, bool bit)
{
	MeineZ�ge[aktiveLoks[Nr]].Set_Funktion_Sound(bit);
}

bool CDatenBankLok::is_aktiveLok_Sound(byte Nr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Decoder_Data.isDecoderType(DecoderTypen::Massoth_Sound);
}

bool CDatenBankLok::Get_aktiveLok_FunktionRangier(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZ�ge[aktiveLoks[Nr]].Decoder_Data.Get_Rangier_FunktionTaste();
	return MeineZ�ge[aktiveLoks[Nr]].Get_Funktion(Funktion_Nr);
}

void CDatenBankLok::Set_aktiveLok_FunktionRangier(byte Nr, bool bit)
{
	MeineZ�ge[aktiveLoks[Nr]].Set_Funktion_Rangieren(bit);
}

bool CDatenBankLok::Get_aktiveLok_FunktionZeit(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZ�ge[aktiveLoks[Nr]].Decoder_Data.Get_Verz�gZ_FunktionTaste();
	return MeineZ�ge[aktiveLoks[Nr]].Get_Funktion(Funktion_Nr);

}

void CDatenBankLok::Set_aktiveLok_FunktionZeit(byte Nr, bool bit)
{
	MeineZ�ge[aktiveLoks[Nr]].Set_Funktion_Verz�gerungsZeit(bit);
}

void CDatenBankLok::Set_aktiveLok_Geschwindigkeit(byte Nr, byte Speed, bool Dir, bool Stop)
{
	if (Stop)	MeineZ�ge[aktiveLoks[Nr]].Set_Stop();
	else
	{
		if(Speed == 0 )
		{
			MeineZ�ge[aktiveLoks[Nr]].Set_Halt();
		}
		else
		{
			MeineZ�ge[aktiveLoks[Nr]].Set_Geschwindigkeit(Speed, Dir);
		}
	}
}

CString CDatenBankLok::Get_aktiveLok_Name(byte Nr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Name;
}

HBITMAP CDatenBankLok::Get_aktiveLok_Image(byte Nr)
{
	return MeineZ�ge[aktiveLoks[Nr]].Bild;
}

bool CDatenBankLok::Set_Zug_Dlg_Nr(byte Nr)
{
	static byte Dlg_nNr = 0;
	if (Anzahl_Dlg_Run < 3)
	{
		if ((MeineZ�ge[aktiveLoks[Nr]].Name == "gr�ne Stainz"))
	    {
		    MeineZ�ge[aktiveLoks[Nr]].Dlg_Nr = 3;
	    }
		else
		{
			MeineZ�ge[aktiveLoks[Nr]].Dlg_Nr = Dlg_nNr;
			Dlg_nNr++;
		}
		Anzahl_Dlg_Run++;
		return true;
	}
	return false;
}

void CDatenBankLok::Set_Prog_Zug(byte Zug)
{
	Selected_Prog_Lok = Zug;
}

byte CDatenBankLok::Get_Prog_Zug()
{
	return Selected_Prog_Lok;
}

DecoderTypen CDatenBankLok::Get_Sel_ZugDecoder_Type()
{
	return MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Get_Decoder_Type(); 
}

CDecoderInfo CDatenBankLok::Get_Sel_Zug_Decoder()
{
	return MeineZ�ge[Selected_Prog_Lok].Decoder_Data;
}

void CDatenBankLok::Set_Sel_Zug_Decoder_Funktion_Name(byte Nr, CString Name)
{
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Set_FunktionBechreibung(Nr, Name);
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	Do_Save_Data = true;
}

void CDatenBankLok::Set_Sel_Zug_Decoder_Funktion_Eigen(byte Nr, bool view, bool change)
{
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Set_FunktionEigenschaften(Nr, view, change);
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	Do_Save_Data = true;
}

void CDatenBankLok::Update_Sel_Zug_FunktionsTasten()
{
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.SortFunktion();
	Do_Save_Data = true;
}

void CDatenBankLok::Sort_Sel_Zug_FunktionsTasten()
{
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.SortFunktion();
}


void CDatenBankLok::PRG_Lok_CV(bool ReadWrite, byte CV, byte Wert) // true dann schreiben
{
	Zug_wartet_auf_Data = true;
	Prog_CV = CV;
	if (ReadWrite)  // true dann schreiben
		XpressNet->Sende_Write_CV(CV, Wert);
	else			// false weil nur lesen
		XpressNet->Sende_Read_CV(CV, Wert);
	do
	{
	} while (Zug_wartet_auf_Data);
}

void CDatenBankLok::PRG_Set_CV()
{
	byte Wert = XpressNet->Hole_CV_Wert();
	MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Set_CV_Wert(Prog_CV, Wert);
	Zug_wartet_auf_Data = false;
}

byte CDatenBankLok::PRG_Get_CV(byte CV)
{
	return MeineZ�ge[Selected_Prog_Lok].Decoder_Data.Get_CV_Wert(CV);
}
