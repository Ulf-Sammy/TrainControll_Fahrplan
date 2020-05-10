#include "pch.h"
#include "CDataFahrplan.h"


CDataFahrplan::CDataFahrplan()
{
	Do_Save_Data = false;
	Pos_Fahrplan = 0;
}


CDataFahrplan::~CDataFahrplan()
{
}

void CDataFahrplan::Load_Daten(byte Plan_Nr)
{
	LPCTSTR			pszPathName;
	CStdioFile		file;
	CFileException	ex;
	CString			strIn, Text;
	FahrplanPos     Befehl;

	Text.Format(_T(FILE_FAHRPLAN), Plan_Nr);
	pszPathName = Text;

	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		ex.ReportError();
		return;
	}
	F_Zeile.clear();
	Pos_Fahrplan = 0;
	file.ReadString(strIn); // ########### 
	file.ReadString(strIn); // Titel Plan Nr
	file.ReadString(strIn); // Titel Plan Name
	file.ReadString(strIn); // Info
	file.ReadString(strIn); // #############
	file.ReadString(Name);
	file.ReadString(strIn); // Befehl einlesen
	while (strIn.Mid(0, 10) != "## Ende ##")
	{
		Befehl.ZeileText(strIn);
		F_Zeile.push_back(Befehl);
		file.ReadString(strIn); // Befehl einlesen
	}
	file.Close();
}

void CDataFahrplan::Save_Daten(byte Plan_Nr)
{
	if (Do_Save_Data)
	{
		LPCTSTR			pszPathName;
		CStdioFile		file;
		CFileException	ex;
		CString			Text, strOut;

		Text.Format(_T(FILE_FAHRPLAN), Plan_Nr);
		pszPathName = Text;
		if (!file.Open(pszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeText, &ex))
		{
			ex.ReportError();
			return;
		}

		try
		{
			file.Seek(0, CFile::end);
			file.WriteString(_T("###########################################\n"));
			strOut.Format(   _T("### Fahrplan Nr. :  %03d    \n"),Plan_Nr);
			file.WriteString(strOut);
			strOut.Format(   _T("### Fahrplan Name: %s  \n"), (LPCTSTR)Name);
			file.WriteString(strOut);
			file.WriteString(_T("### für die Test Strecke \n"));
			file.WriteString(_T("###########################################\n"));
			file.WriteString(Name + "\n");
			for (int I = 0; I < F_Zeile.size(); I++)
			{
				file.WriteString(F_Zeile[I].TextZeile());
			}
			file.WriteString(_T("## Ende ####################################\n"));
			file.Close();
			Do_Save_Data = false;
		}
		catch (CFileException* e)
		{
			e->ReportError();
			e->Delete();
			file.Abort();
		}
	}
}

void CDataFahrplan::Neue_Daten( byte Plan_Nr)
{
	FahrplanPos     Befehl;
	Befehl = FahrplanPos(255, (FahrPlanDo)0, 0, 0);
	F_Zeile.push_back(Befehl);
	Befehl = FahrplanPos(255, (FahrPlanDo)8, 0, 0);
	F_Zeile.push_back(Befehl);
}


FahrplanPos CDataFahrplan::Get_Befehl(byte Z)
{
	return F_Zeile[Z];
}

void CDataFahrplan::DebugInfo()
{
	const CString WasT[9] = { _T("Begin"),_T("Stopp"),_T("Vorwärtz"),_T("Rückwärtz"),_T("Warten Fa"),_T("Warten St"),_T("Funktion"),_T("Weiche"),_T("Ende !") };
	//if (F_Zeile[Pos_Fahrplan].mache == schalten_Weiche)
	{
		TRACE(_T(" { FahrPlan Pos......%i }\n"), Pos_Fahrplan);
		TRACE(_T(" { FahrPlan Melder...%i }\n"), F_Zeile[Pos_Fahrplan].Melder);
		TRACE(_T(" { FahrPlan Was......%s }\n"), WasT[int(F_Zeile[Pos_Fahrplan].mache)]);
	}
}

TrainCon_Paar CDataFahrplan::Get_Plan_WeichenWeg(byte W)
{
	return F_Zeile[Pos_Fahrplan].Weg[W];
}

FahrplanPos CDataFahrplan::Get_Plan_Befehl()
{
	return F_Zeile[Pos_Fahrplan];
}

FahrPlanDo CDataFahrplan::Get_Plan_Was()
{
	return F_Zeile[Pos_Fahrplan].Get_Was();
}

byte CDataFahrplan::Get_Plan_Melder()
{
	return F_Zeile[Pos_Fahrplan].Melder;
}

void CDataFahrplan::Done_Plan_Befehl()
{
	Pos_Fahrplan++;
}

void CDataFahrplan::Del_Befehl(byte Z)
{
	F_Zeile.erase(F_Zeile.begin() + Z);
}

FahrPlanDo CDataFahrplan::Get_Was(byte Z)
{
	return F_Zeile[Z].Get_Was();
}

int CDataFahrplan::Get_PlanBefehleAnzahl()
{
	return (int)F_Zeile.size();
}

CString CDataFahrplan::Get_FahrplanName()
{
	return Name;
}

void CDataFahrplan::Set_FahrplanName(CString N)
{
	Name = N;
	Do_Save_Data = true;
}

void CDataFahrplan::Set_Befehl(byte Z, FahrplanPos Zeile)
{
	F_Zeile[Z] = Zeile;
	Do_Save_Data = true;
}

void CDataFahrplan::Set_neu_Befehl(byte Z, FahrplanPos Zeile)
{
	 F_Zeile.insert(F_Zeile.begin() +Z, Zeile);
	Do_Save_Data = true;
}

bool CDataFahrplan::isnot_PlanStop()
{ 
	bool PSt = true;
	byte n = Pos_Fahrplan;
	byte M = F_Zeile[n].Get_Melder();
	while (M == F_Zeile[n].Get_Melder())
	{
		if (F_Zeile[n].Get_Was() == FahrPlanDo::stoppen) PSt = false;
		n++;
	}
	return PSt;
}

