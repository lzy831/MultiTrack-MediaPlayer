#pragma once

// PlayerAtx.h : PlayerAtx.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号


// CPlayerAtxApp : 有关实现的信息，请参阅 PlayerAtx.cpp。

class CPlayerAtxApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

//创建Component Categories中的初始化安全和脚本安全项
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription);
//在CLSID中创建与Component Categories中初始化安全和脚本安全项中相对应的implemented Categories项
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
//注销与CLSID中的相应implemented Categories项，一般用不到，因为其它程序可能也会用到这此项
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid);

extern const GUID CDECL _tlid;
extern const GUID CDECL CLSID_SafeItem;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

