#include "pch.h"
#include "VC_TableInfo.h"

CDecoderInfo::CDecoderInfo(void)
{
}

CDecoderInfo::~CDecoderInfo(void)
{
}

void CDecoderInfo::SetType(DecoderHersteller Herst, DecoderTypen Type, byte Sub)
{
	Decoder_Hersteller = Herst;
	Decoder_Type       = Type;
	Decoder_Type_Sub   = Sub;
	for (int i = 0; i < 256; i++)
	{
		CV_Wert.push_back(i);
	}
	if (Decoder_Type == DecoderTypen::Tams_Normal) //Tams no Sound
	{
		for (byte v : CV_Taste_Tams_LDG34)
		{			Funktion.push_back(Data_Funktion(v));		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_Sound) //Massoth Sound
	{
		for (byte v : CV_Taste_Massoth_Sound)
		{			Funktion.push_back(Data_Funktion(v));		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_No_Sound) //Massoth no Sound
	{
		for (byte v : CV_Taste_Massoth_Normal)
		{			Funktion.push_back(Data_Funktion(v));		}
	}
}

void CDecoderInfo::Save_Decoder(CStdioFile *File)
{
	CString Text;
	int I = 0;
	Text.Format(_T("Dectype: %2i %2i %2i \n"), Decoder_Hersteller, Decoder_Type, Decoder_Type_Sub);
	File->WriteString(Text);
	File->WriteString(_T("===============================================\n"));
	if (Decoder_Type == DecoderTypen::Tams_Normal) //Tams no Sound
	{
		for (byte v : CV_Taste_Tams_LDG34)
		{
			File->WriteString(Funktion.at(I).SaveText(v, Funk_Beschreibung_Tams_N[I]));
			I++     ;
		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_Sound) //Massoth Sound
	{
		for (byte v : CV_Taste_Massoth_Sound)
		{
			File->WriteString(Funktion.at(I).SaveText(v, FunktionBechreibung(I)));
			I++;
		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_No_Sound) //Massoth no Sound
	{
		for (byte v : CV_Taste_Massoth_Normal)
		{
			File->WriteString(Funktion.at(I).SaveText(v, FunktionBechreibung(I)));
			I++;
		}
	}
	File->WriteString(_T("===============================================\n"));
	for (int i = 0; i < 32; i++)
	{
		Text.Format(_T("%03d %03d %03d %03d %03d %03d %03d %03d\n"), 
					CV_Wert[(    i * 8)], CV_Wert[1 + (i * 8)], CV_Wert[2 + (i * 8)], CV_Wert[3 + (i * 8)],
					CV_Wert[4 + (i * 8)], CV_Wert[5 + (i * 8)], CV_Wert[6 + (i * 8)], CV_Wert[7 + (i * 8)]);
		File->WriteString(Text);
	}
}

CString CDecoderInfo::FunktionBechreibung(byte Nr)
{
	
	if (Decoder_Type == DecoderTypen::Massoth_Sound) //Massoth Sound
	{
		return(Funk_Beschreibung_Massoth_S[Nr]);
	}
	if (Decoder_Type == DecoderTypen::Massoth_No_Sound) //Massoth no Sound
	{
		return(Funk_Beschreibung_Massoth_N[Nr]);
	}
	return _T("-");
}

void CDecoderInfo::Set_FunktionBechreibung(byte Nr, CString Name)
{
	Funktion.at(Nr).Set_Name(Name);
}

void CDecoderInfo::Set_FunktionEigenschaften(byte Nr, bool View, bool Change)
{
	Funktion.at(Nr).Andern = Change;
	Funktion.at(Nr).Zeigen = View;
}

byte CDecoderInfo::Get_Sound_FuntionsTaste()
{
	switch (Decoder_Type)
	{
	case DecoderTypen::Tams_Normal:
		return 0;
		break;
	case DecoderTypen::Massoth_No_Sound:
		return 0;
		break;
	case DecoderTypen::Massoth_Sound:
		return CV_Wert[147];
		break;
	case DecoderTypen::NoName:
		return 0;
		break;
	}
	return 0;
}


byte CDecoderInfo::Get_Rangier_FunktionTaste()
{
	switch (Decoder_Type)
	{
	case DecoderTypen::Tams_Normal:
		return 3;
		break;
	case DecoderTypen::Massoth_No_Sound:
		return CV_Wert[59];
		break;
	case DecoderTypen::Massoth_Sound:
		return CV_Wert[59];
		break;
	case DecoderTypen::NoName:
		return 0;
		break;
	}
	return 0;
}

byte CDecoderInfo::Get_VerzögZ_FunktionTaste()
{
	switch (Decoder_Type)
	{
	case DecoderTypen::Tams_Normal:
		return 4;
		break;
	case DecoderTypen::Massoth_No_Sound:
		return CV_Wert[64];
		break;
	case DecoderTypen::Massoth_Sound:
		return CV_Wert[64];
		break;
	case DecoderTypen::NoName:
		return 0;
		break;
	}
	return 0;
}

void CDecoderInfo::Set_Sound_FunktionWert(bool Bit)
{
}

void CDecoderInfo::Set_Rangier_FunktionWert(bool Bit)
{
}

void CDecoderInfo::SortFunktion()
{
	std::vector <Function_A> Tmp_Funktionen;
	Tmp_Funktionen = Active_Funktionen;
	Active_Funktionen.clear();
	for (int i = 0 ; i < 29; i++)
	{
		for (Function_A F : Tmp_Funktionen)
		{
			if (F.Get_Taste() == i)
			{
				Active_Funktionen.push_back(F);
			}
		}
	}
}

void CDecoderInfo::Set_VerzögZ_FunktionWert(bool Bit)
{
}

DecoderTypen CDecoderInfo::Get_Decoder_Type()
{
	return Decoder_Type;
}

void CDecoderInfo::Set_Decoder(CStdioFile *File)
{   //01234567890123456789
	//Dec Nr : 00 00 00
	CString InText;
	byte Nr;
	File->ReadString(InText);
	
	Decoder_Hersteller = (DecoderHersteller) _ttoi(InText.Mid( 9, 2)); // 2 Hersteller 
	Decoder_Type       = (DecoderTypen)      _ttoi(InText.Mid(12, 2)); // 3 verschiedene
	Decoder_Type_Sub   = _ttoi(InText.Mid(15, 2)); // Untertypen 
	File->ReadString(InText);
	if (Decoder_Type == DecoderTypen::Tams_Normal) //Tams no Sound
	{
		Nr = 0;
		for (byte v : CV_Taste_Tams_LDG34)
		{
			File->ReadString(InText);
			Funktion.push_back(Data_Funktion(InText));
			Funktion[Nr].AusgangInfo = Funk_Beschreibung_Tams_N[Nr];
			Nr++;
		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_Sound) //Massoth Sound
	{
		Nr = 0;
		for (byte v : CV_Taste_Massoth_Sound)
		{
			File->ReadString(InText);
			Funktion.push_back(Data_Funktion(InText));
			Funktion[Nr].AusgangInfo = Funk_Beschreibung_Massoth_S[Nr];
			Nr++;
		}

	}
	if (Decoder_Type == DecoderTypen::Massoth_No_Sound) //Massoth no Sound
	{
		Nr = 0;
		for (byte v : CV_Taste_Massoth_Normal)
		{
			File->ReadString(InText);
			Funktion.push_back(Data_Funktion(InText));
			Funktion[Nr].AusgangInfo = Funk_Beschreibung_Massoth_N[Nr];
			Nr++;
		}
	}
	File->ReadString(InText); // ============
	for (int i = 0; i < 32; i++)
	{
		File->ReadString(InText);
		
		CV_Wert.push_back(_ttoi(InText.Mid( 0, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid( 4, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid( 8, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid(12, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid(16, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid(20, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid(24, 3)));
		CV_Wert.push_back(_ttoi(InText.Mid(28, 3)));
	}
	Set_Funktion();
}

void CDecoderInfo::Fill_Tasten_Info()
{
	int     i = 0;
	CString Text;

	if (Decoder_Type == DecoderTypen::Tams_Normal)	{ Fill_Tasten_Tams(true);}

	if (Decoder_Type == DecoderTypen::Massoth_No_Sound)
	{
		Active_Funktionen.clear();
		Active_Funktionen.push_back(Function_A(0, _T("Licht")));

		for (byte Nr : CV_Taste_Massoth_Normal)
		{
			Fill_Tasten_Massoth(CV_Wert[Nr], i);
			i++;			
		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_Sound)
	{
 		Active_Funktionen.clear();
		Active_Funktionen.push_back(Function_A(0, _T("Licht")));

		for (byte Nr : CV_Taste_Massoth_Sound)
		{
			Fill_Tasten_Massoth(CV_Wert[Nr], i);
			i++;
		}
	}

}

void CDecoderInfo::Fill_Tasten_DlgRun()
{
	int     i = 0;
	CString Text;

	if (Decoder_Type == DecoderTypen::Tams_Normal) { Fill_Tasten_Tams(false); }

	if (Decoder_Type == DecoderTypen::Massoth_No_Sound)
	{
		Active_Funktionen.clear();
		Active_Funktionen.push_back(Function_A(0, _T("Licht")));

		for (byte Nr : CV_Taste_Massoth_Normal)
		{
			if ((Nr != 59) || (Nr != 64))
			{
				Fill_Tasten_Massoth(CV_Wert[Nr], i);
				i++;
			}
		}
	}
	if (Decoder_Type == DecoderTypen::Massoth_Sound)
	{
		Active_Funktionen.clear();
		Active_Funktionen.push_back(Function_A(0, _T("Licht")));

		for (byte Nr : CV_Taste_Massoth_Sound)
		{
			if ((Nr != 59) && (Nr != 64) && (Nr != 147))
			{
				Fill_Tasten_Massoth(CV_Wert[Nr], i);
				i++;
			}
		}
	}
	SortFunktion();
}

void CDecoderInfo::Fill_Tasten_Tams(bool noDLG)
{
	CString AUX_Name;
	bool gefunden;
	byte Bit;
	byte i = 0;

	Active_Funktionen.clear();
	
	for (byte Nr : CV_Taste_Tams_LDG34)
	{
		gefunden = false;
		switch (CV_Wert[Nr])
		{
		case 0:
			gefunden = false;
			AUX_Name = "";
			break;
		case 1:
		case 2:
			gefunden = true;
			AUX_Name = Funktion[0].Name;
			break;
		case 4:
			gefunden = true;
			AUX_Name = Funktion[1].Name;
		break;
		case 8:
			gefunden = true;
			AUX_Name = Funktion[2].Name;
			break;
		case 16:
			gefunden = true;
			AUX_Name = Funktion[3].Name;
			break;
		case 32:
			gefunden = true;
			AUX_Name = Funktion[4].Name;
			break;

		default:
			break;
		}
		if ((Nr > 33) &&(gefunden))
		{
			if (Funktion[i].Zeigen)
			{
				Bit = Nr - 34;
				Active_Funktionen.push_back(Function_A(Bit, AUX_Name));
			}
			i++;
		}
	}
	if (noDLG)
	{
		if (CV_Wert[49] & 0x02) Active_Funktionen.push_back(Function_A(1, _T("Rangiergang")));
		if (CV_Wert[49] & 0x04) Active_Funktionen.push_back(Function_A(2, _T("Rangiergang")));
		if (CV_Wert[49] & 0x08) Active_Funktionen.push_back(Function_A(3, _T("Rangiergang")));
		if (CV_Wert[49] & 0x10) Active_Funktionen.push_back(Function_A(4, _T("Rangiergang")));

		if (CV_Wert[49] & 0x20) Active_Funktionen.push_back(Function_A(3, _T("Verzögerungen schalten")));
		if (CV_Wert[49] & 0x40) Active_Funktionen.push_back(Function_A(4, _T("Verzögerungen schalten")));
	}
}

void CDecoderInfo::Fill_Tasten_Massoth(byte Taste, byte i)
{
	if (Funktion[i].Zeigen)
	{
		if (Taste > 0)
		{
			for (int f = 0; f < Active_Funktionen.size(); f++)
			{
				if (Active_Funktionen[f].Get_Taste() == Taste)
				{
					Active_Funktionen[f].Add_Name(Funktion[i].Name);
					return;
				}
			}
			Active_Funktionen.push_back(Function_A(Taste, Funktion[i].Name));
		}
	}
}



CString CDecoderInfo::Text_Decoder_Type()
{
	return NameTypeDecoder[byte(Decoder_Type)];
}
CString CDecoderInfo::Text_Decoder_Hersteller()
{
	CString TMP;
	TMP.Format(_T(" Hersteller Type: %2i"), CV_Wert[1]);
	if (CV_Wert[8] == 8) TMP = _T("noch nicht gelesen!");

	if (CV_Wert[8] ==  62) TMP = _T("Tams");
	if (CV_Wert[8] == 123) TMP = _T("Massoth");
	if (CV_Wert[8] == 159) TMP = _T("LGB");

	return TMP;
}
CString CDecoderInfo::Text_Decoder_Soft_Version()
{
	CString TMP;
	TMP.Format(_T("V.%2i"), CV_Wert[7]);
	return TMP;
}
CString CDecoderInfo::Text_Adresse()
{
	CString TMP;
	TMP.Format(_T(" Adr: %2i"), CV_Wert[1]);
	return TMP;
}
CString CDecoderInfo::Text_CVWert(byte CV)
{
	CString TMP;
	TMP.Format(_T("%3i"), CV_Wert[CV]);
	return TMP;
}

byte CDecoderInfo::Get_Adresse()
{
	return CV_Wert[1];
}
byte CDecoderInfo::Get_CV_Wert(byte CV)
{
	return CV_Wert[CV];
}
void CDecoderInfo::Set_CV_Wert(byte CV, byte Wert)
{
	CV_Wert[CV] = Wert;
}

bool CDecoderInfo::isDecoderType(DecoderTypen Ty)
{
	return (Ty == Decoder_Type);
}

std::vector<byte> CDecoderInfo::Pointer_Funktion()
{
	switch (Decoder_Type)
	{
	case DecoderTypen::Tams_Normal:
		return CV_Taste_Tams_LDG34;
		break;
	case DecoderTypen::Massoth_No_Sound:
		return CV_Taste_Massoth_Normal;
		break;
	case DecoderTypen::Massoth_Sound:
		return CV_Taste_Massoth_Sound;
		break;
	case DecoderTypen::NoName:
		break;
	}

	return std::vector<byte>();
}

std::vector<Data_Funktion> CDecoderInfo::Get_Funktion()
{
	return Funktion;
}

std::vector<Function_A> CDecoderInfo::Get_Aktive_Funktion()
{
	return Active_Funktionen;
}

void CDecoderInfo::Set_Funktion()
{
	int nr = 0;
	switch (Decoder_Type)
	{
	case DecoderTypen::Tams_Normal:
		for (byte F : CV_Taste_Tams_LDG34)
		{
			Funktion[nr].Taste = F - 34;
			if (F == 33) Funktion[nr].Taste = 0;
			Funktion[nr].TasteCV = F;
			nr++;
		}
		break;
	case DecoderTypen::Massoth_No_Sound:
		for (byte F : CV_Taste_Massoth_Normal)
		{
			Funktion[nr].Taste = CV_Wert[Funktion[nr].TasteCV];
			nr++;
		}
		break;
	case DecoderTypen::Massoth_Sound:
		for (byte F : CV_Taste_Massoth_Sound)
		{
			Funktion[nr].Taste = CV_Wert[Funktion[nr].TasteCV];
			nr++;
		}
		break;
	default:
		break;
	}
}


