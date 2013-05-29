// PlayerAtxCtrl.cpp : CPlayerAtxCtrl ActiveX �ؼ����ʵ�֡�

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

// ��Ϣӳ��

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


// ����ӳ��

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



// �¼�ӳ��

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



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CPlayerAtxCtrl, 1)
	PROPPAGEID(CPlayerAtxPropPage::guid)
END_PROPPAGEIDS(CPlayerAtxCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CPlayerAtxCtrl, "PLAYERATX.PlayerAtxCtrl.1",
	0x8983d1ae, 0x5d45, 0x4810, 0x91, 0x9, 0xec, 0x25, 0x7c, 0xff, 0x7f, 0x97)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CPlayerAtxCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DPlayerAtx = { 0x13051A81, 0x92AD, 0x45D1, { 0xBF, 0xF2, 0xDD, 0x46, 0x2F, 0xF4, 0xC4, 0xEC } };
const IID IID_DPlayerAtxEvents = { 0x3BB4CB3B, 0x2338, 0x419B, { 0xAC, 0x68, 0x27, 0x6C, 0x1C, 0xA, 0x66, 0x88 } };


// �ؼ�������Ϣ

static const DWORD _dwPlayerAtxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPlayerAtxCtrl, IDS_PLAYERATX, _dwPlayerAtxOleMisc)



// CPlayerAtxCtrl::CPlayerAtxCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CPlayerAtxCtrl ��ϵͳע�����

BOOL CPlayerAtxCtrl::CPlayerAtxCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

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



// CPlayerAtxCtrl::CPlayerAtxCtrl - ���캯��

CPlayerAtxCtrl::CPlayerAtxCtrl()
	//:m_PlayerCore(NULL)
{
	InitializeIIDs(&IID_DPlayerAtx, &IID_DPlayerAtxEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�

	TL_PlayerInit();

	memset(&m_cs, 0, sizeof(m_cs));
	InitializeCriticalSection(&m_cs);

	//m_pViewWindow = NULL;
	m_inited  = 0;
	m_PlayList.clear();
	m_DurationInSecond = 0;
	m_ViewPortOpened = 0;
}



// CPlayerAtxCtrl::~CPlayerAtxCtrl - ��������

CPlayerAtxCtrl::~CPlayerAtxCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�

	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> ~CPlayerAtxCtrl\n");
	OutputDebugString(strMsg);

	// �������ͬ���ķ�ʽ�رղ�������ʹ�ؼ�������ǰ��ȷ����������Դ���������ͷ�
	TL_PlayStop();
	TL_PlayerRelease();

	//if(m_pViewWindow != NULL)
	//{
	//	delete m_pViewWindow;
	//	m_pViewWindow = NULL;
	//}
}



// CPlayerAtxCtrl::OnDraw - ��ͼ����

void CPlayerAtxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
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
			// ����PlayerAtx������
			m_pViewWindow->SetBackgroundColor(RGB(0,0,0),1);
			// ����ViewPort
			m_pViewWindow->CreateViewPort();
			// ��ʼ��������
			PlayerAtxInit();
			m_inited = 1;
		}

	}

	if( m_inited == 1 && m_pViewWindow != NULL)
	{
		/*
		* ���������֮ǰ�Ѿ���ʼ�����ˣ������ػ�������
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
			// ����ViewPort
			//m_ViewWinNormal.CreateViewPort();
		}
		
		Ret = m_ViewWinFullScreen.Create(IDD_VIEW_WIN_WRAP_FULLSCREEN,this);
		//m_ViewWinFullScreen.SetParent();
		m_ViewWinFullScreen.SetPlayerAtxCtrl(this);
		ASSERT(Ret);
		if( Ret)
		{
			m_ViewWinFullScreen.SetBackgroundColor(ViewWinWrapBkColor,1);
			// ����ViewPort
			//m_ViewWinFullScreen.CreateViewPort();
			// ��ʼ��������
		}
		//::SetWindowPos(m_ViewWinFullScreen.m_hWnd
		//	, HWND_TOP
		//	, 0
		//	, 0
		//	, 1200
		//	, 800
		//	, SWP_HIDEWINDOW);

		// ��ʼ��������
		PlayerAtxInit();
		m_CurViewWin = 0;// Ĭ������Ϊ ��������
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
		//	int nWidth = GetSystemMetrics(SM_CXSCREEN);  //��Ļ���    
		//	int nHeight = GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�

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



// CPlayerAtxCtrl::DoPropExchange - �־���֧��

void CPlayerAtxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CPlayerAtxCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CPlayerAtxCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}

// CPlayerAtxCtrl ��Ϣ�������















/*
* PlayerAtx�ؼ� ע��� Player�Ļص�����
* ��Щ�ص�������
* ��Щ��Ҫ�ı���棬��Ⱦ��Ƶ�����ʼ���ͷ���ʼ����Ƶ�˿�
* ��Щ��Ҫ����һ��Evnet��������������֪Player��״̬
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
* Player ����״̬֪ͨ�ص�
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
PlayerAtx�ؼ� ��ӦPlayer�Ļص������� ����һЩ��Ҫ����Player�����Event
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
	
	// ���������ͬ����SendMessage���������е������û�����CPlayerAtxCtrl::PlayStart����������Ϊ�����������video_schedule�лص���
	// �����Ƚ����� CPlayerAtxCtrl::PlayStart --> MediaPlayer::PlayStop�У�
	// ��MediaPlayer::PlayStop�� �ȴ� video_schedule���˳�����video_schedule���ô�����Ҫ����CPlayerAtxCtrl::OptVideoFrameDisplay��ʱ��
	// ����ΪCPlayerAtxCtrl::OptVideoFrameDisplay���������Ҫ��Win��Ϣ�߳���ִ�еģ�����Ҫ�ȴ�MediaPlayer::PlayStop����
	// �����������ˣ������ķ����ܶ�
	// ����ʹ��PostMessage���� �첽�� WM_TL_UPDATE_DISPLAY
	// ���������и����⣬���ǿ��� WM_TL_UPDATE_DISPLAY ���� PlayStoped ���¼��� 
	// ������ MediaPlayer���ûص��ر�ViewPort�� ��� WM_TL_UPDATE_DISPLAY�ᵼ����ˢһ֡ͼ��ViewPort���������ȽϺý��
	// ����ͨ�� m_ViewPortOpened �ж��Ƿ�Ҫˢ����ʾ
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
	// ��������ViewPort������
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
	// ���ﲻ��ֱ�Ӽ���FireEvent,�����ڴ�����߳���FireEvent����Ч��,����Event����ʽ����
	// ע��Ҫ�� �첽��PostMessage �������� ͬ����SendMessage
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
	// �����Ѿ�������,��ֹͣ������֪ͨ�ϲ�����
	// ���ǲ����ڻص���ֱ��ִ�У�Ҫ���첽��Ϣתһ��, Ҫ�����ܻ������� ��Ϊ PlayStop�����ӿ� ���ܻ�� Callback�Ĳ����߳� ����
	PostMessage( WM_TL_OPT_PLAY_STOP, 0, 0);
	// ��Ȼ�Ժ������Ӧ�������б��ţ��������Ҫ�Զ�������һ����Ƶ�ˣ�ͬ������Ҫ�����첽��Ϣ
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
* �Զ��崰����Ϣ��Ӧ����
*/
HRESULT CPlayerAtxCtrl::OnMsgOptPlayStop( WPARAM wParam, LPARAM lParam)
{
	TL_PlayStopAsync();
	return 0;
}

/*
��������Ϣ��Ӧ����
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
	// ���ſ�ʼʱ������һ����Ļģʽ�¼�
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

	int    nWidth=GetSystemMetrics(SM_CXSCREEN);  //��Ļ���    
	int    nHeight=GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�

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

	if(index==0)// �󴰿�˫��������л�
	{
		ToggleFullScreen();
	} 
	else if( index > 0 && index < 4) // С�����л�����С���潻��
	{
		TL_SwapVideoViewPort(index,0);
	}
	return 0;
}

/************************************************************************/
/* Atx�ӿں���                                                          */
/************************************************************************/

LONG CPlayerAtxCtrl::PlayAdd(LPCTSTR url)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	//AfxMessageBox(url);

	CString CStrUrl(url);
	m_PlayList.push_back(CStrUrl);
	return 0;
}

LONG CPlayerAtxCtrl::PlayStart(LONG Index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
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

	// TODO: �ڴ���ӵ��ȴ���������
	
	// ���ڲ���ֹͣ��ʹ��ͬ���Ĳ������Ƚϰ�ȫ��
	
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

	// TODO: �ڴ���ӵ��ȴ���������
	int64_t pts_ms = (int64_t)pts_in_s*1000;
	TL_PlaySeekAsync(pts_ms);
	return 0;
}

LONG CPlayerAtxCtrl::PlayTogglePause(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_PlayTogglePauseAsync();
	return 0;
}

LONG CPlayerAtxCtrl::PlayToggleFullScreen(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	ToggleFullScreen();
	return 0;
}


LONG CPlayerAtxCtrl::GetVideoCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	long video_count = (int)TL_GetVideoCount();
	return video_count;
}


LONG CPlayerAtxCtrl::GetAudioCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	long audio_count = (int)TL_GetAudioCount();
	return audio_count;
}


LONG CPlayerAtxCtrl::GetDuration(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	long duration_in_second = (long)TL_GetDuration();
	return duration_in_second;
}


LONG CPlayerAtxCtrl::EnableAudio(LONG AudioIndex, LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_EnableAudio(AudioIndex,Enable);
	return 0;
}


LONG CPlayerAtxCtrl::EnableVideo(LONG VideoIndex, LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_EnableVideo(VideoIndex,Enable);
	return 0;
}


LONG CPlayerAtxCtrl::PriorVideo(LONG VideoIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_SetVideoPrior(VideoIndex);
	return 0;
}


LONG CPlayerAtxCtrl::AudioStatus(LONG AudioIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return TL_AudioStatus(AudioIndex);
}


LONG CPlayerAtxCtrl::VideoStatus(LONG VideoIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return TL_VideoStatus(VideoIndex);
}


LONG CPlayerAtxCtrl::SetVolume(LONG volume)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_SetVolume(volume);
	return 0;
}


LONG CPlayerAtxCtrl::GetVolume(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return TL_GetVolume();
}


LONG CPlayerAtxCtrl::SetViewPortCount(LONG Count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_ViewWinNormal.ResetViewPortCount(Count);
	m_ViewWinFullScreen.ResetViewPortCount(Count);
	return 0;
}


LONG CPlayerAtxCtrl::SetViewWindowMode(LONG Mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_ViewWinNormal.ResetLayoutMode((LAYOUT_MODE)Mode);
	m_ViewWinFullScreen.ResetLayoutMode((LAYOUT_MODE)Mode);
	return 0;
}


LONG CPlayerAtxCtrl::SetDisplayMode(LONG Mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_ViewWinNormal.ResetDisplayMode((DISPLAY_MODE)Mode);
	m_ViewWinFullScreen.ResetDisplayMode((DISPLAY_MODE)Mode);
	return 0;
}


LONG CPlayerAtxCtrl::EnableAutoDirector(LONG Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TL_EnableAutoDirector(Enable);
	return 0;
}


LONG CPlayerAtxCtrl::PlayClearList(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_PlayList.clear();
	return 0;
}


LONG CPlayerAtxCtrl::ScreenShot(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
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
