#pragma once

// PlayerAtx.h : PlayerAtx.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������


// CPlayerAtxApp : �й�ʵ�ֵ���Ϣ������� PlayerAtx.cpp��

class CPlayerAtxApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

//����Component Categories�еĳ�ʼ����ȫ�ͽű���ȫ��
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription);
//��CLSID�д�����Component Categories�г�ʼ����ȫ�ͽű���ȫ�������Ӧ��implemented Categories��
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
//ע����CLSID�е���Ӧimplemented Categories�һ���ò�������Ϊ�����������Ҳ���õ������
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid);

extern const GUID CDECL _tlid;
extern const GUID CDECL CLSID_SafeItem;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

