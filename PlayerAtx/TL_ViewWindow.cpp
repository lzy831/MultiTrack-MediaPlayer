// TL_ViewWindow.cpp : ʵ���ļ�
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
// TL_ViewWindow �Ի���

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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

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

// TL_ViewWindow ��Ϣ�������
HBRUSH TL_ViewWindow::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//if( nCtlColor == CTLCOLOR_STATIC)
	if( pWnd == (CWnd*)(&m_StaticPlayTime))
	{
		// ����PlayTimeStatic��ɫ
		pDC->SetTextColor(PlayTimeFontColor);// ����������ɫ
		pDC->SetBkColor(PlayTimeFontBkColor);// �������屳����ɫ
		return m_StaticPlayTimeBrush;//�����Զ��廭ˢ�����ڻ��������ͻ�����ɫ
	}
	if( pWnd == (CWnd*)(&m_StaticPlayStatus))
	{
		pDC->SetTextColor(PlayTimeFontColor);// ����������ɫ
		pDC->SetBkColor(PlayTimeFontBkColor);// �������屳����ɫ
		return m_StaticPlayStatusBrush;//�����Զ��廭ˢ�����ڻ��������ͻ�����ɫ
	}

	if( nCtlColor == CTLCOLOR_DLG)
	{
		return m_StaticPlayTimeBrush;
	}

	for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
	{
		if( pWnd == (CWnd*)(m_pViewPortArray[i]))
		{
			pDC->SetTextColor(RGB(255,255,0));	// ����������ɫ
			pDC->SetBkColor(RGB(0,0,0));		// �������屳����ɫ
			return m_StaticPlayTimeBrush;		//�����Զ��廭ˢ�����ڻ��������ͻ�����ɫ
		}
	}

	return hbr;
}

void TL_ViewWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	MyOnSize(nType, cx, cy);
}



/************************************************************************************************************************************************/
/*  �Լ����ӵĺ���                                                                     */
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
		// ����ViewPort  ����������ʾλ�÷���ResetViewPortLayout��
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

		// ���õ�ǰ�����ʾģʽ
		m_CurViewPortCount = m_MaxViewPortCount;
		m_CurDisplayMode = DMODE_4_3;
		m_CurLayoutMode = LMODE_1;

		RECT rect;
		// ����ViewPort�Ķ�Ӧ��ǩ  ����������ʾλ�÷���ResetViewPortLayout��
		for(int i = 0; i< m_MaxViewPortCount ;i++)
		{
			m_StaticViewPortTag[i].Create(_T(""),WS_CHILD,rect,this,IDD_STATIC_VIEWPORT_TAG_BASE+i);
		}

		// ���� ���� ����ʱ���ִ�  ����������ʾλ�÷���ResetViewPortLayout��
		m_StaticPlayTime.Create(_T("PlayingTime"),WS_CHILD,rect,this,IDD_STATIC_PLAYING_TIME);
		m_StaticPlayTime.SetWindowText(_T("00:00:00 / 00:00:00"));

		m_StaticPlayStatus.Create(_T("PlayingStatus"),WS_CHILD,rect,this,IDD_STATIC_PLAY_STATUS);;
		m_StaticPlayStatus.SetWindowText(_T("δ����"));

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

	DWORD LittleWinCount = m_CurViewPortCount - 1;	// ����С����(ViewPort�ĸ���)

	DWORD BigWinRangeW = 0;		//�󴰿ڵ���ʾ�����
	DWORD BigWinRangeH = 0;		//�󴰿ڵ���ʾ�����
	DWORD BigWinW = 0;			//�󴰿ڿ�
	DWORD BigWinH = 0;			//�󴰿ڸ�
	DWORD BigWinXPadding = 0;	//�󴰿ں������
	DWORD BigWinYPadding = 0;	//�󴰿��������

	DWORD LittleWinRangeW = 0;	//С���ڵ���ʾ�����
	DWORD LittleWinRangeH = 0;	//С���ڵ���ʾ�����
	DWORD LittleWinW = 0;		//С���ڿ�
	DWORD LittleWinH = 0;		//С���ڸ�
	DWORD LitteWinXPadding = 0; //С���ں������
	DWORD LitteWinYPadding = 0;	//С�����������


	if( LittleWinCount == 0)
	{
		// ���ֻ�д��棬����ͼ���
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
		if( lmode == LMODE_1 /*������С������*/ || lmode == LMODE_2 /*С�����󣬴����� */)
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

				// ���ۼ���С���ڣ��󴰿�λ���ǹ̶���
				WinRect[0].top = ViewPortRect.top + BigWinYPadding;
				WinRect[0].bottom = WinRect[0].top + BigWinH;
				WinRect[0].left = WinRect[1].right + space + BigWinXPadding;
				WinRect[0].right = WinRect[0].left + BigWinW;

			}
			else
			{
				// ���ۼ���С���ڣ��󴰿�λ���ǹ̶���
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
	// ����view port ���� �� ����
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
	case PS_OPENING_MEDIA: m_StaticPlayStatus.SetWindowText(_T("���ڴ�ý��")); break;
	case PS_PLAYING: m_StaticPlayStatus.SetWindowText(_T("������")); break;
	case PS_PAUSED: m_StaticPlayStatus.SetWindowText(_T("��ͣ��")); break;
	case PS_STOPED: m_StaticPlayStatus.SetWindowText(_T("δ����")); break;
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CString strMsg;
	//strMsg.Format(_T("TL_ViewWindow::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	//OutputDebugString(strMsg);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



