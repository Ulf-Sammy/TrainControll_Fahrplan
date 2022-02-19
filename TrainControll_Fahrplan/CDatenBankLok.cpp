#include "pch.h"
#include "CDatenBankLok.h"
#include "TrainControll_FahrplanDlg.h"

CDatenBankLok::CDatenBankLok()
{	
	Load_Zug_Data();
}

CDatenBankLok::~CDatenBankLok()
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
		file.ReadString(strIn); // Überschrift 1
		file.ReadString(strIn); // Überschrift 2
		file.ReadString(strIn); // ### Die Daten der Lok Nr :
		int Z = 0;
		do
		{
			file.ReadString(strIn); //===========
			file.ReadString(strIn); // Lok Na
			MeineZüge.push_back(CDataXpressNet(strIn.Mid(8, 15)));
			file.ReadString(strIn); 
			MeineZüge.at(Z).LokNr = strIn.Mid(8, 15);
			file.ReadString(strIn);
			MeineZüge.at(Z).Decoder_Data.Set_Decoder(&file);
			MeineZüge.at(Z).Decoder_Data.Fill_Tasten_Info();
			MeineZüge.at(Z).Set_Adresse();
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
			strOut.Format(_T("%03d\n"), int(MeineZüge.size()));
			file.WriteString(strOut);
			file.WriteString(_T("### Meine Zug Data for TrainSetup  ###\n"));
			file.WriteString(_T("######################################\n"));
			for (int Z = 0; Z < MeineZüge.size(); Z++)
			{
				strOut.Format(_T("### Die Daten der Lok Nr : %02d ###\n"), Z);
				file.WriteString(strOut);
				file.WriteString(_T("===============================================\n"));
				file.WriteString(_T("Lok Na: ") + MeineZüge[Z].Name + "\n");
				file.WriteString(_T("Lok Nr: ") + MeineZüge[Z].LokNr + "\n");
				file.WriteString(_T("===============================================\n"));
				MeineZüge[Z].Decoder_Data.Save_Decoder(&file);
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
	file.ReadString(strIn); // 1. Zug
	do
	{
		HomeZüge.push_back(Start_Lok_Block(strIn));
		file.ReadString(strIn); // ZugListe
	} while (strIn.Mid(0, 12) != "## Ende ####");

	file.Close();
}

void CDatenBankLok::Save_acktiv_Zug_Data()
{
	LPCTSTR			pszPathName;
	CStdioFile		file;
	CFileException	ex;
	CString			strOut, Text, Name;

	Text.Format(_T(FILE_AKTIVE_ZUEGE), 1);
	pszPathName = Text;
	if (!file.Open(pszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeText, &ex))
	{
		ex.ReportError();
		return;
	}

	try
	{
		file.Seek(0, CFile::end);
		file.WriteString(L"#### Info's zum aktive Züge im Lok Schuppen#### \n");
		file.WriteString(L"## Zug ##- use | Block | Blick     |            \n");
		for (auto &Zug : HomeZüge)
		{
			strOut = Zug.WriteData();
			file.WriteString(strOut+"| \n");
		}
		file.WriteString(L"## Ende ############ \n");
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
		file.Abort();
	}
}

void CDatenBankLok::Verbinde_Zug_zu_XpNet(byte Nr)
{
	MeineZüge[Nr].ConecttoXpressNet();
}

void CDatenBankLok::Sound_acktive_Zug(bool SW)
{
	for (byte Zug : Loks_in_Schuppen)
	{
		MeineZüge[Zug].Set_Funktion_Sound(SW);
	}
}


byte CDatenBankLok::Get_max_Aktiv_Loks()
{
	return (byte) Loks_in_Schuppen.size();
}

void CDatenBankLok::Ask_for_Lok_Data(byte Lok_Nr)
{
	MeineZüge[Lok_Nr].ASK_LokData();
}

void CDatenBankLok::Set_Zug_CV_Wert(byte CV, byte CV_Wert)
{
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Set_CV_Wert(CV, CV_Wert);
	Do_Save_Data = true;
}


CDataXpressNet& CDatenBankLok::Get_Lok_Pointer(CString LName)
{
	for (auto& Zug : MeineZüge)
	{
		if (LName == Zug.Name)
		{
			return Zug;
		}
	}
	
	return MeineZüge[0]; // nullptr;
}

CDataXpressNet & CDatenBankLok::Get_aktiveLok_Pointer(byte Lok_Nr)
{
	return MeineZüge[Loks_in_Schuppen[Lok_Nr]];
}

void CDatenBankLok::Fill_Liste_Zug(CComboBox * Data, DecoderTypen Decoder_Type)
{
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	for (CDataXpressNet D : MeineZüge)
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
	for (CDataXpressNet D : MeineZüge)
	{
		Data->AddString(D.Name);
	}
	Data->SetCurSel(0);
	Selected_Prog_Lok = 0;
}
void CDatenBankLok::Fill_Liste_Zug(CComboBox* Data, CString Name)
{
	byte Pos = 0;
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	Data->AddString(L"-");
	for (CDataXpressNet &D : MeineZüge)
	{
		Data->AddString(D.Name);
		if (D.Name == Name) Pos = (byte)(& D - &MeineZüge[0] +1);
	}
	Data->SetCurSel(Pos);
	Selected_Prog_Lok = 0;
}
void CDatenBankLok::Fill_Act_Liste_Zug(CComboBox * Data)
{
	for (int i = 0; i < Data->GetCount(); i++)
	{
		Data->DeleteString(0);
	}
	for (byte D : Loks_in_Schuppen)
	{
		Data->AddString(MeineZüge[D].Name);
	}
	Data->SetCurSel(0);
}

std::vector<CString> CDatenBankLok::Get_LokGruppe_Decoder(DecoderTypen Decoder)
{
	std::vector<CString> LGruppe;
	for (CDataXpressNet Z : MeineZüge)
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
	for (CDataXpressNet& Z : MeineZüge)
	{
		if (Z.Name == LName)
		{
			Selected_Prog_Lok = (byte)(&Z - &MeineZüge[0]);
			return Selected_Prog_Lok;
		}
	}
	return 0xFF;
}

std::vector<Function_A> CDatenBankLok::Get_Zug_ActivFunktion_Pointer(byte Nr)
{
	return MeineZüge[Nr].Decoder_Data.Get_Aktive_Funktion();
}


void CDatenBankLok::Set_Lok_Startbedingung(byte Nr, Zug_Status Status)
{
	MeineZüge[Nr].Set_Startbedingungen(Status);
}

Zug_Status CDatenBankLok::Get_Lok_Status(byte Nr)
{
	return MeineZüge[Nr].Get_Status();
}


void CDatenBankLok::Set_aktiveLok_FuntiontoRun()
{
	for (byte Zug : Loks_in_Schuppen)
	{
		MeineZüge[Zug].Decoder_Data.Fill_Tasten_DlgRun();
	}
}

void CDatenBankLok::Set_Funktion(byte Nr, FahrplanPos Befehl)
{
	MeineZüge[Nr].Set_Funktion(Befehl);
}

bool CDatenBankLok::Get_Lok_FunktionBit(byte Nr, byte FunktionNr)
{
	return MeineZüge[Nr].Get_Funktion(FunktionNr);
}

bool CDatenBankLok::Get_Lok_FunktionSound(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZüge[Nr].Decoder_Data.Get_Sound_FuntionsTaste();
	return MeineZüge[Nr].Get_Funktion(Funktion_Nr);
}

void CDatenBankLok::Set_Lok_FunktionSound(byte Nr, bool bit)
{
	MeineZüge[Nr].Set_Funktion_Sound(bit);
}

bool CDatenBankLok::is_Lok_Sound(byte Nr)
{
	return MeineZüge[Nr].Decoder_Data.isDecoderType(DecoderTypen::Massoth_Sound);
}

bool CDatenBankLok::Get_Lok_FunktionRangier(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZüge[Nr].Decoder_Data.Get_Rangier_FunktionTaste();
	return MeineZüge[Nr].Get_Funktion(Funktion_Nr);
}

void CDatenBankLok::Set_Lok_FunktionRangier(byte Nr, bool bit)
{
	MeineZüge[Nr].Set_Funktion_Rangieren(bit);
}

bool CDatenBankLok::Get_Lok_FunktionZeit(byte Nr)
{
	byte Funktion_Nr;
	Funktion_Nr = MeineZüge[Nr].Decoder_Data.Get_VerzögZ_FunktionTaste();
	return MeineZüge[Nr].Get_Funktion(Funktion_Nr);

}

void CDatenBankLok::Set_Lok_FunktionZeit(byte Nr, bool bit)
{
	MeineZüge[Nr].Set_Funktion_VerzögerungsZeit(bit);
}

void CDatenBankLok::Set_Lok_Geschwindigkeit(byte Nr, byte Speed, bool Dir, bool Stop)
{
	if (Stop)	MeineZüge[Nr].Set_Stop();
	else
	{
		if(Speed == 0 )
		{
			MeineZüge[Nr].Set_Halt();
		}
		else
		{
			MeineZüge[Nr].Set_Geschwindigkeit(Speed, Dir);
		}
	}
}

CString CDatenBankLok::Get_Lok_Name(byte Nr)
{
	return MeineZüge[Nr].Name;
}

HBITMAP CDatenBankLok::Get_Lok_Image(byte Nr)
{
	return MeineZüge[Nr].Bild;
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
	return MeineZüge[Selected_Prog_Lok].Decoder_Data.Get_Decoder_Type(); 
}

CDecoderInfo CDatenBankLok::Get_Sel_Zug_Decoder()
{
	return MeineZüge[Selected_Prog_Lok].Decoder_Data;
}

void CDatenBankLok::Set_Sel_Zug_Decoder_Funktion_Name(byte Nr, CString Name)
{
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Set_FunktionBechreibung(Nr, Name);
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	Do_Save_Data = true;
}

void CDatenBankLok::Set_Sel_Zug_Decoder_Funktion_Eigen(byte Nr, bool view, bool change)
{
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Set_FunktionEigenschaften(Nr, view, change);
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	Do_Save_Data = true;
}

void CDatenBankLok::Update_Sel_Zug_FunktionsTasten()
{
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	MeineZüge[Selected_Prog_Lok].Decoder_Data.SortFunktion();
	Do_Save_Data = true;
}

void CDatenBankLok::Sort_Sel_Zug_FunktionsTasten()
{
	MeineZüge[Selected_Prog_Lok].Decoder_Data.Fill_Tasten_Info();
	MeineZüge[Selected_Prog_Lok].Decoder_Data.SortFunktion();
}


byte CDatenBankLok::PRG_Lok_CV(bool ReadWrite, byte CV, byte Wert) // true dann schreiben
{
 return	MeineZüge[Selected_Prog_Lok].Progmmiere_RW_CV(ReadWrite, CV, Wert);
}


byte CDatenBankLok::PRG_Get_CV(byte CV)
{
	return MeineZüge[Selected_Prog_Lok].Decoder_Data.Get_CV_Wert(CV);
}
