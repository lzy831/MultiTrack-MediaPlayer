#pragma once

// PlayerAtxPropPage.h : CPlayerAtxPropPage ����ҳ���������


// CPlayerAtxPropPage : �й�ʵ�ֵ���Ϣ������� PlayerAtxPropPage.cpp��

class CPlayerAtxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPlayerAtxPropPage)
	DECLARE_OLECREATE_EX(CPlayerAtxPropPage)

// ���캯��
public:
	CPlayerAtxPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PLAYERATX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

