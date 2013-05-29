// PlayerAtx.cpp : CPlayerAtxApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "PlayerAtx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "comcat.h"
#include "Objsafe.h"


// E03F97A5-5360-452B-8234-0B15E1EE1C7D
// 8983D1AE-5D45-4810-9109-EC257CFF7F97

CPlayerAtxApp theApp;

//���ÿؼ���ע�����ص����Ϳ�ID���������version(1.0)�������ӱ����̵�.idl�ļ��л�ȡ
const GUID CDECL _tlid = { 0xE03F97A5, 0x5360, 0x452B, { 0x82, 0x34, 0xB, 0x15, 0xE1, 0xEE, 0x1C, 0x7D } };
//�ؼ���ע����е�CLSID���������control�������ӱ����̵�.idl�ļ��л�ȡ
const GUID CDECL CLSID_SafeItem ={ 0x8983D1AE, 0x5D45, 0x4810, {0x91, 0x09, 0xEC, 0x25, 0x7C, 0xFF, 0x7F, 0x97}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;







HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (FAILED(hr))
		return hr;

	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered.
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english

	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is.
	int len = wcslen(catDescription);
	if (len>127)
		len = 127;
	wcsncpy(catinfo.szDescription, catDescription, len);
	// Make sure the description is null terminated.
	catinfo.szDescription[len] = '\0';

	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();

	return hr;
}

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);

		if (FAILED(hr))
			return hr;
	}
	if (pcr != NULL)
		pcr->Release();
	return hr;
}


HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}


// CPlayerAtxApp::InitInstance - DLL ��ʼ��

BOOL CPlayerAtxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
		CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> CPlayerAtxApp::InitInstance\n");
		OutputDebugString(strMsg);
	}

	return bInit;
}



// CPlayerAtxApp::ExitInstance - DLL ��ֹ

int CPlayerAtxApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> CPlayerAtxApp::ExitInstance\n");
	OutputDebugString(strMsg);

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

//STDAPI DllRegisterServer(void)
//{
//	AFX_MANAGE_STATE(_afxModuleAddrThis);
//
//	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
//		return ResultFromScode(SELFREG_E_TYPELIB);
//
//	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
//		return ResultFromScode(SELFREG_E_CLASS);
//
//	return NOERROR;
//}

STDAPI DllRegisterServer(void)
{
	HRESULT hr;

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	//�����ű���ȫ�����䡱���CLSID��
	hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
	if (FAILED(hr))
		return hr;
	//������ʼ����ȫ�����䡱���CLSID��
	hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");
	if (FAILED(hr))
		return hr;
	//���ÿؼ�CLSID�в�����Ľű���ȫ��롰���䡱���еĽű���ȫ���Ӧ
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
	if (FAILED(hr))
		return hr;
	//���ÿؼ�CLSID�в�����ĳ�ʼ����ȫ��롰���䡱���еĳ�ʼ����ȫ���Ӧ
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
	if (FAILED(hr))
		return hr;

	return NOERROR;
}

// DllUnregisterServer - �����ϵͳע������Ƴ�

//STDAPI DllUnregisterServer(void)
//{
//	AFX_MANAGE_STATE(_afxModuleAddrThis);
//
//	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
//		return ResultFromScode(SELFREG_E_TYPELIB);
//
//	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
//		return ResultFromScode(SELFREG_E_CLASS);
//
//	return NOERROR;
//}

// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	//HRESULT hr;
	
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	//hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
	//if (FAILED(hr))
	//	return hr;

	//hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
	//if (FAILED(hr))
	//	return hr;

	return NOERROR;
}
