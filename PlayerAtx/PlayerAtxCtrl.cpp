// PlayerAtxCtrl.cpp : CPlayerAtxCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "PlayerAtx.h"
#include "PlayerAtxCtrl.h"
#include "PlayerAtxPropPage.h"
#include "afxdialogex.h"

#include "TL_ViewWindow.h"
#include "TL_Message.h"
#include "TL_Color.h"
#include "PlayerCoreDLL/PlayerCoreInterface.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPlayerAtxCtrl, COleControl)

// 消息映射

BEGIN_MESSAGE_MAP(CPlayerAtxCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)

	ON_MESSAGE(WM_TL_OPT_PLAY_STOP,OnMsgOptPlayStop)
	
	ON_MESSAGE(WM_TL_UPDATE_DISPLAY, OnMsgUpdateDisplay)
	ON_MESSAGE(WM_TL_PLAY_TIME_UPDATE, OnMsgPlayTimeUpdate)
	ON_MESSAGE(WM_TL_PLAY_OPENING_MEDIA, OnMsgOpeningMedia)
	ON_MESSAGE(WM_TL_PLAY_STARTED, OnMsgPlayStarted)
	ON_MESSAGE(WM_TL_PLAY_STOPED, OnMsgPlayStoped)
	ON_MESSAGE(WM_TL_PLAY_PAUSED, OnMsgPlayPaused)
	ON_MESSAGE(WM_TL_PLAY_TO_END, OnMsgPlayToEnd)
	ON_MESSAGE(WM_TL_VOLUME_CHANGED, OnMsgVolumeChanged)
	ON_MESSAGE(WM_TL_AUDIO_CHANGED, OnMsgAudioChanged)
	ON_MESSAGE(WM_TL_VIDEO_CHANGED, OnMsgVideoChanged)

	ON_MESSAGE(WM_TL_PLAY_SCREEN_MODE_CHANGED, OnMsgScreenModeChanged)
	

	ON_MESSAGE(WM_TL_PLAY_EXIT_FULL_SCREEN, OnMsgExitFullScreen)
	ON_MESSAGE(WM_TL_PLAY_VIEW_PORT_ON_DCLICK, OnMsgViewPortDClick)

END_MESSAGE_MAP()


// 调度映射

BEGIN_DISPATCH_MAP(CPlayerAtxCtrl, COleControl)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayAdd", dispidPlayAdd, PlayAdd, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayStart", dispidPlayStart, PlayStart, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayStop", dispidPlayStop, PlayStop, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlaySeek", dispidPlaySeek, PlaySeek, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayTogglePause", dispidPlayTogglePause, PlayTogglePause, VT_I4, VTS_NONE)	
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayToggleFullScreen", dispidPlayToggleFullScreen, PlayToggleFullScreen, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "GetVideoCount", dispidGetVideoCount, GetVideoCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "GetAudioCount", dispidGetAudioCount, GetAudioCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "GetDuration", dispidGetDuration, GetDuration, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "EnableAudio", dispidEnableAudio, EnableAudio, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "EnableVideo", dispidEnableVideo, EnableVideo, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PriorVideo", dispidPriorVideo, PriorVideo, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "AudioStatus", dispidAudioStatus, AudioStatus, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "VideoStatus", dispidVideoStatus, VideoStatus, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "SetVolume", dispidSetVolume, SetVolume, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "GetVolume", dispidGetVolume, GetVolume, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "SetViewPortCount", dispidSetViewPortCount, SetViewPortCount, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "SetViewWindowMode", dispidSetViewWindowMode, SetViewWindowMode, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "SetDisplayMode", dispidSetDisplayMode, SetDisplayMode, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "EnableAutoDirector", dispidEnableAutoDirector, EnableAutoDirector, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "PlayClearList", dispidPlayClearList, PlayClearList, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerAtxCtrl, "ScreenShot", dispidScreenShot, ScreenShot, VT_I4, VTS_NONE)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CPlayerAtxCtrl, COleControl)

	EVENT_CUSTOM_ID("EventOnPlayTimeUpdate", eventidEventOnPlayTimeUpdate, EventOnPlayTimeUpdate, VTS_I4)
	EVENT_CUSTOM_ID("EventOnPlayStarted", eventidEventOnPlayStarted, EventOnPlayStarted, VTS_NONE)
	EVENT_CUSTOM_ID("EventOnPlayStoped", eventidEventOnPlayStoped, EventOnPlayStoped, VTS_NONE)
	EVENT_CUSTOM_ID("EventOnPlayPaused", eventidEventOnPlayPaused, EventOnPlayPaused, VTS_NONE)
	EVENT_CUSTOM_ID("EventOnPlayToEnd", eventidEventOnPlayToEnd, EventOnPlayToEnd, VTS_NONE)
	EVENT_CUSTOM_ID("EventOnScreenChanged", eventidEventOnScreenChanged, EventOnScreenChanged, VTS_I4)
	EVENT_CUSTOM_ID("EventOnVolumeChanged", eventidEventOnVolumeChanged, EventOnVolumeChanged, VTS_I4)
	EVENT_CUSTOM_ID("EventOnAudioChanged", eventidEventOnAudioChanged, EventOnAudioChanged, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("EventOnVideoChanged", eventidEventOnVideoChanged, EventOnVideoChanged, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("EventOnOpeningMedia", eventidEventOnOpeningMedia, EventOnOpeningMedia, VTS_NONE)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CPlayerAtxCtrl, 1)
	PROPPAGEID(CPlayerAtxPropPage::guid)
END_PROPPAGEIDS(CPlayerAtxCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CPlayerAtxCtrl, "PLAYERATX.PlayerAtxCtrl.1",
	0x8983d1ae, 0x5d45, 0x4810, 0x91, 0x9, 0xec, 0x25, 0x7c, 0xff, 0x7f, 0x97)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CPlayerAtxCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DPlayerAtx = { 0x13051A81, 0x92AD, 0x45D1, { 0xBF, 0xF2, 0xDD, 0x46, 0x2F, 0xF4, 0xC4, 0xEC } };
const IID IID_DPlayerAtxEvents = { 0x3BB4CB3B, 0x2338, 0x419B, { 0xAC, 0x68, 0x27, 0x6C, 0x1C, 0xA, 0x66, 0x88 } };


// 控件类型信息

static const DWORD _dwPlayerAtxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPlayerAtxCtrl, IDS_PLAYERATX, _dwPlayerAtxOleMisc)



// CPlayerAtxCtrl::CPlayerAtxCtrlFactory::UpdateRegistry -
// 添加或移除 CPlayerAtxCtrl 的系统注册表项

BOOL CPlayerAtxCtrl::CPlayerAtxCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_PLAYERATX,
			IDB_PLAYERATX,
			afxRegApartmentThreading,
			_dwPlayerAtxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CPlayerAtxCtrl::CPlayerAtxCtrl - 构造函数

CPlayerAtxCtrl::CPlayerAtxCtrl()
	//:m_PlayerCore(NULL)
{
	InitializeIIDs(&IID_DPlayerAtx, &IID_DPlayerAtxEvents);
	// TODO: 在此初始化控件的实例数据。

	TL_PlayerInit();

	memset(&m_cs, 0, sizeof(m_cs));
	InitializeCriticalSection(&m_cs);

	//m_pViewWindow = NULL;
	m_inited  = 0;
	m_PlayList.clear();
	m_DurationInSecond = 0;
	m_ViewPortOpened = 0;
}



// CPlayerAtxCtrl::~CPlayerAtxCtrl - 析构函数

CPlayerAtxCtrl::~CPlayerAtxCtrl()
{
	// TODO: 在此清理控件的实例数据。

	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> ~CPlayerAtxCtrl\n");
	OutputDebugString(strMsg);

	// 这里必须同步的方式关闭播放器，使控件在销毁前，确保播放器资源被正常的释放
	TL_PlayStop();
	TL_PlayerRelease();

	//if(m_pViewWindow != NULL)
	//{
	//	delete m_pViewWindow;
	//	m_pViewWindow = NULL;
	//}
}



// CPlayerAtxCtrl::OnDraw - 绘图函数

void CPlayerAtxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);


	CBrush b;
	b.CreateSolidBrush(PlayerAtxBkColor);
	pdc->FillRect(rcBounds, &b);
#if 0
	if( m_inited == 0)
	{
		if( m_pViewWindow == NULL)
		{
			m_pViewWindow = new TL_ViewWindow();
		}

		BOOL Ret = FALSE;
		Ret = m_pViewWindow->Create(IDD_TL_VIEWWINDOW,this);
		//ASSERT(Ret!=0);
		if( Ret)
		{
			// 设置PlayerAtx主界面
			m_pViewWindow->SetBackgroundColor(RGB(0,0,0),1);
			// 创建ViewPort
			m_pViewWindow->CreateViewPort();
			// 初始化播放器
			PlayerAtxInit();
			m_inited = 1;
		}

	}

	if( m_inited == 1 && m_pViewWindow != NULL)
	{
		/*
		* 如果主界面之前已经初始化好了，这里重绘主界面
		*/

		//CString strMsg;
		//strMsg.Format(_T(">>>>>>>>>>>>>>>>>>>>>>>>> OnDraw %d %d %d %d\n"),rcBounds.top, rcBounds.bottom, rcBounds.left, rcBounds.right);
		//OutputDebugString(strMsg);

		::SetWindowPos(m_pViewWindow->m_hWnd
			, HWND_TOP
			, rcBounds.left
			, rcBounds.top
			, rcBounds.Width()
			, rcBounds.Height()
			, SWP_SHOWWINDOW);
	}
#endif

	if( m_inited == 0)
	{
		BOOL Ret = FALSE;

		Ret = m_ViewWinNormal.Create(IDD_TL_VIEWWINDOW,this);
		ASSERT(Ret);
		if( Ret)
		{
			m_ViewWinNormal.SetBackgroundColor(ViewWinWrapBkColor,1);
			// 创建ViewPort
			//m_ViewWinNormal.CreateViewPort();
		}
		
		Ret = m_ViewWinFullScreen.Create(IDD_VIEW_WIN_WRAP_FULLSCREEN,this);
		//m_ViewWinFullScreen.SetParent();
		m_ViewWinFullScreen.SetPlayerAtxCtrl(this);
		ASSERT(Ret);
		if( Ret)
		{
			m_ViewWinFullScreen.SetBackgroundColor(ViewWinWrapBkColor,1);
			// 创建ViewPort
			//m_ViewWinFullScreen.CreateViewPort();
			// 初始化播放器
		}
		//::SetWindowPos(m_ViewWinFullScreen.m_hWnd
		//	, HWND_TOP
		//	, 0
		//	, 0
		//	, 1200
		//	, 800
		//	, SWP_HIDEWINDOW);

		// 初始化播放器
		PlayerAtxInit();
		m_CurViewWin = 0;// 默认设置为 正常窗口
		m_inited = 1;
	}

	if( m_inited == 1)
	{
		if( m_CurViewWin == 0 && m_ViewWinNormal.m_hWnd != NULL)
		{
			::SetWindowPos(m_ViewWinNormal.m_hWnd
				, HWND_TOP
				, rcBounds.left
				, rcBounds.top
				, rcBounds.Width()
				, rcBounds.Height()
				, SWP_SHOWWINDOW);
		}
		//else if( m_CurViewWin == 1 && m_ViewWinFullScreen.m_hWnd != NULL)
		//{
		//	int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
		//	int nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度

		//	::SetWindowPos(m_ViewWinFullScreen.m_hWnd
		//		, HWND_TOP
		//		, 0
		//		, 0
		//		, nWidth
		//		, nHeight
		//		, SWP_SHOWWINDOW);
		//}
	}
}



// CPlayerAtxCtrl::DoPropExchange - 持久性支持

void CPlayerAtxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CPlayerAtxCtrl::OnResetState - 将控件重置为默认状态

void CPlayerAtxCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}

// CPlayerAtxCtrl 消息处理程序















/*
* PlayerAtx控件 注册给 Player的回调函数
* 这些回调函数，
* 有些需要改变界面，渲染视频，如初始化和反初始化视频端口
* 有些需要触发一个Evnet给外层容器，如告知Player的状态
*/

int func_opt_video_frame_display(void* user_data, int tid, long pts, int viewport_id, UINT8** data, int* linesize, int width, int height)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->OptVideoFrameDisplay(tid,pts,viewport_id,data,linesize,width,height);
}

int func_opt_frame_data_init(void* user_data, int size, int width, int height)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->OptFrameDataInit(size,width,height);
}

int func_opt_open_view_port(void* user_data, long count)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->OptOpenViewPort(count);
}

int func_opt_close_view_port(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->OptCloseViewPort();
}

/*
* Player 播放状态通知回调
*/
int func_cb_on_play_time_update(void* user_data, long pts)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnPlayTimeUpdate(pts);
}

int func_cb_on_opening_media(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnOpeningMeida();
}

int func_cb_on_play_started(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnPlayStarted();
}

int func_cb_on_play_stoped(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnPlayStoped();
}

int func_cb_on_play_paused(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnPlayPaused();
}

int func_cb_on_play_to_end(void* user_data)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnPlayToEnd();
}

int func_cb_on_volume_changed(void* user_data, long volume)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnVolumeChanged(volume);
}

int func_cb_on_audio_changed(void* user_data, int audio_index, int track_id, int status)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnAudioChanged(audio_index,track_id,status);
}

int func_cb_on_video_changed(void* user_data, int video_index, int track_id, int status)
{
	CPlayerAtxCtrl* atx = (CPlayerAtxCtrl*)user_data;
	return atx->CbOnVideoChanged(video_index,track_id,status);
}

/*
PlayerAtx控件 对应Player的回调函数， 其中一些需要激发Player定义的Event
*/
int CPlayerAtxCtrl::OptVideoFrameDisplay(int tid, long pts, int viewport_index, UINT8** data, int* linesize, int width, int height)
{
	//m_pViewWindow->UpdateViewPortData( viewport_index, data, linesize, width, height);

	//if( width == 0 && height == 0)
	//{
	//	printf("123");
	//}

	//CString strMsg;
	//strMsg.Format(_T("tid: %d   pts: %d    vp: %d    %d x %d\n"), tid, pts, viewport_index, width, height);
	//OutputDebugString(strMsg);


	if(m_CurViewWin==0)
	{
		m_ViewWinNormal.UpdateViewPortData( tid, pts, viewport_index, data, linesize, width, height);
	}
	else if(m_CurViewWin==1)
	{
		m_ViewWinFullScreen.UpdateViewPortData(  tid, pts, viewport_index, data, linesize, width, height);
	}
	else
	{
		ASSERT(0);
	}
	
	// 这里如果用同步的SendMessage操作，可有导致在用户调用CPlayerAtxCtrl::PlayStart中死锁，因为这个函数是在video_schedule中回调的
	// 比如先进入了 CPlayerAtxCtrl::PlayStart --> MediaPlayer::PlayStop中，
	// 在MediaPlayer::PlayStop中 等待 video_schedule的退出，而video_schedule正好处于需要调用CPlayerAtxCtrl::OptVideoFrameDisplay的时机
	// 而因为CPlayerAtxCtrl::OptVideoFrameDisplay本身就是需要在Win消息线程中执行的，又需要等待MediaPlayer::PlayStop返回
	// 这样就死锁了，解锁的方法很多
	// 这里使用PostMessage发送 异步的 WM_TL_UPDATE_DISPLAY
	// 但是这样有个问题，就是可能 WM_TL_UPDATE_DISPLAY 晚于 PlayStoped 的事件， 
	// 这样在 MediaPlayer调用回调关闭ViewPort后， 这个 WM_TL_UPDATE_DISPLAY会导致在刷一帧图像到ViewPort，这个问题比较好解决
	// 可以通过 m_ViewPortOpened 判断是否要刷新显示
	//SendMessage( WM_TL_UPDATE_DISPLAY, 0,(LPARAM)viewport_index );
	PostMessage( WM_TL_UPDATE_DISPLAY, 0,(LPARAM)viewport_index );
	return 0;
}

int CPlayerAtxCtrl::OptFrameDataInit(int size, int width, int height)
{
	m_ViewWinNormal.FrameDataInit(size,width,height);
	m_ViewWinFullScreen.FrameDataInit(size,width,height);
	return 0;
}

int CPlayerAtxCtrl::OptOpenViewPort(int viewport_count)
{
	// 重新设置ViewPort的数量
	m_ViewWinNormal.ResetViewPortCount(viewport_count);
	m_ViewWinFullScreen.ResetViewPortCount(viewport_count);
	m_ViewPortOpened = 1;
	return 0;
}

int CPlayerAtxCtrl::OptCloseViewPort()
{
	m_ViewWinNormal.ClearDisplay();
	m_ViewWinFullScreen.ClearDisplay();
	m_ViewPortOpened = 0;
	return 0;
}

int CPlayerAtxCtrl::CbOnPlayTimeUpdate(long pts)
{
	// 这里不能直接激发FireEvent,必须在窗体的线程中FireEvent才有效果,其他Event处理方式类似
	// 注意要用 异步的PostMessage 而不能用 同步的SendMessage
	PostMessage( WM_TL_PLAY_TIME_UPDATE, 0,(LPARAM)pts);
	return 0;
}

int CPlayerAtxCtrl::CbOnOpeningMeida()
{
	PostMessage( WM_TL_PLAY_OPENING_MEDIA, 0, 0);
	return 0;
}

int CPlayerAtxCtrl::CbOnPlayStarted()
{
	PostMessage( WM_TL_PLAY_STARTED, 0, 0);
	return 0;
}

int CPlayerAtxCtrl::CbOnPlayStoped()
{
	PostMessage( WM_TL_PLAY_STOPED, 0, 0);
	return 0;
}

int CPlayerAtxCtrl::CbOnPlayPaused()
{
	PostMessage( WM_TL_PLAY_PAUSED, 0, 0);
	return 0;
}

int CPlayerAtxCtrl::CbOnPlayToEnd()
{
	// 播放已经结束了,先停止播放在通知上层容器
	// 但是不能在回调里直接执行，要用异步消息转一下, 要不可能会死锁， 因为 PlayStop操作接口 可能会和 Callback的操作线程 互斥
	PostMessage( WM_TL_OPT_PLAY_STOP, 0, 0);
	// 当然以后可以适应真正的列表播放，这里就需要自动播放下一个视频了，同样是需要发送异步消息
	PostMessage( WM_TL_PLAY_TO_END, 0, 0);
	return 0;
}

int CPlayerAtxCtrl::CbOnVolumeChanged(long volume)
{
	PostMessage( WM_TL_VOLUME_CHANGED, 0, volume);
	return 0;
}

int CPlayerAtxCtrl::CbOnAudioChanged(int audio_index,int track_id, int status)
{
	CString strMsg;
	strMsg.Format(_T("------------------------------CPlayerAtxCtrl::CbOnAudioChanged %d %d %d\n"),audio_index,track_id,status);
	OutputDebugString(strMsg);

	AVChangedParam* param = new AVChangedParam;
	param->index = audio_index;
	param->id = track_id;
	param->status = status;
	PostMessage( WM_TL_AUDIO_CHANGED, 0, (LPARAM)param);
	return 0;
}

int CPlayerAtxCtrl::CbOnVideoChanged(int video_index,int track_id, int status)
{

	CString strMsg;
	strMsg.Format(_T("------------------------------CPlayerAtxCtrl::CbOnVideoChanged %d %d %d \n"),video_index,track_id,status);
	OutputDebugString(strMsg);

	AVChangedParam* param = new AVChangedParam;
	param->index = video_index;
	param->id = track_id;
	param->status = status;
	PostMessage( WM_TL_VIDEO_CHANGED, 0, (LPARAM)param);
	return 0;
}


/*
* 自定义窗口消息响应函数
*/
HRESULT CPlayerAtxCtrl::OnMsgOptPlayStop( WPARAM wParam, LPARAM lParam)
{
	TL_PlayStopAsync();
	return 0;
}

/*
播放器消息响应函数
*/
HRESULT CPlayerAtxCtrl::OnMsgUpdateDisplay( WPARAM wParam, LPARAM lParam)
{
	//return (HRESULT)(m_pViewWindow->UpdateDisplay((int)lParam));
	if(m_ViewPortOpened==0)
	{
		return 0;
	}
	
	if(m_CurViewWin==0)
	{
		//EnterCriticalSection( &m_cs );
		m_ViewWinNormal.UpdateDisplay((int)lParam);
		//LeaveCriticalSection( &m_cs );
		return 0;
	}
	else if(m_CurViewWin==1)
	{
		//EnterCriticalSection( &m_cs );
		m_ViewWinFullScreen.UpdateDisplay((int)lParam);
		//EnterCriticalSection( &m_cs );
		return 0;
	}
	else
	{
		ASSERT(0);
		return 0;
	}
}

//HRESULT OnMsgOpenViewPort( WPARAM wParam, LPARAM lParam)
//{
//	//
//	return 0;
//}
//
//HRESULT OnMsgCloseViewPort( WPARAM wParam, LPARAM lParam)
//{
//	//
//	return 0;
//}


HRESULT CPlayerAtxCtrl::OnMsgPlayTimeUpdate( WPARAM wParam, LPARAM lParam)
{
	long pts = (long)lParam;

	DoUpdatePlayTime(pts);

	EventOnPlayTimeUpdate(pts);
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgPlayStarted( WPARAM wParam, LPARAM lParam)
{
	if( m_DurationInSecond == 0)
	{
		m_DurationInSecond = TL_GetDuration();
		DoUpdatePlayTime(0);
	}

	EventOnPlayStarted();

	DoUpdatePlayStatus(PS_PLAYING);
	// 播放开始时，触发一次屏幕模式事件
	PostMessage(WM_TL_PLAY_SCREEN_MODE_CHANGED,0,m_CurViewWin);
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgOpeningMedia(WPARAM wParam, LPARAM lParam)
{
	DoUpdatePlayStatus(PS_OPENING_MEDIA);
	EventOnOpeningMedia();

	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgPlayStoped( WPARAM wParam, LPARAM lParam)
{
	// Reset Playing Time
	m_DurationInSecond = 0;
	DoUpdatePlayTime(0);
	
	DoUpdatePlayStatus(PS_STOPED);
	EventOnPlayStoped();
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgPlayPaused( WPARAM wParam, LPARAM lParam)
{
	DoUpdatePlayStatus(PS_PAUSED);
	EventOnPlayPaused();
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgPlayToEnd( WPARAM wParam, LPARAM lParam)
{
	EventOnPlayToEnd();
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgVolumeChanged( WPARAM wParam, LPARAM lParam)
{
	EventOnVolumeChanged((long)lParam);
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgAudioChanged( WPARAM wParam, LPARAM lParam)
{
	AVChangedParam* param = (AVChangedParam*)lParam;
	EventOnAudioChanged((long)(param->index),(long)(param->id),(long)(param->status));
	delete param;
	return 0; 
}

HRESULT CPlayerAtxCtrl::OnMsgVideoChanged( WPARAM wParam, LPARAM lParam)
{
	AVChangedParam* param = (AVChangedParam*)lParam;
	EventOnVideoChanged((long)(param->index),(long)(param->id),(long)(param->status));
	delete param;
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgExitFullScreen(WPARAM wParam, LPARAM lParam)
{
	ToggleFullScreen();
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgScreenModeChanged(WPARAM wParam, LPARAM lParam)
{
	EventOnScreenChanged(long(lParam));
	return 0;
}

HRESULT CPlayerAtxCtrl::OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam)
{
	int index = (int)lParam;
	return DoOnViewPortDClick(index);
	//return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

int CPlayerAtxCtrl::PlayerAtxInit()
{
	TL_SetOptVideoFrameDisplay(&func_opt_video_frame_display, this);
	TL_SetOptFrameDataInit(&func_opt_frame_data_init,this);
	TL_SetOptViewPortOpen(&func_opt_open_view_port,this);
	TL_SetOptViewPortClose(&func_opt_close_view_port,this);

	TL_SetCbOnPlayTimeUpdate(&func_cb_on_play_time_update,this);
	TL_SetCbOnPlayOpeningMeida(&func_cb_on_opening_media,this);
	TL_SetCbOnPlayStarted(&func_cb_on_play_started,this);
	TL_SetCbOnPlayStoped(&func_cb_on_play_stoped,this);
	TL_SetCbOnPlayPaused(&func_cb_on_play_paused,this);
	TL_SetCbOnPlayEnd(&func_cb_on_play_to_end,this);
	TL_SetCbOnVolumeChanged(&func_cb_on_volume_changed,this);
	TL_SetCbOnAudioChanged(&func_cb_on_audio_changed,this);
	TL_SetCbOnVideoChanged(&func_cb_on_video_changed,this);
	return 0;
}

int CPlayerAtxCtrl::ToggleFullScreen()
{

	int    nWidth=GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
	int    nHeight=GetSystemMetrics(SM_CYSCREEN); //屏幕高度

	//EnterCriticalSection( &m_cs );
	if( m_CurViewWin == 0)
	{
		//m_ViewWinFullScreen.ShowWindow(SW_SHOW);
		//::SetFocus(m_ViewWinFullScreen);

		
		::SetWindowPos(m_ViewWinFullScreen.m_hWnd
			, HWND_TOP
			, 0
			, 0
			, nWidth
			, nHeight
			, SWP_SHOWWINDOW);
		//m_ViewWinFullScreen.UpdateWindow();

		//::SetFocus(m_ViewWinFullScreen);
		m_CurViewWin = 1;
		PostMessage(WM_TL_PLAY_SCREEN_MODE_CHANGED,0,m_CurViewWin);
	}
	else if( m_CurViewWin == 1)
	{
		m_ViewWinFullScreen.ShowWindow(SW_HIDE);
		//::SetFocus((HWND)this);
		m_CurViewWin = 0;
		PostMessage(WM_TL_PLAY_SCREEN_MODE_CHANGED,0,m_CurViewWin);
	}
	//LeaveCriticalSection( &m_cs );

	//CDialogEx p;
	//p.Create(888,this);


	//::SetWindowPos(p.m_hWnd,
	//	HWND_TOPMOST,
	//	0, 
	//	0, 
	//	400, 
	//	300, 
	//	SWP_SHOWWINDOW);

	//m_pViewWindow->ShowWindow(SW_SHOWMAXIMIZED);

	//style &= ~(WS_DLGFRAME | WS_THICKFRAME);
	//SetWindowLong(this->m_hWnd,GWL_STYLE, style);
	//this->ShowWindow(SW_SHOWMAXIMIZED);
	//CRect rect;
	//this->GetWindowRect(&rect);
	//::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,rect.left-1, rect.top-1, rect.right-rect.left + 3, rect.bottom-rect.top + 3, SWP_FRAMECHANGED);

	return 0;
}

int CPlayerAtxCtrl::DoUpdatePlayTime(long cur_time)
{
	int total_time_in_ms = m_DurationInSecond*1000;

	//m_pViewWindow->UpdatePlayTime(cur_time,total_time_in_ms);

	if(m_CurViewWin==0)
	{
		m_ViewWinNormal.UpdatePlayTime(cur_time,total_time_in_ms);
	}
	else if(m_CurViewWin==1)
	{
		m_ViewWinFullScreen.UpdatePlayTime(cur_time,total_time_in_ms);
	}
	return 0;
}

int CPlayerAtxCtrl::DoUpdatePlayStatus(PLAY_STATUS status)
{
	m_ViewWinNormal.UpdatePlayStatus(status);
	m_ViewWinFullScreen.UpdatePlayStatus(status);
	return 0;
}

int CPlayerAtxCtrl::DoOnViewPortDClick(int index)
{
	//CString strMsg;
	//strMsg.Format(_T("------------------------------CPlayerAtxCtrl::OnMsgViewPortDClick 0x%x\n"),index);
	//OutputDebugString(strMsg);

	if(index==0)// 大窗口双击，最大化切换
	{
		ToggleFullScreen();
	} 
	else if( index > 0 && index < 4) // 小窗口切换，大小画面交换
	{
		TL_SwapVideoViewPort(index,0);
	}
	return 0;
}

/************************************************************************/
/* Atx接口函数                                                          */
/************************************************************************/

LONG CPlayerAtxCtrl::PlayAdd(LPCTSTR url)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	//AfxMessageBox(url);

	CString CStrUrl(url);
	m_PlayList.push_back(CStrUrl);
	return 0;
}

LONG CPlayerAtxCtrl::PlayStart(LONG Index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	DWORD item_index = (int)Index;
	//CString msg;
	//msg.Format(_T("index: %d m_PlayList.size: %d"), item_index, m_PlayList.size());
	//AfxMessageBox(msg);
	
	if( item_index >= 0 && item_index < m_PlayList.size())
	{
		CString& CStrUrl = m_PlayList[item_index];
		//AfxMessageBox(CStrUrl);
		CStringA Urla(CStrUrl.GetBuffer(0));
		CStrUrl.ReleaseBuffer();
		std::string stdURL(Urla.GetBuffer(0));
		Urla.ReleaseBuffer();

		TL_PlayInitAsync(stdURL);
		TL_PlayStartAsync();
		return 0;
	}
	return -1;
}

LONG CPlayerAtxCtrl::PlayStop(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	
	// 对于播放停止，使用同步的操作，比较安全点
	
	{
		CString msg("CPlayerAtxCtrl::PlayStop B TL_PlayStop\n");
		OutputDebugString(msg);
	}

	//TL_PlayStopAsync();
	TL_PlayStop();//

	{
		CString msg("CPlayerAtxCtrl::PlayStop A TL_PlayStop\n");
		OutputDebugString(msg);
	}

	return 0;
}


LONG CPlayerAtxCtrl::PlaySeek(LONG pts_in_s)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	int64_t pts_ms = (int64_t)pts_in_s*1000;
	TL_PlaySeekAsync(pts_ms);
	return 0;
}

LONG CPlayerAtxCtrl::PlayTogglePause(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_PlayTogglePauseAsync();
	return 0;
}

LONG CPlayerAtxCtrl::PlayToggleFullScreen(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	ToggleFullScreen();
	return 0;
}


LONG CPlayerAtxCtrl::GetVideoCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	long video_count = (int)TL_GetVideoCount();
	return video_count;
}


LONG CPlayerAtxCtrl::GetAudioCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	long audio_count = (int)TL_GetAudioCount();
	return audio_count;
}


LONG CPlayerAtxCtrl::GetDuration(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	long duration_in_second = (long)TL_GetDuration();
	return duration_in_second;
}


LONG CPlayerAtxCtrl::EnableAudio(LONG AudioIndex, LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_EnableAudio(AudioIndex,Enable);
	return 0;
}


LONG CPlayerAtxCtrl::EnableVideo(LONG VideoIndex, LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_EnableVideo(VideoIndex,Enable);
	return 0;
}


LONG CPlayerAtxCtrl::PriorVideo(LONG VideoIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_SetVideoPrior(VideoIndex);
	return 0;
}


LONG CPlayerAtxCtrl::AudioStatus(LONG AudioIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return TL_AudioStatus(AudioIndex);
}


LONG CPlayerAtxCtrl::VideoStatus(LONG VideoIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return TL_VideoStatus(VideoIndex);
}


LONG CPlayerAtxCtrl::SetVolume(LONG volume)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_SetVolume(volume);
	return 0;
}


LONG CPlayerAtxCtrl::GetVolume(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return TL_GetVolume();
}


LONG CPlayerAtxCtrl::SetViewPortCount(LONG Count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_ViewWinNormal.ResetViewPortCount(Count);
	m_ViewWinFullScreen.ResetViewPortCount(Count);
	return 0;
}


LONG CPlayerAtxCtrl::SetViewWindowMode(LONG Mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_ViewWinNormal.ResetLayoutMode((LAYOUT_MODE)Mode);
	m_ViewWinFullScreen.ResetLayoutMode((LAYOUT_MODE)Mode);
	return 0;
}


LONG CPlayerAtxCtrl::SetDisplayMode(LONG Mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_ViewWinNormal.ResetDisplayMode((DISPLAY_MODE)Mode);
	m_ViewWinFullScreen.ResetDisplayMode((DISPLAY_MODE)Mode);
	return 0;
}


LONG CPlayerAtxCtrl::EnableAutoDirector(LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	TL_EnableAutoDirector(Enable);
	return 0;
}


LONG CPlayerAtxCtrl::PlayClearList(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_PlayList.clear();
	return 0;
}


LONG CPlayerAtxCtrl::ScreenShot(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if(m_CurViewWin==0)
	{
		m_ViewWinNormal.ScreenShot();
	}
	else if(m_CurViewWin==1)
	{
		m_ViewWinFullScreen.ScreenShot();
	}

	return 0;
}
