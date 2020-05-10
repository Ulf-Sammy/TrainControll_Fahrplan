#pragma once
#include "pch.h"
#include <DShow.h>

class CameraBild
{
public:
	CameraBild();
	~CameraBild();
	bool SucheKamera(CString Cam_Name);
	void StarteKamera();
	void Set_Bild_inDlg(HWND Owner);
	void StartBild(bool OnCam);
protected:
	IMoniker				*pmVideo = NULL;
	IPropertyBag			*pPropBag = NULL;
	IMediaControl			*pMediaContol = NULL;
	IBaseFilter				*pVCap = NULL;
	IGraphBuilder			*pGraphBuilder = NULL;
	IAMStreamConfig			*pVSC = NULL;
	IVideoWindow			*pVW = NULL;
	ICaptureGraphBuilder2   *pGraphBuilder2 = NULL;
	LPCWSTR DeviceName = _T(" ");
	CString ErrorText;
	HRESULT hr;
private:
};