// TL_ViewWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "PlayerAtx.h"

#include <string>
#include "TL_ViewPort.h"
#include "TL_ViewWindow.h"
#include "TL_Resource.h"
#include "TL_Message.h"
#include "TL_Color.h"
#include "PlayerCoreDLL/PlayerCoreInterface.h"


#define DEBUG_MSG_SIZE 512
// TL_ViewWindow 对话框

IMPLEMENT_DYNAMIC(TL_ViewWindow, CDialogEx)


TL_ViewWindow::TL_ViewWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(TL_ViewWindow::IDD, pParent)
	, m_ViewPortCreated(0)
	, m_MaxViewPortCount(MAX_VIEWPORT_COUNT)
	, m_CurViewPortCount(0)
	, m_CurDisplayMode(DMODE_NONE)
	, m_CurLayoutMode(LMODE_NONE)
{
	OnConstruct();
}

TL_ViewWindow::~TL_ViewWindow()
{
	OnDestroy();
}

void TL_ViewWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TL_ViewWindow, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()

	ON_MESSAGE(WM_TL_PLAY_VIEW_PORT_ON_DCLICK, OnMsgViewPortDClick)
	//ON_WM_PAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void TL_ViewWindow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>> ----------------->         TL_ViewWindow OnPaint\n");
	OutputDebugString(strMsg);

	//if(m_hBitmap!=NULL)
	//{
	//	UpdateDisplay();
	//}
}

HRESULT TL_ViewWindow::OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam)
{
	//CString strMsg;
	//strMsg.Format(_T("------------------------------TL_ViewWindow::OnMsgViewPortDClick 0x%x\n"),(int)lParam);
	//OutputDebugString(strMsg);

	::SendMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_VIEW_PORT_ON_DCLICK,wParam,lParam);
	return 0;
}

// TL_ViewWindow 消息处理程序
HBRUSH TL_ViewWindow::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//if( nCtlColor == CTLCOLOR_STATIC)
	if( pWnd == (CWnd*)(&m_StaticPlayTime))
	{
		// 设置PlayTimeStatic颜色
		pDC->SetTextColor(PlayTimeFontColor);// 设置字体颜色
		pDC->SetBkColor(PlayTimeFontBkColor);// 设置字体背景颜色
		return m_StaticPlayTimeBrush;//返回自定义画刷，用于绘制整个客户区颜色
	}
	if( pWnd == (CWnd*)(&m_StaticPlayStatus))
	{
		pDC->SetTextColor(PlayTimeFontColor);// 设置字体颜色
		pDC->SetBkColor(PlayTimeFontBkColor);// 设置字体背景颜色
		return m_StaticPlayStatusBrush;//返回自定义画刷，用于绘制整个客户区颜色
	}

	if( nCtlColor == CTLCOLOR_DLG)
	{
		return m_StaticPlayTimeBrush;
	}

	for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
	{
		if( pWnd == (CWnd*)(m_pViewPortArray[i]))
		{
			pDC->SetTextColor(RGB(255,255,0));	// 设置字体颜色
			pDC->SetBkColor(RGB(0,0,0));		// 设置字体背景颜色
			return m_StaticPlayTimeBrush;		//返回自定义画刷，用于绘制整个客户区颜色
		}
	}

	return hbr;
}

void TL_ViewWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MyOnSize(nType, cx, cy);
}



/************************************************************************************************************************************************/
/*  自己增加的函数                                                                     */
/************************************************************************************************************************************************/
void TL_ViewWindow::OnConstruct()
{
	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> TL_ViewWindow::OnConstruct\n");
	OutputDebugString(strMsg);

	m_MaxViewPortCount = MAX_VIEWPORT_COUNT;
	m_CurViewPortCount = 0;
	m_StaticPlayTimeBrush.CreateSolidBrush(PlayTimeBkColor);
	m_StaticPlayStatusBrush.CreateSolidBrush(PlayStatusBkColor);

	for(WORD i = 0; i < m_MaxViewPortCount; i++)
	{
		m_pViewPortArray[i] = new TL_ViewPort();
		ASSERT(m_pViewPortArray!=NULL);
	}
}

void TL_ViewWindow::OnDestroy()
{
	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> TL_ViewWindow::OnDestroy\n");
	OutputDebugString(strMsg);
	for(WORD i = 0; i < m_MaxViewPortCount; i++)
	{
		ASSERT(m_pViewPortArray[i]!=NULL);
		delete m_pViewPortArray[i];
		m_pViewPortArray[i] = NULL;
		ASSERT(m_pViewPortArray[i]==NULL);
	}
}

void TL_ViewWindow::MyOnSize(UINT nType, int cx, int cy)
{
	ResetLayout();
}

int TL_ViewWindow::SetViewPortPos(int index, int x, int y, int w, int h)
{
	TL_ViewPort* p_ViewPort = NULL;
	p_ViewPort = m_pViewPortArray[index];

	CRect VPRect;
	VPRect.top = y;
	VPRect.left = x;
	VPRect.right = x + w;
	VPRect.bottom = y + h;
	::SetWindowPos(p_ViewPort->m_hWnd
		, HWND_TOP
		, VPRect.left
		, VPRect.top
		, VPRect.Width()
		, VPRect.Height()
		, SWP_SHOWWINDOW);
		p_ViewPort->SetBackgroundColor(ViewPortBkColor,1);

	return 0;
}

int TL_ViewWindow::SetViewPortPos(int index, CRect& VPRect, int hide)
{
	TL_ViewPort* p_ViewPort = NULL;
	p_ViewPort = m_pViewPortArray[index];
	
	int flag = SWP_SHOWWINDOW;
	if(hide==1)
	{
		flag = SWP_HIDEWINDOW;
	}
	
	::SetWindowPos(p_ViewPort->m_hWnd
		, HWND_TOP
		, VPRect.left
		, VPRect.top
		, VPRect.Width()
		, VPRect.Height()
		, flag);

	p_ViewPort->SetBackgroundColor(ViewPortBkColor,1);
	return 0;
}

void TL_ViewWindow::GetScaledSize(DWORD* width, DWORD* height, DISPLAY_MODE mode)
{
	DOUBLE scale;

	if( mode == DMODE_16_9)
	{
		scale = 16.0 / 9;
	}
	else if( mode == DMODE_4_3)
	{
		scale = 4.0 / 3;
	}
	else
	{
		return ;
	}

	DWORD w = *width;
	DWORD h = *height;

	if( h * scale >= w)
	{
		*height = (DWORD)(w/scale);
	}
	else
	{
		*width = (DWORD)(h*scale);
	}
}

int	TL_ViewWindow::CreateViewPort()
{
	if(m_ViewPortCreated == 0)
	{
		// 创建ViewPort  具体设置显示位置放在ResetViewPortLayout中
		if( m_CurViewPortCount == 0)
		{
			BOOL Ret = 0;
			for(WORD i = 0; i < m_MaxViewPortCount; i++)
			{
				Ret = m_pViewPortArray[i]->Create(IDD_TL_VIEWPORT,this);
				m_pViewPortArray[i]->SetIndex(i);
				ASSERT(Ret!=0);
			}
		}

		// 设置当前相关显示模式
		m_CurViewPortCount = m_MaxViewPortCount;
		m_CurDisplayMode = DMODE_4_3;
		m_CurLayoutMode = LMODE_1;

		RECT rect;
		// 创建ViewPort的对应标签  具体设置显示位置放在ResetViewPortLayout中
		for(int i = 0; i< m_MaxViewPortCount ;i++)
		{
			m_StaticViewPortTag[i].Create(_T(""),WS_CHILD,rect,this,IDD_STATIC_VIEWPORT_TAG_BASE+i);
		}

		// 创建 设置 播放时间字串  具体设置显示位置放在ResetViewPortLayout中
		m_StaticPlayTime.Create(_T("PlayingTime"),WS_CHILD,rect,this,IDD_STATIC_PLAYING_TIME);
		m_StaticPlayTime.SetWindowText(_T("00:00:00 / 00:00:00"));

		m_StaticPlayStatus.Create(_T("PlayingStatus"),WS_CHILD,rect,this,IDD_STATIC_PLAY_STATUS);;
		m_StaticPlayStatus.SetWindowText(_T("未播放"));

		m_ViewPortCreated = 1;
	}
	return 0;
}

int TL_ViewWindow::PrepareViewPortRect(CRect& ViewPortRect, int count, DISPLAY_MODE dmode, LAYOUT_MODE lmode, CRect* WinRect)
{
	if( count < 1 || count > m_MaxViewPortCount)
	{
		return -1;
	}
	if( dmode != DMODE_4_3 && dmode != DMODE_16_9)
	{
		return -1;
	}
	if( lmode != LMODE_1 && lmode != LMODE_2)
	{
		return -1;
	}

	DWORD space = 5;

	DWORD LittleWinCount = m_CurViewPortCount - 1;	// 计算小窗口(ViewPort的个数)

	DWORD BigWinRangeW = 0;		//大窗口的显示区域宽
	DWORD BigWinRangeH = 0;		//大窗口的显示区域高
	DWORD BigWinW = 0;			//大窗口宽
	DWORD BigWinH = 0;			//大窗口高
	DWORD BigWinXPadding = 0;	//大窗口横向填充
	DWORD BigWinYPadding = 0;	//大窗口纵向填充

	DWORD LittleWinRangeW = 0;	//小窗口的显示区域宽
	DWORD LittleWinRangeH = 0;	//小窗口的显示区域高
	DWORD LittleWinW = 0;		//小窗口宽
	DWORD LittleWinH = 0;		//小窗口高
	DWORD LitteWinXPadding = 0; //小窗口横向填充
	DWORD LitteWinYPadding = 0;	//小窗口纵向填充


	if( LittleWinCount == 0)
	{
		// 如果只有大画面，情况就简单了
		BigWinRangeW = ViewPortRect.right - ViewPortRect.left;
		BigWinRangeH = ViewPortRect.bottom - ViewPortRect.top;
		BigWinW = BigWinRangeW;
		BigWinH = BigWinRangeH;
		GetScaledSize(&BigWinW,&BigWinH,dmode);

		BigWinXPadding = (BigWinRangeW - BigWinW)/2;
		BigWinYPadding = (BigWinRangeH - BigWinH)/2;

		WinRect[0].top = ViewPortRect.top + BigWinYPadding;
		WinRect[0].bottom = WinRect[0].top + BigWinH;
		WinRect[0].left = WinRect[1].right + BigWinXPadding;
		WinRect[0].right = WinRect[0].left + BigWinW;
	}
	else
	{
		if( lmode == LMODE_1 /*大窗在左，小窗在右*/ || lmode == LMODE_2 /*小窗在左，大窗在右 */)
		{
			BigWinRangeW = ((ViewPortRect.right - ViewPortRect.left) - space) * 3/4;
			BigWinRangeH = (ViewPortRect.bottom - ViewPortRect.top);
			BigWinW = BigWinRangeW;
			BigWinH = BigWinRangeH;
			GetScaledSize(&BigWinW,&BigWinH,dmode);

			LittleWinRangeW = ((ViewPortRect.right - ViewPortRect.left) - space) * 1/4;
			LittleWinRangeH = ((ViewPortRect.bottom - ViewPortRect.top) - space*(LittleWinCount-1)) * 1/LittleWinCount;
			LittleWinW = LittleWinRangeW;
			LittleWinH = LittleWinRangeH;
			GetScaledSize(&LittleWinW,&LittleWinH,dmode);

			BigWinXPadding = (BigWinRangeW - BigWinW)/2;
			BigWinYPadding = (BigWinRangeH - BigWinH)/2;
			LitteWinXPadding = (LittleWinRangeW - LittleWinW)/2;
			LitteWinYPadding = (LittleWinRangeH - LittleWinH)/2;

			if( lmode == LMODE_1)
			{

				if( LittleWinCount == 1)
				{
					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = ViewPortRect.left + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;
				}
				else if( LittleWinCount == 2)
				{
					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = ViewPortRect.left + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;

					WinRect[2].top = WinRect[1].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[2].bottom = WinRect[2].top + LittleWinH;
					WinRect[2].left = WinRect[1].left;
					WinRect[2].right = WinRect[2].left + LittleWinW;
				}
				else
				{
					ASSERT(LittleWinCount == 3);

					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = ViewPortRect.left + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;

					WinRect[2].top = WinRect[1].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[2].bottom = WinRect[2].top + LittleWinH;
					WinRect[2].left = WinRect[1].left;
					WinRect[2].right = WinRect[2].left + LittleWinW;

					WinRect[3].top = WinRect[2].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[3].bottom = WinRect[3].top + LittleWinH;
					WinRect[3].left = WinRect[1].left;
					WinRect[3].right = WinRect[3].left + LittleWinW;
				}

				// 无论几个小窗口，大窗口位置是固定的
				WinRect[0].top = ViewPortRect.top + BigWinYPadding;
				WinRect[0].bottom = WinRect[0].top + BigWinH;
				WinRect[0].left = WinRect[1].right + space + BigWinXPadding;
				WinRect[0].right = WinRect[0].left + BigWinW;

			}
			else
			{
				// 无论几个小窗口，大窗口位置是固定的
				WinRect[0].top = ViewPortRect.top + BigWinYPadding;
				WinRect[0].bottom = WinRect[0].top + BigWinH;
				WinRect[0].left = ViewPortRect.left + BigWinXPadding;
				WinRect[0].right = WinRect[0].left + BigWinW;

				if( LittleWinCount == 1)
				{
					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = WinRect[0].right + space + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;
				}
				else if( LittleWinCount == 2)
				{
					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = WinRect[0].right + space + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;

					WinRect[2].top = WinRect[1].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[2].bottom = WinRect[2].top + LittleWinH;
					WinRect[2].left = WinRect[1].left;
					WinRect[2].right = WinRect[2].left + LittleWinW;
				}
				else
				{
					ASSERT(LittleWinCount == 3);

					WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
					WinRect[1].bottom = WinRect[1].top + LittleWinH;
					WinRect[1].left = WinRect[0].right + space + LitteWinXPadding;
					WinRect[1].right = WinRect[1].left + LittleWinW;

					WinRect[2].top = WinRect[1].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[2].bottom = WinRect[2].top + LittleWinH;
					WinRect[2].left = WinRect[1].left;
					WinRect[2].right = WinRect[2].left + LittleWinW;

					WinRect[3].top = WinRect[2].bottom + LitteWinYPadding + space + LitteWinYPadding;
					WinRect[3].bottom = WinRect[3].top + LittleWinH;
					WinRect[3].left = WinRect[1].left;
					WinRect[3].right = WinRect[3].left + LittleWinW;
				}
			}
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

void TL_ViewWindow::ResetViewPortLayout(CRect& Rect)
{
	if(m_ViewPortCreated == 0 || /*m_CurViewPortCount == 0 || */
		m_CurDisplayMode == DMODE_NONE || m_CurLayoutMode == LMODE_NONE)
	{
		return ;
	}

	CRect ViewPortRect = Rect;
	DWORD space = 5;
	CRect WinRect[4];
	ViewPortRect.top = ViewPortRect.top + space;
	ViewPortRect.bottom = ViewPortRect.bottom - space;
	ViewPortRect.left = ViewPortRect.left + space;
	ViewPortRect.right = ViewPortRect.right - space;

	for(int i = 0; i < m_MaxViewPortCount; i++)
	{
		SetViewPortPos( i, WinRect[i], 1);
	}

	if( m_CurViewPortCount > 0)
	{
		int Ret = PrepareViewPortRect(ViewPortRect,m_CurViewPortCount,m_CurDisplayMode,m_CurLayoutMode,WinRect);
		if( Ret != 0)
		{
			return ;
		}

		for(int i = 0; i < m_CurViewPortCount; i++)
		{
			SetViewPortPos( i, WinRect[i]);
		}
	}




	//DWORD LittleWinCount = 3;

	//DWORD BigWinRangeW = ((ViewPortRect.right - ViewPortRect.left) - space) * 3/4;
	//DWORD BigWinRangeH = (ViewPortRect.bottom - ViewPortRect.top);
	//DWORD BigWinW = BigWinRangeW;
	//DWORD BigWinH = BigWinRangeH;
	//GetScaledSize(&BigWinW,&BigWinH,DMODE_4_3);

	//DWORD LittleWinRangeW = ((ViewPortRect.right - ViewPortRect.left) - space) * 1/4;
	//DWORD LittleWinRangeH = ((ViewPortRect.bottom - ViewPortRect.top) - space*(LittleWinCount-1)) * 1/3;
	//DWORD LittleWinW = LittleWinRangeW;
	//DWORD LittleWinH = LittleWinRangeH;
	//GetScaledSize(&LittleWinW,&LittleWinH,DMODE_4_3);

	//DWORD BigWinXPadding = (BigWinRangeW - BigWinW)/2;
	//DWORD BigWinYPadding = (BigWinRangeH - BigWinH)/2;
	//DWORD LitteWinXPadding = (LittleWinRangeW - LittleWinW)/2;
	//DWORD LitteWinYPadding = (LittleWinRangeH - LittleWinH)/2;

	//WinRect[1].top = ViewPortRect.top + LitteWinYPadding;
	//WinRect[1].bottom = WinRect[1].top + LittleWinH;
	//WinRect[1].left = ViewPortRect.left + LitteWinXPadding;
	//WinRect[1].right = WinRect[1].left + LittleWinW;

	//WinRect[2].top = WinRect[1].bottom + LitteWinYPadding + space + LitteWinYPadding;
	//WinRect[2].bottom = WinRect[2].top + LittleWinH;
	//WinRect[2].left = ViewPortRect.left + LitteWinXPadding;
	//WinRect[2].right = WinRect[2].left + LittleWinW;

	//WinRect[3].top = WinRect[2].bottom + LitteWinYPadding + space + LitteWinYPadding;
	//WinRect[3].bottom = WinRect[3].top + LittleWinH;
	//WinRect[3].left = ViewPortRect.left + LitteWinXPadding;
	//WinRect[3].right = WinRect[3].left + LittleWinW;

	//WinRect[0].top = ViewPortRect.top + BigWinYPadding;
	//WinRect[0].bottom = WinRect[0].top + BigWinH;
	//WinRect[0].left = WinRect[1].right + space + BigWinXPadding;
	//WinRect[0].right = WinRect[0].left + BigWinW;

	//CString strMsg;
	//strMsg.Format( _T("\n\n-----> %d %d"), WinRect[0].top, WinRect[0].left);
	//OutputDebugString(strMsg);
	//strMsg.Format( _T("-----> %d %d"), WinRect[1].top, WinRect[1].left);
	//OutputDebugString(strMsg);
	//strMsg.Format( _T("-----> %d %d"), WinRect[2].top, WinRect[2].left);
	//OutputDebugString(strMsg);
	//strMsg.Format( _T("-----> %d %d\n\n"), WinRect[3].top, WinRect[3].left);
	//OutputDebugString(strMsg);

	//SetViewPortPos(0, WinRect[0]);
	//SetViewPortPos(1, WinRect[1]);
	//SetViewPortPos(2, WinRect[2]);
	//SetViewPortPos(3, WinRect[3]);



}


void TL_ViewWindow::ResetStatusBarLayout(CRect& Rect)
{
	DWORD TextWidth = 150;
	DWORD TextHeight = 25;

	CRect PlayingTimeRect;
	CRect PlayingStatusRect;

	PlayingTimeRect.top = Rect.top + 5;
	PlayingTimeRect.bottom = PlayingTimeRect.top + TextHeight;
	PlayingTimeRect.left = Rect.left + 10;
	PlayingTimeRect.right = PlayingTimeRect.left + TextWidth;

	::SetWindowPos(m_StaticPlayTime.m_hWnd
		, HWND_TOP
		, PlayingTimeRect.left
		, PlayingTimeRect.top
		, PlayingTimeRect.Width()
		, PlayingTimeRect.Height()
		, SWP_SHOWWINDOW);


	PlayingStatusRect.top = Rect.top + 5;
	PlayingStatusRect.bottom = PlayingStatusRect.top + TextHeight;
	PlayingStatusRect.left = PlayingTimeRect.right + 10;
	PlayingStatusRect.right = PlayingStatusRect.left + TextWidth;

	::SetWindowPos(m_StaticPlayStatus.m_hWnd
		, HWND_TOP
		, PlayingStatusRect.left
		, PlayingStatusRect.top
		, PlayingStatusRect.Width()
		, PlayingStatusRect.Height()
		, SWP_SHOWWINDOW);

}

void TL_ViewWindow::ResetLayout()
{
	CRect Rect;
	GetClientRect(&Rect);

	DWORD StatusBarHeight = 25;

	CRect ViewPortRect;
	ViewPortRect = Rect;
	ViewPortRect.bottom = Rect.bottom - StatusBarHeight;

	CRect StatusBarPect;
	StatusBarPect = Rect;
	StatusBarPect.top = ViewPortRect.bottom;

	ResetViewPortLayout(ViewPortRect);
	ResetStatusBarLayout(StatusBarPect);
}

int TL_ViewWindow::ResetViewPortCount(int count)
{
	// 重设view port 数量 并 布局
	if( m_CurViewPortCount == count)
	{
		return 0;
	}
	m_CurViewPortCount = count;
	ResetLayout();
	return 0;
}

int TL_ViewWindow::ResetDisplayMode(DISPLAY_MODE dmode)
{
	m_CurDisplayMode = dmode;
	ResetLayout();
	return 0;
}

int TL_ViewWindow::ResetLayoutMode(LAYOUT_MODE lmode)
{
	m_CurLayoutMode = lmode;
	ResetLayout();
	return 0;
}

int TL_ViewWindow::FrameDataInit(int size, int width, int height)
{
	TL_ViewPort* p_ViewPort = NULL;
	for(int i = 0; i < m_CurViewPortCount; i++)
	{
		p_ViewPort = m_pViewPortArray[i];
		p_ViewPort->FrameDataInit(size,width,height);
	}
	return 0;
}

int TL_ViewWindow::UpdateViewPortData(int tid, long pts, int index, UINT8* data[], int linesize[], int src_w, int src_h)
{
	if( index < 0 || index >= m_CurViewPortCount)
	{
		return -1;
	}

	TL_ViewPort* p_ViewPort = NULL;
	p_ViewPort = m_pViewPortArray[index];
	p_ViewPort->SetRGBData( tid, pts, data, linesize, src_w, src_h);
	return 0;
}

int TL_ViewWindow::UpdateDisplay(int index)
{
	if( index < 0 || index >= m_CurViewPortCount)
	{
		return -1;
	}

	TL_ViewPort* p_ViewPort = NULL;
	p_ViewPort = m_pViewPortArray[index];
	p_ViewPort->UpdateDisplay();
	return 0;
}

int TL_ViewWindow::UpdatePlayTime(int cur_time, int total_time)
{
	std::string cur_time_string;
	std::string total_time_string;
	CString cur_time_cstr;
	CString total_time_cstr;

	TL_Pts2StdString(int64_t(cur_time),cur_time_string);
	TL_Pts2StdString(int64_t(total_time),total_time_string);
	cur_time_cstr = cur_time_string.c_str();
	total_time_cstr = total_time_string.c_str();

	CString play_time_cstr;
	play_time_cstr = cur_time_cstr + CString(" / ") + total_time_cstr;
	m_StaticPlayTime.SetWindowText(play_time_cstr);
	return 0;
}

int TL_ViewWindow::UpdatePlayStatus(PLAY_STATUS status)
{
	switch(status)
	{
	case PS_OPENING_MEDIA: m_StaticPlayStatus.SetWindowText(_T("正在打开媒体")); break;
	case PS_PLAYING: m_StaticPlayStatus.SetWindowText(_T("播放中")); break;
	case PS_PAUSED: m_StaticPlayStatus.SetWindowText(_T("暂停中")); break;
	case PS_STOPED: m_StaticPlayStatus.SetWindowText(_T("未播放")); break;
	default:break;
	}
	return 0;
}

int TL_ViewWindow::ClearDisplay()
{
	//SetBackgroundColor(RGB(0,0,0),1);
	TL_ViewPort* p_ViewPort = NULL;
	for(int i = 0; i < m_CurViewPortCount; i++)
	{
		p_ViewPort = m_pViewPortArray[i];
		p_ViewPort->ClearDisplay();
	}
	return 0;
}

int TL_ViewWindow::ScreenShot()
{
	TL_ViewPort* p_ViewPort = NULL;
	for(int i = 0; i < m_CurViewPortCount; i++)
	{
		p_ViewPort = m_pViewPortArray[i];
		p_ViewPort->SaveImage();
	}
	return 0;
}

void TL_ViewWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CString strMsg;
	//strMsg.Format(_T("TL_ViewWindow::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	//OutputDebugString(strMsg);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



