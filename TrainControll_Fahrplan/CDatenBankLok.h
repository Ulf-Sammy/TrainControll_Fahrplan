#pragma once
#include"pch.h"
#include"CDataXpressNet.h"
#include<vector>


class CDatenBankLok
{
public:
	CDatenBankLok();
	~CDatenBankLok();

	void Load_Zug_Data();
	void Save_Zug_Data();

	void Load_acktiv_Zug_Data();
	void Save_acktiv_Zug_Data();

	void Verbinde_Zug_zu_XpNet(byte Nr);
	void Sound_acktive_Zug(bool SW);

	byte Get_max_Aktiv_Loks();
	
	CDataXpressNet &Get_Lok_Pointer(CString LName);
	CDataXpressNet &Get_aktiveLok_Pointer(byte Nr);

	void Ask_for_Lok_Data(byte Lok_Nr);

	void Fill_Liste_Zug(CComboBox* Data, DecoderTypen Decoder_Type);
	void Fill_Liste_Zug(CComboBox* Data);
	void Fill_Liste_Zug(CComboBox* Data, CString Name);
	void Fill_Act_Liste_Zug(CComboBox* Data);

	std::vector<CString> Get_LokGruppe_Decoder(DecoderTypen Decoder);
	byte Get_Pos_LokName(CString LName);
	std::vector <Function_A> Get_Zug_ActivFunktion_Pointer(byte Nr);
	void Set_Lok_Startbedingung(byte Nr, Zug_Status Status);
	Zug_Status Get_Lok_Status(byte Nr);
	
	void Set_aktiveLok_FuntiontoRun();
	void Set_Funktion(byte Nr, FahrplanPos Befehl);
	bool Get_Lok_FunktionBit(byte Nr, byte FunktionNr);
	bool Get_Lok_FunktionSound(byte Nr);
	void Set_Lok_FunktionSound(byte Nr, bool bit);
	bool is_Lok_Sound(byte Nr);
	bool Get_Lok_FunktionRangier(byte Nr);
	void Set_Lok_FunktionRangier(byte Nr, bool bit);
	bool Get_Lok_FunktionZeit(byte Nr);
	void Set_Lok_FunktionZeit(byte Nr, bool bit);
	void Set_Lok_Geschwindigkeit(byte Nr, byte Speed, bool Dir, bool Stop);
	CString Get_Lok_Name(byte Nr);
	HBITMAP	Get_Lok_Image(byte Nr);
	void Set_Prog_Zug(byte Zug);
	void Set_Zug_CV_Wert(byte CV, byte CV_Wert);
	byte Get_Prog_Zug();
	DecoderTypen Get_Sel_ZugDecoder_Type();
	CDecoderInfo Get_Sel_Zug_Decoder();
	void Set_Sel_Zug_Decoder_Funktion_Name(byte Nr, CString Name);
	void Set_Sel_Zug_Decoder_Funktion_Eigen(byte Nr, bool view, bool change);
	void Update_Sel_Zug_FunktionsTasten();
	void Sort_Sel_Zug_FunktionsTasten();
	byte PRG_Lok_CV(bool ReadWrite, byte CV, byte Wert);

	byte PRG_Get_CV(byte CV);

	std::vector<CDataXpressNet> MeineZüge;
	std::vector<Start_Lok_Block> HomeZüge;

protected:
	std::vector<byte> Loks_in_Schuppen;
	byte			Ask_Lok_Nr = 0;
	byte			Selected_Prog_Lok = 0;
	byte			Prog_CV = 0;
	bool			Do_Save_Data = false;
};

