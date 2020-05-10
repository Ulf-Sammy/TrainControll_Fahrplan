#pragma once
#include "pch.h"
#include <vector>

struct CV_Info
{
	CString Beschreibung;
	CString StWert;
	CString Wertebereich;
	CString Bemerkung;
	byte CV;
	bool Bit;
	byte Anhang;
};

struct Function_A
{
public:
	Function_A(byte T, CString B)
	{
		Taste = T;
		Beschreibung = B;
	}
	void Add_Name(CString AddName)
	{
		Beschreibung = Beschreibung + _T(" | ") + AddName;
	}
	CString Text_Beschreibung()
	{
		return Beschreibung;
	}
	CString Text_Taste()
	{
		CString Txt;
		Txt.Format(_T("F%02i"), Taste);
		return Txt;
	}
	byte Get_Taste()
	{
		return Taste;
	}
protected:
	byte Taste;
	CString Beschreibung;
};
struct Funktion_Taste
{
	byte CV;
	CString Ausgang;
	CString Funktion;
	bool aendern;
	bool zeigen;
	byte neuTaste;
	byte Taste;
};

struct CV_Zug
{
   CString Name;
   byte maxCV;
   byte type;
   std::vector <byte> CV[256];
   std::vector <Funktion_Taste> FT[35];
};

struct Data_Funktion
{
	Data_Funktion()
	{
		TasteCV = 0;
		Zeigen = true;
		Andern = true;
		Name.Empty();
		Taste = 0;
		Decoder = 0;
	}
	Data_Funktion(byte CV)
	{
		TasteCV = CV;
		Zeigen = true;
		Andern = true;
		Name.Empty();
		Taste = 0;
		Decoder = 0;
	}
	Data_Funktion(CString Text)
	{
		LoadText(Text);
	}
	void Set_Name(CString Text)
	{
		Name = Text;
	}
	CString TextTaste()
	{
		CString TMP;
		TMP.Format(_T("F%02i"),Taste);
		return TMP;
	}
	CString Text_CV() 
	{
		CString TMP;
		TMP.Format(_T("CV[ %02i ]"), TasteCV);
		return TMP;
	}
	CString Text_Zeigen() 
	{
		CString TMP;
		if (Zeigen)	TMP.Format(_T("on"));
		else     	TMP.Format(_T("--"));
		return TMP;
	}
	CString Text_Name() 
	{
		return Name;
	}
	CString Text_AusgangInfo()
	{
		return AusgangInfo;
	}
	CString SaveText(byte CV, CString Bemerkung)
	{
		CString Text;
		Name.TrimLeft();
		Name.TrimRight();
		Text.Format(_T("Fuk CV: %03i %1d%1d |%30s|%s\n"), CV, Zeigen, Andern, (LPCTSTR)Name, (LPCTSTR)Bemerkung);
		return Text;
	}
	void LoadText(CString Text)
	{
		{
			TasteCV = _ttoi(Text.Mid(8, 3));
			if (Text[12] == '1') Zeigen = true;
			else                 Zeigen = false;
			if (Text[13] == '1') Andern = true;
			else				Andern = false;
			Name = Text.Mid(16, 30);
			Name.TrimRight();
			Name.TrimLeft();
		}
	}
	void Set_Decoder(byte Dec)
	{
		Decoder = Dec;
	}
	byte Get_CVofTaste()
	{
	}
	byte    Decoder; // 0 = Tams, 1= Massoth-Sound , 2 Massoth-normal
	bool    Zeigen;	// anzeigen der Funktion
	bool    Andern;	// ändern der Funktion möglich
	byte    TasteCV;  // CV-Adresse für FunktionsTasten
	byte	Taste;    // Nummer der Funktionstaste
	CString Name;		// Namen zu den Funktionen
	CString AusgangInfo;
protected:

};
class CDecoderInfo_CV 
{
public:
	CDecoderInfo_CV (){}
	CDecoderInfo_CV(byte cv) { CV = cv; }
	~CDecoderInfo_CV (){}
	byte Get_CV()            { return CV; }
	byte Get_Anhang()        { return Anhang; }
	CString Text_CV()			
	{ 
		CString TMP;
		TMP.Format(_T("%03i"), CV);
		return CString(TMP);
	}
	CString Text_Beschreibung() { return Beschreibung;  }
	CString Text_Standard_Wert(){ return Standard_Wert; }
	CString Text_Wertebereich() { return Wertebereich;  }
	CString Text_Bemerkung()    { return Bemerkung;     }
	byte Set_Werte(CString InText)
	{
		CV            = _ttoi(InText.Mid(3, 3));
		Anhang        = _ttoi(InText.Mid(0, 2));
		Beschreibung  = InText.Mid(7, (64 - 7));
		Standard_Wert = InText.Mid(64, 3);
		Wertebereich  = InText.Mid(68, 9);
		Bemerkung     = InText.Mid(77, InText.GetLength());
		return CV;
	}
private:
	byte CV;
	byte Anhang;
	CString Beschreibung;
	CString Standard_Wert;
	CString Wertebereich;
	CString Bemerkung;
};

class CDecoder_All_Infos
{
public:
	CDecoder_All_Infos( )
	{
		for(int i =0 ; i< 256 ; i++)
		{
			Docu_Data.push_back(0x00);
		}
		Decoder_Type = 0;
	}
	void Set_DecoderType(DecoderTypen Type)
	{
		Decoder_Type = byte(Type);

	}
	void Load_Data()
	{
		LPCTSTR			pszPathName;
		CStdioFile		file;
		CFileException	ex;
		CString			strIn;
		byte			ReadCV;
		CDecoderInfo_CV ReadData;

		pszPathName = ConfigFile.at(Decoder_Type);

		if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			ex.ReportError();
			return;
		}

		file.ReadString(strIn); // Überschrift
		file.ReadString(strIn); // Weiche_Type Decoder
		file.ReadString(strIn); // Tabellen Überschrift
		do
		{
			file.ReadString(strIn);
			ReadCV = ReadData.Set_Werte(strIn);
			Docu_Data.at(ReadCV) = ReadData;
		} while (strIn.Mid(0,4) != "ENDE");
		file.Close();
	}
	std::vector<byte> Pointer_MaxCV() 
	{
		switch (Decoder_Type)
		{
		case 0:
			return CV_Tams_LDG34;
			break;
		case 1:
			return CV_Massoth_Normal;
			break;
		case 2:
			return CV_Massoth_Sound;
			break;
		default:
			return CV_No_Decoder;
			break;
		}
	}
	std::vector <CDecoderInfo_CV> Docu_Data;
private:
	const std::vector<CString> ConfigFile{ _T("Daten\\Tams_LD_G_34.TXT"),_T("Daten\\Massoth_NoSound.TxT") ,_T("Daten\\Massoth_Sound.TxT") };

	const std::vector<byte> CV_Tams_LDG34{ 1,2,3,4,5,7,8,9,12,13,17,18,19,27,29,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,112,113,115,116,118,119,120,121,122,123,124,126 };
	const std::vector<byte> CV_Massoth_Sound{ 1,2,3,4,5,6,7,8,9,13,15,16,17,18,19,29,37,38,39,40,41,42,43,44,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,64,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,
										   87,88,89,90,91,92,93,94,107,108,112,113,114,115,116,117,118,119,120,121,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,139,140,141,142,147,148,149,150,
										  151,152,153,154,156,157,158,159,160,161,162,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,184,185,186,187,188,189,190,191,192,192,193,194,195,196,197,198,200,
										  201,202,203,204,205,206,207,208,209,210,211,212,217,218,219,220,221,222,223,224,229,230,231,232,233,234,235,236,255 };
	const std::vector<byte> CV_Massoth_Normal{ 1,2,3,4,5,6,7,8,9,13,15,16,17,18,19,29,37,38,39,40,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,
										   87,88,89,90,91,92,93,94,112,113,114,115,116,117,118,119,120,121,122,123,127,128,255 };
	const std::vector<byte> CV_No_Decoder{ 0,0,0,0,9 };
	DecoderTypen Type;
	byte Decoder_Type;
};

class CDecoderInfo
{
public:
	CDecoderInfo();
	~CDecoderInfo();
	void SetType(DecoderHersteller Herst, DecoderTypen Type, byte Sub);
	void Save_Decoder(CStdioFile *File);
	void Set_Decoder(CStdioFile *File);
	void Fill_Tasten_Info();
	void Fill_Tasten_DlgRun();
	void SortFunktion();
	
	CString Text_Decoder_Type();
	CString Text_Decoder_Hersteller();
	CString Text_Decoder_Soft_Version();
	CString Text_Adresse();
	CString Text_CVWert(byte CV);
	CString FunktionBechreibung(byte Nr);
	void Set_FunktionBechreibung(byte Nr, CString Name);
	void Set_FunktionEigenschaften(byte Nr, bool View, bool Change);
	byte Get_Sound_FuntionsTaste();
	byte Get_Rangier_FunktionTaste();
	byte Get_VerzögZ_FunktionTaste();
	void Set_Sound_FunktionWert(bool Bit);
	void Set_Rangier_FunktionWert(bool Bit);
	void Set_VerzögZ_FunktionWert(bool Bit);
	DecoderTypen Get_Decoder_Type();
	byte Get_Adresse();
	byte Get_CV_Wert(byte CV);
	void Set_CV_Wert(byte CV, byte Wert);
	bool isDecoderType(DecoderTypen Ty);
	std::vector<byte> Pointer_Funktion();
	std::vector<Data_Funktion> Get_Funktion();
	std::vector <Function_A> Get_Aktive_Funktion();
protected:
	const std::vector<CString> NameHersteller{ _T("Tams") ,_T("Massoth") };
	const std::vector<CString> ConfigFile{ _T("Tams_LD_G_34.TXT"),_T("Massoth_NoSound.TxT") ,_T("Massoth_Sound.TxT") };
	const std::vector<CString> NameTypeDecoder{ _T("0. Tams LD-G-34     "), _T("1. Massoth Sound."), _T("2. Massoth ohne Sound  ") };
	// Decoder == 0 ==========================================================================================
	const std::vector<byte> CV_Taste_Tams_LDG34{ 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 };
	const std::vector<CString> Funk_Beschreibung_Tams_N{ _T("F0 vor"), _T("F0 rück"), _T("F01"), _T("F02"), _T("F03"), _T("F04"), _T("F05"),_T("F06"),_T("F07"),_T("F08"),_T("F09"),_T("F10"),_T("F11"),_T("F12") };
	// Decoder == 1 ===================================================================================================
	const std::vector<byte> CV_Taste_Massoth_Normal{ 54, 56,113,115,117,119, 59, 64 }; 
	const std::vector<CString> Funk_Beschreibung_Massoth_N{ _T("AUX_1"),_T("AUX_2"),_T("AUX_3"),_T("AUX_4"),_T("AUX_5"),_T("AUX_6"),_T("Rangiergang"),_T("Verzögerungs an/aus") };
	// Decoder == 2 ===================================================================================================
	const std::vector<byte> CV_Taste_Massoth_Sound  { 54, 56,113,115,117,119,121,123,131,132,133,134,135,136,137,138,139,140,141,142, 59, 64,147,148 }; // Massoth Sound
	const std::vector<CString> Funk_Beschreibung_Massoth_S{ _T("AUX_1"),_T("AUX_2"),_T("AUX_3"),_T("AUX_4"),_T("AUX_5"),_T("AUX_6"),_T("AUX_7"),_T("AUX_8"),_T("Sound_1"),_T("Sound_2"),_T("Sound_3"),_T("Sound_4"),_T("Sound_5"),_T("Sound_6"),_T("Sound_7"),_T("Sound_8"),_T("Sound_9"),
												   _T("Sound_10"),_T("Sound_11"),_T("Sound_12"),_T("Rangiergang"),_T("Verzögerungs an/aus") ,_T("Sound an/aus"),_T("Zylindergeräusch") };

	
//	const std::vector<CString> NameTypeDecoder{ _T("0. Tams LD-G-34     "), _T("1. Massoth XL Sound."), _T("2-5. Massoth LS Sound  "),_T("3-2. Massoth XL       "),_T("4-4. Massoth L       ") };
	const std::vector<byte> SoundCVDecodertype  { 133,135,144,145,148,157,158 };
	const std::vector<CString> SoundDecoderName { _T("eMOTION XL II PluG"),_T("eMOTION XXL II"), _T("eMOTION LS"), _T("eMOTION S"),_T("eMOTION XLS"),_T("eMOTION XLS - Onboard"),_T("eMOTION XLS - M1") };

	DecoderTypen      Decoder_Type;       // 1 verschiedene
	byte              Decoder_Type_Sub;   // Untertypen 
	DecoderHersteller Decoder_Hersteller; // 2 Hersteller 

	void Set_Funktion();
	void Fill_Tasten_Tams(bool noDLG);
	void Fill_Tasten_Massoth(byte Taste, byte i);
	std::vector<byte> CV_Wert;
	std::vector< Data_Funktion> Funktion;
	std::vector <Function_A> Active_Funktionen;
};
