// TL_ViewWinWrapNormal.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayerAtx.h"
#include "afxdialogex.h"

#include "TL_ViewWinWrapNormal.h"
#include "TL_Message.h"
#include "TL_Color.h"


// TL_ViewWinWrapNormal 对话框

IMPLEMENT_DYNAMIC(TL_ViewWinWrapNormal, CDialogEx)

TL_ViewWinWrapNormal::TL_ViewWinWrapNormal(CWnd* pParent /*=NULL*/)
	: CDialogEx(TL_ViewWinWrapNormal::IDD, pParent)
	, m_inited(0)
{

}

TL_ViewWinWrapNormal::~TL_ViewWinWrapNormal()
{
}

void TL_ViewWinWrapNormal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TL_ViewWinWrapNormal, CDialogEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()

	ON_MESSAGE(WM_TL_PLAY_VIEW_PORT_ON_DCLICK, OnMsgViewPortDClick)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void TL_ViewWinWrapNormal::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>> ----------------->         TL_ViewWinWrapNormal OnPaint\n");
	OutputDebugString(strMsg);
}

HRESULT TL_ViewWinWrapNormal::OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;
	strMsg.Format(_T("------------------------------TL_ViewWinWrapNormal::OnMsgViewPortDClick 0x%x\n"),(int)lParam);
	OutputDebugString(strMsg);

	::PostMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_VIEW_PORT_ON_DCLICK,wParam,lParam);
	return 0;
}

void TL_ViewWinWrapNormal::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_inited==1)
	{
		SetViewWinPos();
	}
}

void TL_ViewWinWrapNormal::SetViewWinPos()
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


// TL_ViewWinWrapNormal 消息处理程序
BOOL TL_ViewWinWrapNormal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	BOOL Ret = FALSE;
	Ret = m_ViewWindow.Create(IDD_TL_VIEWWINDOW,this);
	ASSERT(Ret);
	SetViewWinPos();
	m_ViewWindow.CreateViewPort();
	m_inited = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int	TL_ViewWinWrapNormal::CreateViewPort()
{
	return m_ViewWindow.CreateViewPort();
}

int TL_ViewWinWrapNormal::ResetViewPortCount(int count)
{
	return m_ViewWindow.ResetViewPortCount(count);
}

int TL_ViewWinWrapNormal::ResetDisplayMode(DISPLAY_MODE dmode)
{
	return m_ViewWindow.ResetDisplayMode(dmode);
}

int TL_ViewWinWrapNormal::ResetLayoutMode(LAYOUT_MODE lmode)
{
	return m_ViewWindow.ResetLayoutMode(lmode);
}

int TL_ViewWinWrapNormal::ClearDisplay()
{
	return m_ViewWindow.ClearDisplay();
}

int TL_ViewWinWrapNormal::ScreenShot()
{
	return m_ViewWindow.ScreenShot();
}

int TL_ViewWinWrapNormal::FrameDataInit(int size, int width, int height)
{
	return m_ViewWindow.FrameDataInit(size,width,height);
}

int TL_ViewWinWrapNormal::UpdateViewPortData(int tid, long pts, int index, UINT8* data[], int linesize[], int src_w, int src_h)
{
	return m_ViewWindow.UpdateViewPortData(tid,pts,index,data,linesize,src_w,src_h);
}

int TL_ViewWinWrapNormal::UpdateDisplay(int index)
{
	return m_ViewWindow.UpdateDisplay(index);
}

int TL_ViewWinWrapNormal::UpdatePlayTime(int cur_time, int total_time)
{
	return m_ViewWindow.UpdatePlayTime(cur_time,total_time);
}


int TL_ViewWinWrapNormal::UpdatePlayStatus(PLAY_STATUS status)
{
	return m_ViewWindow.UpdatePlayStatus(status);
}



void TL_ViewWinWrapNormal::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strMsg;
	strMsg.Format(_T("TL_ViewWinWrapNormal::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	OutputDebugString(strMsg);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



