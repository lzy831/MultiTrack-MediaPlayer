#pragma once

// PlayerAtxPropPage.h : CPlayerAtxPropPage 属性页类的声明。


// CPlayerAtxPropPage : 有关实现的信息，请参阅 PlayerAtxPropPage.cpp。

class CPlayerAtxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPlayerAtxPropPage)
	DECLARE_OLECREATE_EX(CPlayerAtxPropPage)

// 构造函数
public:
	CPlayerAtxPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PLAYERATX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

