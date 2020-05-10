#pragma once
#include"pch.h"
#include"CDataXpressNet.h"
#include<vector>


class CDatenBankLok
{
public:
	CDatenBankLok();
	~CDatenBankLok();
	byte Init();
	CXpressNetCom *Get_XpressNet();
	void Test_Data();
	void Load_Zug_Data();
	void Save_Zug_Data();

	void Load_acktiv_Zug_Data();
	void Save_acktiv_Zug_Data();
	void Sound_acktive_Zug(bool SW);

	void SetAktiv_Lok(CString InText);
	byte Get_max_Aktiv_Loks();
	
	CDataXpressNet &Get_aktiveLok_Pointer(byte Nr);
	bool is_Zug_Pointer(byte Nr);
	void Ask_aktivLok_Data(byte Lok_Nr);
	void New_Lok_Data();
	

	void Fill_Liste_Zug(CComboBox* Data, DecoderTypen Decoder_Type);
	void Fill_Liste_Zug(CComboBox* Data);
	void Fill_Act_Liste_Zug(CComboBox* Data);

	std::vector<CString> Get_LokGruppe_Decoder(DecoderTypen Decoder);
	byte Get_Pos_LokName(CString LName);
	std::vector <Function_A> Get_Zug_ActivFunktion_Pointer(byte Nr);
	void Set_aktiveLok_Startbedingung(byte Nr, Zug_Status Status);
	Zug_Status Get_aktiveLok_Status(byte Nr);
	void Set_aktiveLoks_aufsGleis();
	void Set_aktiveLok_FuntiontoRun();
	bool Get_aktiveLok_FunktionBit(byte Nr, byte FunktionNr);
	bool Get_aktiveLok_FunktionSound(byte Nr);
	void Set_aktiveLok_FunktionSound(byte Nr, bool bit);
	bool is_aktiveLok_Sound(byte Nr);
	bool Get_aktiveLok_FunktionRangier(byte Nr);
	void Set_aktiveLok_FunktionRangier(byte Nr, bool bit);
	bool Get_aktiveLok_FunktionZeit(byte Nr);
	void Set_aktiveLok_FunktionZeit(byte Nr, bool bit);
	void Set_aktiveLok_Geschwindigkeit(byte Nr, byte Speed, bool Dir, bool Stop);
	CString Get_aktiveLok_Name(byte Nr);
	HBITMAP	Get_aktiveLok_Image(byte Nr);
	bool Set_Zug_Dlg_Nr(byte Nr);
	void Set_Prog_Zug(byte Zug);
	void Set_Zug_CV_Wert(byte CV, byte CV_Wert);
	byte Get_Prog_Zug();
	DecoderTypen Get_Sel_ZugDecoder_Type();
	CDecoderInfo Get_Sel_Zug_Decoder();
	void Set_Sel_Zug_Decoder_Funktion_Name(byte Nr, CString Name);
	void Set_Sel_Zug_Decoder_Funktion_Eigen(byte Nr, bool view, bool change);
	void Update_Sel_Zug_FunktionsTasten();
	void Sort_Sel_Zug_FunktionsTasten();
	void PRG_Lok_CV(bool ReadWrite, byte CV, byte Wert);
	void PRG_Set_CV();
	byte PRG_Get_CV(byte CV);

	std::vector<CDataXpressNet> MeineZüge;


protected:
	std::vector<byte> aktiveLoks;
	byte			Anzahl_Dlg_Run = 0;
	byte			Ask_Lok_Nr = 0;
	byte			Selected_Prog_Lok = 0;
	byte			Prog_CV = 0;
	bool			Zug_wartet_auf_Data = false;
	bool			Do_Save_Data = false;
	CXpressNetCom   *XpressNet = NULL;
};

