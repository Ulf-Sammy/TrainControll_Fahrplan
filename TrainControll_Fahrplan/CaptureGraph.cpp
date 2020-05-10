//------------------------------------------------------------------------------
// Zeigt das bild meiner Kamera im Zug
//
// Ulf (c) Mai / 2019
//----------------------------------------------
#include "pch.h"
#include  "CaptureGraph.h"


CameraBild::CameraBild()
{
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

CameraBild::~CameraBild()
{
	CoUninitialize();
	if (pmVideo != NULL) StartBild(false);
}

bool CameraBild::SucheKamera(CString Cam_Name)
{
	VARIANT var;
	ICreateDevEnum *pDevEnum = 0;
	IEnumMoniker *pEnum = 0;

	bool Cam_found = false;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pDevEnum);
	if (hr != NOERROR)
	{
		ErrorText.Format(TEXT("Error Creating Device Enumerator"));
		return false;
	}

	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
	if (hr != NOERROR)
	{
		ErrorText.Format(TEXT("!!!  No webcam found !!!"));
		return false;
	}
	pEnum->Reset();

	while (pEnum->Next(1, &pmVideo, NULL) == S_OK)
	{
		hr = pmVideo->BindToStorage(NULL, NULL , IID_IPropertyBag, (void **)&pPropBag);
		if (FAILED(hr))
		{
			pmVideo->Release();
			continue;
		}
		VariantInit(&var);
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			TRACE(L"\n Device {%s}  \n", var.bstrVal);
			if (Cam_Name == CString(var.bstrVal))
			{
				DeviceName = var.bstrVal;
				Cam_found = true;
				break;
			}
			VariantClear(&var);
		}
		pPropBag->Release();
		pmVideo->Release();
	}
	pEnum->Release();
	pDevEnum->Release();
	return Cam_found;
}

void CameraBild::StarteKamera()
{
	pmVideo->AddRef();
	pmVideo->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pVCap);
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pGraphBuilder2);
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);
	hr = pGraphBuilder2->SetFiltergraph(pGraphBuilder);
	hr = pGraphBuilder->AddFilter(pVCap, DeviceName);
	hr = pGraphBuilder2->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pVCap, IID_IAMStreamConfig, (void **)&pVSC);
	//if (pVCap == NULL)  ErrorText.Format(TEXT("Error %x: Cannot create video capture filter"), hr)
	//if (hr != NOERROR)  ErrorText.Format(TEXT("Cannot give graph to builder")); 
	//if (hr != NOERROR)  ErrorText.Format(TEXT("Error %x: Cannot add vidcap to filtergraph"), hr); 
	//hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVCap, NULL, NULL);
	hr = pGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVCap, NULL, NULL);
}

void CameraBild::Set_Bild_inDlg(HWND Owner)
{
	CRect rc;
	hr = pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVW);

	if (hr != NOERROR) ErrorText.Format(TEXT("!!! This graph cannot preview properly !!!"));
	else
	{
		pVW->put_Owner((OAHWND)Owner);    // We own the window now
		pVW->put_WindowStyle(WS_CHILD);      // you are now a child
		GetWindowRect(Owner, &rc);
		pVW->SetWindowPosition(0,0, rc.Width(), rc.Height()); // be this big
		pVW->put_Visible(OATRUE);
	}
}

void CameraBild::StartBild(bool OnCam)
{
	HRESULT hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&pMediaContol);
	if (SUCCEEDED(hr))
	{
		if (OnCam)
		{
			hr = pMediaContol->Run();
			if (FAILED(hr))		pMediaContol->Stop();
		}
		else
		{
			pMediaContol->Stop();
		}
		pMediaContol->Release();
	}
}
