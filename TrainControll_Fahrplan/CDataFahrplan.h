#pragma once
#include <vector>
#include "pch.h"

class FahrplanPos
{
public:
	byte Melder = 0;
	FahrPlanDo mache = FahrPlanDo::letzte_Zeile;
	int WertA = 0; 
	int WertB = 0;
	TrainCon_Paar Weg[4];
	CString Bemerkung;
	FahrplanPos()
	{};
	FahrplanPos(byte Me, FahrPlanDo Do, int WA, int WB)
	{
		Melder = Me;
		mache = Do;
		WertA = WA;
		WertB = WB;
	};
	FahrPlanDo Get_Was()
	{
		return mache;
	}
	byte Get_Melder()
	{
		return Melder;
	}
	byte GetGeschwindigkeit()
	{
		return WertA;
	}
	TrainCon_Paar Get_Funtion()
	{
		return (TrainCon_Paar(WertA, (bool) WertB));
	}
	bool isGeschwindigkeit()
	{
		if ((mache == FahrPlanDo::vorwaerz_fahren)
			|| (mache == FahrPlanDo::rueckwaerz_fahren)
			|| (mache == FahrPlanDo::stoppen))
			return true;
		else
			return false;

	}
	void SetWeg(TrainCon_Paar *WegPlan)
	{
		for (int i = 0; i < 4; i++)
		{
			Weg[i] = *(WegPlan + i);
		}
	}
	TrainCon_Paar GetWeg(byte w)
	{
		return Weg[w];
	}
	void GetWerte(int *A, int *B)
	{
		*A = WertA;
		*B = WertB;
	};
	CString TextZeile()
	{
		Bemerkung.Empty();
		CString Text; 
		Text.Format(_T(" %3i <%3i> | %5i %5i |%s%s%s%s | %s\n"),Melder,mache,WertA, WertB,
			(LPCTSTR)Weg[0].GetText(), (LPCTSTR)Weg[1].GetText(), (LPCTSTR)Weg[2].GetText(), (LPCTSTR)Weg[3].GetText(),
			(LPCTSTR) Bemerkung);
		return Text;
	}
	void ZeileText(CString InStr)
	{
		Melder = _ttoi(InStr.Mid(1, 3));
		mache = (FahrPlanDo)_ttoi(InStr.Mid(6, 3));
		WertA = _ttoi(InStr.Mid(13, 5));
		WertB = _ttoi(InStr.Mid(19, 5));
		Weg[0].SetbyText(InStr.Mid(27, 3));
		Weg[1].SetbyText(InStr.Mid(31, 3));
		Weg[2].SetbyText(InStr.Mid(35, 3));
		Weg[3].SetbyText(InStr.Mid(39, 3));
		Bemerkung = _T("| geht noch nicht ");
		if ((mache == FahrPlanDo::schalten_Weiche)&& (Weg[0].GetWert() == 0))
			TRACE(_T(" ERROR Weichenweg von %i zu %i nicht definiert [%i] !!! \n"),WertA, WertB, Melder);
	}
	CString DebugWeg()
	{
		CString Text;
		Text.Format(_T("Weg [%2i]-(%i)"), Weg[0].GetWert(), Weg[0].GetBit());
		TRACE(_T("Weg [0] = %2i (%i) "), Weg[0].GetWert(), Weg[0].GetBit());
		TRACE(_T("Weg [1] = %2i (%i) "), Weg[1].GetWert(), Weg[1].GetBit());
		TRACE(_T("Weg [2] = %2i (%i) "), Weg[2].GetWert(), Weg[2].GetBit());
		TRACE(_T("Weg [3] = %2i (%i) "), Weg[3].GetWert(), Weg[3].GetBit());
		return(Text);
	}
};

class CDataFahrplan
{
public:
	CDataFahrplan();
	~CDataFahrplan();
	void Load_Daten( byte Plan_Nr);
	void Save_Daten( byte Plan_Nr);
	void Neue_Daten( byte Plan_Nr);
	FahrplanPos Get_Befehl(byte Z);
	void Del_Befehl(byte Z);
	FahrPlanDo Get_Was(byte Z);
	int Get_PlanBefehleAnzahl();
	CString Get_FahrplanName();
	void Set_FahrplanName(CString N);
	void Set_Befehl(byte Z, FahrplanPos Zeile);
	void Set_neu_Befehl(byte Z, FahrplanPos Zeile);
	bool isnot_PlanStop();
	void DebugInfo();
	TrainCon_Paar Get_Plan_WeichenWeg(byte Z);
	FahrplanPos Get_Plan_Befehl();
	FahrPlanDo Get_Plan_Was();
	byte Get_Plan_Melder();
	void Done_Plan_Befehl();
protected:
	std::vector<FahrplanPos>   F_Zeile;
	byte			Pos_Fahrplan;
	bool			Do_Save_Data;
	CString			Name;

};
