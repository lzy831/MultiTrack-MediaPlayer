// PlayerAtxPropPage.cpp : CPlayerAtxPropPage 属性页类的实现。

#include "stdafx.h"
#include "PlayerAtx.h"
#include "PlayerAtxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPlayerAtxPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CPlayerAtxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CPlayerAtxPropPage, "PLAYERATX.PlayerAtxPropPage.1",
	0x6b83dcab, 0x9f15, 0x4d6e, 0x86, 0x40, 0x7f, 0xf6, 0x26, 0x31, 0xf2, 0xf2)



// CPlayerAtxPropPage::CPlayerAtxPropPageFactory::UpdateRegistry -
// 添加或移除 CPlayerAtxPropPage 的系统注册表项

BOOL CPlayerAtxPropPage::CPlayerAtxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PLAYERATX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPlayerAtxPropPage::CPlayerAtxPropPage - 构造函数

CPlayerAtxPropPage::CPlayerAtxPropPage() :
	COlePropertyPage(IDD, IDS_PLAYERATX_PPG_CAPTION)
{
}



// CPlayerAtxPropPage::DoDataExchange - 在页和属性间移动数据

void CPlayerAtxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CPlayerAtxPropPage 消息处理程序
