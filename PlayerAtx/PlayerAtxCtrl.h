#pragma once


//class CPlayerCoreDlg;
// PlayerAtxCtrl.h : CPlayerAtxCtrl ActiveX 控件类的声明。

class TL_ViewWindow;
#include "TL_ViewWinWrapNormal.h"
#include "TL_ViewWinWrapFullScreen.h"

#include <vector>
// CPlayerAtxCtrl : 有关实现的信息，请参阅 PlayerAtxCtrl.cpp。




class CPlayerAtxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPlayerAtxCtrl)

// 构造函数
public:
	CPlayerAtxCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CPlayerAtxCtrl();

	DECLARE_OLECREATE_EX(CPlayerAtxCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CPlayerAtxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPlayerAtxCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CPlayerAtxCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		eventidEventOnOpeningMedia = 10L,
		dispidScreenShot = 21L,
		dispidPlayClearList = 20L,
		dispidEnableAutoDirector = 19L,
		eventidEventOnVideoChanged = 9L,
		eventidEventOnAudioChanged = 8L,
		dispidSetDisplayMode = 18L,
		dispidSetViewWindowMode = 17L,
		dispidSetViewPortCount = 16L,
		eventidEventOnVolumeChanged = 7L,
		dispidGetVolume = 15L,
		dispidSetVolume = 14L,
		dispidVideoStatus = 13L,
		dispidAudioStatus = 12L,
		eventidEventOnScreenChanged = 6L,
		dispidPriorVideo = 11L,
		dispidEnableVideo = 10L,
		dispidEnableAudio = 9L,
		dispidGetDuration = 8L,
		dispidGetAudioCount = 7L,
		dispidGetVideoCount = 6L,
		dispidPlayToggleFullScreen = 5L,
		eventidEventOnPlayToEnd = 5L,
		eventidEventOnPlayPaused = 4L,
		eventidEventOnPlayStoped = 3L,
		eventidEventOnPlayStarted = 2L,
		eventidEventOnPlayTimeUpdate = 1L,
		dispidPlayTogglePause = 4L,
		dispidPlaySeek = 3L,
		dispidPlayStop = 2L,
		dispidPlayStart = 1L,
		dispidPlayAdd = 0L,
	};

	struct AVChangedParam
	{
		int index;
		int id;
		int status;
	};

private:
	//TL_ViewWindow*				m_pViewWindow;	// 视频输出窗口指针
	CRITICAL_SECTION			m_cs;
	TL_ViewWinWrapNormal		m_ViewWinNormal;			// 正常视频输出窗口
	TL_ViewWinWrapFullScreen	m_ViewWinFullScreen;		// 全拼视频输出窗口
	int							m_CurViewWin;				// 0=正常 1=全屏

	int							m_inited;				
	int							m_ViewPortOpened;		// 播放端口是否已打开
	std::vector<CString>		m_PlayList;				// 播放列表
	DWORD						m_DurationInSecond;		// 当前播放媒体文件总时长

private:
	int PlayerAtxInit();
	int ToggleFullScreen();
	int DoUpdatePlayTime(long pts);
	int DoUpdatePlayStatus(PLAY_STATUS status);
	
public:
	int DoOnViewPortDClick(int index);
	/*
	* 这些函数 留给非成员函数的callback函数调用
	*/
	// PlayerAtx对应Player的回调函数
	int OptVideoFrameDisplay(int tid, long pts, int viewport_id, UINT8** data, int* linesize, int width, int height);
	int OptFrameDataInit(int size, int width, int height);
	int OptOpenViewPort(int viewport_count);
	int OptCloseViewPort();

	int CbOnPlayTimeUpdate(long pts);
	int CbOnOpeningMeida();
	int CbOnPlayStarted();
	int CbOnPlayStoped();
	int CbOnPlayPaused();
	int CbOnPlayToEnd();
	int CbOnVolumeChanged(long volume);
	int CbOnAudioChanged(int audio_index,int track_id, int status);
	int CbOnVideoChanged(int video_index,int track_id, int status);

protected:
	// 播放器动作
	afx_msg HRESULT OnMsgOptPlayStop( WPARAM wParam, LPARAM lParam);

	afx_msg HRESULT OnMsgUpdateDisplay( WPARAM wParam, LPARAM lParam);
	//afx_msg HRESULT OnMsgOpenViewPort( WPARAM wParam, LPARAM lParam);
	//afx_msg HRESULT OnMsgCloseViewPort( WPARAM wParam, LPARAM lParam);
	
	// 播放器事件
	afx_msg HRESULT OnMsgPlayTimeUpdate( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgOpeningMedia(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgPlayStarted( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgPlayStoped( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgPlayPaused( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgPlayToEnd( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgVolumeChanged( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgAudioChanged( WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgVideoChanged( WPARAM wParam, LPARAM lParam);

	afx_msg HRESULT OnMsgExitFullScreen(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgScreenModeChanged(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam);

protected:
	/************************************************************************/
	/* 插件接口                                                             */
	/************************************************************************/
	LONG PlayAdd(LPCTSTR url);
	LONG PlayStart(LONG Index);
	LONG PlayStop(void);
	LONG PlaySeek(LONG Pts);
	LONG PlayTogglePause(void);
	LONG PlayToggleFullScreen(void);
	LONG GetVideoCount(void);
	LONG GetAudioCount(void);
	LONG GetDuration(void);
	LONG EnableAudio(LONG AudioIndex, LONG Enable);
	LONG EnableVideo(LONG VideoIndex, LONG Enable);
	LONG PriorVideo(LONG VideoIndex);
	LONG AudioStatus(LONG AudioIndex);
	LONG VideoStatus(LONG VideoIndex);
	LONG SetVolume(LONG volume);
	LONG GetVolume(void);
	LONG SetViewPortCount(LONG Count);
	LONG SetViewWindowMode(LONG Mode);
	LONG SetDisplayMode(LONG Mode);
	LONG EnableAutoDirector(LONG Enable);
	LONG PlayClearList(void);
	LONG ScreenShot(void);

	/************************************************************************/
	/* 插件Event                                                            */
	/************************************************************************/
	void EventOnPlayTimeUpdate(LONG time)
	{
		FireEvent(eventidEventOnPlayTimeUpdate, EVENT_PARAM(VTS_I4), time);
	}

	void EventOnPlayStarted(void)
	{
		FireEvent(eventidEventOnPlayStarted, EVENT_PARAM(VTS_NONE));
	}

	void EventOnPlayStoped(void)
	{
		FireEvent(eventidEventOnPlayStoped, EVENT_PARAM(VTS_NONE));
	}

	void EventOnPlayPaused(void)
	{
		FireEvent(eventidEventOnPlayPaused, EVENT_PARAM(VTS_NONE));
	}

	void EventOnPlayToEnd(void)
	{
		FireEvent(eventidEventOnPlayToEnd, EVENT_PARAM(VTS_NONE));
	}
	
	void EventOnScreenChanged(LONG mode)
	{
		FireEvent(eventidEventOnScreenChanged, EVENT_PARAM(VTS_I4), mode);
	}

	void EventOnVolumeChanged(LONG NewVolume)
	{
		FireEvent(eventidEventOnVolumeChanged, EVENT_PARAM(VTS_I4), NewVolume);
	}

	void EventOnAudioChanged(LONG AudioIndex, LONG TrackID, LONG Status)
	{
		FireEvent(eventidEventOnAudioChanged, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4), AudioIndex, TrackID, Status);
	}

	void EventOnVideoChanged(LONG VideoIndex, LONG TrackID, LONG Status)
	{
		FireEvent(eventidEventOnVideoChanged, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4), VideoIndex, TrackID, Status);
	}

	void EventOnOpeningMedia(void)
	{
		FireEvent(eventidEventOnOpeningMedia, EVENT_PARAM(VTS_NONE));
	}
};

