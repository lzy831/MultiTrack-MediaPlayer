// TL_ViewWinWrapFullScreen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlayerAtx.h"
#include "PlayerAtxCtrl.h"

#include "afxdialogex.h"

#include "TL_ViewWinWrapFullScreen.h"
#include "TL_Message.h"
#include "TL_Color.h"



// TL_ViewWinWrapFullScreen �Ի���

IMPLEMENT_DYNAMIC(TL_ViewWinWrapFullScreen, CDialogEx)

TL_ViewWinWrapFullScreen::TL_ViewWinWrapFullScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(TL_ViewWinWrapFullScreen::IDD, pParent)
	, m_pPlayerAtxCtrl(NULL)
{

}

TL_ViewWinWrapFullScreen::~TL_ViewWinWrapFullScreen()
{
}

void TL_ViewWinWrapFullScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TL_ViewWinWrapFullScreen, CDialogEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_TL_PLAY_VIEW_PORT_ON_DCLICK, OnMsgViewPortDClick)
END_MESSAGE_MAP()

HRESULT TL_ViewWinWrapFullScreen::OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;
	strMsg.Format(_T("------------------------------TL_ViewWinWrapFullScreen::OnMsgViewPortDClick 0x%x\n"),(int)lParam);
	OutputDebugString(strMsg);
	
	// ��֪�����﷢��������Ϊ����Ӧ���ˣ� ��������Ϊ  TL_ViewWinWrapFullScreen Ϊ�������� ��������ʾ��Z������ԭ��
	//::SendMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_VIEW_PORT_ON_DCLICK,wParam,lParam);

	m_pPlayerAtxCtrl->DoOnViewPortDClick((int)lParam);
	return 0;
}
// TL_ViewWinWrapFullScreen ��Ϣ�������

void TL_ViewWinWrapFullScreen::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( m_inited==1)
	{
		SetViewWinPos();
	}
}

void TL_ViewWinWrapFullScreen::SetViewWinPos()
{
	CRect Rect;
	GetClientRect(&Rect);
	DWORD space = 5;
	Rect.top = Rect.top + space;
	Rect.bottom = Rect.bottom - space;
	Rect.left = Rect.left + space;
	Rect.right = Rect.right - space;
	::SetWindowPos(m_ViewWindow.m_hWnd
		, HWND_TOP
		, Rect.left
		, Rect.top
		, Rect.Width()
		, Rect.Height()
		, SWP_SHOWWINDOW);
	m_ViewWindow.SetBackgroundColor(ViewWinBkColor,1);
}


// TL_ViewWinWrapNormal ��Ϣ�������
BOOL TL_ViewWinWrapFullScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	BOOL Ret = FALSE;
	Ret = m_ViewWindow.Create(IDD_TL_VIEWWINDOW,this);
	ASSERT(Ret);
	SetViewWinPos();
	m_ViewWindow.CreateViewPort();
	m_inited = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int	TL_ViewWinWrapFullScreen::CreateViewPort()
{
	return m_ViewWindow.CreateViewPort();
}

int TL_ViewWinWrapFullScreen::ResetViewPortCount(int count)
{
	return m_ViewWindow.ResetViewPortCount(count);
}

int TL_ViewWinWrapFullScreen::ResetDisplayMode(DISPLAY_MODE dmode)
{
	return m_ViewWindow.ResetDisplayMode(dmode);
}

int TL_ViewWinWrapFullScreen::ResetLayoutMode(LAYOUT_MODE lmode)
{
	return m_ViewWindow.ResetLayoutMode(lmode);
}

int TL_ViewWinWrapFullScreen::ClearDisplay()
{
	return m_ViewWindow.ClearDisplay();
}

int TL_ViewWinWrapFullScreen::ScreenShot()
{
	return m_ViewWindow.ScreenShot();
}

int TL_ViewWinWrapFullScreen::FrameDataInit(int size, int width, int height)
{
	return m_ViewWindow.FrameDataInit(size,width,height);
}

int TL_ViewWinWrapFullScreen::UpdateViewPortData(int tid, long pts, int index, UINT8* data[], int linesize[], int src_w, int src_h)
{
	return m_ViewWindow.UpdateViewPortData(tid,pts,index,data,linesize,src_w,src_h);
}

int TL_ViewWinWrapFullScreen::UpdateDisplay(int index)
{
	return m_ViewWindow.UpdateDisplay(index);
}

int TL_ViewWinWrapFullScreen::UpdatePlayTime(int cur_time, int total_time)
{
	return m_ViewWindow.UpdatePlayTime(cur_time,total_time);
}

int TL_ViewWinWrapFullScreen::UpdatePlayStatus(PLAY_STATUS status)
{
	return m_ViewWindow.UpdatePlayStatus(status);
}


BOOL TL_ViewWinWrapFullScreen::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		::SendMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_EXIT_FULL_SCREEN,0,0);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void TL_ViewWinWrapFullScreen::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strMsg;
	strMsg.Format(_T("TL_ViewWinWrapFullScreen::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	OutputDebugString(strMsg);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
